#ifndef PERSON_SENSOR_H
#define PERSON_SENSOR_H

#include <Wire.h>

/**
 * @brief A class to interface with the Person Sensor module over I2C.
 * 
 * This class provides functionality to communicate with the Person Sensor
 * through I2C. It encapsulates commands to read sensor data, configure the sensor,
 * trigger single shots, manage ID recognition, and handle debugging.
 */
class PersonSensor {

private:
    // The I2C address of the person sensor board.
    static const uint8_t I2C_ADDRESS = 0x62;

    // Configuration commands for the sensor. Write this as a byte to the I2C bus
    // followed by a second byte as an argument value.
    static const uint8_t REG_MODE = 0x01;           // 0x00 = Standby, 0x01 = Continuous (default)
    static const uint8_t REG_ENABLE_ID = 0x02;      // 0x00 = Disable (default), 0x01 = Enable
    static const uint8_t REG_SINGLE_SHOT = 0x03;    // 0x00 = False (default), 0x01 = Trigger single shot
    static const uint8_t REG_CALIBRATE_ID = 0x04;   // 0x00 (default) to 0x07 = ID to calibrate
    static const uint8_t REG_PERSIST_IDS = 0x05;    // 0x01 = True (default), 0x00 = False
    static const uint8_t REG_ERASE_IDS = 0x06;      // 0x00 = False (default), 0x01 = True (Trigger erase)
    static const uint8_t REG_DEBUG_MODE = 0x07;     // 0x00 = False, 0x01 = True (default, enabling LED indicator)

    // The person sensor will never output more than four faces.
    static const uint8_t MAX_FACES_COUNT = 4;

    // How many different faces the sensor can recognize.
    static const uint8_t MAX_IDS_COUNT = 7;
    
public:    
    // Public data structures

    /**
     * @brief Results header structure for sensor communication.
     * 
     * The results returned from the sensor have a short header providing
     * information about the length of the data packet.
     * - reserved: Currently unused bytes.
     * - data_size: Length of the entire packet, excluding the header and checksum.
     *     For version 1.0 of the sensor, this should be 40.
     */
    struct ResultsHeader {
        uint8_t reserved[2];    // Bytes 0-1.
        uint16_t dataSize;     // Bytes 2-3.
    };

    /**
     * @brief Face details structure for sensor results.
     * 
     * Each face found by the sensor has a set of information associated with it:
     * - box_confidence: How certain we are we have found a face, from 0 to 255.
     * - box_left: X coordinate of the left side of the box, from 0 to 255.
     * - box_top: Y coordinate of the top edge of the box, from 0 to 255.
     * - box_width: Width of the box, where 255 is the full view port size.
     * - box_height: Height of the box, where 255 is the full view port size.
     * - id_confidence: How sure the sensor is about the recognition result.
     * - id: Numerical ID assigned to this face.
     * - is_looking_at: Whether the person is facing the camera, 0 or 1.
     */
    struct FaceDetails {
        uint8_t boxConfidence;     // Byte 1.
        uint8_t boxLeft;           // Byte 2.
        uint8_t boxTop;            // Byte 3.
        uint8_t boxRight;          // Byte 4.
        uint8_t boxBottom;         // Byte 5.
        int8_t idConfidence;       // Byte 6.
        int8_t id;                  // Byte 7
        uint8_t isFacing;          // Byte 8.
    };

    /**
     * @brief Complete results structure for sensor communication.
     * 
     * This structure represents the entire packet returned over I2C from the sensor.
     * - header: The results header.
     * - num_faces: Number of faces detected.
     * - faces: Array of face details.
     * - checksum: CRC16 of bytes 0 to 38.
     */
    struct CompleteResults {
        ResultsHeader header;
        int8_t numFaces;
        FaceDetails faces[MAX_FACES_COUNT];
        uint16_t checksum;
    };
    

    // Constructor
    /**
     * @brief Default constructor for the PersonSensor class.
     * 
     * Initializes the PersonSensor object and any necessary properties or settings.
     */
    PersonSensor();


    // Destructor
    /**
     * @brief Default destructor for the PersonSensor class.
     * 
     * Destroys the PersonSensor object and any associated resources.
     */
    ~PersonSensor();


    // Public member functions

    /**
     * @brief Read the latest results from the sensor.
     * 
     * @param results Pointer to the data structure to store the results.
     * @return true if the read operation was successful, false otherwise.
     */
    bool readResults(CompleteResults* results);

    /**
     * @brief Write a value to a specific sensor register over the I2C bus.
     * 
     * @param reg The register address.
     * @param value The value to write to the register.
     */
    void writeReg(uint8_t reg, uint8_t value);

    /**
     * @brief Trigger a single shot of the sensor.
     */
    void triggerSingleShot();

    /**
     * @brief Enable the sensor's ID recognition feature.
     */
    void enableIdRecognition();

    /**
     * @brief Disable the sensor's ID recognition feature.
     */
    void disableIdRecognition();

    /**
     * @brief Retrieve the next ID to be calibrated.
     * 
     * @return The next ID value.
     */
    uint8_t getNextId();

    /**
     * @brief Set the next ID to be calibrated.
     * 
     * @param id The ID value to set for calibration.
     */
    void setNextId(uint8_t id);

    /**
     * @brief Erase the recognized IDs from the sensor's storage.
     */
    void eraseIds();

    /**
     * @brief Enable the debug mode on the sensor.
     */
    void enableDebugMode();

    /**
     * @brief Disable the debug mode on the sensor.
     */
    void disableDebugMode();
};

#endif  // PERSON_SENSOR_H
