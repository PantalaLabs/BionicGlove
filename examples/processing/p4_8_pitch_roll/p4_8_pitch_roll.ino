#include <BionicGlove.h>
BionicGlove bionic;
float smoothed[2];
void setup()
{
  Serial.begin(115200);
  bionic.start();
}

void loop()
{
  if (bionic.read())
  {
    ALPHAFILTER(smoothed[0], bionic.getRaw(DATA_A_X_ANG), bionic.getRaw(DATA_SMOOTHFACTOR));
    ALPHAFILTER(smoothed[1], bionic.getRaw(DATA_A_Y_ANG), bionic.getRaw(DATA_SMOOTHFACTOR));
    Serial.println(String(smoothed[0]) + " " + String(smoothed[1]));
  }
}
