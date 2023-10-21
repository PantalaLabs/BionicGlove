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
        Serial.println(">" + String(bionic.getF(DATA_F_INDEX)/8) + " " + //
                           String(bionic.getF(DATA_F_MIDDLE)/8) + " " +  //
                           String(bionic.getF(DATA_F_RING)/8) + " " +    //
                           String(bionic.getF(DATA_F_LITTLE)/8) + " " +  //
                           String(bionic.getF(DATA_SMOOTHFACTOR)/8) + "<");
}