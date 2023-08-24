#include "Buffer.h"
#include "Errors.h"
#include <Arduino.h>

// Check if the buffer is empty
bool SensorDataBuffer::isBufferEmpty() const {
    return bufferSize == 0;
}

// Check if the buffer is full
bool SensorDataBuffer::isBufferFull() const {
    return bufferSize >= BUFFER_CAPACITY;
}

// Get the capacity of the buffer
int SensorDataBuffer::getBufferCapacity() const {
    return BUFFER_CAPACITY;
}

// Get the number of samples in the buffer
int SensorDataBuffer::getBufferSize() const {
    return bufferSize;
}

// Get the index of the next sample to be read
int SensorDataBuffer::getReadIndex() const {
    return readIndex;
}

// Get the index of the next sample to be written
int SensorDataBuffer::getWriteIndex() const {
    return writeIndex;
}

// Move the read index to the next sample
void SensorDataBuffer::moveReadIndexForward() {
    readIndex = (readIndex + 1) % BUFFER_CAPACITY;
    bufferSize--;
}

// Move the read index to the previous sample
void SensorDataBuffer::moveReadIndexBackward() {
    readIndex = (readIndex - 1 + BUFFER_CAPACITY) % BUFFER_CAPACITY;
    bufferSize++;
}

// Move the write index to the next sample
void SensorDataBuffer::moveWriteIndexForward() {
    writeIndex = (writeIndex + 1) % BUFFER_CAPACITY;
    bufferSize++;
}

// Move the write index to the previous sample
void SensorDataBuffer::moveWriteIndexBackward() {
    writeIndex = (writeIndex - 1 + BUFFER_CAPACITY) % BUFFER_CAPACITY;
    bufferSize--;
}

// Get the current sample date path
void SensorDataBuffer::getCurrentSampleDatePath() {
    // Get the next sample from the buffer
    time_t sampleTimestampSec = static_cast<time_t>(buffer[readIndex].timestampMillis / 1000);

    struct tm timeInfo;
    // Get the struct tm from the time_t object
    localtime_r(&sampleTimestampSec, &timeInfo);
    // Convert the struct tm to a formatted string like "YYYY-MM-DD/\0"
    strftime(sampleDate, 12, "%F/", &timeInfo);

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

// Check if the date has changed and update the sample date path if it did
bool SensorDataBuffer::hasDateChanged() {
    time_t seconds = static_cast<time_t>(buffer[readIndex].timestampMillis / 1000);
    // Compare the current timestamp with the timestamp of the next day
    bool result = seconds >= nextDay;
    if (result) {
        getCurrentSampleDatePath();
    }
    return result;
}

// Check if the content of the sample is null
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

// Return the pointer to the next sample to be written
sensorData* SensorDataBuffer::addSample() {
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

// Get the next sample from the buffer
const sensorData* SensorDataBuffer::getSample() {
    // If the buffer is empty
    if (isBufferEmpty()) {
        // Return nullptr if the sample was not retrieved from the buffer
        return nullptr;
    }

    // Get the next sample from the buffer
    const sensorData* sample = &buffer[readIndex];

    // Move the read index to the next sample
    moveReadIndexForward();

    // Return the sample if it was retrieved from the buffer
    return sample;
}

// Print the buffer state
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

// Print the indexes of the buffer
void SensorDataBuffer::printBufferIndexes() const {
    Serial.print("Read index: ");
    Serial.print(readIndex);
    Serial.print(" / Write index: ");
    Serial.println(writeIndex);
}

// Dump the buffer content or a part of it
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
