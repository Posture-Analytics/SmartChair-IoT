#ifndef VL53L5CXWRAPPER_H
#define VL53L5CXWRAPPER_H

#include <Wire.h>
#include "VL53L5CX.h"
#include "TCA9548A.h"

class VL53L5CXWrapper {
private:
    VL53L5CX* sensors;       // Array of VL53L5CX sensor objects
    int sensorCount;         // Number of sensors
    const int* tcaChannels;  // Array of TCA9548A channels for each sensor
    TCA9548A& tca;           // Reference to the TCA9548A instance
    bool* initialized;       // Array to track initialization status of each sensor

public:
    /**
     * @brief Constructor for the VL53L5CXWrapper class.
     * @param addresses Array of I2C addresses for the sensors.
     * @param addrCount Number of sensors.
     * @param tcaChans Array of TCA9548A channels corresponding to each sensor.
     * @param tcaRef Reference to the TCA9548A instance.
     */
    VL53L5CXWrapper(const int addresses[], int addrCount, const int* tcaChans, TCA9548A& tcaRef)
        : sensorCount(addrCount), tcaChannels(tcaChans), tca(tcaRef) {
        sensors = new VL53L5CX[sensorCount];
        initialized = new bool[sensorCount];
        for (int i = 0; i < sensorCount; i++) {
            initialized[i] = false; // Mark all sensors as not initialized
        }
    }

    /**
     * @brief Destructor for the VL53L5CXWrapper class.
     */
    ~VL53L5CXWrapper() {
        delete[] sensors;
        delete[] initialized;
    }

    /**
     * @brief Initializes all sensors.
     * @param addresses Array of I2C addresses for the sensors.
     */
    void initializeSensors(const int addresses[]) {
        for (int i = 0; i < sensorCount; i++) {
            tca.selectBus(tcaChannels[i]);
            delay(20); // Allow time for the TCA channel to stabilize
            initialized[i] = sensors[i].bInit(i + 1, 8, 10, addresses[i], -1);
            if (initialized[i]) {
                Serial.print("Sensor ");
                Serial.print(i + 1);
                Serial.println(" initialized successfully!");
            } else {
                Serial.print("Failed to initialize sensor ");
                Serial.println(i + 1);
            }
        }
    }

    /**
     * @brief Starts ranging for all initialized sensors.
     */
    void startAllSensors() {
        for (int i = 0; i < sensorCount; i++) {
            if (initialized[i]) {
                tca.selectBus(tcaChannels[i]);
                sensors[i].bGetRaningData(); // Ensure sensors are ready
            } else {
                Serial.print("Sensor ");
                Serial.print(i + 1);
                Serial.println(" not initialized.");
            }
        }
    }

    /**
     * @brief Collects data from all initialized sensors and stores it in the provided array.
     * @param readings Array to store the distance readings for each sensor.
     *                 Format: readings[sensorIndex][row][column].
     */
    void collectData(int16_t readings[][8][8]) {
        for (int i = 0; i < sensorCount; i++) {
            if (initialized[i]) {
                tca.selectBus(tcaChannels[i]);
                if (sensors[i].bIsDataReady()) {
                    if (sensors[i].bGetRaningData()) {
                        for (int y = 0; y < 8; y++) {
                            for (int x = 0; x < 8; x++) {
                                int index = y * 8 + x; // Calculate the index for the cell
                                int16_t distance = sensors[i].iGetDataMM(index);
                                readings[i][y][x] = (distance < 0) ? 0 : distance; // Replace invalid data with 0
                            }
                        }
                    } else {
                        Serial.print("Failed to get ranging data for sensor ");
                        Serial.println(i + 1);
                        for (int y = 0; y < 8; y++) {
                            for (int x = 0; x < 8; x++) {
                                readings[i][y][x] = -1; // Mark invalid data
                            }
                        }
                    }
                } else {
                    /*Serial.print("Sensor ");
                    Serial.print(i + 1);
                    Serial.println(" data not ready.");*/
                }
            } else {
                /*Serial.print("Sensor ");
                Serial.print(i + 1);
                Serial.println(" not initialized.");*/
            }
        }
    }
};

#endif // VL53L5CXWRAPPER_H