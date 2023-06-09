#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.attachCallOnPosVerStump(message1);
    bionic.attachCallOnNegVerStump(message2);
    bionic.attachCallOnPosHorStump(message3);
    bionic.attachCallOnNegHorStump(message4);
    bionic.start();
}

void loop()
{
    bionic.read();
}

void message1()
{
    Serial.println("+Vertical");
}
void message2()
{
    Serial.println("-Vertical");
}
void message3()
{
    Serial.println("+Horizontal");
}
void message4()
{
    Serial.println("-Horizontal");
}
