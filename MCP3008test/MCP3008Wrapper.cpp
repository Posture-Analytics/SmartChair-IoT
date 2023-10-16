#include "MCP3008Wrapper.h"
#include <Adafruit_MCP3008.h>

MCP3008Wrapper::MCP3008Wrapper(int* CSPins, int CSPCount) {
    convCount = CSPCount;
    converters = new Adafruit_MCP3008[CSPCount];

    for (int i = 0; i < CSPCount; i++) {
        converters[i].begin(CSPins[i]);
    }
}

MCP3008Wrapper::~MCP3008Wrapper() {
    delete[] converters;
}

int MCP3008Wrapper::read(int chip, int channel) {
    // Throw an error here if chip is out of bound.
    return converters[chip].readADC(channel);
}

void MCP3008Wrapper::read(int chip, int (&readings)[CHANNEL_COUNT]) {
    // Throw an error here if chip is out of bound.
    for (int i = 0; i < CHANNEL_COUNT; i++) {
        readings[i] = converters[chip].readADC(i);
    }

    return;
}
