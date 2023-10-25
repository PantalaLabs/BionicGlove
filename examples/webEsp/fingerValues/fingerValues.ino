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
        String s;
        s = String(bionic.getRaw(DATA_F_INDEX)) + String(",") + String(bionic.getRaw(DATA_F_MIDDLE)) + String(",") + String(bionic.getRaw(DATA_F_RING)) + String(",") + String(bionic.getRaw(DATA_F_LITTLE));
        Serial.println(s);
    }
}