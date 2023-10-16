/*
    Credentials.h

    * This module stores the credentials for the WiFi connection and the Firebase Database 
      connection. It also stores the database user e-mail and password that were already 
      registered or added to the database project.
    * Please fill the credentials with your own data to be able to connect to the WiFi 
      network and to the Firebase Database. Otherwise, the device will not be able to send data 
      to the database or even connect to the WiFi network.
*/

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// Define the WiFi credentials (https://youtu.be/pDDdA9qEFwY)
static const char* WIFI_SSID = "YOUR_WIFI_SSID";
static const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Define the RTDB (Realtime Database) URL and the API Key
static const char* DATABASE_URL = "YOUR_DATABASE_URL";
static const char* DATABASE_API_KEY = "YOUR_DATABASE_API_KEY";

// Define the database user e-mail and password that were already registered or added to the database project
static const char* DATABASE_USER_EMAIL = "YOUR_DATABASE_USER_EMAIL";
static const char* DATABASE_USER_PASSWORD = "YOUR_DATABASE_USER_PASSWORD";

#endif