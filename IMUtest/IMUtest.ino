#include <Wire.h> // Include the Wire library for I2C communication
#include "IMU.h"  // Include the IMU module

// Define the I2C address of the IMU
#define IMU_ADDRESS 0x69 // Use 0x68 or 0x69 based on the IMU's AD0 pin configuration

// Create an instance of the IMU class
IMU imu(IMU_ADDRESS);

void setup() {
	Serial.begin(115200);

	Wire.begin(); // Initialize the I2C bus

	// Initialize the IMU
	if (imu.init()) {
		Serial.println("IMU initialized successfully!");
	} else {
		Serial.println("Failed to initialize IMU.");
		while (1); // Loop indefinitely if initialization fails
	}
}

void loop() {

	// Ask for a new sample from the IMU
	imu.collectSample();

	// Attempt to read a sample from the IMU with a timeout of 100 milliseconds
	if (imu.readSample()) {
		// Print the Euler angles
		Serial.print("Roll: ");
		Serial.print(imu.roll);
		Serial.print(" degrees, Pitch: ");
		Serial.print(imu.pitch);
		Serial.print(" degrees, Yaw: ");
		Serial.print(imu.yaw);
		Serial.println(" degrees");
	} else {
		Serial.println("Failed to read IMU sample.");
	}

	// Delay for a short interval
	delay(100); // Adjust the delay as needed
}
