/* Debug.h

    * This module handles the debug messages that may occur during the execution of the sketch.

    TODO:
    * (Only apply) It also adds or removes sensors from the list of sensors to be read, turn on or off functionalities, etc.
    * The debug messages are sent to the serial port, but it defines a routine to send logs to the database as well.

*/

#ifndef Debug_H_
#define Debug_H_

#include <Arduino.h>

#define DEBUG_LEVEL_NONE                0 // No messages
#define DEBUG_LEVEL_FATAL               1 // Fatal errors
#define DEBUG_LEVEL_ERROR               2 // All errors
#define DEBUG_LEVEL_WARNING             3 // Errors and warnings
#define DEBUG_LEVEL_INFO                4 // Errors, warnings, and info
#define DEBUG_LEVEL_DEBUG               5 // Errors, warnings, info, and debug
#define DEBUG_LEVEL_VERBOSE             7 // All (Errors, warnings, info, debug, and trace (step by step execution))

#define DISABLE                         0 // Disable the sensor/functionality
#define ENABLE                          1 // Enable the sensor/functionality
#define MOCK_SENSOR                     2 // Mock the sensor

// Set the debug level
#define DEBUG_LEVEL                     DEBUG_LEVEL_VERBOSE

// Set the status of the sensors
#define PRESSURE_SENSOR_STATUS          ENABLE
#define DISTANCE_SENSOR_STATUS          DISABLE
#define DISTANCE_MATRIX_SENSOR_STATUS   DISABLE
#define IMU_SENSOR_STATUS               DISABLE

// Set the status of the functionalities (ENABLE_SENSOR or DISABLE_SENSOR)
#define WIFI_STATUS                     ENABLE
#define NTP_STATUS                      ENABLE
#define DATABASE_STATUS                 ENABLE

static const char *debugLevelLabels[] = {
    "",
    "FATAL",
    "ERROR",
    "WARNING",
    "INFO",
    "DEBUG",
    "TRACE",
    "VERBOSE"
};

#if defined(DEBUG_LEVEL) && DEBUG_LEVEL > DEBUG_LEVEL_NONE

#define DEBUG_ENABLED 1

template <typename T>
void logArgs(const T& t) {
    Serial.print(t);
}

template <typename T, typename... Args>
void logArgs(const T& t, const Args&... args) {
    Serial.print(t);
    logArgs(args...);
}

#define LOG_LEVEL(level, ...)                   \
    if (level <= DEBUG_LEVEL)                   \
    {                                           \
        Serial.print(debugLevelLabels[level]);  \
        Serial.print(": ");                     \
        logArgs(__VA_ARGS__);                   \
    }

#define LOG_LEVEL_LN(level, ...)                \
    if (level <= DEBUG_LEVEL)                   \
    {                                           \
        Serial.print(debugLevelLabels[level]);  \
        Serial.print(": ");                     \
        logArgs(__VA_ARGS__);                   \
        Serial.println();                       \
    }

#define LogFatal(...) LOG_LEVEL(DEBUG_LEVEL_FATAL, __VA_ARGS__)
#define LogFatalln(...) LOG_LEVEL_LN(DEBUG_LEVEL_FATAL, __VA_ARGS__)

#define LogError(...) LOG_LEVEL(DEBUG_LEVEL_ERROR, __VA_ARGS__)
#define LogErrorln(...) LOG_LEVEL_LN(DEBUG_LEVEL_ERROR, __VA_ARGS__)

#define LogWarning(...) LOG_LEVEL(DEBUG_LEVEL_WARNING, __VA_ARGS__)
#define LogWarningln(...) LOG_LEVEL_LN(DEBUG_LEVEL_WARNING, __VA_ARGS__)

#define LogInfo(...) LOG_LEVEL(DEBUG_LEVEL_INFO, __VA_ARGS__)
#define LogInfoln(...) LOG_LEVEL_LN(DEBUG_LEVEL_INFO, __VA_ARGS__)

#define LogDebug(...) LOG_LEVEL(DEBUG_LEVEL_DEBUG, __VA_ARGS__)
#define LogDebugln(...) LOG_LEVEL_LN(DEBUG_LEVEL_DEBUG, __VA_ARGS__)

#define LogVerbose(...) LOG_LEVEL(DEBUG_LEVEL_VERBOSE, __VA_ARGS__)
#define LogVerboseln(...) LOG_LEVEL_LN(DEBUG_LEVEL_VERBOSE, __VA_ARGS__)

#else

#define DEBUG_ENABLED 0

#define LogFatal(...)
#define LogFatalln(...)

#define LogError(...)
#define LogErrorln(...)

#define LogWarning(...)
#define LogWarningln(...)

#define LogInfo(...)
#define LogInfoln(...)

#define LogDebug(...)
#define LogDebugln(...)

#define LogVerbose(...)
#define LogVerboseln(...)

#endif // DEBUG_LEVEL

#endif // Debug_H_
