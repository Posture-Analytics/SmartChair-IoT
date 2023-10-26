/*
    IMU

    * This module implements a wrapper for the ICM-20948 IMU, according to the library provided by
    Sparkfun (SparkFun_ICM-20948_ArduinoLibrary).
    * The IMU's DMP provides us with the orientation of the IMU in the form of a quaternion, which
    can be converted to Euler angles (roll, pitch and yaw) using the "computeAngles" method.
    * It sets up the IMU's DMP into 55Hz Output Data Rate (ODR) and, in order to collect a brand
    new sample from a FIFO buffer (FIFO stands for "First In First Out"), it resets the FIFO and
    waits for the next sample (~18ms). This way, we can make sure to handle a sample that reflects
    the actual state of the IMU.
    * TODO: The roll and pitch angles are very acurate, but the yaw angle is not. The reason to thid must 
    be investigated further.
*/

#ifndef MPU_h
#define MPU_h

#include "ICM_20948.h"

/**
 * IMU class that implements the ICM-20948 IMU units.
 * 
 * This class implements a wrapper for the ICM-20948 IMU, according to the library provided by
 * Sparkfun (SparkFun_ICM-20948_ArduinoLibrary).
 * 
 * The IMU's DMP provides us with the orientation of the IMU in the form of a quaternion, which
 * can be converted to Euler angles (roll, pitch and yaw) using the "computeAngles" method. 
*/
class IMU {

private:
    // ICM-20948 IMU object
    ICM_20948_I2C imu;

    // Data structure that holds the IMU's DMP sample data
    icm_20948_DMP_data_t imu_data;

    // Flag that indicates if the IMU has been initialized
    bool initialized = false;

    // Quaternion components and auxiliary variables
    double q0, q1, q2, q3, q2sqr, t0, t1, t2, t3, t4;

    // Scale factor for the quaternion components (2^30)
    const double SCALE_FACTOR = 1073741824.0;

public:

    /**
     * Constructor for the IMU class.
    */
    IMU(uint8_t imu_address = 0x69);

    /**
     * Destructor for the IMU class.
    */
    ~IMU();

    /**
     * Initializes the IMU according to the provided address.
    */
    bool init();

    /**
     * Computes the Euler angles (roll, pitch, yaw) from the quaternion components.
    */
    void computeAngles();

    /**
     * Resets the FIFO buffer to collect a new sample.
    */
    void collectSample();

    /**
     * Reads a sample from the FIFO buffer and computes the Euler angles.
     * 
     * Takes a timeoutMillis parameter that indicates the maximum time to wait for a sample to be
     * available in the FIFO buffer.
     * Returns true if the sample was successfully read, false otherwise.
    */
    bool readSample(unsigned long timeoutMillis = 100);

    // Euler angles
    double roll;    // X-axis rotation
    double pitch;   // Y-axis rotation
    double yaw;     // Z-axis rotation
};

#endif
