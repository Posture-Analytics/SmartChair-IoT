#ifndef ExternalADCs_H_
#define ExternalADCs_H_

#include <ADS1115_WE.h>
#include <Wire.h>

#include "Errors.h"

// Define the addresses of the external ADCs connected to the I2C bus
#define I2C_ADDRESS_1 0x48
#define I2C_ADDRESS_2 0x49
// Define constants to set the external ADCs (Analog to Digital Converter)
#define VOLTAGE_RANGE ADS1115_RANGE_4096
#define MEASURE_MODE ADS1115_SINGLE
#define CONVERSION_RATE ADS1115_860_SPS

class ADCs {
    // Set the list of channels that will be iterated during the data collection
    static const ADS1115_MUX channels[4];

    ADS1115_WE adcs[2];
    int reads[2];

 public:
    ADCs();

    void setup();

    void read(int channelIndex);

    int get(int index) const;
};

#endif  // ExternalADCs_H_
