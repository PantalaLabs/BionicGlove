#include <BionicGlove.h>
#include <Servo.h>

BionicGlove bionic;
Servo myServo;
int16_t servoAngle;

void setup()
{
    myServo.attach(15);
    bionic.start();
}
void loop()
{
    if (bionic.read())
    {
        servoAngle = map(bionic.getAAngsmoothed(AXL_X), -90, 90, 0, 180);
        myServo.write(servoAngle);
        delay(15);
    }
}