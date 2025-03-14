#include "DataReader.h"
#include "Network.h"
#include "Buffer.h"
#include "ADCs.h"

// Define the static member outside the class
const int DataReader::VL6180_ADDRESSES[1] = {0x29};
const int DataReader::VL6180_TCA_CHANNELS[1] = {0}; // Change if needed.
const int DataReader::VL53L4CD_ADDRESSES[4] = {0x52, 0x52, 0x52, 0x52};
const int DataReader::VL53L4CD_TCA_CHANNELS[4] = {1, 2, 5 , 6}; // Change if needed.

void DataReader::updateCurrentTime() {
    // Set the variable 'currentMicros' with the current time in microseconds (us)
    currentMicros = micros();
}

bool DataReader::setup() {
    // Setup the external ADCs 
    if (!adcs.setup()) {
        return false;
    }

    // Initialize the VL6180 sensor
    vl6180.startSensors();

    // Initialize the VL53L4CD sensors
    vl53l4cds.startSensors();

    // If everything went well, return true
    return true;
}

void DataReader::addDataToSample(sensorData* newSample) {
    // Fill the buffer with current timestamp (in milliseconds)
    newSample->timestampMillis = getCurrentMillisTimestamp();

    // Fill the buffer with the pressure sensor data
    adcs.readAll(newSample->pressureSensor);

    // Read VL6180 data
    int vl6180Readings[VL6180_ADDR_COUNT];
    vl6180.readSensors(vl6180Readings);
    newSample->vl6180Distance = vl6180Readings[0];

    // Read VL53L4CDs data
    int vl53l4cdReadings[VL53L4CD_SENSOR_COUNT];
    vl53l4cds.readSensors(vl53l4cdReadings);
    memcpy(newSample->vl53L4CDDistances, vl53l4cdReadings, sizeof(vl53l4cdReadings));
}

void DataReader::fillBuffer(SensorDataBuffer* dataBuffer) {
    // Save the time when the device start to collect the data from the sensors,
    // to keep control of the intervals between data collection
    updateCurrentTime();

    // If the time elapsed since the last data collection is greater than the
    // set interval for data collections, collect more data
    if (currentMicros - dataPrevColletionMicros > dataCollectIntervalMicros) {

        // Pointer to the next sample to be written
        sensorData* newSample = dataBuffer->getNewSample();

        addDataToSample(newSample);

        // Update the time variable that controls the collect interval
        dataPrevColletionMicros = currentMicros;
    }
}
