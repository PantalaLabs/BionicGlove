#include <BionicGlove.h>
uint32_t nextRead;
BionicGlove bionic;

void setup()
{
    Serial.begin(38400);
    bionic.start();
    bionic.attachCallOnVerticalPositiveStump(message1);
    bionic.attachCallOnVerticalNegativeStump(message2);
    bionic.attachCallOnHorizontalPositiveStump(message3);
    bionic.attachCallOnHorizontalNegativeStump(message4);
}

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
    Serial.println("3");
}
void message4()
{
    Serial.println("4");
}
