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
        servoAngle = bionic.getAAngsmoothed(AXL_X);
        myServo.write(servoAngle);
        delay(15);
    }
}