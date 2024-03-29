
#include <WiFi.h>
#include <time.h>
// Provide the token generation process info for database autentication
#include <addons/TokenHelper.h>

#include "Database.h"
#include "Errors.h"
#include "Network.h"
#include "Buffer.h"
#include "Debug.h"

Database::Database() : last_was_valid(true) {}

void Database::updateCurrentTime() {
    // Set the variable `currentMicros` with the current time in microseconds (us)
    currentMicros = micros();
}

void Database::setup(time_t timestampUnix) {
    // Assign the api key (required) of the database
    config.api_key = DATABASE_API_KEY;

    // Assign the RTDB URL (required)
    config.database_url = DATABASE_URL;

    // Assign the user sign in credentials
    auth.user.email = DATABASE_USER_EMAIL;
    auth.user.password = DATABASE_USER_PASSWORD;

    // Assign the callback function for the long running token generation task
    config.token_status_callback = tokenStatusCallback;  // see addons/TokenHelper.h

    // Authenticate and initialize the communication with the Firebase database
    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);
}

void Database::bootLog() {
    // If the database is ready to receive the data, we record the timestamp of the device's boot
    if (Firebase.ready()) {
        // Record the current timestamp string to the database
        if (Firebase.pushInt(fbdo, "/bootLog/", getCurrentMillisTimestamp())) {
            LogInfoln("Inicialização registrada com sucesso!");
        // If an error occurs during this process, we show it as a fatal
        // database error and restart the device
        } else {
            LogFatalln("Ocorreu um erro ao registrar a inicialização:\n", fbdo.errorReason());
            errorHandler.showError(ErrorType::NoDatabaseConnection, true);
        }
    // If the Firebase Database is not ready, we test the connection to
    // the network and to the database and display it on the LED indicator
    } else {
        if (WiFi.status() != WL_CONNECTED) {
            LogFatalln("Não foi possível conectar à rede para registrar a inicialização");
            errorHandler.showError(ErrorType::NoInternet, true);
        } else {
            LogFatalln("Não foi possível conectar ao banco de dados para registrar a inicialização");
            errorHandler.showError(ErrorType::NoDatabaseConnection, true);
        }
    }
}

void Database::appendDataToJSON(const sensorData* data) {
    // Clears the previous data stored in the payload array
    payload.clear();
    // Add the pressure sensors' data to the payload
    for (int i = 0; i < PRESSURE_SENSOR_COUNT; i++) {
        payload.add(data->pressureSensor[i]);
    }

    // Set the node where the data will be stored as a the date, with a milliseconds subkey.
    jsonBuffer.add(data->timestampMillis, payload);

    // Increment the jsonSize to keep control of how many data samples are been stored in the JSON
    // buffer
    jsonSize++;
}

bool Database::pushData() {
    #ifdef DEBUG

        // In debug mode, we only print the values instead of sending them to the database
        json.toString(Serial, true, 255);
        return true;

    #else

        // If the Firebase Database is ready to receive the data, we send it asynchronously
        // to be faster and to be able the send a larger amount of the data points per second
        if (Firebase.ready()) {
            // Send the data to database
            if (Firebase.updateNodeSilentAsync(fbdo, fullDataPath, jsonBuffer)) {
                // Update the LED indicator, showing that everything works fine
                errorHandler.showError(ErrorType::None);

                // Clear the JSON buffer and reset the counter
                jsonBuffer.clear();
                jsonSize = 0;

                // If the data was sent successfully, we return true
                return true;
            // If some error occurs during this process, we show as a fatal database error and
            // restart the device
            } else {
                LogErrorln("Database error on ", fullDataPath, ": ", fbdo.errorReason());
                LogErrorln("Payload buffer length: ", jsonBuffer.serializedBufferLength());
                errorHandler.showError(ErrorType::NoDatabaseConnection);

                return false;
            }
        } else {
            return false;
        }

    #endif
}

void Database::sendData(SensorDataBuffer* dataBuffer) {
    // Save the time when the device start to send the data from the sensors,
    // to keep control of the intervals between data uploads
    updateCurrentTime();

    // If the time elapsed since the last data sending is greater than the interval between
    // the data uploads, if the JSON buffer is full or if the next package is from another day, we
    // send the data to the database
    if ((jsonSize > 0 && currentMicros - dataPrevSendingMicros > dataSendIntervalMicros)
            || jsonSize >= jsonBatchSize
            || jsonSize > 0 && dataBuffer->hasDateChanged()) {
        pushData();

        // Update the time variable that controls the send interval
        dataPrevSendingMicros = currentMicros;
    }

    // Otherwise, we keep filling the JSON buffer with the data from the main buffer
    else {

        // Get one sample from the sensor data buffer
        const sensorData* sample = dataBuffer->getSample();

        // Check if the current sample is valid
        bool current_is_valid = !dataBuffer->isSampleNull(sample);
        // bool current_is_valid = true; // DEBUG

        /**
         * If the current or the last sample is valid, we send the data to the database.
         * If the sample being processed is non-zero, it is always sent to the database.
         * Else, it is only sent if the last sample was valid, so that we don't send
         * too many null values to the database in succession.
         */
        if (current_is_valid || last_was_valid) {
            // Concatenate the sample in a JSON buffer
            appendDataToJSON(sample);
        }

        last_was_valid = current_is_valid;
    }

    // If necessary, we update the path of the database node that will receive the data
    if (dataBuffer->hasDateChanged()) {
        // Get the date string of the current sample
        dataBuffer->computeCurrentSampleDate(sampleDate);
        // Add a slash to the end of the date string to make the path valid
        sampleDate[10] = '/';
        sampleDate[11] = '\0';
        // Get the seconds that represent the following day, used to check if the date changed
        dataBuffer->computeNextDaySeconds();
        // Update the path of the database node that will receive the data
        fullDataPath = DATABASE_BASE_PATH + sampleDate;
    }

    dataBuffer->printBufferState();

    // Print the size of the JSON buffer
    LogVerboseln("JSON buffer: ", jsonSize, "/", jsonBatchSize);

    dataBuffer->printBufferIndexes();

    // If we just send an amount of data to the database,
    // give an interval to Core 0 to work on maintence activities, avoiding crash problems
    vTaskDelay(10);
    yield();
}
