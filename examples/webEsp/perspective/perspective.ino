#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.start();
}

void loop()
{
    if (bionic.read())
    {
        String dataPack = "s" + String(bionic.getAAngsmoothed(AXL_X)) + "," +
                          String(bionic.getAAngsmoothed(AXL_Y)) + "e*";
        Serial.println(dataPack);
    }
}