#ifndef Database_H_
#define Database_H_

#include <FirebaseESP32.h>

class DataReader;

// Define the API Key
#define DATABASE_API_KEY "AIzaSyCO3WjQJodTcimQjzQnQ5_ZpEgxyaQR-0o";
// Define the RTDB (Realtime Database) URL
#define DATABASE_URL "https://friendly-bazaar-334818-default-rtdb.firebaseio.com/";

// Define the user e-mail and password that were already registered or added to the project
#define DATABASE_USER_EMAIL "admin@admin.com";
#define DATABASE_USER_PASSWORD "adminadmin";


class Database {
    FirebaseData fbdo;
    FirebaseAuth auth;
    FirebaseConfig config;

    // Create a JSON object to hold and organize the data before be sended to database
    FirebaseJson jsonBuffer;
    // Create some variables to help to fill the JSON object until a certain size
    const int jsonCapacity = 15;
    int jsonSize = 0;

    // Define variables to handle the timestamp to datetime conversion
    struct tm * timeinfo;

    // Set the data path on the database where the sensors data will be stored
    String dataPath;

    // Stores whether or not the last sample from the sensors was valid (non-zero)
    bool last_was_valid;

 public:
    Database();

    void bootLog();

    void setup(time_t timestampUnix);

    void appendDataToJSON(const sensorData* data);

    void sendData(DataReader& dataReader);
};

#endif
