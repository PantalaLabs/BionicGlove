#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.attachCallOnVerticalPositiveKnock(message1);
    bionic.attachCallOnVerticalNegativeKnock(message2);
    bionic.attachCallOnHorizontalPositiveKnock(message1);
    bionic.attachCallOnHorizontalNegativeKnock(message2);
    bionic.attachCallOnWideClosedFingerIndex(message3);
    bionic.attachCallOnWideOpenedFingerIndex(message3);
    bionic.setAllRedlinePercentage(10); // wide open all fingers
    bionic.setClosedRedLinePercentage(FINGER_INDEX, 30); // index finger closes sooner
    bionic.setLedBuiltIn();
    bionic.start();
}

float acc;
void loop()
{
    bionic.read();
}

void message1()
{
    Serial.println("1");
}
void message2()
{
    Serial.println("2");
}
void message3()
{
    Serial.println("5");
}
