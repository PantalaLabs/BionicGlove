import controlP5.*;
import processing.serial.*;

ControlP5 cp5;
Serial serialPort;

int counter1 = 0;

PImage[] digits = new PImage[10];

void setup() {
  size(400, 400);

  cp5 = new ControlP5(this);

  // Inicialização da comunicação serial
  String[] serialPorts = Serial.list();
  if (serialPorts.length > 0) {
    serialPort = new Serial(this, "COM7", 38400);
    serialPort.bufferUntil('\n');
  } else {
    println("Nenhuma porta serial encontrada!");
  }

  // Carregar as imagens dos dígitos
  for (int i = 0; i < 10; i++) {
    digits[i] = loadImage("digits/" + i + ".jpg");
  }

  // Configuração do contador 1
  cp5.addBang("increment1")
    .setPosition(4.5*width/10, 1*height/10)
    .setSize(50, 50)
    .setCaptionLabel("+")
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("decrement1")
    .setPosition(1.5*width/10, 4*height/10)
    .setSize(50, 50)
    .setCaptionLabel("-")
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  // Configuração do contador 2
  cp5.addBang("increment2")
    .setPosition(7.5*width/10, 4*height/10)
    .setSize(50, 50)
    .setCaptionLabel("+")
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("decrement2")
    .setPosition(4.5*width/10, 7*height/10)
    .setSize(50, 50)
    .setCaptionLabel("-")
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  // Inicialização dos contadores com 0
  counter1 = 0;
}

void draw() {
  clear();
  background(255);
  drawCounter(3.9*width/10, 3.5*height/10, counter1);
}

void drawCounter(float x, float y, int value) {
  int digit1 = abs(value) % 10;
  image(digits[digit1], x, y);
}

void increment1() {
  counter1++;
}

void decrement1() {
  counter1--;
}

void increment2() {
  counter1++;
}

void decrement2() {
  counter1--;
}

void serialEvent(Serial port) {
  String data = port.readStringUntil('\n');

  if (data != null) {
    data = data.trim();
    if (data.equals("1")) {
      increment1();
    } else if (data.equals("2")) {
      decrement1();
    } else if (data.equals("4")) {
      increment2();
    } else if (data.equals("3")) {
      decrement2();
    }
  }
}
