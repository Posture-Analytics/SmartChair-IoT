/**
 * @file VL6180Wrapper.h
 * @brief Wrapper class for managing VL6180X proximity and ambient light
 * sensors.
 *
 * This module provides a convenient interface for initializing, controlling,
 * and reading from VL6180X sensors connected through a TCA9548A I2C
 * multiplexer. The VL6180X is a fully integrated proximity sensor that includes
 * a ranging sensor, ambient light sensor, and an infrared emitter in a single
 * package.
 */

#ifndef VL6180_WRAPPER_H_
#define VL6180_WRAPPER_H_

#include "TCA9548A.h"
#include <VL6180X.h>

/**
 * @class VL6180Wrapper
 * @brief Wrapper class for managing VL6180X proximity and ambient light
 * sensors.
 *
 * This class provides a unified interface for controlling VL6180X sensors
 * connected through a TCA9548A I2C multiplexer. It handles sensor
 * initialization, configuration, and reading distance measurements with
 * configurable measurement periods and scaling.
 */
class VL6180Wrapper {
private:
  int sensorCount;  // Number of VL6180X sensors managed by this wrapper
  int period = 100; // Measurement period in milliseconds (default: 100ms)
  int scale = 1;    // Scaling factor for distance measurements (default: 1)
  VL6180X *sensors; // Array of VL6180X sensor objects
  const int *tcaChannels; // Array of TCA9548A channel numbers for each sensor
  TCA9548A &tca;          // Reference to the TCA9548A I2C multiplexer

public:
  /**
   * @brief Constructor for the VL6180Wrapper class.
   *
   * @param addresses Array of I2C addresses for the VL6180X sensors
   * @param addrCount Number of sensors to manage
   * @param prd Measurement period in milliseconds (time between measurements)
   * @param scl Scaling factor for distance measurements (1x, 2x, or 3x scaling)
   * @param tcaChans Array of TCA9548A channel numbers corresponding to each
   * sensor
   * @param tcaRef Reference to the TCA9548A multiplexer instance
   */
  VL6180Wrapper(const int addresses[], int addrCount, int prd, int scl,
                const int *tcaChans, TCA9548A &tcaRef);

  /**
   * @brief Destructor for the VL6180Wrapper class.
   *
   * Cleans up dynamically allocated memory for the sensor array.
   */
  ~VL6180Wrapper();

  /**
   * @brief Initializes and starts distance measurements on all sensors.
   *
   * This method configures each sensor with the specified period and scaling,
   * then begins continuous distance measurements. It switches between TCA
   * channels to communicate with each sensor individually.
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
   *                 corresponding sensor, scaled according to the configured
   *                 scaling factor.
   */
  void readSensors(int readings[]);
};

#endif // VL6180_WRAPPER_H_
