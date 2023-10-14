#include <WiFi.h>

#include "Network.h"
#include "Errors.h"

void setupWiFi() {
    // Connect to the WiFi network
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to Wi-Fi");

    // If the connection fails, start a loop, retrying every 0.5 seconds
    while (WiFi.status() != WL_CONNECTED) {
        errorHandler.showError(ErrorType::NoInternet);
        Serial.print(".");
        delay(500);
    }

    // If the connection works, print the IP of the ESP32 on the local network
    Serial.print("\n\nConnected with IP: ");
    Serial.println(WiFi.localIP());
}

void syncWithNTPTime() {
    // Set the time obtained from the NTP Server
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    // Print the time obtained from the NTP Server
    printLocalTime();
}

void printLocalTime() {
    struct tm timeInfo;

    // If the local time is not available, we have a fatal error
    if (!getLocalTime(&timeInfo)) {
        errorHandler.showError(ErrorType::NoNTPdata, true);
        Serial.println("Failed to obtain time");
        return;
    }

    Serial.println(&timeInfo, "%A, %d/%m/%Y %H:%M:%S");
    Serial.println();
}

time_t getCurrentTime() {
    time_t now;
    struct tm timeInfo;

    // If the local time is not available, we have a fatal error
    if (!getLocalTime(&timeInfo)) {
        errorHandler.showError(ErrorType::NoNTPdata, true);
        Serial.println("Failed to obtain Unix time");
        return (0);
    }

    // Return the current time
    time(&now);
    return now;
}

// Function that returns the current timestamp in milliseconds since 01 January 1970
unsigned long long getCurrentMillisTimestamp() {
    struct timeval tv;

    // Fill the timeval struct with the current time (tv_sec and tv_usec)
    gettimeofday(&tv, NULL);

    // Return the current timestamp in milliseconds
    return tv.tv_sec * 1000LL + tv.tv_usec / 1000LL;
}
