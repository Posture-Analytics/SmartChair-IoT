
#include <WiFi.h>
#include <time.h>
// Provide the token generation process info for database autentication
#include <addons/TokenHelper.h>

#include "Database.h"
#include "Errors.h"
#include "Network.h"
#include "Buffer.h"

Database::Database() : last_was_valid(true) {}

// Update the current time variable
void Database::updateCurrentTime() {
    // Set the variable 'currentMicros' with the current time in microseconds (us)
    currentMicros = micros();
}

// Function that setup the database connection
void Database::setup(time_t timestampUnix) {
    // Assign the api key (required) of the database
    config.api_key = DATABASE_API_KEY;

    // Assign the RTDB URL (required)
    config.database_url = DATABASE_URL;

    // Assign the user sign in credentials
    auth.user.email = DATABASE_USER_EMAIL;
    auth.user.password = DATABASE_USER_PASSWORD;

    // Assign the callback function for the long running token generation task
    config.token_status_callback = tokenStatusCallback; // see addons/TokenHelper.h

    // Authenticate and initialize the communication with the Firebase database
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    // Convert the timestamp to a struct tm
    struct tm* timeInfo = localtime(&timestampUnix);
    // Create a buffer to store the date string
    char dateBuffer[12];
    // Convert the struct tm to a formatted string like "YYYY-MM-DD/\0"
    strftime(dateBuffer, 12, "%F/", timeInfo);

    // Sets the upload to the node named with the current date
    fullDataPath = DATABASE_BASE_PATH + dateBuffer;
}

// Function that logs the device's boot. Useful to analyze crashes, stability, reboots...
void Database::bootLog() {
    // If the Firebase Database is ready to receive the data, we record the timestamp of the device's boot
    if (Firebase.ready()) {
        // Record the current timestamp string to the database
        if (Firebase.pushInt(fbdo, "/bootLog/", getCurrentMillisTimestamp())) {
            Serial.println("Inicialização registrada com sucesso!");
        // If some error occur during this record, we show as a fatal database error and restart the device
        } else {
            Serial.println("Ocorreu um erro ao registrar a inicialização:");
            Serial.println(fbdo.errorReason().c_str());
            showError(noDatabaseConnection, true);
        }
    // If the Firebase Database is not ready, we try to reconnect to the network and to the database
    } else {
        if (WiFi.status() != WL_CONNECTED) {
            showError(noInternet, true);
            Serial.println("Reconectando à rede...");
        } else {
            showError(noDatabaseConnection, true);
            Serial.println("Reconectando ao banco de dados...");
        }
    }
}

// Funcion that append sensor data into the JSON object
void Database::appendDataToJSON(const sensorData* data) {
    payload.clear();
    // Add the pressure sensors' data to the payload
    for (int i = 0; i < PRESSURE_SENSOR_COUNT; i++) {
        payload.add(data->pressureSensor[i]);
    }

    // Set the node where the data will be stored as a the date, with a milliseconds subkey.
    jsonBuffer.add(data->timestampMillis, payload);

    // Increment the jsonSize to keep control of how many data samples are been stored in the JSON buffer
    jsonSize++;
}

// Function that sends the JSON object to the database, update the node asynchronously
bool Database::pushData() {
    #ifdef DEBUG

        // In debug mode, we only print the values instead of sending them to the database
        json.toString(Serial, true);
        return true;

    #else

        // If the Firebase Database is ready to receive the data, we send it asynchronously
        // to be faster and to be able the send a larger amount of the data points per second
        if (Firebase.ready()) {
            // Send the data to database
            if (Firebase.updateNodeSilentAsync(fbdo, fullDataPath, jsonBuffer)) {
                // Update the LED indicator, showing that everything works fine
                showError(none);

                // Clear the JSON buffer and reset the counter
                jsonBuffer.clear();
                jsonSize = 0;

                // If the data was sent successfully, we return true
                return true;
            // If some error occur during this process, we show as a fatal database error and restart the device
            } else {
                Serial.print("Database error: ");
                Serial.println(fbdo.errorReason());
                Serial.println(fullDataPath);
                Serial.println(jsonBuffer.serializedBufferLength());
                showError(noDatabaseConnection);

                return false;
            }
        } else {
            return false;
        }

    #endif
}


// Function that track the incoming data and fill the json buffer to be sent to the database
void Database::sendData(SensorDataBuffer* dataBuffer) {
    // Save the time when the device start to send the data from the sensors, to keep control of the intervals of data sending
    updateCurrentTime();

    // If the time elapsed since the last data sending is greater than the interval between data sending (or if it's the first sending), send the data
    if (currentMicros - dataPrevSendingMicros > dataSendIntervalMicros || dataPrevSendingMicros == 0) {
        
        // Send the data to the database
        pushData();

        // Check if the date changed
        if (dataBuffer->hasDateChanged()) {
            // If the push data succeed, we update the current path with the new date
            fullDataPath = DATABASE_BASE_PATH + dataBuffer->sampleDate;
        }

        // Update the time variable that controls the send interval
        dataPrevSendingMicros = currentMicros;
    }
    else {
        return;
    }

    // Print the buffer state
    dataBuffer->printBufferState();

    // Print the size of the JSON buffer
    Serial.print(jsonSize);

    // Print the indexes of the buffer
    dataBuffer->printBufferIndexes();

    // If we just send an amount of data to the database, give an interval to Core 0 to work on maintence activities, avoiding crash problems
    vTaskDelay(10);
    yield();
}
