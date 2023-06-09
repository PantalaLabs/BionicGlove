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
        Serial.println(bionic.getRaw(RAW_F_LITTLE));
        delay(50);
    }
}