#include "VL53L4CD.h"
#include <SparkFun_VL53L1X.h>

#define BUDGET_PERIOD 50
#define PERIOD 50

VL53L4CDWrapper::VL53L4CDWrapper(int addresses[], int addrCount) {
    sensorCount = addrCount;
    sensors = new VL53L4CD[addrCount];

    for (int i = 0; i < sensorCount; i++) {
        sensors[i].setI2CAddress(addresses[i]);
        sensors[i].begin();
        sensors[i].setTimingBudgetInMs(BUDGET_PERIOD);
        sensors[i].setIntermeasurementPeriod(PERIOD);
        // Throw an error here if one of them didn't start correctly
    }
}

VL53L4CDWrapper::~VL53L4CDWrapper() {
    delete[] sensors;
}

void VL53L4CDWrapper::startSensors() {
    for (int i = 0; i < sensorCount; i++) {
        sensors[i].startRanging();
    }

    return;
}

void VL53L4CDWrapper::stopSensors() {
    for (int i = 0; i < sensorCount; i++) {
        sensors[i].stopRanging();
    }

    return;
}

void VL53L4CDWrapper::readSensors(int readings[]) {
    for (int i = 0; i < sensorCount; i++) {
        readings[i] = sensors[i].getDistance();
        sensors[i].clearInterrupt()
    }

    return;
}
