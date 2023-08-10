import processing.serial.*;
Serial myPort;
String data="";
int counter = 0;
int startCounter = 0;
boolean sign;
boolean animating = false;
int defaultInterval = 20;
int interval;
int lastTick = 0;

void setup() {
  size(700, 700);
  textSize(300);
  interval = defaultInterval;
  myPort = new Serial(this, "COM7", 115200); // starts the serial communication
  myPort.bufferUntil('\n');
}

void serialEvent (Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = trim(data);
    String items[] = split(data, ' ');
    if (items.length == 2) {
      int command = int(items[0]);
      if (command == 1)
      {
        interval = defaultInterval;
        sign = true;
        animating = true;
        lastTick = millis(); // Inicializa o tempo da última atualização
      } else if (command == 2)
      {
        interval = defaultInterval;
        sign = false;
        animating = true;
        lastTick = millis(); // Inicializa o tempo da última atualização
      } else if (command == 3)
      {
        animating = false;
        startCounter = counter;
      } else if (command == 4)
      {
        animating = false;
        counter = startCounter + int(items[1]);
      }
    }
  }
}


void draw() {
  background(0);
  if (animating) {
    int now = millis();
    if ((now - lastTick) >= interval) {
      lastTick = now;
      if (sign) {
        counter++;
      } else {
        counter--;
      }
      interval += interval/8;
      if (interval > 150)
      {
        animating = false;
      }
    }
  }
  textAlign(CENTER, CENTER);
  text(counter, width/2, (height/2) - 30);
}
