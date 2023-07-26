#include <BionicGlove.h>

BionicGlove bionic;

String dataPack = "";
void setup()
{
    Serial.begin(38400);
    bionic.start();
}

void loop()
{
    if (bionic.read())
    {
        dataPack = String(bionic.getF(DATA_F_INDEX)) + "," +
                   String(bionic.getF(DATA_F_MIDDLE)) + "," +
                   String(bionic.getF(DATA_F_RING)) + "," +
                   String(bionic.getF(DATA_F_LITTLE)) + ",";
        Serial.println(dataPack);
    }
}