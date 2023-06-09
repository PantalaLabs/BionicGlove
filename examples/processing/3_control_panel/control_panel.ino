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
        Serial.print(bionic.getF(RAW_F_INDEX));
        Serial.print(",");
        Serial.print(bionic.getF(RAW_F_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getF(RAW_F_RING));
        Serial.print(",");
        Serial.print(bionic.getF(RAW_F_LITTLE));

        Serial.print(",");
        Serial.print(bionic.getClosedFingerStatus(RAW_F_INDEX));
        Serial.print(",");
        Serial.print(bionic.getOpenedFingerStatus(RAW_F_INDEX));

        Serial.print(",");
        Serial.print(bionic.getClosedFingerStatus(RAW_F_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getOpenedFingerStatus(RAW_F_MIDDLE));

        Serial.print(",");
        Serial.print(bionic.getClosedFingerStatus(RAW_F_RING));
        Serial.print(",");
        Serial.print(bionic.getOpenedFingerStatus(RAW_F_RING));

        Serial.print(",");
        Serial.print(bionic.getClosedFingerStatus(RAW_F_LITTLE));
        Serial.print(",");
        Serial.print(bionic.getOpenedFingerStatus(RAW_F_LITTLE));

        // serial.print(","); // NUNCA USAR ÚLTIMA VIRGULA
        Serial.println("");
    }
}