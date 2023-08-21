/*
    Database.h

    * This module handle the database connection and provides functions to send data to the database.
    * It uses the FirebaseESP32 library to connect and send data directly to the Firebase Realtime Database.
    * It also provides a function to structure the collected data into JSON formatted batches to be sent to the database.
    * It also log the device's boot, useful to analyze crashes, stability, reboots...
*/

#ifndef Database_H_
#define Database_H_

#include <FirebaseESP32.h>

#include "Buffer.h"

// // Define the API Key
#define DATABASE_API_KEY "AIzaSyCO3WjQJodTcimQjzQnQ5_ZpEgxyaQR-0o";
// // Define the RTDB (Realtime Database) URL
#define DATABASE_URL "https://friendly-bazaar-334818-default-rtdb.firebaseio.com/";

// // Test database credentials
// #define DATABASE_API_KEY "AIzaSyASxq5x-HYYTv1jLM2L1WJoC2xFTOaCj2E";
// #define DATABASE_URL "https://esp32test-115a2-default-rtdb.firebaseio.com/";

// Define the user e-mail and password that were already registered or added to the project
#define DATABASE_USER_EMAIL "admin@admin.com";
#define DATABASE_USER_PASSWORD "adminadmin";

class Database {
    // Define the Firebase Data, Authentication and Configuration objects
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;
    
    // Set the base path in the database where the json will be pushed
    String DATABASE_BASE_PATH = "/sensor_readings_NEW_STRUCTURE/";

    // Create a JSON object to hold and organize the data before be sended to database
    FirebaseJson jsonBuffer;
    // Create some variables to help to fill the JSON object until a certain size
    const int jsonBatchSize = 10;
    int jsonSize = 0;

    // Define variables to handle the timestamp to datetime conversion
    struct tm * timeinfo;

    // Set the date path on the database where the sensors data will be stored
    String datePath;

    // Set the data path on the database where the sensors data will be stored
    String fullDataPath;

    // Set the string where the payload will be built
    String payload;

    // Stores whether or not the last sample from the sensors was valid (non-zero)
    bool last_was_valid;
 
    char key[32];
    // int index = 0;

 public:
    Database();

    // Function that logs the device's boot. Useful to analyze crashes, stability, reboots...
    void bootLog();

    // Function that setup the database connection
    void setup(time_t timestampUnix);

    // Funcion that append sensor data into the JSON object
    void appendDataToJSON(const sensorData* data);

    // Function that sends the JSON object to the database, update the node asynchronously
    bool pushData();

    // Function that track the incoming data and fill the json buffer to be sent to the database
    void sendData(SensorDataBuffer *dataBuffer);
};

#endif
