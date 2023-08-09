import processing.serial.*;
import java.awt.event.KeyEvent;
import java.io.IOException;

Serial myPort;

String data="";
float y, x;

void setup() {
  size (960, 640, P3D);
  myPort = new Serial(this, "COM7", 115200); // starts the serial communication
  myPort.bufferUntil('\n');
}

void draw() {
  translate(width/2, height/2, 0);
  background(33);
  //textSize(22);
  //text("y: " + int(y) + "     x: " + int(x), -100, 265);

  // Rotate the object
  rotateX(radians(y));
  rotateY(radians(x));
  rotateZ(radians(-90.0));
  // 3D 0bject
  fill(0, 76, 153);
  box (386, 40, 200);
}

void serialEvent (Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = trim(data);
    String items[] = split(data, ' '); // split the string at "/"
    if (items.length > 1) {
      x = -float(items[0]);
      y = -float(items[1]);
    }
  }
}
