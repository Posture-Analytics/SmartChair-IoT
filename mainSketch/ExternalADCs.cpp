#include "ExternalADCs.h"

const ADS1115_MUX ADCs::channels[4] = {ADS1115_COMP_3_GND, ADS1115_COMP_2_GND,
                                       ADS1115_COMP_1_GND, ADS1115_COMP_0_GND};

ADCs::ADCs() {
    // Instantiate the external ADCs, according to each address
    adcs[0] = ADS1115_WE(I2C_ADDRESS_1);
    adcs[1] = ADS1115_WE(I2C_ADDRESS_2);
}

void ADCs::setup() {
    // Initialize both the external ADCs
    for (int i = 0; i < 2; i++) {
        // If the ADCs are not connected, show an error and restart the device
        if (adcs[i].init()) {
            Serial.print("ADS1115 No ");
            Serial.print(i);
            Serial.println(" not connected!");
            showError(externalADCInitFailure, true);
        }
        // Set the voltage range of the ADCs
        adcs[i].setVoltageRange_mV(VOLTAGE_RANGE);
        // Set the conversion rate (SPS: Samples per second) of the ADCs
        adcs[i].setConvRate(CONVERSION_RATE);
        // Set the measure mode (SINGLE or CONTINUOUS) of the ADCs
        adcs[i].setMeasureMode(MEASURE_MODE);
    }
}

// Read the external ADCs in parallel, according to the channel index
void ADCs::read(int channelIndex) {
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
    reads[0] = adcs[0].getResultWithRange(-5082, 5082);
    reads[1] = adcs[1].getResultWithRange(-5082, 5082);
}

int ADCs::get(int index) const {
    // Fit the values into a positive range before giving the read
    return max(0, reads[index]);
}
