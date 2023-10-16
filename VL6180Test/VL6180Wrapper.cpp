#include "VL6180Wrapper.h"
#include <VL6180X.h>


VL6180Wrapper::VL6180Wrapper(int addresses[], int addrCount, int prd, int scl): period(prd), scale(scl) {
    sensorCount = addrCount;
    sensors = new VL6180X[addrCount];

    for (int i = 0; i < sensorCount; i++) {
        sensors[i].setAddress(addresses[i]);
        sensors[i].init();
        sensors[i].configureDefault();
        sensors[i].setScaling(scale);
        sensors[i].setTimeout(period+100);

        // Increase max convergence time to provide extra range.
        sensors[i].writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 60);

        sensors[i].stopContinuous();
    }

    // Wait to make sure all sensors are stopped and not reading
    delay(300);
}

VL6180Wrapper::~VL6180Wrapper() {
    delete[] sensors;
}

void VL6180Wrapper::startSensors() {
    for (int i = 0; i < sensorCount; i++) {
        sensors[i].startRangeContinuous(100);
    }

    return;
}

void VL6180Wrapper::stopSensors() {
    for (int i = 0; i < sensorCount; i++) {
        sensors[i].stopContinuous();
    }

    // Wait to make sure all sensors are stopped and not reading
    delay(300);

    return;
}

void VL6180Wrapper::readSensors(int* reading) {
    for (int i = 0; i < sensorCount; i++) {
        reading[i] = sensors[i].readRangeContinuousMillimeters();
        //reading[i] = sensors[i].getScaling();
    }

    return;
}
