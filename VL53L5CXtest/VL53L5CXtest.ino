#include <Wire.h>
#include "VL53L5CXWrapper.h"
#include "TCA9548A.h"

// Define sensor parameters
#define SENSOR_COUNT 1
const int SENSOR_ADDRESSES[SENSOR_COUNT] = {0x29};
const int TCA_CHANNELS[SENSOR_COUNT] = {7};

// Create a VL53L5CXWrapper instance
VL53L5CXWrapper sensorWrapper(SENSOR_ADDRESSES, SENSOR_COUNT, TCA_CHANNELS, tca);

// Array to store sensor readings
int16_t sensorReadings[SENSOR_COUNT][8][8];

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(2000); // Allow time for the serial monitor to start
    Serial.println("\n\n===============================");
    Serial.println("VL53L5CX Multi-Sensor Test Starting...");
    Serial.println("===============================");

    // Initialize I2C
    Wire.begin();
    Wire.setClock(100000); // Use 100kHz for stability
    Serial.println("I2C initialized");

    // Initialize all sensors
    sensorWrapper.initializeSensors(SENSOR_ADDRESSES);

    // Start all sensors
    sensorWrapper.startAllSensors();
}

void loop() {
    // Collect data from all sensors
    sensorWrapper.collectData(sensorReadings);

    // Print the readings for each sensor
    for (int i = 0; i < SENSOR_COUNT; i++) {
        Serial.print("Sensor ");
        Serial.print(i + 1);
        Serial.println(" Distance measurements (in mm):");
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 8; x++) {
                Serial.print(sensorReadings[i][y][x]);
                Serial.print("\t");
            }
            Serial.println();
        }
        Serial.println();
    }

    delay(1000); // Wait 1 second before the next reading
}