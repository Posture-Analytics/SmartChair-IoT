#ifndef TCA9548A_H
#define TCA9548A_H

#include <Wire.h>

class TCA9548A {
public:
    TCA9548A(uint8_t address = 0x70) : _address(address) {}
    
    void selectBus(uint8_t bus) {
        if (bus > 7) return;  // Only channels 0-7 are valid
        Wire.beginTransmission(_address);
        Wire.write(1 << bus);  // Convert channel number to bitmask
        Wire.endTransmission();
        delay(10);  // Small delay for stability
    }
    
private:
    uint8_t _address;
};

// Create a global instance that can be used throughout the program
extern TCA9548A tca;

#endif // TCA9548A_H