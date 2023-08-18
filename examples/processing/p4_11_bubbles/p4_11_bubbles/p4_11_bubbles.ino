#include <BionicGlove.h>
BionicGlove bionic;
void setup()
{
    Serial.begin(115200);
    bionic.setAllOpenedFingersThresholdPercentage(15);
    bionic.start();
}
void loop()
{
    if (bionic.read())
    {
        String str;
        str = String(bionic.getSmooth(DATA_A_X_ANG)) + " " +
              String(bionic.getSmooth(DATA_A_Y_ANG)) + " " +
              String(!bionic.getFingerOpenedStatus(FINGER_INDEX)) + " " +
              String(!bionic.getFingerOpenedStatus(FINGER_MIDDLE)) + " " +
              String(!bionic.getFingerOpenedStatus(FINGER_RING)) + " " +
              String(!bionic.getFingerOpenedStatus(FINGER_LITTLE));
        Serial.println(str);
    }
}
