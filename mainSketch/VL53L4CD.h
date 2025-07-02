/**
 * @file VL53L4CD.h
 * @brief Wrapper class for managing multiple VL53L4CD Time-of-Flight distance
 * sensors.
 *
 * This module provides a convenient interface for initializing, controlling,
 * and reading from multiple VL53L4CD sensors connected through a TCA9548A I2C
 * multiplexer.
 */

#ifndef VL53L4CD_WRAPPER_H_
#define VL53L4CD_WRAPPER_H_

#include "TCA9548A.h"
#include <SparkFun_VL53L1X.h>

/**
 * @class VL53L4CDWrapper
 * @brief Wrapper class for managing multiple VL53L4CD Time-of-Flight distance
 * sensors.
 *
 * This class provides a unified interface for controlling multiple VL53L4CD
 * sensors connected through a TCA9548A I2C multiplexer. It handles sensor
 * initialization, starting/stopping measurements, and reading distance data
 * from all sensors.
 */
class VL53L4CDWrapper {
private:
  int sensorCount;        // Number of VL53L4CD sensors managed by this wrapper
  SFEVL53L1X *sensors;    // Array of VL53L4CD sensor objects
  const int *tcaChannels; // Array of TCA9548A channel numbers for each sensor
  TCA9548A &tca;          // Reference to the TCA9548A I2C multiplexer

public:
  /**
   * @brief Constructor for the VL53L4CDWrapper class.
   *
   * @param addresses Array of I2C addresses for the VL53L4CD sensors
   * @param addrCount Number of sensors to manage
   * @param tcaChans Array of TCA9548A channel numbers corresponding to each
   * sensor
   * @param tcaRef Reference to the TCA9548A multiplexer instance
   */
  VL53L4CDWrapper(const int addresses[], int addrCount, const int tcaChans[],
                  TCA9548A &tcaRef);

  /**
   * @brief Destructor for the VL53L4CDWrapper class.
   *
   * Cleans up dynamically allocated memory for the sensor array.
   */
  ~VL53L4CDWrapper();

  /**
   * @brief Initializes and starts distance measurements on all sensors.
   *
   * This method configures each sensor and begins continuous distance
   * measurements. It switches between TCA channels to communicate with each
   * sensor individually.
   */
  void startSensors();

  /**
   * @brief Stops distance measurements on all sensors.
   *
   * This method stops the continuous measurement mode on all managed sensors.
   */
  void stopSensors();

  /**
   * @brief Reads distance measurements from all sensors.
   *
   * @param readings Array to store the distance readings in millimeters.
   *                 The array should have at least sensorCount elements.
   *                 Each element will contain the distance reading from the
   *                 corresponding sensor, or -1 if the reading failed.
   */
  void readSensors(int readings[]);
};

#endif
