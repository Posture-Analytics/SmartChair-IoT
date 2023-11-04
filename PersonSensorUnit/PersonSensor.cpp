#include "PersonSensor.h"

#include <Arduino.h>

// Constructor
PersonSensor::PersonSensor() {
    // Initialization, if required, can be done here.
    Wire.begin();
}

// Destructor
PersonSensor::~PersonSensor() {
    // Any cleanup can be done here.
}

bool PersonSensor::readResults(CompleteResults* results) {

    // const int maxBytesPerChunk = 32;
    // const int totalBytes = sizeof(CompleteResults);
    // int8_t* results_bytes = (int8_t*)(results);
    // int index = 0;
    // while (index < totalBytes) {
    //     const int bytesRemaining = totalBytes - index;
    //     const int bytesThisChunk = min(bytesRemaining, maxBytesPerChunk);
    //     const int endIndex = index + bytesThisChunk;
    //     const bool isLastChunk = (bytesRemaining <= maxBytesPerChunk);
    //     Wire.requestFrom(I2C_ADDRESS, bytesThisChunk, isLastChunk);
    //     for (; index < endIndex; ++index) {
    //         if (Wire.available() < 1) {
    //             Serial.print("Only ");
    //             Serial.print(index);
    //             Serial.print(" bytes available on I2C, but we need ");
    //             Serial.println(bytesThisChunk);
    //             return false;
    //         }
    //         results_bytes[index] = Wire.read();
    //     }
    // }
    // return true;

    static const int MAX_BYTES_PER_CHUNK = 32;
    const int TOTAL_BYTES = sizeof(CompleteResults);
    int8_t* results_bytes = reinterpret_cast<int8_t*>(results);

    for (int index = 0; index < TOTAL_BYTES; ) {
        int bytesRemaining = TOTAL_BYTES - index;
        int bytesToRead = (bytesRemaining > MAX_BYTES_PER_CHUNK) ? MAX_BYTES_PER_CHUNK : bytesRemaining;

        // Request bytesToRead from I2C device
        Wire.requestFrom(I2C_ADDRESS, static_cast<uint8_t>(bytesToRead));

        for (int i = 0; i < bytesToRead; ++i) {
            if (!Wire.available()) {
                Serial.print("Only ");
                Serial.print(index + i);
                Serial.print(" bytes available on I2C, but we need ");
                Serial.println(bytesToRead);
                return false;
            }
            results_bytes[index] = Wire.read();
            index++;
        }
    }
    return true;
}

void PersonSensor::writeReg(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

void PersonSensor::triggerSingleShot() {
    writeReg(REG_SINGLE_SHOT, 0x01);
}

void PersonSensor::enableIdRecognition() {
    writeReg(REG_ENABLE_ID, 0x01);
}

void PersonSensor::disableIdRecognition() {
    writeReg(REG_ENABLE_ID, 0x00);
}

uint8_t PersonSensor::getNextId() {
    // Point to the register that holds the next ID to be calibrated.
    Wire.beginTransmission(I2C_ADDRESS);
    Wire.write(REG_CALIBRATE_ID);
    Wire.endTransmission();

    // Request the next ID value.
    Wire.requestFrom(I2C_ADDRESS, static_cast<uint8_t>(1));
    return Wire.read();
}

void PersonSensor::setNextId(uint8_t id) {
    writeReg(REG_CALIBRATE_ID, id);
}

void PersonSensor::eraseIds() {
    writeReg(REG_ERASE_IDS, 0x01);
}

void PersonSensor::enableDebugMode() {
    writeReg(REG_DEBUG_MODE, 0x01);
}

void PersonSensor::disableDebugMode() {
    writeReg(REG_DEBUG_MODE, 0x00);
}
