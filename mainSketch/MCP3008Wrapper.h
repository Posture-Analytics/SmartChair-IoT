/*

    MCP3008Wrapper

    * This module implements a wrapper class to setup and read a set of MCP3008 ADCs.
    * The constructor of the class takes in the CS pins of the ADCs and starts the Adafruit's MCP3008 objects stored in the wrapper.
    * The read function of the wrapper takes in a chip number and a channel (both starting at 0) and returns the analog read in the specified chip's channel.

*/

#ifndef MCP3008_WRAPPER_H_
#define MCP3008_WRAPPER_H_

#include <Adafruit_MCP3008.h>

#define CHANNEL_COUNT 8
//#define ADC_COUNT 3

// The class is defined as a template to allow for any amount of ADCs to be wrapped by the same object.
template<int ADC_COUNT>
class MCP3008Wrapper {

    Adafruit_MCP3008 converters[ADC_COUNT];

public:

    MCP3008Wrapper(int CSPins[ADC_COUNT]);

    int read(int chip, int channel);
};


// -#- The definitions of the methods are in the header file due to template limitations -#-
// A separated header file - cpp file configuration can be obtained by defining ADC_COUNT as a macro, removing the template syntax and moving the definitions below to a MCP3008Wrapper.cpp file.
// We assume all ADCs have 8 channels, but this number could also be templated into the class.

template<int ADC_COUNT>
MCP3008Wrapper<ADC_COUNT>::MCP3008Wrapper(int CSPins[ADC_COUNT]) {

    for (int i = 0; i < ADC_COUNT; i++) {
        //Assing each MCP3008 object to a CS pin.
        converters[i].begin(CSPins[i]);
    }
}

template<int ADC_COUNT>
int MCP3008Wrapper<ADC_COUNT>::read(int chip, int channel) {
    return converters[chip].readADC(channel);
}

#endif
