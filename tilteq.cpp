#include <lv2plugin.hpp>

// have to make sure these are the same as in tilteq.ttl
#define TILT_PORT_INDEX 0
#define INPUT_PORT_INDEX 1
#define OUTPUT_PORT_INDEX 2

using namespace LV2;

class TiltEQ : public Plugin<TiltEQ>
{
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
            p(OUTPUT_PORT_INDEX)[i] = p(INPUT_PORT_INDEX)[i];
        }
    }

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
};

unsigned _ = TiltEQ::register_class("https://github.com/moltenot/lv2-tilteq");