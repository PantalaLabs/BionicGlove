#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.start();
    bionic.attachCallOnVerticalPositiveKnock(printMessageA);
    bionic.attachCallOnVerticalNegativeKnock(printMessageB);
    bionic.setAllFingersThresholdPercentage(20);
}

void loop()
{
    bionic.read();
}

void printMessageA()
{
    if (bionic.getFingerClosedStatus(DATA_F_INDEX))
    {
        Serial.println("1");
    }
    else if (bionic.getFingerClosedStatus(DATA_F_MIDDLE))
    {
        Serial.println("3");
    }
}
void printMessageB()
{
    if (bionic.getFingerClosedStatus(DATA_F_INDEX))
    {
        Serial.println("2");
    }
    else if (bionic.getFingerClosedStatus(DATA_F_MIDDLE))
    {
        Serial.println("4");
    }
}