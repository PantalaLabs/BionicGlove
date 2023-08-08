import processing.serial.*;
Serial serialPort;


float ellipseWidth = 600;
float ellipseHeight = 300;
float[] circleAngles = {0, 0, 0, 0, 0};
float[] circleSizes = {1, 1, 1, 1, 1};
float circleSizeMultiplier = 60;
float circleSpeed = 2;
float maxAngle = 25;
int delayTime = 3;
int maxCircles = 5;
color[] circleColors = {color(255, 0, 0), color(0, 255, 0), color(0, 0, 255), color(255, 255, 0), color(255, 255, 255)};

void setup() {
  
  size(1000, 1000);
  // Inicialização da comunicação serial
  String[] serialPorts = Serial.list();
  if (serialPorts.length > 0) {
    serialPort = new Serial(this, "COM20", 115200);
    serialPort.bufferUntil('\n');
  } else {
    println("Nenhuma porta serial encontrada!");
  }
  smooth();
  //delay(10000); //wait until bionic glove calibrate all readings
}

void draw() {
  background(0);

  // Desenhar os círculos
  for (int i = 0; i < maxCircles; i++) {
    float angle = map(i, 0, maxCircles, 0, TWO_PI);
    float x = width/2 + ellipseWidth/2 * cos(angle + circleAngles[i]);
    float y = height/2 + ellipseHeight/2 * sin(angle + circleAngles[i]);
    float size = 30+((cos(angle + circleAngles[i]-1.5)) * circleSizeMultiplier);

    fill(circleColors[i]);
    ellipse(x, y, size, size);
  }
}

void moveCirclesRight() {
  for (int j = 0; j < maxAngle; j++) {
    for (int i = 0; i < maxCircles; i++) {
      circleAngles[i] += radians(circleSpeed);
    }
    delay(delayTime);
  }
}

void moveCirclesLeft() {

  for (int j = 0; j < maxAngle; j++) {
    for (int i = 0; i < maxCircles; i++) {
      circleAngles[i] -= radians(circleSpeed);
    }
    delay(delayTime);
  }
}

void serialEvent(Serial port) {
  String data = port.readStringUntil('\n');

  if (data != null) {
    data = data.trim();
    if (data.equals("1")) {
      moveCirclesRight();
    } else if (data.equals("2")) {
      moveCirclesLeft();
    } else if (data.equals("3")) {
      maxCircles = maxCircles - 1;
      if (maxCircles==-1) {
        maxCircles = 5;
        for (int i = 0; i < maxCircles; i++)
        {
          circleAngles[i] = 0;
        }
      }
      draw();
    }
  }
}
