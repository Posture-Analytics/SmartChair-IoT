#include "VL6180Wrapper.h"
#include <Wire.h>

#define READING_PERIOD 100
#define DISTANCE_SCALE 3

int sensorAddrs[] = {0x29};
int sensorCount = sizeof(sensorAddrs)/sizeof(*sensorAddrs);

VL6180Wrapper distSensors(sensorAddrs, sensorCount, READING_PERIOD, DISTANCE_SCALE);

void setup() {
    Serial.begin(9600);
    while (!Serial);

    Wire.begin();

    distSensors.startSensors();
}

void loop() {
    int readings[sensorCount];

    distSensors.readSensors(readings);

    for (int i = 0; i < sensorCount; i++) {
        Serial.print(readings[i]); Serial.print(" ");
    }
    Serial.println("|");
}
