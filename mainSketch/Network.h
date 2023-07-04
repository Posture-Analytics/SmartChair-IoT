#ifndef Network_H_
#define Network_H_

#include <time.h>

// Define the WiFi credentials (https://youtu.be/pDDdA9qEFwY)
static const char* WIFI_SSID = "Cadeirudo";
static const char* WIFI_PASSWORD = "cadeirinha123*#";
// #define WIFI_SSID "Cadeirudo"
// #define WIFI_PASSWORD "cadeirinha123*#"

// Define constants for the connection with the NTP Server, in order to get the current time
static const char* ntpServer = "pool.ntp.org";  // NTP Server address
static const long gmtOffset_sec = -10800;  // GMT Offset in seconds (-3 hours)
static const int daylightOffset_sec = 0;  // Daylight Offset in seconds (0)
// #define ntpServer "pool.ntp.org"
// #define gmtOffset_sec -10800
// #define daylightOffset_sec 0

void setupWiFi();

void syncWithNTPTime();

void printLocalTime();

time_t getCurrentTime();

unsigned long long getCurrentMillisTimestamp();

#endif  // Network_H_
