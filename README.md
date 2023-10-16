# SmartChair-IoT

Code and documentation of the SmartChair project's IoT device, handling the microcontroller and the sensors connected to a office chair, collecting data and sending it a the Firebase Realtime Database for further analysis.

All the information about the IoT device of this open-source project can be found in this repository, including the sketch, diagrams, documentation, instructions and more. For more information about the project's machine learning algorithm and visulization tool, please visit the [Posture-Analytics organization](https://github.com/Posture-Analytics) on GitHub.

---

## Table of Contents

- [Installation & Setup](#installation--setup)
- [Diagrams](#diagrams)
- [Modules](#modules)
- [Configuration & Variables](#configuration--variables)
- [Database Structure](#database-structure)
- [Future Improvements](#future-improvements)
- [Acknowledgements](#acknowledgements)
- [Contact](#contact)

---

## Installation & Setup

1. **GitHub Repository Clone**: Clone or download the repository.
2. **Install Arduino IDE 2.0**: Download and install the latest version of the Arduino IDE 2.0 from [here](https://www.arduino.cc/en/software).
3. **Install ESP32 boards dependencies**: Open the Arduino IDE 2.0 `Preferences` and add the following link to the `Additional Boards Manager URLs` field: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`.
3. **Install ESP32 Board on Arduino IDE**: Open the Arduino IDE 2.0 and go to `Tools > Board > Boards Manager`. Search for `esp32` and install the latest version of the board.
4. **Install the necessary libraries**: Open the Arduino IDE 2.0 and go to `Tools > Manage Libraries`. Search and install the following libraries:
    - `ADS1115_WE`
    - `FastLED`
    - `Firebase ESP32 Client`
5. **Open the sketch and configure the code**: Open the `mainSketch.ino` file in the Arduino IDE 2.0 and configure the code (see [Configuration & Variables](#configuration--variables)). You must configure the WiFi network and the Firebase Realtime Database keys, URLs and credentials with your own data. For that, you must fill `Credentials.h` file with your WiFi and Firebase Realtime Database credentials. Otherwise, the code will not work (either because it will not be able to connect to the WiFi network or because it will not be able to connect to the Firebase Realtime Database)
6. **Connect the microcontroller to the computer**: Connect the microcontroller to the computer using a USB cable. Also select the correct COM port in the Arduino IDE 2.0 (`Tools > Port`) and the correct board (`Tools > Board > esp32 Arduino > SparkFun ESP32 Thing Plus C`).
7. **Upload the code to the microcontroller**: Open the `mainSketch.ino` file in the Arduino IDE 2.0 and upload the code to the microcontroller (`Sketch > Upload` / `Ctrl+U`). Wait until the code is uploaded and the microcontroller is ready to use.

## Diagrams

1. [Conceptual Diagram](<Diagrams/Conceptual Diagram/Conceptual Diagram.png>)
2. [Pressure Sensors Distribution](<Diagrams/Pressure Sensors Distribution/Pressure Sensors Distribution.png>)
3. [Circuit Diagram](<Diagrams/Circuit Diagram/Circuit Diagram.png>)
4. [Circuit Schematic](<Diagrams/Circuit Schematic/Circuit Schematic.png>)
5. [Sketch Flowchart](<Diagrams/Sketch Flowchart/Sketch Flowchart.png>)

## Modules

| Module Name | Description |
|-------------|-------------|
| `Buffer` | Handle the buffer that stores the data collected from the sensors. |
| `Network` | Handle the WiFi network connection of the device. |
| `DataReader` | Read the data from the sensors and store it in the buffer. |
| `Database` | Establishes a connection to the Firebase Realtime Database and push the data from the buffer to the database. |
| `ExternalADCs` | Handle the external ADCs that are connected to the microcontroller and convert the data from the sensors to digital values. |
| `Errors` | Handle the errors that occur during the execution of the program. | 
| `Credentials` | Store the credentials of the WiFi network and the Firebase Realtime Database. |

## Configuration & Variables

This section highlights the key variables in the project which can be changed for customization.

**Note**: The variables about the WiFi network and the Firebase Realtime Database keys, URLs and credentials must be configured in the `Credentials.h` file in order to be able to use the device. Otherwise, the code will not work (either because it will not be able to connect to the WiFi network or because it will not be able to connect to the Firebase Realtime Database). For more information, see [Installation & Setup](#installation--setup).

| Variable Name | Module | Description | Default Value |
|---------------|---------------|-------------|---------------|
| `SAMPLE_RATE`  | `DataReader` | Sample rate of the data collection, in hertz (Hz) | `2` |
| `SEND_RATE`  | `Database` | Send rate of the data to the database, in hertz (Hz) | `2` |
| `WIFI_SSID`  | `Credentials` | WiFi network SSID | Your network SSID |
| `WIFI_PASSWORD`  | `Credentials` | WiFi network password | Your network password|
| `DATABASE_API_KEY`  | `Credentials` | Firebase Realtime Database API key | Your Firebase Realtime Database API key |
| `DATABASE_URL`  | `Credentials` | Firebase Realtime Database URL | Your Firebase Realtime Database URL |
| `DATABASE_USER_EMAIL`  | `Credentials` | Firebase Realtime Database registered access email | Your Firebase Realtime Database registered access email |
| `DATABASE_USER_PASSWORD`  | `Credentials` | Firebase Realtime Database registered access password | Your Firebase Realtime Database registered access password |

## Database Structure

We've decided to use **Google's Firebase Realtime Database** due to its simplicity, storing sensor valures in a JSON tree structure. Furthermore, the existing integration with the Arduino IDE and the ESP32 microcontroller through a [database client library](https://github.com/mobizt/Firebase-ESP32) makes it easy to use and implement, sending the data directly to the database in real time, without the need of a local server to redirect the data.

The database JSON tree structure is as follows:

```json
{
    "bootLog": {
        "HASH": "INITIALIZATION_TIMESTAMP_MILLIS"
    },
    "sensor_data": {
        "YYYY-MM-DD": {
            "COLLECT_TIMESTAMP_MILLIS": [
                "0": "SENSOR_1_VALUE",
                "1": "SENSOR_2_VALUE",
                "2": "SENSOR_3_VALUE",
                "3": "SENSOR_4_VALUE",
                "4": "SENSOR_5_VALUE",
                "5": "SENSOR_6_VALUE",
                "6": "SENSOR_7_VALUE",
                "7": "SENSOR_8_VALUE",
                "8": "SENSOR_9_VALUE",
                "9": "SENSOR_10_VALUE",
                "10": "SENSOR_11_VALUE",
                "11": "SENSOR_12_VALUE"
            ]
        }
    }
}
```

Where:
- `HASH`: Unique identifier generated by the Firebase Realtime Database when asked to append a new child to the `bootLog` node.
- `INITIALIZATION_TIMESTAMP_MILLIS`: Timestamp in milliseconds of the initialization of the device.
- `YYYY-MM-DD`: Date of the data collection.
- `COLLECT_TIMESTAMP_MILLIS`: Timestamp in milliseconds of the data collection.
- `SENSOR_X_VALUE`: Value of the pressure sensor X at the time of the data collection.

## Future Improvements

- **New version of the SmartChair**: Now, using a ergonomically certified office chair
- **New sensor array**: Extend the current array to 15 pressure sensors on the seat and 4 ToF sensors (VL5L4CD) on the backrest, to be able to detect the user's posture in more detail. Furthermore, add a IMU (ICM-20948) to each surface to detect significant movements of the user (like seating, standing up, leaning forward or backward) and some advanced ToF matrix sensors (VL53L5CX) to detect the spine curvature of the user and the positioning of the calves. This way, we hope to be able to detect the user's posture in more detail and with more accuracy.
- **User recognition**: Implement a user recognition system to be able to identify the user and store the data in the correct user profile in the database. This way, we can relate the data from the chair with the user's profile/antrhopometric data and analyze the data in a more detailed way.
- **Extend battery life**: Use a more efficient components, a larger battery, a E-Ink display to show relevant information and implement a sleep mode to extend the battery life
- **Professional assembly**: Use a custom made PCB design and industrial connectors to improve the reliability of the device
- **Improve data storage**: Optimize the data storage in the Firebase Realtime Database to reduce the network/storage usage and improve the data analysis by ensuring a live data stream to the database/dashboard. Furthermore, evolve the database structure to store the data of multiple chairs and users.

## Acknowledgements

Thank you to the team members, our Professor Rafael de Pinho André and FGV EMAp for their contributions, support, or resources provided to this project.

## Contact

For any queries or feedback, please contact us at: posture.analytics@gmail.com
