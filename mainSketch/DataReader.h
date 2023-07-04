#ifndef DataReader_H_
#define DataReader_H_

#include <FirebaseESP32.h>
#include "ExternalADCs.h"

// time_t getCurrentTime();

// Define a struct to organize the collected data
struct sensorData {
    const static int sensorCount = 12;

    // 4 bytes
    unsigned long long timestampMillis;
    unsigned long sum = 0;

    // 2 bytes each
    int pressure[sensorCount];
};

class DataReader {
    const static int bufferCapacity = 1024;

    // External ADCs that will be used to read the pressure sensors
    ADCs adcs;

    // Sample Rate of the data collection, in hertz (Hz)
    const int SAMPLE_RATE = 50;
    // Define the pins that will be used to read the pressure sensors through the internal ADC (ADC1)
    const uint8_t pins[4] = {A2, A3, A4, A5};

    // Create variables to help with the timing of data collection
    const int dataCollectInterval = 1000000 / SAMPLE_RATE;
    unsigned long dataPrevColletionMicros = 0;
    unsigned long currentMicros = 0;

    // Create some variables to help with the data collection and sensor handling
    const int sensorCount = 12;
    const int pinCount = sizeof(pins) / sizeof(pins[0]);

    // Create a circular buffer and some variables to help with its use, like pointers and capacity indicators
    // Set the capactity of the buffer and create the buffer
    sensorData buffer[bufferCapacity];

    // Create a variable to help with the buffer's size (number of samples in the buffer)
    int bufferSize = 0;
    // Create some variables to help with the buffer's pointers
    int writeIndex = 0;
    int readIndex = 0;


    void updateCurrentTime();

 public:
    void setup();

    void fillBuffer();

    const sensorData* getSample();

    bool isBufferEmpty() const;

    void printBufferState() const;
};

#endif  // DataReader_H_
