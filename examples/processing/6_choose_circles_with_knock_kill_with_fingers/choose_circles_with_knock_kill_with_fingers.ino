#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.attachCallOnVerPosKnock(message1);
    bionic.attachCallOnVerNegKnock(message2);
    bionic.attachCallOnHorPosKnock(message1);
    bionic.attachCallOnHorNegKnock(message2);
    bionic.attachCallOnClosedFingerIndex(message3);
    bionic.attachCallOnOpenedFingerIndex(message3);
    bionic.setAllRedlinePercentage(10); // wide open all fingers
    bionic.setClosedRedLinePercentage(RAW_F_INDEX, 30); // index finger closes sooner
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
