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
        : Plugin<TiltEQ>(3) {} // since we have 3 ports

    void run(uint32_t sample_count)
    { // currently pass through
        for (uint32_t i = 0; i < sample_count; ++i)
        {
            p(OUTPUT_PORT_INDEX)[i] = p(INPUT_PORT_INDEX)[i];
        }
    }
};

unsigned _ = TiltEQ::register_class("https://github.com/moltenot/lv2-tilteq");