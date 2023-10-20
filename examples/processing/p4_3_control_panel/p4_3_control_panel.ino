// BionicGlove\examples\processing\p4_3_control_panel\p4_3_control_panel

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
    {
        Serial.print(bionic.getF(FINGER_INDEX));
        Serial.print(",");
        Serial.print(bionic.getF(FINGER_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getF(FINGER_RING));
        Serial.print(",");
        Serial.print(bionic.getF(FINGER_LITTLE));

        Serial.print(",");
        Serial.print(bionic.getFingerClosedStatus(FINGER_INDEX));
        Serial.print(",");
        Serial.print(bionic.getFingerOpenedStatus(FINGER_INDEX));

        Serial.print(",");
        Serial.print(bionic.getFingerClosedStatus(FINGER_MIDDLE));
        Serial.print(",");
        Serial.print(bionic.getFingerOpenedStatus(FINGER_MIDDLE));

        Serial.print(",");
        Serial.print(bionic.getFingerClosedStatus(FINGER_RING));
        Serial.print(",");
        Serial.print(bionic.getFingerOpenedStatus(FINGER_RING));

        Serial.print(",");
        Serial.print(bionic.getFingerClosedStatus(FINGER_LITTLE));
        Serial.print(",");
        Serial.print(bionic.getFingerOpenedStatus(FINGER_LITTLE));

        Serial.println("");
    }
}