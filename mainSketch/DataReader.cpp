#include "DataReader.h"

void DataReader::updateCurrentTime() {
    currentMicros = micros();
}

void DataReader::setup() {
    // Set each pressure sensor pin as an input
    for (int i = 0; i < pinCount; ++i) {
        pinMode(pins[i], INPUT);
    }
    // Setup the external ADCs
    adcs.setup();
}

void DataReader::fillBuffer() {
    // Save the time when the device start to collect the data from the sensors, to keep control of the intervals of data collect
    updateCurrentTime();

    if (currentMicros - dataPrevColletionMicros > dataCollectInterval || dataPrevColletionMicros == 0) {
        // Fill the buffer with collect time data
        buffer[writeIndex].timestampUnix = getCurrentTime();
        buffer[writeIndex].timestampMillis = millis();

        // Fill the buffer with sensors data connected to the internal ADC
        int i = 0;
        for (i; i < pinCount; ++i) {
            buffer[writeIndex].pressure[i] = analogRead(pins[i]);
            buffer[writeIndex].sum += buffer[writeIndex].pressure[i];
        }

        // Fill the buffer with sensors data connected to the external ADC
        int channelIndex = 0;
        for (i; i < sensorCount; i += 2) {
            adcs.read(channelIndex);

            // Fill the buffer with the collected values
            buffer[writeIndex].pressure[i] = adcs.get(0);
            buffer[writeIndex].pressure[i + 1] = adcs.get(1);
            buffer[writeIndex].sum += (buffer[writeIndex].pressure[i] +
                                       buffer[writeIndex].pressure[i + 1]);

            channelIndex++;
        }

        // After storing the data on the buffer, we need to update the write index, making
        // the buffer work as a circular buffer, resetting the index when it reaches the end
        writeIndex = (writeIndex + 1) % bufferCapacity;

        // And we also need to update the variables that show the buffer state
        bufferSize++;

        // Print a dot just to show that the data it's been collected
        Serial.println(".");

        // Print the buffer state
        printBufferState();

        // Update the time variable that controls the collect interval
        dataPrevColletionMicros = currentMicros;
    }
}

const sensorData* DataReader::getSample() {
    sensorData* result = buffer + readIndex;
    // Updates the read index, making sure it is reset when reaching the end of the buffer
    readIndex = (readIndex + 1) % bufferCapacity;
    bufferSize--;
    return result;
}

bool DataReader::isBufferEmpty() const {
    return bufferSize <= 0;
}

void DataReader::printBufferState() const {
    // If the buffer gets full, show the corresponding fatal error
    if (bufferSize >= bufferCapacity) {
        showError(bufferFull, true);
    }

    // Prints the buffer state
    Serial.print(bufferSize);
    Serial.print("/");
    Serial.println(bufferCapacity);
}
