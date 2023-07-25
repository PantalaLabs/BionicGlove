#include <BionicGlove.h>

BionicGlove bionic;
String separator = " ";
String dataPack = "";
bool flick[8];
bool knock[4];

void setup()
{
    Serial.begin(38400);
    bionic.setAllRedlinePercentage(25);

    bionic.attachCallOnFlickOpenedFingerIndex(message1);
    bionic.attachCallOnFlickClosedFingerIndex(message2);
    bionic.attachCallOnFlickOpenedFingerMiddle(message3);
    bionic.attachCallOnFlickClosedFingerMiddle(message4);
    bionic.attachCallOnFlickOpenedFingerRing(message5);
    bionic.attachCallOnFlickClosedFingerRing(message6);
    bionic.attachCallOnFlickOpenedFingerLittle(message7);
    bionic.attachCallOnFlickClosedFingerLittle(message8);

    bionic.attachCallOnVerticalPositiveKnock(message9);
    bionic.attachCallOnVerticalNegativeKnock(message10);
    bionic.attachCallOnHorizontalPositiveKnock(message11);
    bionic.attachCallOnHorizontalNegativeKnock(message12);

    bionic.setBuiltInLedOn();
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

        for (uint8_t i = 0; i < 8; i++)
        {
            dataPack += separator;
            dataPack += flick[i];
            flick[i] = false;
        }
        dataPack += separator;
        dataPack += map(bionic.getAAngsmoothed(AXL_X), -90, 90, 0, 180);
        dataPack += separator;
        dataPack += map(bionic.getAAngsmoothed(AXL_Y), -90, 90, 180, 0);

        for (uint8_t i = 0; i < 4; i++)
        {
            dataPack += separator;
            dataPack += knock[i];
            knock[i] = false;
        }

        Serial.println(dataPack);
    }
}

void message1()
{
    flick[0] = true;
}
void message2()
{
    flick[1] = true;
}
void message3()
{
    flick[2] = true;
}
void message4()
{
    flick[3] = true;
}
void message5()
{
    flick[4] = true;
}
void message6()
{
    flick[5] = true;
}
void message7()
{
    flick[6] = true;
}
void message8()
{
    flick[7] = true;
}

void message9()
{
    knock[0] = true;
}
void message10()
{
    knock[1] = true;
}
void message11()
{
    knock[2] = true;
}
void message12()
{
    knock[3] = true;
}
