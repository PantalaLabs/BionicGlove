#include <BionicGlove.h>

BionicGlove bionic;

String dataPack = "";
String initString = "s";
String endString = "e*";

void setup()
{
    Serial.begin(38400);
    bionic.start();
}

void loop()
{
    if (bionic.read())
    {
        dataPack = initString + String(bionic.getAAngsmoothed(AXL_X)) + "," +
                   String(bionic.getAAngsmoothed(AXL_Y)) + endString;
        Serial.println(dataPack);
    }
}