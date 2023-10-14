/*
	Errors.h

	* This module handle the errors that may occur during the execution of the sketch.
	* In case of an error, the RGB built-in LED will change its color to indicate the error type.
	* If the error is fatal, the device will be restarted in 3 seconds, in order to try to recover from the error.
*/

#ifndef Errors_H_
#define Errors_H_

#include <FastLED.h>

/**
 * Enumerate the erros that will be tracked by the LED colors
 * 
 * none: No error (Green)
 * noInternet: No internet connection (Yellow)
 * noDatabaseConnection: No database connection (Dark Blue)
 * noNTPdata: NTP Sync failed (Magenta)
 * bufferFull: Buffer full (Red)
 * externalADCInitFailure: External ADCs initialization failure (Aqua)
*/
enum class ErrorType {
	None,
	NoInternet,
	NoDatabaseConnection,
	NoNTPdata,
	BufferFull,
	ExternalADCInitFailure
};

class Errors {
public:
	/** Initialize the RGB built-in LED */
	Errors();

	/** Update the LED color according to the current error status */
	void showError(ErrorType error, bool fatal = false);

	/** Update the LED brightness */
	void updateBrightness(uint8_t brightness);

private:
	// Instance of the built-in RGB LED
	CRGBArray<1> leds;

	// Array of colors related to each error
	const CRGB errorColors[6] = {
		CRGB::Green, CRGB::Yellow, CRGB::DarkBlue,
		CRGB::Magenta, CRGB::Red, CRGB::Aqua
	};

	// Current brightness level
	uint8_t brightnessLevel;
};

// Declare the extern instance of the Errors class
extern Errors errorHandler;

#endif  // Errors_H_
