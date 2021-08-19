# LV2 Tilt Equalizer
This EQ has 2 parameters:

 * Angle 
 * Pivot (Hz)

If the angle is positive, then frequencies below the pivot will be made quieter and above the pivot will be made louder. If the angle is negative the effect is reversed.

How much the frequency is amplified or made quieter is a linear function of it's distance from the pivot.

## Installation
To install type `make install` which will build the bundle and put in in `~/.lv2` by default. The required packages to compile are:
```
fftw (for the fft)
lv2-c++-tools (c++ wrapper for the lv2 specification)
```
On top of the usual C++ ones.
