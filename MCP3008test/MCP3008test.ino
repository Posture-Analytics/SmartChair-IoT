#include "MCP3008Wrapper.h"

int CSPins[] = {15};
int CSPCount = sizeof(CSPins)/sizeof(*CSPins);

MCP3008Wrapper adcs(CSPins, CSPCount);

void setup() {
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  for (int chip = 0; chip < CSPCount; chip++) {
    for (int channel = 0; channel < CHANNEL_COUNT; channel++) {
      Serial.print(adcs.read(chip, channel)); Serial.print(" ");
    }

    Serial.print("|");
  }

  Serial.println("#");
  delay(500);

  int readings[CHANNEL_COUNT];
  for (int chip = 0; chip < CSPCount; chip++) {
    adcs.read(chip, readings);

    for (int channel = 0; channel < CHANNEL_COUNT; channel++) {
      Serial.print(readings[channel]); Serial.print(" ");
    }

    Serial.print("|");
  }

  Serial.println("||");
  delay(500);
}
