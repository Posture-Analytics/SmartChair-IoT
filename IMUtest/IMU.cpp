#include "IMU.h"

IMU::IMU(uint8_t imu_address) {

    // Set the IMU's object _ad0val (last bit of the address) according to the provided address
    if (imu_address == 0x68){
        imu._ad0val = false;
    } else if (imu_address == 0x69){
        imu._ad0val = true;
    } else {
        // Serial.println("Invalid IMU address!");
    }
}

IMU::~IMU() {
    delete &imu;
}

bool IMU::init() {

    // Serial.println("Initializing IMU...");
    while (!initialized) {
        imu.begin();

        if (imu.status == ICM_20948_Stat_Ok) {
            initialized = true;
        } else {
            // Serial.print(".");
            delay(500);
        }
    }

    // Serial.println("\nDevice connected!");

    // Serial.println("Beginning DMP setup...");

    // Auxiliary variable to check if all the DMP setup steps were successful
    bool success = true;

    // Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
    success &= (imu.initializeDMP() == ICM_20948_Stat_Ok);

    // Enable the DMP orientation sensor
    success &= (imu.enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION) == ICM_20948_Stat_Ok);

    success &= (imu.setDMPODRrate(DMP_ODR_Reg_Quat9, 0) == ICM_20948_Stat_Ok); // Set to the maximum

    // Enable the FIFO
    success &= (imu.enableFIFO() == ICM_20948_Stat_Ok);

    // Enable the DMP
    success &= (imu.enableDMP() == ICM_20948_Stat_Ok);

    // Reset DMP
    success &= (imu.resetDMP() == ICM_20948_Stat_Ok);

    // Reset FIFO
    success &= (imu.resetFIFO() == ICM_20948_Stat_Ok);

    // Check success
    if (success) {
        // Serial.println("DMP setup successful!");
        return true;
    } else {
        // Serial.println("DMP setup failed!");
        return false;
    }
}

void IMU::computeAngles() {

    // Auxiliary variable to avoid repeated operations
    q2sqr = q2 * q2;

    // roll (x-axis rotation)
    t0 = +2.0 * (q0 * q1 + q2 * q3);
    t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
    roll = atan2(t0, t1) * 180.0 / PI;

    // pitch (y-axis rotation)
    t2 = +2.0 * (q0 * q2 - q3 * q1);
    t2 = t2 > 1.0 ? 1.0 : t2;
    t2 = t2 < -1.0 ? -1.0 : t2;
    pitch = asin(t2) * 180.0 / PI;

    // yaw (z-axis rotation)
    t3 = +2.0 * (q0 * q3 + q1 * q2);
    t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
    yaw = atan2(t3, t4) * 180.0 / PI;
}

void IMU::collectSample() {
    // Reset FIFO in order to erase old data and store the next sample in the first position
    imu.resetFIFO();
}

bool IMU::readSample(unsigned long timeoutMillis) {
    // Limit the time to read the sample
    unsigned long startMillis = millis();
    while (millis() - startMillis < timeoutMillis) {

        imu.readDMPdataFromFIFO(&imu_data);

        // Check if reading status is okay or if more data is available in FIFO
        if ((imu.status == ICM_20948_Stat_Ok) || (imu.status == ICM_20948_Stat_FIFOMoreDataAvail)) {

            if ((imu_data.header & DMP_header_bitmap_Quat9) > 0) {
                // Convert the values into double and scale by 2^30	
                q1 = ((double)imu_data.Quat9.Data.Q1) / SCALE_FACTOR; 
                q2 = ((double)imu_data.Quat9.Data.Q2) / SCALE_FACTOR;
                q3 = ((double)imu_data.Quat9.Data.Q3) / SCALE_FACTOR;

                // Compute the fourth quaternion component (q0)
                q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

                // Compute the Euler angles
                computeAngles();
            }

            // Return true to indicate that the sample was successfully read
            return true;
        }
    }
    // Return false to indicate that the sample was not read
    return false;
}
