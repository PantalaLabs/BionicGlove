#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
  Serial.begin(115200);
  bionic.attachCallOnCrossMinXangle(printMessage1);
  bionic.attachCallOnCrossMaxXangle(printMessage2);
  bionic.attachCallOnCrossMinYangle(printMessage3);
  bionic.attachCallOnCrossMaxYangle(printMessage4);
  bionic.start();
}

void loop()
{
  bionic.read();
}

void printMessage1()
{
  Serial.println("left");
}
void printMessage2()
{
  Serial.println("right");
}
void printMessage3()
{
  Serial.println("front");
}
void printMessage4()
{
  Serial.println("rear");
}
