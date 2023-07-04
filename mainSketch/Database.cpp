
#include <WiFi.h>
#include <time.h>
// Provide the token generation process info for database autentication
#include <addons/TokenHelper.h>

#include "DataReader.h"
#include "Database.h"
#include "Errors.h"

Database::Database() : last_was_valid(true), dataPath("/sensor_readings_test/") {}

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

    // Sets the upload to the node named with the current date
    dataPath += dateBuffer;
}

// Function that logs the device's boot. Useful to analyze crashes, stability, reboots...
void Database::bootLog() {
    // If the Firebase Database is ready to receive the data, we record the timestamp of the device's boot
    if (Firebase.ready()) {
        // Record the current timestamp string to the database
        if (Firebase.pushInt(fbdo, "/bootLog/", getCurrentTime())) {
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

// Function to check if the sample is valid (if it has at least one non-zero value)
bool checkSample(const sensorData* sample) {
    // Check if the sum of the sample values is greater than zero
    if (sample->sum > 0) {
        return true;
    }else{
        return false;
    }
}

// Funcion that append sensor data into the JSON object
void Database::appendDataToJSON(const sensorData* data) {
    // Key: 14 characters for the initial timestamp, 3 for the key (p00, for example) and 1 for the terminator
    // The buffers are bigger to avoid overflow and to use a multiple of 2
    /*
    *  Set the node where the data will be stored as a concatenation of the date, UNIX/Epoch Time
    *  and the device's millis timestamp, getting the index where the pressure key will be stored.
    *  Note: the database doesn't seem to accept a key with a '.' in it, so the split will remain a '_'.
    */
    
    index = 0;
    index += snprintf(key, 32, "%lu_%03d/", data->timestampUnix, data->timestampMillis % 1000);

    // Iterate over each sensor data, adding it into the node with an identification index
    for (int i = 0; i < data->sensorCount; i++) {
        // Adds the datapoint to the string after the timestamp with a padding of '0'
        snprintf(key + index, 4, "p%02d", i);
        jsonBuffer.add(key, data->pressure[i]);
    }

    jsonBuffer.add("timestampUnix", data->timestampUnix);

    // Increment the jsonSize to keep control of how many data samples are been stored in the JSON buffer
    jsonSize++;
}

// Function that sends the JSON object to the database, update the node asynchronously
void Database::sendData(DataReader& dataReader) {
        
    // If the Firebase Database is ready to receive the data, we send it asynchronously
    // to be faster and to be able the send a larger amount of the data points per second
    if (Firebase.ready()) {

        // Get one sample from the sensor data buffer
        const sensorData* sample = dataReader.getSample();

        // Check if the current sample is valid
        bool current_is_valid = checkSample(sample);

        // If the current or the last sample is valid, we send the data to the database
        // If the sample being processed is non-zero, it is always sent to the database
        // Else, it is only sent if the last sample was valid, so that we don't send
        // too many null values to the database in succession
        if (current_is_valid || last_was_valid) {

            // If debugging, only print the values instead of sending them to the database
            #ifdef DEBUG
                char string[13 * sensorCount + 1];
                int stringIndex = 0;
                // Print all the collected sensors data on the serial port
                for (int i = 0; i < sensorCount; i++) {
                    stringIndex += snprintf(string + index, 13, "p%02d: %d; ", i, buffer[readIndex].pressure[i]);
                }
                Serial.println(string);
                jsonSize = 0;
            #else
            // Concatenate the sample in a JSON buffer
            appendDataToJSON(sample);

            // If the JSON buffer is full, send the data to the database
            if (jsonSize >= jsonBatchSize) {
                // Send the data to database
                if(!Firebase.updateNodeSilentAsync(fbdo, dataPath, jsonBuffer)) {
                    // Update the LED indicator, showing that everything works fine
                    showError(none);
                } else {
                    Serial.println(fbdo.errorReason().c_str());
                    showError(noDatabaseConnection);
                }
                // Clear the JSON buffer and reset the counter
                jsonBuffer.clear();
                jsonSize = 0;
            }
            #endif
        }

        // Update the last_was_valid variable
        last_was_valid = current_is_valid;

    // If the Firebase Database is not ready, we search for database or network errors
    } else {
        if (WiFi.status() != WL_CONNECTED) {
            showError(noInternet);
            Serial.println("Reconectando à rede...");
        } else {
            showError(noDatabaseConnection);
            Serial.println("Reconectando ao banco de dados...");
        }
    }

    // Print the buffer state
    dataReader.printBufferState();

    // If we just send an amount of data to the database, give an interval to Core 0 to work on maintence activities, avoiding crash problems
    vTaskDelay(10);
    yield();
}
