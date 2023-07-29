#include <BionicGlove.h>

BionicGlove bionic;
String separator = " ";
String dataPack = "";

void setup()
{
    Serial.begin(38400);
    bionic.setAllRedlinePercentage(15);
    bionic.start();
}

uint32_t nextread;
void loop()
{
    if (bionic.read() && (millis() > nextread))
    {
        nextread = millis() + 30;
        dataPack = bionic.getF(FINGER_INDEX);
        dataPack += separator;
        dataPack += bionic.getF(FINGER_MIDDLE);
        dataPack += separator;
        dataPack += bionic.getF(FINGER_RING);
        dataPack += separator;
        dataPack += bionic.getF(FINGER_LITTLE);

        dataPack += separator;
        dataPack += bionic.getFopenedStatus(FINGER_INDEX);
        dataPack += separator;
        dataPack += bionic.getFopenedStatus(FINGER_MIDDLE);
        dataPack += separator;
        dataPack += bionic.getFopenedStatus(FINGER_RING);
        dataPack += separator;
        dataPack += bionic.getFopenedStatus(FINGER_LITTLE);
        dataPack += separator;
        dataPack += bionic.getFclosedStatus(FINGER_INDEX);
        dataPack += separator;
        dataPack += bionic.getFclosedStatus(FINGER_MIDDLE);
        dataPack += separator;
        dataPack += bionic.getFclosedStatus(FINGER_RING);
        dataPack += separator;
        dataPack += bionic.getFclosedStatus(FINGER_LITTLE);

        dataPack += separator;
        dataPack += constrain(map(bionic.getAAngsmoothed(AXL_X), -90, 90, 0, 180), 0, 180);
        dataPack += separator;
        dataPack += constrain(map(bionic.getAAngsmoothed(AXL_Y), -90, 90, 180, 0), 0, 180);

        Serial.println(dataPack);
    }
}