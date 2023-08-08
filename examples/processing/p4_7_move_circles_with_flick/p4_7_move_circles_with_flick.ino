#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.attachCallOnFlickOpenedFingerIndex(message2);
    bionic.attachCallOnFlickClosedFingerIndex(message1);
    bionic.attachCallOnHorizontalPositiveKnock(message3);
    bionic.attachCallOnHorizontalNegativeKnock(message3);
    bionic.setKnockThreshold(6.0);
    bionic.setKnockDebounceInterval(500);
    bionic.start();
}

void loop()
{
    bionic.read();
}

void message1()
{
    bionic.freeze(500);
    Serial.println("1");
}
void message2()
{
    bionic.freeze(500);
    Serial.println("2");
}
void message3()
{
    bionic.freeze(500);
    Serial.println("3");
}
