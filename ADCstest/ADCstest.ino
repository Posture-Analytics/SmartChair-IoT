#include "ADCs.h"

ADCs adcs;

void setup() {
  Serial.begin(115200);

  // Initialize ADCs with default CS pins
  if (!adcs.setup()) {
    Serial.println("Error initializing MCP3008 ADCs!");
    while (true); // Halt if there's an initialization error
  }
}

void loop() {
  // Read each channel of each MCP3008 chip and print the values
  for (int chip = 0; chip < ADCS_COUNT; chip++) {
    for (int channel = 0; channel < ADCS_CHANNEL_COUNT; channel++) {
      int value = adcs.read(chip, channel);
      if (value >= 0) {  // Check if read was successful
        Serial.print(value); Serial.print(" ");
      } else {
        Serial.print("E ");  // Print an error indicator
      }
    }
    Serial.print("|");
  }
  Serial.println("#");
  delay(500);

  // Read all channels of each MCP3008 chip and print the values
  int readings[ADCS_COUNT * ADCS_CHANNEL_COUNT];
  if (adcs.readAll(readings)) {
    for (int i = 0; i < ADCS_COUNT * ADCS_CHANNEL_COUNT; i++) {
      Serial.print(readings[i]); Serial.print(" ");
      if ((i + 1) % ADCS_CHANNEL_COUNT == 0) {  // Check if end of chip's channels
        Serial.print("|");
      }
    }
    Serial.println("||");
    delay(500);
  } else {
    Serial.println("Error reading values from ADCs.");
    delay(1000);
  }
}
