#include "time.h"

#include <WiFi.h>
#include <Wire.h>

// DEBUG Flag, prints data to Serial instead of sending to the database
// #define DEBUG

#include "DataReader.h"
#include "Database.h"
#include "Network.h"
#include "Errors.h"


// Create a task to assign the data push to the database to Core 0
TaskHandle_t sendToDatabaseTask;

DataReader dataReader;
Database database;

// Initialization void
void setup() {
    Serial.begin(115200);  // Open the Serial Port for communication with baudrate 115200
    Wire.begin();  // Start the I2C communication

    // Setup the RGB built-in LED to show the device's current status or errors
    setupLED();

    // Setup the external ADCs
    dataReader.setup();

    // Setup WiFi connection
    setupWiFi();

    // Sync the device's time with an NTP Server time
    syncWithNTPTime();

    // Setup the Firebase Database connection
    database.setup(getCurrentTime());

    // Assign the task of sending data to the database to Core 0
    xTaskCreatePinnedToCore(
        sendToDatabase,          // Task function
        "sendToDatabaseLoop",    // Name of task
        10000,                   // Stack size of task
        NULL,                    // Parameter of the task
        1,                       // Priority of the task
        &sendToDatabaseTask,     // Task handle to keep track of created task
        0);                      // Pin task to core 0

    // Sanity delay
    delay(100);

    // Register the boot on the database ("/bootLog")
    database.bootLog();
}

// Main loop, that keep running on Core 1
void loop() {
    // Disable the watchdog of Core 0, avoiding reboots caused by the working time of the sendToDatabase task
    disableCore0WDT();

    dataReader.fillBuffer();
}

// Task attached to core 0
void sendToDatabase(void* pvParameters) {

    while(true){
      // If the buffer is empty, wait for the data collection task to fill it
      if (!dataReader.isBufferEmpty()) {
          database.sendData(dataReader);
      }else{
          vTaskDelay(10);
          yield();
      }
    }
    
    Serial.println("OUTER LOOP");
}
