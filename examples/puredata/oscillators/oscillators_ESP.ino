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
        String str;
        str = String(bionic.getSmooth(DATA_F_INDEX) + 32) + " " +
              String(bionic.getSmooth(DATA_F_MIDDLE) + 32) + " " +
              String(bionic.getSmooth(DATA_F_RING) + 32) + " " +
              String(bionic.getSmooth(DATA_F_LITTLE) + 32);
        Serial.println(str);
    }
}
