#include "Buffer.h"
#include "Errors.h"
#include <Arduino.h>

bool SensorDataBuffer::isBufferEmpty() const {
    return bufferSize == 0;
}

bool SensorDataBuffer::isBufferFull() const {
    return bufferSize >= BUFFER_CAPACITY;
}

int SensorDataBuffer::getBufferCapacity() const {
    return BUFFER_CAPACITY;
}

int SensorDataBuffer::getBufferSize() const {
    return bufferSize;
}

int SensorDataBuffer::getReadIndex() const {
    return readIndex;
}

int SensorDataBuffer::getWriteIndex() const {
    return writeIndex;
}

void SensorDataBuffer::moveReadIndexForward() {
    readIndex = (readIndex + 1) % BUFFER_CAPACITY;
    bufferSize--;
}

void SensorDataBuffer::moveReadIndexBackward() {
    readIndex = (readIndex - 1 + BUFFER_CAPACITY) % BUFFER_CAPACITY;
    bufferSize++;
}

void SensorDataBuffer::moveWriteIndexForward() {
    writeIndex = (writeIndex + 1) % BUFFER_CAPACITY;
    bufferSize++;
}

void SensorDataBuffer::moveWriteIndexBackward() {
    writeIndex = (writeIndex - 1 + BUFFER_CAPACITY) % BUFFER_CAPACITY;
    bufferSize--;
}

time_t SensorDataBuffer::getCurrentSampleSeconds() const {
    return buffer[readIndex].timestampMillis / 1000ULL;
}

void SensorDataBuffer::computeCurrentSampleDate(char* sampleDate) {
    // Get the next sample from the buffer
    time_t sampleTimestamp = getCurrentSampleSeconds();
    // Get the time information from the time_t object
    localtime_r(&sampleTimestampSec, &timeInfo);
    // Convert the struct tm to a formatted string like "YYYY-MM-DD\0"
    strftime(sampleDate, 11, "%F", &timeInfo);
}

void SensorDataBuffer::computeNextDaySeconds() {
    time_t sampleTimestamp = getCurrentSampleSeconds();
    // Get the time info of the next day
    sampleTimestampSec += 24 * 60 * 60;
    localtime_r(&sampleTimestampSec, &timeInfo);
    // Set the time info to the start of the day
    timeInfo.tm_hour = 0;
    timeInfo.tm_min = 0;
    timeInfo.tm_sec = 0;

    // Convert back to time_t and save it
    nextDay = mktime(&timeInfo);
}

bool SensorDataBuffer::hasDateChanged() {
    // Compare the current timestamp with the timestamp of the next day
    return getCurrentSampleSeconds() >= nextDay;
}

bool SensorDataBuffer::isSampleNull(const sensorData* sample) const {
    // Check if we have some non null data in the sample struct
    for (int i = 0; i < PRESSURE_SENSOR_COUNT; i++) {
        if (sample->pressureSensor[i] != 0) {
            return false;
        }
    }

    // Return true if all the sample data is null
    return true;
}

const sensorData* SensorDataBuffer::getSample() {
    // If the buffer is empty
    if (isBufferEmpty()) {
        // Return nullptr if the sample was not retrieved from the buffer
        return nullptr;
    }

    // Get the next sample from the buffer
    const sensorData* ptrSample = &buffer[readIndex];

    // Move the read index to the next sample
    moveReadIndexForward();

    // Return the sample if it was retrieved from the buffer
    return ptrSample;
}

sensorData* SensorDataBuffer::getNewSample() {
    // If the buffer is full
    if (isBufferFull()) {
        // Return nullptr if the sample was not added to the buffer
        return nullptr;
    }

    // Get the pointer to the next sample to be written
    sensorData* ptrSample = &buffer[writeIndex];

    // Move the write index to the next sample
    moveWriteIndexForward();

    // Return the pointer to the next sample to be written
    return ptrSample;
}

void SensorDataBuffer::printBufferState() const {
    // If the buffer gets full
    if (isBufferFull()) {
        // Print an error message and restart the device
        showError(bufferFull, true);
    }

    // Prints the buffer state
    Serial.print(bufferSize);
    Serial.print("/");
    Serial.println(BUFFER_CAPACITY);
    Serial.print("; ");
}

void SensorDataBuffer::printBufferIndexes() const {
    Serial.print("Read index: ");
    Serial.print(readIndex);
    Serial.print(" / Write index: ");
    Serial.println(writeIndex);
}

void SensorDataBuffer::dumpBufferContent(int start, int end) const {
    // Iterate over the buffer content on the desired range
    for (int i = start; i < end; i++) {
        // Get the current sample
        const sensorData* sample = &buffer[i];

        // Print the sample timestamp
        Serial.print(sample->timestampMillis);
        Serial.print(" ");

        // Print the sample pressure sensor values
        for (int j = 0; j < PRESSURE_SENSOR_COUNT; j++) {
            Serial.print(sample->pressureSensor[j]);
            Serial.print(" ");
        }

        // Print the end of line
        Serial.println();
    }
}
