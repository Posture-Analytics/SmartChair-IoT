# SmartChair-IoT

Code and documentation of the SmartChair project's IoT device, handling the microcontroller and the sensors connected to it.

All the information about this open-source project can be found in this repository, including the sketch, diagrams, documentation, instructions and more.

---

## Table of Contents

- [Installation & Setup](#installation--setup)
- [Diagrams](#diagrams)
- [Modules](#modules)
- [Configuration & Variables](#configuration--variables)
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
5. **Open the sketch and configure the code**: Open the `mainSketch.ino` file in the Arduino IDE 2.0 and configure the code (see [Configuration & Variables](#configuration--variables)). You must configure the WiFi network and the Firebase Realtime Database keys, URLs and credentials. Otherwise, the code will not work (either because it will not be able to connect to the WiFi network or because it will not be able to connect to the Firebase Realtime Database)
6. **Connect the microcontroller to the computer**: Connect the microcontroller to the computer using a USB cable. Also select the correct COM port in the Arduino IDE 2.0 (`Tools > Port`) and the correct board (`Tools > Board > esp32 Arduino > SparkFun ESP32 Thing Plus C`).
7. **Upload the code to the microcontroller**: Open the `mainSketch.ino` file in the Arduino IDE 2.0 and upload the code to the microcontroller (`Sketch > Upload` / `Ctrl+U`). Wait until the code is uploaded and the microcontroller is ready to use.

## Diagrams

1. [Conceptual Diagram](<Diagrams/Conceptual Diagram/Conceptual Diagram.png>)
2. **Pressure Sensor Array Distribution**: [Link to the pressure sensor array distribution or embed the image]
3. **Circuit Diagram**: [Link to the circuit diagram or embed the image]
4. **Circuit Schematic**: [Link to the circuit schematic or embed the image]
5. **Flow Chart**: [Link to the flowchart or embed the image]

## Modules

| Module Name | Description |
|-------------|-------------|
| `Buffer` | Handle the buffer that stores the data collected from the sensors. |
| `Network` | Handle the WiFi network connection of the device. |
| `DataReader` | Read the data from the sensors and store it in the buffer. |
| `Database` | Establishes a connection to the Firebase Realtime Database and push the data from the buffer to the database. |
| `ExternalADCs` | Handle the external ADCs that are connected to the microcontroller and convert the data from the sensors to digital values. |
| `Errors` | Handle the errors that occur during the execution of the program. | 

## Configuration & Variables

This section highlights the key variables in the project which can be changed for customization.

**Note**: The variables about the WiFi network and the Firebase Realtime Database keys, URLs and credentials are not shown here. You must configure them in the code (see [Installation & Setup](#installation--setup)) to be able to use the device.

| Variable Name | Module | Description | Default Value |
|---------------|---------------|-------------|---------------|
| `SAMPLE_RATE`  | `DataReader` | Sample rate of the data collection, in hertz (Hz) | `2` |
| `WIFI_SSID`  | `Network` | WiFi network SSID | Your network SSID |
| `WIFI_PASSWORD`  | `Network` | WiFi network password | Your network password|
| `DATABASE_API_KEY`  | `Database` | Firebase Realtime Database API key | Your Firebase Realtime Database API key |
| `DATABASE_URL`  | `Database` | Firebase Realtime Database URL | Your Firebase Realtime Database URL |
| `DATABASE_USER_EMAIL`  | `Database` | Firebase Realtime Database registered access email | Your Firebase Realtime Database registered access email |
| `DATABASE_USER_PASSWORD`  | `Database` | Firebase Realtime Database registered access password | Your Firebase Realtime Database registered access password |

## Future Improvements

- **New version of the SmartChair**: Now, using a ergonomically certified office chair
- **More pressure sensor array**: Extend the array to 24 sensors, to be able to detect the optimal positioning of the sensors on the chair's surface (seat and backrest)
- **More sensors**: Add new types of sensors to the chair, like IMUs (ICM-20948) and ToF sensors (VL6180 and VL53L5CX) to to improve the user's posture recognition
- **Extend battery life**: Use a more efficient components and implement a sleep mode to extend the battery life
- **Professional assembly**: Use a custom made PCB design and industrial connectors to improve the reliability of the device
- **Improve data storage**: Optimize the data storage in the Firebase Realtime Database to reduce the data usage and improve the data analysis and increase the amount of packages, to ensure a live data stream to the database/dashboard

## Acknowledgements

Thank you to the team members, our Professor Rafael de Pinho  and FGV EMAp for their contributions, support, or resources provided to this project.

## Contact

For any queries or feedback, please contact us at: posture.analytics@gmail.com
