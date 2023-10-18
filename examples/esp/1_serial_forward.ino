#include <BionicGlove.h>

BionicGlove bionic;
bool knockStatus;
void setup()
{
    Serial.begin(115200);
    bionic.attachCallOnVerticalNegativeKnock(knock);
    bionic.start();
}

void loop()
{
    if (bionic.read())
    {
        String o;
        for (uint8_t i = 0; i < MAXBTDATAPACK; i++)
        {
            o += bionic.getUnitSmoothed(i);
            o += ",";
        }
        o += knockStatus;
        if(knockStatus)
        knockStatus = false;
        Serial.println(o);
    }
}

void knock()
{
    knockStatus = true;
}