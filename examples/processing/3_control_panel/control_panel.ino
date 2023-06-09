#include <BionicGlove.h>
uint32_t nextRead;
BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.start();
}

void loop()
{
    bionic.read();
    if (millis() > nextRead)
    {
        nextRead = millis() + 30;
        Serial.print(bionic.getF(FINGER_INDEX));
        Serial.print(",");
        Serial.print(bionic.getF(FINGER_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getF(FINGER_RING));
        Serial.print(",");
        Serial.print(bionic.getF(FINGER_LITTLE));

        Serial.print(",");
        Serial.print(bionic.getFclosedStatus(FINGER_INDEX));
        Serial.print(",");
        Serial.print(bionic.getFopenedStatus(FINGER_INDEX));

        Serial.print(",");
        Serial.print(bionic.getFclosedStatus(FINGER_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getFopenedStatus(FINGER_MIDDLE));

        Serial.print(",");
        Serial.print(bionic.getFclosedStatus(FINGER_RING));
        Serial.print(",");
        Serial.print(bionic.getFopenedStatus(FINGER_RING));

        Serial.print(",");
        Serial.print(bionic.getFclosedStatus(FINGER_LITTLE));
        Serial.print(",");
        Serial.print(bionic.getFopenedStatus(FINGER_LITTLE));

        // serial.print(","); // NUNCA USAR ÚLTIMA VIRGULA
        Serial.println("");
    }
}