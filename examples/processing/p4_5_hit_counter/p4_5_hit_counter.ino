#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.attachCallOnVerticalPositiveKnock(printMessageA);
    bionic.attachCallOnVerticalNegativeKnock(printMessageB);
    bionic.setAllFingersThresholdPercentage(40);
    bionic.start();
}

void loop()
{
    bionic.read();
}

void printMessageA()
{
    if (bionic.getFingerClosedStatus(FINGER_INDEX))
    {
        Serial.println("1");
    }
    else if (bionic.getFingerClosedStatus(FINGER_MIDDLE))
    {
        Serial.println("3");
    }
}
void printMessageB()
{
    if (bionic.getFingerClosedStatus(FINGER_INDEX))
    {
        Serial.println("2");
    }
    else if (bionic.getFingerClosedStatus(FINGER_MIDDLE))
    {
        Serial.println("4");
    }
}