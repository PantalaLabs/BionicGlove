#include <BionicGlove.h>
#include <Servo.h>

// #define DO_CALIBRATE true

#define MAXSERVOS 4

#define DELAYPADRAO 20

#define POT_PIN 34

#define BASE_PIN 26
#define ALTURA_PIN 27
#define GARRA_PIN 25
#define ALCANCE_PIN 33

#define BASE_READ_MIN 40   // analogRead angle x
#define BASE_READ_MAX 140  //
#define BASE_ANGLE_MIN 1   // effective servo angle movement
#define BASE_ANGLE_MAX 180 //

#define ALCANCE_READ_MIN 70 // analogRead angle y
#define ALCANCE_READ_MAX 160
#define ALCANCE_ANGLE_MIN 34  // effective servo angle movement
#define ALCANCE_ANGLE_MAX 155 //

#define GARRA_READ_MIN 20 // analogRead index finger
#define GARRA_READ_MAX 250
#define GARRA_ANGLE_MIN 105 // effective servo angle movement
#define GARRA_ANGLE_MAX 160 //

#define ALTURA_READ_MIN 35 // analogRead little finger
#define ALTURA_READ_MAX 470
#define ALTURA_ANGLE_MIN 35  // effective servo angle movement
#define ALTURA_ANGLE_MAX 110 //

Servo servoBase, servoAlcance, servoGarra, servoAltura;
Servo *servos[4] = {&servoBase, &servoAlcance, &servoGarra, &servoAltura};
int cropped[MAXSERVOS];
float smoothedRead[MAXSERVOS];
uint32_t nextWrite;
float smoothness;

BionicGlove bionic;

void setup()
{
  Serial.begin(115200);
  Serial.println("debugging..");

  servoBase.attach(BASE_PIN);
  servoAlcance.attach(ALCANCE_PIN);
  servoGarra.attach(GARRA_PIN);
  servoAltura.attach(ALTURA_PIN);

  servoBase.write((BASE_ANGLE_MIN + BASE_ANGLE_MAX) / 2);
  delay(DELAYPADRAO);
  servoAlcance.write((ALCANCE_ANGLE_MAX + ALCANCE_ANGLE_MIN) / 2);
  delay(DELAYPADRAO);
  servoGarra.write((GARRA_ANGLE_MIN + GARRA_ANGLE_MAX) / 2);
  delay(DELAYPADRAO);
  servoAltura.write((ALTURA_ANGLE_MAX + ALTURA_ANGLE_MIN) / 2);
  delay(DELAYPADRAO);

#ifdef DO_CALIBRATE
  while (true)
  {
    int pot = map(analogRead(POT_PIN), 0, 4096, 1, 180);
    servoAltura.write(pot);
    Serial.println(pot);
    delay(DELAYPADRAO);
  }
#endif

  bionic.start();
  Serial.println("end setup");
}

void loop()
{
  if (bionic.read())
  {
    // crop Bionic Glove reading to create more responsiveness = less hand movement -> more arm movement
    cropped[0] = constrain(bionic.getRaw(DATA_A_X_ANG), BASE_READ_MIN, BASE_READ_MAX);
    cropped[1] = constrain(bionic.getRaw(DATA_A_Y_ANG), ALCANCE_READ_MIN, ALCANCE_READ_MAX);
    cropped[2] = constrain(bionic.getRaw(DATA_F_LITTLE), GARRA_READ_MIN, GARRA_READ_MAX);
    cropped[3] = constrain(bionic.getRaw(DATA_F_INDEX), ALTURA_READ_MIN, ALTURA_READ_MAX);

    smoothness = bionic.getRaw(DATA_SMOOTHFACTOR);
    ALPHAFILTER(smoothedRead[0], map(cropped[0], BASE_READ_MIN, BASE_READ_MAX, BASE_ANGLE_MAX, BASE_ANGLE_MIN), 0.6 * smoothness);         // base
    ALPHAFILTER(smoothedRead[1], map(cropped[1], ALCANCE_READ_MAX, ALCANCE_READ_MIN, ALCANCE_ANGLE_MIN, ALCANCE_ANGLE_MAX), 0.8* smoothness);   // altura
    ALPHAFILTER(smoothedRead[2], map(cropped[2], GARRA_READ_MAX, GARRA_READ_MIN, GARRA_ANGLE_MIN, GARRA_ANGLE_MAX), smoothness);           // garra
    ALPHAFILTER(smoothedRead[3], map(cropped[3], ALTURA_READ_MIN, ALTURA_READ_MAX, ALTURA_ANGLE_MIN, ALTURA_ANGLE_MAX), 0.6 * smoothness); // alcance

    if (millis() > nextWrite) // nunca use delay() !!!!!
    {
      nextWrite = millis() + DELAYPADRAO;
      for (uint8_t i = 0; i < MAXSERVOS; i++)
      {
        servos[i]->write(smoothedRead[i]);
      }
      Serial.print(String(bionic.getRaw(DATA_A_X_ANG)) + " " + smoothedRead[0] + "\t | ");
      Serial.print(String(bionic.getRaw(DATA_A_Y_ANG)) + " " + smoothedRead[1] + "\t | ");
      Serial.print(String(bionic.getRaw(DATA_F_INDEX)) + " " + smoothedRead[2] + "\t | ");
      Serial.print(String(bionic.getRaw(DATA_F_LITTLE)) + " " + smoothedRead[3] + "\t | ");
      Serial.print(String(bionic.getRaw(DATA_SMOOTHFACTOR)) + "\t | ");
      Serial.println("");
    }
  }
}