#include "DataReader.h"
#include "Network.h"
#include "Buffer.h"
#include "ADCs.h"

void DataReader::updateCurrentTime() {
    // Set the variable 'currentMicros' with the current time in microseconds (us)
    currentMicros = micros();
}

bool DataReader::setup() {
    // Setup the external ADCs 
    if (!adcs.setup()) {
        return false;
    }

    // If everything went well, return true
    return true;
}

void DataReader::addDataToSample(sensorData* newSample) {
    // Fill the buffer with current timestamp (in milliseconds)
    newSample->timestampMillis = getCurrentMillisTimestamp();

    // Fill the buffer with the pressure sensor data collected from the ADCs
    adcs.readAll(newSample->pressureSensor);
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
