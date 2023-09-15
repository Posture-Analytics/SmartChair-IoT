/*
    ExternalADCs.h

    * This module handles the external ADCs and the data collection through them.
    * It setups the external ADCs and checks the status of the initialization process.
    * It also reads the data from the external ADCs in parallel, according to the channel
    index, and stores it in the externalAdcsValues array (as an internal buffer).
*/

#ifndef ExternalADCs_H_
#define ExternalADCs_H_

#include <ADS1115_WE.h>
#include <Wire.h>

#include "Errors.h"

// Define the addresses of the external ADCs connected to the I2C bus
#define I2C_ADDRESS_1 0x48
#define I2C_ADDRESS_2 0x49

// Define constants to setup the external ADCs (Analog to Digital Converter)
#define VOLTAGE_RANGE ADS1115_RANGE_4096
#define MEASURE_MODE ADS1115_SINGLE
#define CONVERSION_RATE ADS1115_860_SPS

class ExternalADCs {

    // Instantiate 2 ADS1115_WE objects, one for each external ADC
    ADS1115_WE adcs[2];

    // Save the reads from the ADCs
    int externalAdcsValues[2];

 public:

    // Setup the external ADCs
    bool setup();

    // Read the external ADCs in parallel, according to the channel index
    void read(int channelIndex);

    // Get the read from the external ADCs, according to the index
    int get(int index) const;
};

#endif  // ExternalADCs_H_
