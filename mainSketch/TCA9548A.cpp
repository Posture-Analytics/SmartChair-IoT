#include "TCA9548A.h"

// Define Global tca instance
TCA9548A tca;

// Constructor implementation (even if empty)
TCA9548A::TCA9548A() {
    // Initialize I2C here if needed
}

// Method to select TCA bus
void TCA9548A::selectBus(uint8_t bus) {
  if (bus > 7) return;
  Wire.beginTransmission(TCA9548A_ADDR);
  Wire.write(1 << bus);
  Wire.endTransmission();
  delay(5); // for stability
}