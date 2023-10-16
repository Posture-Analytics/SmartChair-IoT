#include "Errors.h"

#include <FastLED.h>

// Define constants to set the built-in RGB LED
#define LED_PIN 2
#define COLOR_ORDER GRB
#define CHIPSET WS2812
#define NUM_LEDS 1
const int DEFAULT_BRIGHTNESS = 10;

Errors::Errors() {
    brightnessLevel = DEFAULT_BRIGHTNESS;

    // Setup the built-in RGB LED
    FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(brightnessLevel);
}

void Errors::showError(ErrorType error, bool fatal) {
    // Pick the color releated to the error
    leds[0] = errorColors[static_cast<int>(error)];

    // If the error is fatal, increase the brightness to the maximum
    if (fatal) updateBrightness(255);
    else updateBrightness(DEFAULT_BRIGHTNESS);

    FastLED.show();

    // If the error is fatal, restart the device after 3 seconds
    if (fatal) {
        Serial.println("Reiniciando o dispositivo em 3 segundos...");
        delay(3000);
        ESP.restart();
    }
}

void Errors::updateBrightness(uint8_t brightness) {
    brightnessLevel = brightness;
    FastLED.setBrightness(brightnessLevel);
}
