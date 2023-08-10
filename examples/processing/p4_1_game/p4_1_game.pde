import processing.serial.*;
Serial myPort;

String data="";
int bionicY;

PImage img1;
PImage img2;
int ballY;
boolean holdingBall = false;
int safe = 30;
float y;

boolean gameWon = false;
boolean gameLost = false;
int endTime = 0;

boolean ballFalling = false;
float fallingSpeed = 10;  // Velocidade de queda da bolinha
float fallingPosition;

int ballX;

  void setup() {
  size(400, 600);
  myPort = new Serial(this, "COM7", 115200); // starts the serial communication
  myPort.bufferUntil('\n');
  ballY = height - 20;
  ballX = (width/2) - 30;
  img1 = loadImage("opened.png");
  img2 = loadImage("closed.png");
}

void serialEvent (Serial myPort) {
  data = myPort.readStringUntil('\n');
  if (data != null) {
    data = trim(data);
    String items[] = split(data, ' ');
    if (items.length > 1) {
      float newY = constrain(float(items[0]), 80, 110);
      bionicY = (int)map(newY, 80, 110, height, 0);
      int newHold = int(items[1]);
      if ((newHold == 1 ) && (bionicY > (height - safe))) {
        holdingBall = true;
      } else if ((newHold == 0 ) && (bionicY < (height - safe))) {
        holdingBall = false;
      }
    }
  }
}

void draw() {
  background(220);
  if (holdingBall) {
    ballY = constrain(bionicY, 10, height - 10);
  }

  if ((ballY < safe) && !holdingBall) {
    textSize(24);
    textAlign(CENTER, CENTER);
    text("Você venceu!", width/2, height/2);

    if (!gameWon) {
      gameWon = true;
      endTime = millis() + 2000;  // Define o tempo para 2 segundos (2000 milissegundos) a partir de agora
    }
  }

  if (((ballY > safe) && (ballY < (height - safe))) && !holdingBall) {
    if (!gameLost) {
      gameLost = true;
      endTime = millis() + 2000;
      ballFalling = true;
      fallingPosition = ballY;
    }
  }

  if (holdingBall) {
    image(img2, 60 + img2.width/2, 15 + bionicY - img2.height/2);
    ellipse(ballX, ballY, 40, 40);
  } else {
    fill(255, 0, 0);
    image(img1, 60 + img1.width/2, 15 + bionicY - img1.height/2);
    if (!ballFalling) {
      ellipse(ballX, ballY, 40, 40);
    }
  }

  if (ballFalling) {
    fallingPosition += fallingSpeed;  // Atualiza a posição de queda
    fallingPosition = min(fallingPosition, height - 10);  // Limite a posição de queda
    ellipse(ballX, fallingPosition, 40, 40);
    if (fallingPosition >= height - 10) {
      ballFalling = false;  // A animação de queda terminou
      ballY = height - 20;  // Redefine a posição da bolinha para a posição inicial
      holdingBall = false;
    }
  }

  if ((gameWon || gameLost) && millis() >= endTime) {
    gameWon = false;
    gameLost = false;
    ballY = height - 20;
    holdingBall = false;
  }
}
