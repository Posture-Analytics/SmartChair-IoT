#ifndef Network_H_
#define Network_H_

#include <time.h>

// Define the WiFi credentials (https://youtu.be/pDDdA9qEFwY)
static const char* WIFI_SSID = "Cadeirudo";
static const char* WIFI_PASSWORD = "cadeirinha123*#";

// Define constants for the connection with the NTP Server, in order to get the current time
static const char* ntpServer = "pool.ntp.org";  // NTP Server address
static const long gmtOffset_sec = -10800;  // GMT Offset in seconds (-3 hours)
static const int daylightOffset_sec = 0;  // Daylight Offset in seconds (0)

// Setup the WiFi connection
void setupWiFi();

// Sync the device's time with an NTP Server time
void syncWithNTPTime();

// Prints the time obtained from the NTP server during the initialization
void printLocalTime();

// Function that gets current epoch time
time_t getCurrentTime();

// Function that return the current timestamp in milliseconds since 01 January 1970
unsigned long long getCurrentMillisTimestamp();

#endif  // Network_H_
