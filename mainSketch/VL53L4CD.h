#ifndef VL53L4CD_WRAPPER_H_
#define VL53L4CD_WRAPPER_H_

#include <SparkFun_VL53L1X.h>

class VL53L4CDWrapper {
  int sensorCount;
  SFEVL53L1X *sensors;
  const int* tcaChannels;
  TCA9548A& tca;
public:
  VL53L4CDWrapper(const int addresses[], int addrCount, const int tcaChans[], TCA9548A& tcaRef);
  ~VL53L4CDWrapper();

  void startSensors();
  void stopSensors();
  void readSensors(int readings[]);
};

#endif