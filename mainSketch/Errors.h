#ifndef Errors_H_
#define Errors_H_

// Enumerate the erros that will be tracked by the LED colors
enum errors {
  none,
  noInternet,
  noDatabaseConnection,
  noNTPdata,
  bufferFull,
  externalADCInitFailure
};

void setupLED();

void showError(errors error, bool fatal = false);

#endif  // Errors_H_
