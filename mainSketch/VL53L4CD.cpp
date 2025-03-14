#include "VL53L4CD.h"
#include <SparkFun_VL53L1X.h>
#include "TCA9548A.h"

VL53L4CDWrapper::VL53L4CDWrapper(const int addresses[], int addrCount, const int* tcaChans) 
  : sensorCount(addrCount), 
    tcaChannels(tcaChans) // Now matches const-ness
{
  sensors = new SFEVL53L1X[sensorCount];
  delay(100);

  for (int i = 0; i < sensorCount; i++) {
  tca.selectBus(tcaChannels[i]);
  delay(200);

  sensors[i].setI2CAddress(addresses[i]);

  // Corrected: Check for SUCCESS (0)
  if (sensors[i].begin() == 0) { 
    sensors[i].setTimingBudgetInMs(500);
    sensors[i].setIntermeasurementPeriod(550);
  } /* else {
    Serial.print("Sensor ");
    Serial.print(i);
    Serial.println(" failed to initialize!");
  } */
  delay(50);
  }
}

VL53L4CDWrapper::~VL53L4CDWrapper() {
  delete[] sensors;
}

void VL53L4CDWrapper::startSensors() {
  for (int i = 0; i < sensorCount; i++) {
    tca.selectBus(tcaChannels[i]); // Select the correct TCA channel
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
    tca.selectBus(tcaChannels[i]);
    
    // Add data-ready waiting with timeout (like your test code)
    sensors[i].startRanging();
    unsigned long start = millis();
    while (!sensors[i].checkForDataReady() && (millis() - start < 500)) {
      delay(5);
    }

    if (sensors[i].checkForDataReady()) {
      readings[i] = sensors[i].getDistance();
      sensors[i].clearInterrupt();
    } else {
      readings[i] = -1; // Timeout
    }
    sensors[i].stopRanging();
  }
}