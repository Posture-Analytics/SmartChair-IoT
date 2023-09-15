#include "DataReader.h"
#include "Network.h"
#include "Buffer.h"

void DataReader::updateCurrentTime() {
    // Set the variable 'currentMicros' with the current time in microseconds (us)
    currentMicros = micros();
}

bool DataReader::setup() {
    // Set each pressure sensor pin as an input
    for (int i = 0; i < internalAdcPinsCount; ++i) {
        pinMode(internalAdcPins[i], INPUT);
    }

    // Setup the external ADCs
    if (!externalAdcs.setup()) {
        return false;
    }

    // If everything went well, return true
    return true;
}

void DataReader::addDataToSample(sensorData* newSample) {
    // Fill the buffer with current timestamp (in milliseconds)
    newSample->timestampMillis = getCurrentMillisTimestamp();

    // Fill the buffer with sensor data connected to the internal ADC
    int i = 0;
    for (i; i < internalAdcPinsCount; ++i) {
        newSample->pressureSensor[i] = analogRead(internalAdcPins[i]);
    }

    // Fill the buffer with sensor data connected to the external ADC
    int externalAdcsChannelIndex = 0;
    for (i; i < internalAdcPinsCount + externalAdcPinsCount; i += 2) {
        externalAdcs.read(externalAdcsChannelIndex);

        // Fill the buffer with the collected values
        newSample->pressureSensor[i] = externalAdcs.get(0);
        newSample->pressureSensor[i + 1] = externalAdcs.get(1);

        // Go to next channel of the external ADCs
        externalAdcsChannelIndex++;
    }
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
