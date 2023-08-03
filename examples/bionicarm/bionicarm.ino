#include <BionicGlove.h>
#include <Servo.h>

#define MAXSERVOS 4

#define DELAYPADRAO 30

#define POT_PIN 34

#define BASE_PIN 26
#define ALTURA_PIN 27
#define GARRA_PIN 25
#define ALCANCE_PIN 33

#define BASE_ESQUERDA 180
#define BASE_DIREITA 1

#define GARRA_FECHADA 80
#define GARRA_ABERTA 180

#define ALTURA_ALTO 155
#define ALTURA_BAIXO 60

#define ALCANCE_CURTO 160
#define ALCANCE_LONGO 30

Servo servoBase, servoAlcance, servoGarra, servoAltura;
Servo *servos[4] = {&servoBase, &servoAlcance, &servoGarra, &servoAltura};
int smoothedRead[MAXSERVOS], newAngles[MAXSERVOS];
uint32_t nextWrite;

BionicGlove bionic;

void setup()
{
    Serial.begin(115200);
    Serial.println("debugging..");

    servoBase.attach(BASE_PIN);
    servoAlcance.attach(ALCANCE_PIN);
    servoGarra.attach(GARRA_PIN);
    servoAltura.attach(ALTURA_PIN);

    // reset arm
    servoBase.write((BASE_ESQUERDA + BASE_DIREITA / 2));
    delay(DELAYPADRAO);
    servoAlcance.write((ALCANCE_LONGO + ALCANCE_CURTO) / 2);
    delay(DELAYPADRAO);
    servoGarra.write((GARRA_FECHADA + GARRA_ABERTA) / 2);
    delay(DELAYPADRAO);
    servoAltura.write((ALTURA_ALTO + ALTURA_BAIXO) / 2);
    delay(DELAYPADRAO);

    bionic.start();
    Serial.println("end setup");
}

void loop()
{
    if (bionic.read())
    {
        ALPHAFILTER(smoothedRead[0], map(bionic.getRaw(DATA_A_X_ANG), 0, 180, BASE_ESQUERDA, BASE_DIREITA), bionic.getRaw(DATA_SMOOTHFACTOR));  // base
        ALPHAFILTER(smoothedRead[1], map(bionic.getRaw(DATA_A_Y_ANG), 0, 180, ALCANCE_CURTO, ALCANCE_LONGO), bionic.getRaw(DATA_SMOOTHFACTOR)); // altura
        ALPHAFILTER(smoothedRead[2], map(bionic.getRaw(DATA_F_INDEX), 0, 512, GARRA_ABERTA, GARRA_FECHADA), bionic.getRaw(DATA_SMOOTHFACTOR));  // garra
        ALPHAFILTER(smoothedRead[3], map(bionic.getRaw(DATA_F_LITTLE), 0, 512, ALTURA_BAIXO, ALTURA_ALTO), bionic.getRaw(DATA_SMOOTHFACTOR));   // alcance
        // nunca use delay() !!!!!
        if (millis() > nextWrite)
        {
            nextWrite = millis() + DELAYPADRAO;
            for (uint8_t i = 0; i < MAXSERVOS; i++)
                servos[i]->write(smoothedRead[i]);
        }
    }
}