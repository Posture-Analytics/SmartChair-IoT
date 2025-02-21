#include "VL53L4CD.h"
#include <SparkFun_VL53L1X.h>

#define TCA9548A_ADDR 0x70

void TCA9548A(uint8_t bus) {
  if (bus > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << bus);
  Wire.endTransmission();
  delay(1); // Critical for TCA stability
}

VL53L4CDWrapper::VL53L4CDWrapper(const int addresses[], int addrCount, const int tcaChans[]) 
  : sensorCount(addrCount), 
    tcaChannels(tcaChans) // Now matches const-ness
{
  sensors = new SFEVL53L1X[sensorCount];
  delay(100);

  for (int i = 0; i < sensorCount; i++) {
    TCA9548A(tcaChannels[i]); // Select TCA channel first!
    delay(200); // Increased power-up delay

    sensors[i].setI2CAddress(addresses[i]);
    if (sensors[i].begin()) {
      sensors[i].setTimingBudgetInMs(200); // From 50ms to 200ms
      sensors[i].setIntermeasurementPeriod(250); // Must be ≥ timing budget
    }
    delay(50);
  }
}

VL53L4CDWrapper::~VL53L4CDWrapper() {
  delete[] sensors;
}

void VL53L4CDWrapper::startSensors() {
  for (int i = 0; i < sensorCount; i++) {
    TCA9548A(tcaChannels[i]); // Select the correct TCA channel
    sensors[i].startRanging();
    delay(50); // Optional short delay for stability
  }
}

void VL53L4CDWrapper::stopSensors() {
  for (int i = 0; i < sensorCount; i++) {
    sensors[i].stopRanging();
  }

  return;
}

void VL53L4CDWrapper::readSensors(int readings[]) {
  for (int i = 0; i < sensorCount; i++) {
    TCA9548A(tcaChannels[i]);
    // delay(100);
    
    bool dataReady = sensors[i].checkForDataReady();
    // delay(50);
    
    if (dataReady) {
      readings[i] = sensors[i].getDistance();
      sensors[i].clearInterrupt();
    } else {
      readings[i] = -1;
    }
  }
}