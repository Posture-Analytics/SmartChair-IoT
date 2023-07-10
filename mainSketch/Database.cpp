
#include <WiFi.h>
#include <time.h>
// Provide the token generation process info for database autentication
#include <addons/TokenHelper.h>

#include "Database.h"
#include "Errors.h"
#include "Network.h"
#include "Buffer.h"

Database::Database() : last_was_valid(true) {}

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
    timeinfo = localtime(&timestampUnix);
    // Create a buffer to store the date string
    char dateBuffer[12];
    // Convert the struct tm to a formatted string like "YYYY-MM-DD/\0"
    strftime(dateBuffer, 12, "%F/", timeinfo);

    // Save the current date path on a string to be compared when the date changes
    datePath = dateBuffer;

    // Sets the upload to the node named with the current date
    fullDataPath += DATABASE_BASE_PATH + datePath;
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
    // The buffers are bigger to avoid overflow and to use a multiple of 2
    /*
    *  Set the node where the data will be stored as a concatenation of the date, UNIX/Epoch Time
    *  and the device's millis timestamp, getting the index where the collect data will be stored.
    */

    // Set the key of the payload as a concatenation of the date, UNIX/Epoch Time
    snprintf(key, 32, "%llu/", data->timestampMillis);

    // Add the pressure sensors data to the payload
    payload = "P";
    for (int i = 0; i < PRESSURE_SENSOR_COUNT; i++) {
        payload += data->pressureSensor[i];
        payload += ";";
    }

    // Add the payload to the JSON buffer
    jsonBuffer.add(key, payload);

    // Increment the jsonSize to keep control of how many data samples are been stored in the JSON buffer
    jsonSize++;
}

// Function that sends the JSON object to the database, update the node asynchronously
bool Database::pushData(){
    #ifdef DEBUG

        // In debug mode, we only print the values instead of sending them to the database
        json.toString(Serial, true);
        return true;
    
    #else

        // If the Firebase Database is ready to receive the data, we send it asynchronously
        // to be faster and to be able the send a larger amount of the data points per second
        if (Firebase.ready()) {
            // Send the data to database
            if(Firebase.updateNodeSilentAsync(fbdo, fullDataPath, jsonBuffer)) {
                // Update the LED indicator, showing that everything works fine
                showError(none);

                // Clear the JSON buffer and reset the counter
                jsonBuffer.clear();
                jsonSize = 0;

                // If the data was sent successfully, we return true
                return true;
            // If some error occur during this process, we show as a fatal database error and restart the device
            } else {
                Serial.println(fbdo.errorReason().c_str());
                showError(noDatabaseConnection);

                return false;
            }
        }

    #endif
}


// Function that track the incoming data and fill the json buffer to be sent to the database
void Database::sendData(SensorDataBuffer *dataBuffer) {
        
    // If we don't have a complete batch of data, we keep adding samples to the JSON buffer
    if (jsonSize < jsonBatchSize) {

        // Check if the date changed
        if ((*dataBuffer).hasDateChanged(datePath)) {
            // We force push the data to the database
            if(pushData()) {
                // If the push data succeed, we get the current date path
                (*dataBuffer).getCurrentSampleDatePath();

                // Update the full data path
                datePath = (*dataBuffer).sampleDate;
                fullDataPath = DATABASE_BASE_PATH + datePath;
            } else {
                // If the push data failed, we show an error
                showError(noDatabaseConnection);

                return;
            }
        }

        // Get one sample from the sensor data buffer
        const sensorData* sample =(*dataBuffer).getSample();

        // Check if the current sample is valid
        bool current_is_valid = !(*dataBuffer).isSampleNull(sample);

        // If the current or the last sample is valid, we send the data to the database
        // If the sample being processed is non-zero, it is always sent to the database
        // Else, it is only sent if the last sample was valid, so that we don't send
        // too many null values to the database in succession
        if (current_is_valid || last_was_valid) {

            // Concatenate the sample in a JSON buffer
            appendDataToJSON(sample);

            // If the JSON buffer is full, send the data to the database
            if (jsonSize >= jsonBatchSize) {
                pushData();
            }
        }

        // Update the last_was_valid variable
        last_was_valid = current_is_valid;

    // Otherwise, we push the data to the database
    } else {
        pushData();
    }

    // Print the buffer state
    (*dataBuffer).printBufferState();

    // Print the indexes of the buffer
    (*dataBuffer).printBufferIndexes();

    // If we just send an amount of data to the database, give an interval to Core 0 to work on maintence activities, avoiding crash problems
    vTaskDelay(10);
    yield();
}
