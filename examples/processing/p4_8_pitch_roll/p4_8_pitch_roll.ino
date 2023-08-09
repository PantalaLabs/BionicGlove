#include <BionicGlove.h>
BionicGlove bionic;
void setup()
{
  Serial.begin(115200);
  bionic.start();
}

void loop()
{
  if (bionic.read())
    Serial.println(String(bionic.getSmooth(DATA_A_X_ANG)) + " " + String(bionic.getSmooth(DATA_A_Y_ANG)));
}
