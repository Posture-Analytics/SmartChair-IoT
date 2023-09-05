#include "MCP3008Wrapper.h"

#define CHIP_COUNT 2

int CSPins[] = {15, 27};
MCP3008Wrapper<CHIP_COUNT> adcs(CSPins);

void setup() {
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  for (int chip = 0; chip < CHIP_COUNT; chip++) {
    for (int channel = 0; channel < 8; channel++) {
      Serial.print(adcs.read(chip, channel)); Serial.print(" ");
    }

    Serial.print("|");
  }

  Serial.println("#");
  delay(500);
}