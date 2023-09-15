/*
    Buffer.h

    * This module handles the buffer that stores the data collected from the sensors.
    * The buffer consists of an array of sensorData structs and some indexes to keep track of
    the buffer state. It also provides functions to add and get samples from the buffer,
    handle buffer capacity and indexes.
    * For debug purposes, it also provides functions to print the buffer state and dump
    its content.
*/

#ifndef Buffer_H_
#define Buffer_H_

#include "time.h"

#include <Arduino.h>

// Define the capacity of the buffer
const int BUFFER_CAPACITY = 1024;

// Define the amount of pressure sensors
const int PRESSURE_SENSOR_COUNT = 12;

// Define a struct to organize the collected data
struct sensorData {
    // 8 bytes
    unsigned long long timestampMillis = 0;

    // 2 bytes each
    int pressureSensor[PRESSURE_SENSOR_COUNT] = {0};
};

// Define a class to store the collected data
class SensorDataBuffer {

    // Sotre information regarding the time of the read samples
    struct tm timeInfo;

    // Store the timestamp of the following day to check if the date has changed
    // The initial value is 0 so that it gets updated during the first use
    time_t nextDay = 0;

public:

    // Create a buffer based on the sensorData struct
    sensorData buffer[BUFFER_CAPACITY];

    // Hold the number of samples in the buffer
    int bufferSize = 0;

    // Index to read the next sample
    int readIndex = 0;
    // Index to write the next sample
    int writeIndex = 0;

    // Check if the buffer is empty
    bool isBufferEmpty() const;

    // Check if the buffer is full
    bool isBufferFull() const;

    // Get the capacity of the buffer
    int getBufferCapacity() const;

    // Get the number of samples in the buffer
    int getBufferSize() const;

    // Get the index of the next sample to be read
    int getReadIndex() const;

    // Get the index of the next sample to be written
    int getWriteIndex() const;

    // Move the read index to the next sample
    void moveReadIndexForward();

    // Move the read index to the previous sample
    void moveReadIndexBackward();

    // Move the write index to the next sample
    void moveWriteIndexForward();

    // Move the write index to the previous sample
    void moveWriteIndexBackward();

    // Get the timestamp of the next sample to be read from the buffer
    time_t getCurrentSampleMillis() const;

    // Get the current sample date. Should receive an array of at least 11 chars
    void computeCurrentSampleDate(char* sampleDate);

    // Compute the timestamp of the start of the following day
    void computeNextDaySeconds();

    // Check if the date has changed
    bool hasDateChanged();

    // Check if the content of the sample is null
    bool isSampleNull(const sensorData* sample) const;

    // Get the next sample from the buffer at the read index and increment it
    const sensorData* getSample();

    // Return a pointer to the new sample to be written and increment the write index
    sensorData* getNewSample();

    // Print the buffer state
    void printBufferState() const;

    // Print the indexes of the buffer
    void printBufferIndexes() const;

    // Dump the buffer content or a part of it
    void dumpBufferContent(int start = 0, int end = BUFFER_CAPACITY) const;
};

#endif  // Buffer_H_
