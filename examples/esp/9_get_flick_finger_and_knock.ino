#include <BionicGlove.h>
BionicGlove bionic;

void setup()
{
  bionic.attachCallOnFlickClosedFingerIndex(message1);
  bionic.attachCallOnFlickOpenedFingerIndex(message2);
  bionic.attachCallOnVerticalPositiveKnock(message3);
  bionic.attachCallOnVerticalNegativeKnock(message4);
  bionic.start();
  Serial.begin(115200);
}

void loop()
{
  bionic.read();
}

void message1()
{
  Serial.println("closed");
}
void message2()
{
  Serial.println("open");
}
void message3()
{
  Serial.println("knock1");
}
void message4()
{
  Serial.println("knock2");
}
