#include <Wire.h>
#include "PersonSensor.h"

const int32_t SAMPLE_DELAY_MS = 2000;

// Create an instance of the PersonSensor class
PersonSensor personSensor;

void setup() {
  Wire.begin();
  Serial.begin(115200);

  uint8_t nextID = personSensor.getNextId();
  personSensor.setNextId(nextID);

  personSensor.enableIdRecognition();
}

void loop() {
  PersonSensor::CompleteResults results = {};
  
  if (!personSensor.readResults(&results)) {
    Serial.println("No person sensor results found on the i2c bus");
    delay(SAMPLE_DELAY_MS);
    return;
  }

  Serial.println("********");
  Serial.print(results.numFaces);
  Serial.println(" faces found");
  
  for (int i = 0; i < results.numFaces; ++i) {
    const PersonSensor::FaceDetails* face = &results.faces[i];
    Serial.print("Face #");
    Serial.print(i);
    Serial.print(" (id=");
    Serial.print(face->id);
    Serial.print(", ");
    Serial.print(face->idConfidence);
    Serial.print("): ");
    Serial.print(face->boxConfidence);
    Serial.print(" confidence, (");
    Serial.print(face->boxLeft);
    Serial.print(", ");
    Serial.print(face->boxTop);
    Serial.print("), (");
    Serial.print(face->boxRight);
    Serial.print(", ");
    Serial.print(face->boxBottom);
    Serial.print("), ");
    
    if (face->isFacing) {
      Serial.println("facing");
    } else {
      Serial.println("not facing");
    }
  }
  delay(SAMPLE_DELAY_MS);
}
