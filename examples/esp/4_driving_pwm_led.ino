#include <Arduino.h>
#include <BionicGlove.h>

#define LED_GPIO 5
#define PWM1_Ch 0
#define PWM1_Res 12
#define PWM1_Freq 1000

BionicGlove bionic;

void setup()
{
  pinMode(LED_GPIO, OUTPUT);
  ledcAttachPin(LED_GPIO, PWM1_Ch);
  ledcSetup(PWM1_Ch, PWM1_Freq, PWM1_Res);

  bionic.start();
}

void loop()
{
  bionic.read();
  ledcWrite(PWM1_Ch, bionic.getRaw(FINGER_LITTLE)/8);
}
