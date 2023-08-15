#include <BionicGlove.h>
BionicGlove bionic;
void setup()
{
    Serial.begin(115200);
    bionic.setAllClosedFingersThresholdPercentage(85);
    bionic.attachCallOnWideClosedFingerIndex(message1);
    bionic.attachCallOnWideClosedFingerMiddle(message2);
    bionic.start();
}
void loop()
{
    bionic.read();
}

void message1()
{
    Serial.println(1);
}
void message2()
{
    Serial.println(2);
}