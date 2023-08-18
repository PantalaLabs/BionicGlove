import processing.serial.*;
Serial myPort;
String data="";

int cols, rows;  // Número de colunas e linhas na malha
float w = 20;    // Tamanho de cada célula
float b = 14;    // Tamanho de cada bolha
PVector[][] grid; // Matriz para armazenar os pontos da malha
float bionicXindex, bionicYindex;
float minPressure = 0.1;
float maxPressure = 2.0;
int numFingers = 4;    // Número máximo de dedos de pressão
PVector[] fingerPos;   // Armazena as posições dos dedos
boolean[] touched; // Indica se cada dedo está pressionado
float[] fingerPressure;
float p = 350.0;

void setup() {
  size(1000, 800);
  cols = width / int(w);
  rows = height / int(w);
  grid = new PVector[cols][rows];
  fingerPos = new PVector[numFingers];
  touched = new boolean[numFingers];
  fingerPressure = new float[numFingers];

  // Inicializa a matriz da malha
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      grid[i][j] = new PVector(i * w, j * w);
    }
  }
  myPort = new Serial(this, "COM7", 115200); // starts the serial communication
  myPort.bufferUntil('\n');
}

void draw() {
  background(255);

  // Atualiza as posições dos dedos
  fingerPos[0] = new PVector(bionicXindex, bionicYindex);
  fingerPos[1] = new PVector(bionicXindex + 150, bionicYindex - 40);
  fingerPos[2] = new PVector(bionicXindex + 300, bionicYindex - 30);
  fingerPos[3] = new PVector(bionicXindex + 450, bionicYindex + 40);

  // Desenha a malha
  for (int i = 0; i < cols; i++) {
    for (int j = 0; j < rows; j++) {
      float x = grid[i][j].x;
      float y = grid[i][j].y;

      float offset = 0;
      for (int k = 0; k < numFingers; k++) {
        if (touched[k]) {
          float d = dist(fingerPos[k].x, fingerPos[k].y, x, y);
          if (d < 50) {
            offset += map(d, 0, 50, -15 * fingerPressure[k], 0); // Diminui o tamanho no ponto de pressão
          } else if (d >= 50 && d < 100) {
            offset += map(d, 50, 100, 0, 10 * fingerPressure[k]); // Diminui gradualmente nos arredores
          }
        }
      }

      x += offset;
      y += offset;

      noFill();
      ellipse(x, y, b, b);
      fill(0);
      text("o", x, y);
    }
  }
}

float newY;
void serialEvent (Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = trim(data);
    String items[] = split(data, ' ');
    if (items.length > 1) {
      bionicXindex = map(float(items[0]), 0, 180, 0, width - (width/2) );
      newY = float(items[1]);
      newY = constrain(newY, 70, 140);
      newY = map(newY, 50, 120, height, 260)+280;
      bionicYindex = newY;
      touched[0] = boolean(int(items[2]));
      touched[1] = boolean(int(items[3]));
      touched[2] = boolean(int(items[4]));
      touched[3] = boolean(int(items[5]));
      fingerPressure[0] = map(float(items[6]), 0, 360, maxPressure, minPressure );
      fingerPressure[1] = map(float(items[7]), 0, 360, maxPressure, minPressure );
      fingerPressure[2] = map(float(items[8]), 0, 360, maxPressure, minPressure );
      fingerPressure[3] = map(float(items[9]), 0, 360, maxPressure, minPressure );
    }
  }
}

float lin2exp(float val)
{
  return ((val * val) / 512); // lin to exp
}
