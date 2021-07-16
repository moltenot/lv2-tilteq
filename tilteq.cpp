#include <lv2plugin.hpp>
#include <fftw3.h>

// have to make sure these are the same as in tilteq.ttl
#define TILT_PORT_INDEX 0
#define INPUT_PORT_INDEX 1
#define OUTPUT_PORT_INDEX 2

using namespace LV2;

class TiltEQ : public Plugin<TiltEQ>
{

protected:
    bool has_printed = false;

    void print_nums(float *port, uint32_t n)
    {
        int num_cols = 6;
        for (uint32_t i = 0; i < n; i++)
        {
            printf("%f\t", port[i]);
            if (i % num_cols == 0)
            {
                printf("\n");
            }
        }
    }

    void print_fft_complex(fftwf_complex *data, int n)
    {
        // just prints the input fftw_complex struct to standard out

        // data: the data to print
        // N: the length of the data

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

    void normalize_amplitude(fftwf_complex *frequencies, uint32_t n)
    {
        /* divide each element of freqencies by n (in amplitude) */
        for (int i = 0; i<n; i++) {
            frequencies[i][0] = frequencies[i][0]/n;
            frequencies[i][1] = frequencies[i][1]/n;
        }
    }

public:
    TiltEQ(double rate)
        : Plugin<TiltEQ>(3)
    {
        printf("created tilteq with rate %f\n", rate);
    } // since we have 3 ports

    void run(uint32_t sample_count)
    { // currently pass through

        printf("\ncalled run\n");
        // print_nums(p(INPUT_PORT_INDEX), sample_count);
        fftwf_complex *freq_bins = fft_forward(p(INPUT_PORT_INDEX), sample_count);

        // change the frequencies in here
        normalize_amplitude(freq_bins, sample_count);

        float *processed_signal = fft_backward(freq_bins, sample_count);

        // print the frequency bins (once)
        if (has_printed == false)
        {
            printf("printing frequency bins\n");
            print_fft_complex(freq_bins, sample_count);
            print_nums(processed_signal, sample_count);
            has_printed = true;
        }

        for (uint32_t i = 0; i < sample_count; ++i)
        {
            p(OUTPUT_PORT_INDEX)[i] = processed_signal[i];
        }

        fftwf_free(freq_bins);
    }
};

unsigned _ = TiltEQ::register_class("https://github.com/moltenot/lv2-tilteq");