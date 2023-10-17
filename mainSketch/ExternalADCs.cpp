#include "ExternalADCs.h"
#include "Debug.h"

// #define DEBUG_EXTERNAL_ADCS

// Set the list of channels that will be iterated during the data collection
const ADS1115_MUX channels[4] = {ADS1115_COMP_3_GND, ADS1115_COMP_2_GND,
                                ADS1115_COMP_1_GND, ADS1115_COMP_0_GND};

// Setup the external ADCs
bool ExternalADCs::setup() {
    // Instantiate the external ADCs, according to each address
    adcs[0] = ADS1115_WE(I2C_ADDRESS_1);
    adcs[1] = ADS1115_WE(I2C_ADDRESS_2);

    #ifdef DEBUG_EXTERNAL_ADCS

        return true;

    #else

        // Initialize both the external ADCs
        for (int i = 0; i < 2; i++) {
            // If the ADCs are not connected, show an error and restart the device
            if (!adcs[i].init()) {
                LogFatalln("ADS1115 No ", i, " not connected!")

                errorHandler.showError(ErrorType::ExternalADCInitFailure, true);
                return false;
            }
            // Set the voltage range of the ADCs
            adcs[i].setVoltageRange_mV(VOLTAGE_RANGE);
            // Set the conversion rate (SPS: Samples per second) of the ADCs
            adcs[i].setConvRate(CONVERSION_RATE);
            // Set the measure mode (SINGLE or CONTINUOUS) of the ADCs
            adcs[i].setMeasureMode(MEASURE_MODE);
        }

    return true;

    #endif
}

// Read the external ADCs in parallel, according to the channel index
void ExternalADCs::read(int channelIndex) {
    // Choose the channel to be read
    ADS1115_MUX channel = channels[channelIndex];

    // Set the channel in the ADCs
    adcs[0].setCompareChannels(channel);
    adcs[1].setCompareChannels(channel);

    // Start the measurement, if the mode is SINGLE
    if (MEASURE_MODE == ADS1115_SINGLE) {
        adcs[0].startSingleMeasurement();
        adcs[1].startSingleMeasurement();
        // Wait until the measurement is done
        while (adcs[0].isBusy() || adcs[1].isBusy()) {}
    }

    // Read the results of the ADCs
    externalAdcsValues[0] = adcs[0].getResultWithRange(-5082, 5082);
    externalAdcsValues[1] = adcs[1].getResultWithRange(-5082, 5082);
}

// Get the read from the external ADCs, according to the index
int ExternalADCs::get(int index) const {
    // Fit the values into a positive range before giving the read
    return max(0, externalAdcsValues[index]);
}
