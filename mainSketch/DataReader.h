/*
    DataReader.h

    * This module handle the sensors and the data collection from them.
    * It setup the sensors and the constants related to them
    * It also handle the routine to collect data from the sensors and store it on the buffer.
*/

#ifndef DataReader_H_
#define DataReader_H_

// #include <FirebaseESP32.h>
#include "ExternalADCs.h"
#include "Buffer.h"

// Sample Rate of the data collection, in hertz (Hz)
const int SAMPLE_RATE = 2;


/**
 * This class handle the sensors and the data collection from them.
 * It setup the sensors and the constants related to them
 * It also handle the routine to collect data from the sensors and store it on the buffer.
*/
class DataReader {
    // External ADCs that will be used to read the pressure sensors
    ExternalADCs externalAdcs;
    // Define the amount of pressure sensors hooked up to the external ADCs
    const int externalAdcPinsCount = 8;

    // Define the pins that will be used to read the pressure sensors through the internal ADC(ADC1)
    const uint8_t internalAdcPins[4] = {A2, A3, A4, A5};
    // Define the amount of pressure sensors hooked up to the internal ADC (ADC1)
    const int internalAdcPinsCount = sizeof(internalAdcPins) / sizeof(internalAdcPins[0]);

    // Set the interval between data collect, in microseconds (us)
    const int dataCollectIntervalMicros = 1e6 / SAMPLE_RATE;
    // Save the time of the last data collect, in microseconds (us)
    unsigned long dataPrevColletionMicros = 0;
    // Save the current time, in microseconds (us)
    unsigned long currentMicros = 0;

    /** Update the current time variable */
    void updateCurrentTime();  

public:

    /**
     * Setup the sensors and the devices' pins
     * 
     * @return true if everything went well, false otherwise
     */
    bool setup();

    /**
     * Collect data from the sensors and store it in the buffer location represented by the pointer
     * 
     * @param dataBuffer: Pointer to the buffer where the data will be stored
     */
    void addDataToSample(sensorData* newSample);

    /**
     * Fill buffer if the moment of the function call is greater than the data collection interval
     * 
     * @param dataBuffer: Pointer to the buffer where the data will be stored
     */
    void fillBuffer(SensorDataBuffer* dataBuffer);
};

#endif  // DataReader_H_
