#ifndef TCA9548A_H_
#define TCA9548A_H_

#include <Wire.h>

#define TCA9548A_ADDR 0x70

class TCA9548A {
public: 
    TCA9548A(); // Constructor declaration
    void selectBus(uint8_t bus); // Method to switch TCA channels
};

extern TCA9548A tca; // Declare global instance

#endif // TCA9548A_H_