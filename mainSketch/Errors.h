#ifndef Errors_H_
#define Errors_H_

// Enumerate the erros that will be tracked by the LED colors
enum errors {
  none, // No error (Green)
  noInternet, // No internet connection (Yellow)
  noDatabaseConnection, // No database connection (Dark Blue)
  noNTPdata, // NTP Sync failed (Magenta)
  bufferFull, // Buffer full (Red)
  externalADCInitFailure // External ADCs initialization failure (Aqua)
};

// Void that initialize the RGB built-in LED
void setupLED();

// Void that updates the LED color according to the current error status
void showError(errors error, bool fatal = false);

#endif  // Errors_H_
