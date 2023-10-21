#include <BionicGlove.h>
#include <Servo.h>

#define DELAYPADRAO 20

#define BASE_PIN 26
#define ALTURA_PIN 27
#define GARRA_PIN 25
#define ALCANCE_PIN 33

#define BASE_ANGLE_MIN 1.0   // effective servo angle movement
#define BASE_ANGLE_MAX 180.0 //

#define ALCANCE_ANGLE_MIN 34.0  // effective servo angle movement
#define ALCANCE_ANGLE_MAX 155.0 //

#define GARRA_ANGLE_MIN 105.0 // effective servo angle movement
#define GARRA_ANGLE_MAX 160.0 //

#define ALTURA_ANGLE_MIN 35.0  // effective servo angle movement
#define ALTURA_ANGLE_MAX 110.0 //

Servo servoBase, servoAlcance, servoGarra, servoAltura;
uint32_t nextWrite;
float baseAng, alcanceAng, garraAng, alturaAng;

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

    bionic.setAllAxleThresholdAngle(60);
    bionic.setAllFingersThresholdPercentage(15);

    bionic.start();
    Serial.println("end setup");
}

void loop()
{
    if (bionic.read())
    {
        if (bionic.getAxleMinStatus(AXL_X) || bionic.getAxleMaxStatus(AXL_X))
        {
            baseAng -= bionic.getUnitSmoothed(DATA_A_X_ANG);
            baseAng = constrain(baseAng, BASE_ANGLE_MIN, BASE_ANGLE_MAX);
            servoBase.write(baseAng);
        }
        if (bionic.getAxleMinStatus(AXL_Y) || bionic.getAxleMaxStatus(AXL_Y))
        {
            alcanceAng -= bionic.getUnitSmoothed(DATA_A_Y_ANG);
            alcanceAng = constrain(alcanceAng, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX);
            servoAlcance.write(alcanceAng);
        }
        if (bionic.getFingerClosedStatus(DATA_F_INDEX) || bionic.getFingerOpenedStatus(DATA_F_INDEX))
        {
            alturaAng += bionic.getUnitSmoothed(DATA_F_INDEX)/8;
            alturaAng = constrain(alturaAng, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX);
            servoAltura.write(alturaAng);
        }
        if (bionic.getFingerClosedStatus(DATA_F_LITTLE) || bionic.getFingerOpenedStatus(DATA_F_LITTLE))
        {
            garraAng -= bionic.getUnitSmoothed(DATA_F_LITTLE)/8;
            garraAng = constrain(garraAng, GARRA_ANGLE_MIN, GARRA_ANGLE_MAX);
            servoGarra.write(garraAng);
        }
    }
}
