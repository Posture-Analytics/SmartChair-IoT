/* Debug.h

    * This module handle the debug messages that may occur during the execution of the sketch.
    * It also add or remove sensors from the list of sensors to be read, turn on or off funcionalities, etc.

    TODO:
    * The debug messages are sent to the serial port, but it defines an routine to send logs to the database as well.

*/

#ifndef Debug_H_
#define Debug_H_

#define DEBUG_LEVEL_NONE                0 // No messages
#define DEBUG_LEVEL_FATAL               1 // Fatal errors
#define DEBUG_LEVEL_ERROR               2 // All errors
#define DEBUG_LEVEL_WARNING             3 // Errors and warnings
#define DEBUG_LEVEL_INFO                4 // Errors, warnings and info
#define DEBUG_LEVEL_DEBUG               5 // Errors, warnings, info and debug
#define DEBUG_LEVEL_TRACE               6 // Errors, warnings, info, debug and trace (step by step execution)
#define DEBUG_LEVEL_VERBOSE             7 // All

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

// Set the status of the funcionalities (ENABLE_SENSOR or DISABLE_SENSOR)
#define WIFI_STATUS                     ENABLE
#define NTP_STATUS                      ENABLE
#define DATABASE_STATUS                 ENABLE

const char *debugLevelLabels[] = {
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

#define LOG_LEVEL(level, format, ...)          \
    if (level <= DEBUG_LEVEL)                  \
    {                                          \
        Serial.print(debugLevelLabels[level]); \
        Serial.print(": ");                    \
        Serial.print(format, ##__VA_ARGS__);   \
    }

#define LOG_LEVEL_LN(level, format, ...)       \
    if (level <= DEBUG_LEVEL)                  \
    {                                          \
        Serial.print(debugLevelLabels[level]); \
        Serial.print(": ");                    \
        Serial.print(format, ##__VA_ARGS__);   \
        Serial.println();                      \
    }

#define LogFatal(format, ...) LOG_LEVEL(DEBUG_LEVEL_FATAL, format, ##__VA_ARGS__)
#define LogFatalLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_FATAL, format, ##__VA_ARGS__)

#define LogError(format, ...) LOG_LEVEL(DEBUG_LEVEL_ERROR, format, ##__VA_ARGS__)
#define LogErrorLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_ERROR, format, ##__VA_ARGS__)

#define LogWarning(format, ...) LOG_LEVEL(DEBUG_LEVEL_WARNING, format, ##__VA_ARGS__)
#define LogWarningLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_WARNING, format, ##__VA_ARGS__)

#define LogInfo(format, ...) LOG_LEVEL(DEBUG_LEVEL_INFO, format, ##__VA_ARGS__)
#define LogInfoLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_INFO, format, ##__VA_ARGS__)

#define LogDebug(format, ...) LOG_LEVEL(DEBUG_LEVEL_DEBUG, format, ##__VA_ARGS__)
#define LogDebugLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_DEBUG, format, ##__VA_ARGS__)

#define LogTrace(format, ...) LOG_LEVEL(DEBUG_LEVEL_TRACE, format, ##__VA_ARGS__)
#define LogTraceLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_TRACE, format, ##__VA_ARGS__)

#define LogVerbose(format, ...) LOG_LEVEL(DEBUG_LEVEL_VERBOSE, format, ##__VA_ARGS__)
#define LogVerboseLn(format, ...) LOG_LEVEL_LN(DEBUG_LEVEL_VERBOSE, format, ##__VA_ARGS__)

#else

#define DEBUG_ENABLED 0

#define LogFatal(format, ...)
#define LogFatalLn(format, ...)

#define LogError(format, ...)
#define LogErrorLn(format, ...)

#define LogWarning(format, ...)
#define LogWarningLn(format, ...)

#define LogInfo(format, ...)
#define LogInfoLn(format, ...)

#define LogDebug(format, ...)
#define LogDebugLn(format, ...)

#define LogTrace(format, ...)
#define LogTraceLn(format, ...)

#define LogVerbose(format, ...)
#define LogVerboseLn(format, ...)

#endif // DEBUG_LEVEL

#endif // Debug_H_
