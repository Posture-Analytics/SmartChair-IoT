#ifndef VL53L4CD_WRAPPER_H_
#define VL53L4CD_WRAPPER_H_

#include <SparkFun_VL53L1X.h>

class VL53L4CDWrapper {
    int sensorCount;
    int period = 100;

    SFEVL53L1X *sensors;

 public:
    VL53L4CDWrapper(int addresses[], int addrCount);
    ~VL53L4CDWrapper();

    void startSensors();
    void stopSensors();

    void readSensors(int readings[]);
};

#endif  // VL53L4CD_WRAPPER_H_
