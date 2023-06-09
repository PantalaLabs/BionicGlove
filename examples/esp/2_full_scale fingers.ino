#include <BionicGlove.h>

BionicGlove bionic;
void setup()
{
    Serial.begin(115200);
    bionic.start();
}

void loop()
{
    if (bionic.read())
    {
        Serial.print(bionic.getF(RAW_F_INDEX));
        Serial.print(",");
        Serial.print(bionic.getF(RAW_F_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getF(RAW_F_RING));
        Serial.print(",");
        // serial.print(","); // NUNCA USAR ÚLTIMA VIRGULA
        Serial.println(bionic.getF(RAW_F_LITTLE));
    }
}