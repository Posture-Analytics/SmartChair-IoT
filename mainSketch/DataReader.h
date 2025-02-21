/*
    DataReader.h

    * This module handle the sensors and the data collection from them.
    * It setup the sensors and the constants related to them
    * It also handle the routine to collect data from the sensors and store it on the buffer.
*/

#ifndef DataReader_H_
#define DataReader_H_

#include "ADCs.h"
#include "Buffer.h"
#include "VL6180Wrapper.h"
#include "VL53L4CD.h"

// Sample Rate of the data collection, in hertz (Hz)
const int SAMPLE_RATE = 2;


/**
 * This class handle the sensors and the data collection from them.
 * It setup the sensors and the constants related to them
 * It also handle the routine to collect data from the sensors and store it on the buffer.
*/
class DataReader {
    // External ADCs (2x MCP3008) that will be used to read the pressure sensors
    ADCs adcs;

    // VL6180 Sensor
    static const int VL6180_ADDRESSES[1];  // I2C address of the sensor
    static constexpr int VL6180_ADDR_COUNT = 1;
    static constexpr int VL6180_PERIOD = 100;  // Measurement period (ms)
    static constexpr int VL6180_SCALE = 1;     // Scaling factor
    VL6180Wrapper vl6180;

    // VL53L4CD Sensors
    static const int VL53L4CD_ADDRESSES[4];
    static constexpr int VL53L4CD_ADDR_COUNT = 4;
    static const int VL53L4CD_TCA_CHANNELS[4];
    VL53L4CDWrapper vl53l4cds;

    // Set the interval between data collect, in microseconds (us)
    const int dataCollectIntervalMicros = 1e6 / SAMPLE_RATE;
    // Save the time of the last data collect, in microseconds (us)
    unsigned long dataPrevColletionMicros = 0;
    // Save the current time, in microseconds (us)
    unsigned long currentMicros = 0;

    /** Update the current time variable */
    void updateCurrentTime();  

public:

    // Initialize VL6180 and the VL53L4CDs in the constructor
    DataReader() : 
        vl6180(VL6180_ADDRESSES, VL6180_ADDR_COUNT, VL6180_PERIOD, VL6180_SCALE),
        vl53l4cds(VL53L4CD_ADDRESSES, VL53L4CD_ADDR_COUNT, VL53L4CD_TCA_CHANNELS) 
    {}
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
