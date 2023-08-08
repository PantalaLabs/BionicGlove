#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.attachCallOnVerticalPositiveKnock(message1);
    bionic.attachCallOnVerticalNegativeKnock(message2);
    bionic.attachCallOnHorizontalPositiveKnock(message3);
    bionic.attachCallOnHorizontalNegativeKnock(message4);
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
