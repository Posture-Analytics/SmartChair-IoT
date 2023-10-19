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

class MCP3008Wrapper {

    int convCount;
    Adafruit_MCP3008* converters;

public:

    MCP3008Wrapper(int* CSPins, int CSPCount);
    ~MCP3008Wrapper();

    int read(int chip, int channel);
    void read(int chip, int (&readings)[CHANNEL_COUNT]);
};

#endif
