#ifndef TCA9548A_H
#define TCA9548A_H

#include <Wire.h>

/**
 * @class TCA9548A
 * @brief A class to interface with the TCA9548A I2C multiplexer.
 *
 * The TCA9548A is an I2C multiplexer that allows multiple I2C devices with the same address
 * to be connected to a single I2C bus by switching between different channels.
 */
class TCA9548A {
public:
    /**
     * @brief Constructor for the TCA9548A class.
     * @param address The I2C address of the TCA9548A device. Default is 0x70.
     */
    TCA9548A(uint8_t address = 0x70) : _address(address) {}
    
    /**
     * @brief Selects a specific I2C bus channel on the TCA9548A.
     * 
     * This function enables communication with a specific channel on the TCA9548A
     * by writing a bitmask to the device. Only one channel can be active at a time.
     * 
     * @param bus The channel number to select (0-7). If the channel number is greater
     *            than 7, the function will return without making any changes.
     */
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