#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.start();
    bionic.attachCallOnClosedFingerIndex(printMessage);
}
 
void loop()
{
    bionic.read();
}

void printMessage()
{
    Serial.println("callback");
}