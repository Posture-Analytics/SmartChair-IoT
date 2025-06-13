#ifndef VL6180_WRAPPER_H_
#define VL6180_WRAPPER_H_

#include <VL6180X.h>

class VL6180Wrapper {
    int sensorCount;
    int period = 100;
    int scale = 1;
    VL6180X *sensors;
    const int* tcaChannels;
    TCA9548A& tca;

 public:
    VL6180Wrapper(const int addresses[], int addrCount, int prd, int scl, const int* tcaChans, TCA9548A& tcaRef);
    ~VL6180Wrapper();

    void startSensors();
    void stopSensors();

    void readSensors(int readings[]);
};

#endif  // VL6180_WRAPPER_H_
