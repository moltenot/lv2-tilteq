/**
 * Tilt EQ written in the LV2 format using the fftw library for freqency analysis
 * and lv2-c++tools to allow the LV2 specification to be written in c++
 * 
 * @author Theo
 * @date Jul 2021
 */

#include <lv2plugin.hpp>
#include <fftw3.h>
#include <iostream>

// have to make sure these are the same as in tilteq.ttl
#define TILT_PORT_INDEX 0
#define INPUT_PORT_INDEX 1
#define OUTPUT_PORT_INDEX 2
#define PIVOT_PORT_INDEX 3

using namespace LV2;

class TiltEQ : public Plugin<TiltEQ>
{
private:
    double sample_rate;

protected:
    bool has_printed = false;

    /**
     * prints the numbers in a float array in 6 columns
     * 
     * @param data the data to print
     * @param n the length of the data
     */
    void print_nums(float *data, uint32_t n)
    {
        int num_cols = 6;
        for (uint32_t i = 0; i < n; i++)
        {
            printf("%f\t", data[i]);
            if (i % num_cols == 0)
            {
                printf("\n");
            }
        }
    }

    /**
     * prints an fftwf_complex array to stdout in 3 columns
     * 
     * @param data to print
     * @param n length of the data
     */
    void print_fft_complex(fftwf_complex *data, int n)
    {
        int num_cols = 3;

        for (int i = 0; i < n; i++)
        {
            printf("%lf + %lf i\t", data[i][0], data[i][1]);
            if (i % num_cols == 0)
            {
                printf("\n");
            }
        }
    }

    /**
     * Tilts the given frequency bins by whatever angle is specified by
     * the control ports
     */
    void tilt_bins(fftwf_complex *bins, uint32_t n)
    {
        float input = *p(TILT_PORT_INDEX);  // on range(-1, 1)
        float pivot = *p(PIVOT_PORT_INDEX); // Hz, where to pivot the eq around

        double fundamental = sample_rate / n;

        for (uint32_t i = 1; i < (n / 2) + 1; i++)
        {
            float cur_freq = fundamental * i;
            float mult = 1 + (cur_freq - pivot) * input / 5000;
            // the further from the pivot, the bigger the multiplier (on amplitude)
            bins[i][0] = bins[i][0] * mult;
            bins[i][1] = bins[i][1] * mult;
        }
    }

    /**
     * Compute the forward fft going from the time domain to the frequency domain.
     * 
     * @param port pointer to the port buffer
     * @param n size of the buffer
     * 
     * @returns a pointer of type fftwf_complex of the same length as n
     */
    fftwf_complex *fft_forward(float *port, uint32_t n)
    {
        /* compute the forward fft of the input port of size n */

        fftwf_complex *out = fftwf_alloc_complex(sizeof(fftwf_complex) * n);

        int tmp = (int)n;
        const int *tmpptr = &tmp;

        // create a plan
        //TODO don't create the plan each time, although this depends on
        //      sample numbers change a lot
        fftwf_plan plan = fftwf_plan_dft_r2c(1, tmpptr, port,
                                             out, FFTW_ESTIMATE);
        fftwf_execute(plan);
        fftwf_destroy_plan(plan);

        // print_fft_complex(out, n);
        return out;
    }

    /**
     * Compute the backward fft going from the freqency domain to the time domain
     * 
     * @param freq_bins the freqency bins to transform
     * @param n then number of frequency bins
     * 
     * @returns a pointer to a buffer of length n
     */
    float *fft_backward(fftwf_complex *freq_bins, uint32_t n)
    {
        float *out = (float *)malloc(sizeof(float) * n);

        // convrt int to ptr
        int tmp = (int)n;
        const int *tmpptr = &tmp;

        fftwf_plan plan = fftwf_plan_dft_c2r(1, tmpptr, freq_bins, out, FFTW_ESTIMATE);
        fftwf_execute(plan);
        fftwf_destroy_plan(plan);
        return out;
    }

    /**
     * divides each element of the frequencies parameter by n. This is because fftw
     * does an unnormalized fft so this step is necessary if the backward fft is to
     * have the same ampltiude as the input data
     * 
     * @param frequencies the frequency bins
     * @param n the number of frequency bins
     */
    void normalize_amplitude(fftwf_complex *frequencies, uint32_t n)
    {
        /* divide each element of freqencies by n (in amplitude) */
        for (int i = 0; i < n; i++)
        {
            frequencies[i][0] = frequencies[i][0] / n;
            frequencies[i][1] = frequencies[i][1] / n;
        }
    }

public:
    TiltEQ(double rate)
        : Plugin<TiltEQ>(4) // since there are 4 ports in this plugin
    {
        printf("created tilteq with rate %f\n", rate);
        sample_rate = rate;
    }

    /**
     * runs the plugin on a chunk of data. currently it computes the forward fft,
     * then reverts it back to the time domain without doing anything, and writes
     * that to the output port.
     */
    void run(uint32_t sample_count)
    {

        fftwf_complex *freq_bins = fft_forward(p(INPUT_PORT_INDEX), sample_count);

        tilt_bins(freq_bins, sample_count);
        normalize_amplitude(freq_bins, sample_count);

        float *processed_signal = fft_backward(freq_bins, sample_count);

        // write to the output
        for (uint32_t i = 0; i < sample_count; ++i)
        {
            p(OUTPUT_PORT_INDEX)[i] = processed_signal[i];
        }

        fftwf_free(freq_bins);
    }
};

unsigned _ = TiltEQ::register_class("https://github.com/moltenot/lv2-tilteq");