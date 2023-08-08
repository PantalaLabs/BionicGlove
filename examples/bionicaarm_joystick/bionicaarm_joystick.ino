#include <BionicGlove.h>
#include <Servo.h>

#define DELAYPADRAO 20

#define BASE_PIN 26
#define ALTURA_PIN 27
#define GARRA_PIN 25
#define ALCANCE_PIN 33

#define BASE_ANGLE_MIN 1   // effective servo angle movement
#define BASE_ANGLE_MAX 180 //

#define ALCANCE_ANGLE_MIN 34  // effective servo angle movement
#define ALCANCE_ANGLE_MAX 155 //

#define GARRA_ANGLE_MIN 105 // effective servo angle movement
#define GARRA_ANGLE_MAX 160 //

#define ALTURA_ANGLE_MIN 35  // effective servo angle movement
#define ALTURA_ANGLE_MAX 110 //

Servo servoBase, servoAlcance, servoGarra, servoAltura;
uint32_t nextWrite;
int baseAng, alcanceAng, garraAng, alturaAng;

BionicGlove bionic;

void setup()
{
  Serial.begin(115200);
  Serial.println("debugging..");

  servoBase.attach(BASE_PIN);
  servoAlcance.attach(ALCANCE_PIN);
  servoGarra.attach(GARRA_PIN);
  servoAltura.attach(ALTURA_PIN);

  baseAng = (BASE_ANGLE_MIN + BASE_ANGLE_MAX) / 2;
  servoBase.write(baseAng);
  delay(DELAYPADRAO);
  alcanceAng = (ALCANCE_ANGLE_MAX + ALCANCE_ANGLE_MIN) / 2;
  servoAlcance.write(alcanceAng);
  delay(DELAYPADRAO);
  garraAng = (GARRA_ANGLE_MIN + GARRA_ANGLE_MAX) / 2;
  servoGarra.write(garraAng);
  delay(DELAYPADRAO);
  alturaAng = (ALTURA_ANGLE_MAX + ALTURA_ANGLE_MIN) / 2;
  servoAltura.write(alturaAng);
  delay(DELAYPADRAO);

  bionic.setAxleMinThresholdAngle(AXL_X, 50); // aumenta a sensibilidade
  bionic.setAxleMaxThresholdAngle(AXL_X, 50);
  bionic.setAxleMinThresholdAngle(AXL_Y, 50); // aumenta a sensibilidade
  bionic.setAxleMaxThresholdAngle(AXL_Y, 50);
  bionic.setAllAxleThresholdAngle(15); // diminui a sensibilidade

  bionic.start();
  Serial.println("end setup");
}

void loop()
{
  if (bionic.read())
  {
    if (millis() > nextWrite) // nunca use delay() !!!!!
    {
      nextWrite = millis() + DELAYPADRAO;
      if (bionic.getAxleMinStatus(AXL_X))
      {
        baseAng = constrain(baseAng++, BASE_ANGLE_MIN, BASE_ANGLE_MAX);
        servoBase.write(baseAng);
      }
      else if (bionic.getAxleMaxStatus(AXL_X))
      {
        baseAng = constrain(baseAng--, BASE_ANGLE_MIN, BASE_ANGLE_MAX);
        servoBase.write(baseAng);
      }
      if (bionic.getAxleMinStatus(AXL_Y))
      {
        alcanceAng = constrain(alcanceAng++, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX);
        servoAlcance.write(alcanceAng);
      }
      else if (bionic.getAxleMaxStatus(AXL_Y))
      {
        alcanceAng = constrain(alcanceAng--, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX);
        servoAlcance.write(alcanceAng);
      }
      if (bionic.getFingerClosedStatus(DATA_F_INDEX))
      {
        alturaAng = constrain(alturaAng--, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX);
        servoAltura.write(alturaAng);
      }
      else if (bionic.getFingerOpenedStatus(DATA_F_INDEX))
      {
        alturaAng = constrain(alturaAng++, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX);
        servoAltura.write(alturaAng);
      }

      if (bionic.getFingerClosedStatus(DATA_F_LITTLE))
      {
        garraAng = constrain(garraAng++, GARRA_ANGLE_MIN, GARRA_ANGLE_MAX);
        servoGarra.write(garraAng);
      }
      else if (bionic.getFingerOpenedStatus(DATA_F_LITTLE))
      {
        garraAng = constrain(garraAng--, GARRA_ANGLE_MIN, GARRA_ANGLE_MAX);
        servoGarra.write(garraAng);
      }
    }
  }
}
