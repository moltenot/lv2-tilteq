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

    void print_output(fftwf_complex *data, int n)
    {
        // just prints the input fftw_complex struct to standard out

        // data: the data to print
        // N: the length of the data

        for (int i = 0; i < n; i++)
        {
            printf("%lf + %lf i\n", data[i][0], data[i][1]);
        }
    }

    fftwf_complex *fft(float *port, uint32_t n)
    {
        /* compute the forward fft of the input port of size n */

        fftwf_complex *out = fftwf_alloc_complex(sizeof(fftwf_complex) * n);

        // convert size from uint32_t to const int*
        int intversion = (int)n;
        const int *length_of_array = &intversion;

        // create a plan
        //TODO don't create the plan each time, although this depends on
        //      sample numbers change a lot
        fftwf_plan plan = fftwf_plan_dft_r2c(1, length_of_array, port, out, FFTW_ESTIMATE);
        fftwf_execute(plan);

        print_output(out, n);
    }

public:
    TiltEQ(double rate)
        : Plugin<TiltEQ>(3)
    {
        printf("created tilteq with rate %f\n", rate);
    } // since we have 3 ports

    void run(uint32_t sample_count)
    { // currently pass through
        for (uint32_t i = 0; i < sample_count; ++i)
        {
            // printf("\ncalled run\n");
            // print_nums(p(INPUT_PORT_INDEX), sample_count);
            fftwf_complex *freq_bins = fft(p(INPUT_PORT_INDEX), sample_count);
            p(OUTPUT_PORT_INDEX)[i] = p(INPUT_PORT_INDEX)[i];
        }
    }
};

unsigned _ = TiltEQ::register_class("https://github.com/moltenot/lv2-tilteq");