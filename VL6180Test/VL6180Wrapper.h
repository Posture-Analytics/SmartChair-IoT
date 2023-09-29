#ifndef VL6180_WRAPPER_H_
#define VL6180_WRAPPER_H_

#include <VL6180X.h>

class VL6180Wrapper {
    int sensorCount;
    int period = 100;
    int scale = 3;
    VL6180X *sensors;

 public:
    VL6180Wrapper(int addresses[], int addrCount, int prd, int scl);
    ~VL6180Wrapper();

    void startSensors();
    void stopSensors();

    void readSensors(int readings[]);
};

#endif  // VL6180_WRAPPER_H_
