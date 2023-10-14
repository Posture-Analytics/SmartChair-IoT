/*
  Errors.h

  * This module handles the errors that may occur during the execution of the sketch.
  * In case of an error, the RGB built-in LED will change its color to indicate the error type.
  * If the error is fatal, the device will be restarted in 3 seconds, in order to try to
  recover from the error.
*/

#ifndef Errors_H_
#define Errors_H_

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
enum errors {
  none,
  noInternet,
  noDatabaseConnection,
  noNTPdata,
  bufferFull,
  externalADCInitFailure
};

/** Initialize the RGB built-in LED */
void setupLED();

/** Update the LED color according to the current error status */
void showError(errors error, bool fatal = false);

#endif  // Errors_H_
