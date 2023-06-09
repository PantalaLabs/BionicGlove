import processing.serial.*;
Serial arduino;
float angleX, angleZ;
boolean z;

void setup() {
  size(800, 600, P3D); // Define o tamanho da janela

  // Verifica as portas disponíveis e conecta ao Arduino
  arduino = new Serial(this, "COM20", 38400); // Altere a porta e velocidade se necessário
  arduino.bufferUntil('\n'); // Lê os dados quando uma nova linha é recebida
}

void draw() {
  background(0); // Fundo branco

  // Atualiza os ângulos com base nos dados recebidos do Arduino
  if (arduino != null && arduino.available() > 0) {
    String[] data = arduino.readString().trim().split(",");
    angleZ = radians(Integer.parseInt(data[0]));
    angleX = radians(Integer.parseInt(data[1]));
  }

  // Define a posição do cubo no centro da tela
  translate(width / 2, height / 2, 0);

  // Rotaciona o cubo nas coordenadas X e Y
  rotateX(angleX);
  rotateZ(angleZ);
  box(200);
}
