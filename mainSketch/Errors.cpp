#include "Errors.h"

#include <FastLED.h> // Library to control the built-in RGB LED

// Define constants to set the built-in RGB LED
#define LED_PIN 2  // Pin 2 on Thing Plus C is connected to WS2812 LED
#define COLOR_ORDER GRB  // Type of color scheme
#define CHIPSET WS2812  // Type of the LED

const int NUM_LEDS = 1;  // Number of LEDs
const int BRIGHTNESS = 100;  // LED brightness

// Create a LED array with the RGB built-in LED
CRGB leds[NUM_LEDS];

// Void that initialize the RGB built-in LED
void setupLED() {
    // Setup the built-in RGB LED
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

// Void that updates the LED color according to the current error status
void showError(errors error, bool fatal) {
    const CRGB values[6] = {CRGB::Green, CRGB::Yellow, CRGB::DarkBlue,
                            CRGB::Magenta, CRGB::Red, CRGB::Aqua};
    // Set the LED color, according to error enumeration
    leds[0] = values[error];

    // Update the LED color
    FastLED.show();

    // If the error was fatal, wait for 3 seconds and restart the device
    if (fatal) {
        Serial.println("Reiniciando o dispositivo em 3 segundos...");
        delay(3000);
        ESP.restart();
    }
}
