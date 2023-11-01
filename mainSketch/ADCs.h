/**
 * @file ADCs.h
 * @brief Wrapper class to interface with MCP3008 ADCs using Adafruit's MCP3008 library.
 */

#ifndef ADCS_H
#define ADCS_H

#include <Adafruit_MCP3008.h>

// Number of channels in each MCP3008 chip.
#define ADCS_CHANNEL_COUNT 8

// Default CS pins for the MCP3008 chips.
static const int ADCS_CS_PINS[] = {14, 32};
static const int ADCS_COUNT = sizeof(ADCS_CS_PINS)/sizeof(*ADCS_CS_PINS);

/**
 * @class ADCs
 * @brief Wrapper class for MCP3008 ADC chips.
 * 
 * Provides a convenient interface for setting up and reading from multiple MCP3008 ADCs.
 */
class ADCs {

private:

    // Number of connected MCP3008 chips.
    int convCount;

    // Pointer to array of Adafruit's MCP3008 objects.
    Adafruit_MCP3008* converters;

public:

    /**
     * @brief Construct the ADCs wrapper.
     * 
     * @param ADCsCount The number of MCP3008 chips to be managed by the wrapper. Defaults to ADCS_COUNT.
     */
    ADCs(int ADCsCount=ADCS_COUNT);

    /**
     * @brief Destructor for the ADCs wrapper. 
     * Frees dynamically allocated memory.
     */
    ~ADCs();

    /**
     * @brief Initializes the MCP3008 chips.
     * 
     * @param ADCsCSPins Array of CS pin numbers for the MCP3008 chips. Defaults to ADCS_CS_PINS.
     * @return true if initialization is successful for all chips, false otherwise.
     */
    bool setup(const int (&ADCsCSPins)[ADCS_COUNT]=ADCS_CS_PINS);

    /**
     * @brief Reads an analog value from a specified channel of a given MCP3008 chip.
     * 
     * @param chip Index of the MCP3008 chip.
     * @param channel Channel number on the specified chip.
     * @return Analog read value, or -1 if chip index is out of bounds, -2 if channel number is out of bounds.
     */
    int read(int chip, int channel) const;

    /**
     * @brief Fetches analog values from all channels of all MCP3008 chips.
     * 
     * @param readings Array to store analog values from all channels and chips.
     * @return true if all reads are successful, false otherwise.
     */
    bool readAll(int (&readings)[ADCS_COUNT * ADCS_CHANNEL_COUNT]) const;
};

#endif
