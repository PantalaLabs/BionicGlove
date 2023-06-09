#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.attachCallOnPosVerKnock(message1);
    bionic.attachCallOnNegVerKnock(message2);
    bionic.attachCallOnPosHorKnock(message3);
    bionic.attachCallOnNegHorKnock(message4);
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
