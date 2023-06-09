#include <BionicGlove.h>

BionicGlove bionic;

uint32_t nextRead;

void setup()
{
    Serial.begin(38400);
    bionic.start();
}

void loop()
{
    if (bionic.read() && (millis() > nextRead))
    {
        nextRead = millis() + 30;
        Serial.print((int)bionic.getRaw(RAW_A_X_DEG));
        Serial.print(",");
        Serial.print((int)bionic.getRaw(RAW_A_Y_DEG));
        // serial.print(","); // NUNCA USAR ÚLTIMA VIRGULA
        Serial.println("");
    }
}

