#define BIONICGLOVE_EURORACK
#include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
  bionic.startEurorack();
}

void loop()
{
  if (bionic.read())
  {
    digitalWrite(bionic.eurorackDigitalOutChannels[i], bionic.getFingerOpenedStatus(i));
  }
}
