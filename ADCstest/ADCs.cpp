#include "ADCs.h"
#include <Adafruit_MCP3008.h>

ADCs::ADCs(int ADCsCount)
    : convCount(ADCsCount), converters(new Adafruit_MCP3008[ADCsCount]) {}

ADCs::~ADCs() {
    delete[] converters;
}

bool ADCs::setup(const int (&ADCsCSPins)[ADCS_COUNT]) {
    for (int i = 0; i < convCount; i++) {
        if (!converters[i].begin(ADCsCSPins[i])) {
            Serial.println("Failed to initialize MCP3008 chip number " + String(i));
            return false;
        }
    }

    Serial.println("MCP3008 chips initialized successfully.");
    return true;
}

int ADCs::read(int chip, int channel) const {

    if (chip < 0 || chip >= convCount) {
        // Handle error for out-of-bound chip
        Serial.println("Error: Chip number out of bounds.");
        return -1;
    } else if (channel < 0 || channel >= ADCS_CHANNEL_COUNT) {
        // Handle error for out-of-bound channel
        Serial.println("Error: Channel number out of bounds.");
        return -2;
    }

    return converters[chip].readADC(channel);
}

bool ADCs::readAll(int (&readings)[ADCS_COUNT * ADCS_CHANNEL_COUNT]) const {

    int value = 0;
    for (int chip = 0; chip < convCount; chip++) {
        for (int channel = 0; channel < ADCS_CHANNEL_COUNT; channel++) {
            value = read(chip, channel);

            // Check for error values from read function
            if (value == -1 || value == -2) {
                return false;  // Return false if an error was detected
            }

            readings[chip * ADCS_CHANNEL_COUNT + channel] = value;        }
    }

    return true;
}
