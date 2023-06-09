    #include <BionicGlove.h>

BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    bionic.start();
    bionic.attachCallOnPosVerKnock(printMessageA);
    bionic.attachCallOnNegVerKnock(printMessageB);
    bionic.setAllRedlinePercentage(20);
}

void loop()
{
    bionic.read();
}

void printMessageA()
{
    if (bionic.getClosedFingerStatus(RAW_F_INDEX))
    {
        Serial.println("1");
    }
    else if (bionic.getClosedFingerStatus(RAW_F_MIDDLE))
    {
        Serial.println("3");
    }
}
void printMessageB()
{
    if (bionic.getClosedFingerStatus(RAW_F_INDEX))
    {
        Serial.println("2");
    }
    else if (bionic.getClosedFingerStatus(RAW_F_MIDDLE))
    {
        Serial.println("4");
    }
}