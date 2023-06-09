#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.attachCallOnVerPosStump(message1);
    bionic.attachCallOnVerNegStump(message2);
    bionic.attachCallOnHorPosStump(message1);
    bionic.attachCallOnHorNegStump(message2);
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
