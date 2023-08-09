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
        Serial.println(">" + String(bionic.getF(DATA_F_INDEX)) + " " + //
                           String(bionic.getF(DATA_F_MIDDLE)) + " " +  //
                           String(bionic.getF(DATA_F_RING)) + " " +    //
                           String(bionic.getF(DATA_F_LITTLE)) + " " +  //
                           String(bionic.getF(DATA_SMOOTHFACTOR)) + "<");
}