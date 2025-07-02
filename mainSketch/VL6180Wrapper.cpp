#include "VL6180Wrapper.h"
#include "TCA9548A.h"
#include <VL6180X.h>

VL6180Wrapper::VL6180Wrapper(const int addresses[], int addrCount, int prd,
                             int scl, const int *tcaChans, TCA9548A &tcaRef)
    : period(prd), scale(scl), tcaChannels(tcaChans), tca(tcaRef) {
  sensorCount = addrCount;
  sensors = new VL6180X[addrCount];

  for (int i = 0; i < sensorCount; i++) {
    tca.selectBus(tcaChannels[i]);
    sensors[i].setAddress(addresses[i]);
    sensors[i].init();
    sensors[i].configureDefault();
    sensors[i].setScaling(scale);
    sensors[i].setTimeout(period + 100);

    // Increase max convergence time to provide extra range.
    sensors[i].writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 60);

    sensors[i].stopContinuous();
  }

  // Wait to make sure all sensors are stopped and not reading
  delay(300);
}

VL6180Wrapper::~VL6180Wrapper() { delete[] sensors; }

void VL6180Wrapper::startSensors() {
  for (int i = 0; i < sensorCount; i++) {
    tca.selectBus(tcaChannels[i]);
    sensors[i].startRangeContinuous(100);
  }
  delay(100); // Add stabilization delay
}

void VL6180Wrapper::stopSensors() {
  for (int i = 0; i < sensorCount; i++) {
    tca.selectBus(tcaChannels[i]);
    sensors[i].stopContinuous();
  }

  // Wait to make sure all sensors are stopped and not reading
  delay(300);

  return;
}

void VL6180Wrapper::readSensors(int *reading) {
  for (int i = 0; i < sensorCount; i++) {
    tca.selectBus(tcaChannels[i]);
    if (sensors[i].timeoutOccurred()) {
      reading[i] = -1;
      sensors[i].stopContinuous(); // Reset sensor
      sensors[i].startRangeContinuous(100);
    } else {
      reading[i] = sensors[i].readRangeContinuousMillimeters();
    }
  }
}
