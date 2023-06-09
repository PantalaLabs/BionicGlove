import controlP5.*;
import processing.serial.*;

ControlP5 cp5;

int counter1 = 0;
int counter2 = 0;
int counter3 = 0;
int counter4 = 0;
int maxDataPack = 4;

Serial arduino;  // Objeto Serial para comunicação com o Arduino

int[] dataPack = new int[maxDataPack];  // Array para armazenar os valores das dataPack
int larguraBarra = 100;
int alturaMaxima = 400;

void setup() {
  // Define o tamanho da janela
  size(600, 600);
  cp5 = new ControlP5(this);
  arduino = new Serial(this, "COM20", 38400);
  arduino.bufferUntil('\n');

  cp5.addBang("indexO")
    .setPosition(100, 50)
    .setSize(100, 50)
    .setCaptionLabel("+")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("middleO")
    .setPosition(200, 50)
    .setSize(100, 50)
    .setCaptionLabel("+")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("ringO")
    .setPosition(300, 50)
    .setSize(100, 50)
    .setCaptionLabel("+")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("littleO")
    .setPosition(400, 50)
    .setSize(100, 50)
    .setCaptionLabel("+")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);


  cp5.addBang("indexC")
    .setPosition(100, 500)
    .setSize(100, 50)
    .setCaptionLabel("-")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("middleC")
    .setPosition(200, 500)
    .setSize(100, 50)
    .setCaptionLabel("-")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("ringC")
    .setPosition(300, 500)
    .setSize(100, 50)
    .setCaptionLabel("-")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

  cp5.addBang("littleC")
    .setPosition(400, 500)
    .setSize(100, 50)
    .setCaptionLabel("-")
    .setColorForeground(color(0, 0, 255))
    .getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER);

}

void draw() {
  background(0);
  // Desenha as dataPack
  // for (int i = 0; i < dataPack.length; i++) {
  for (int i = 0; i < 4; i++) {
    int altura = (int)map(dataPack[3-i], 0, 4095, 0, alturaMaxima);
    int x = (i+1) * larguraBarra;
    int y = height - altura - 100;
    fill(255);
    rect(x, y, larguraBarra, altura);
  }
  if(maxDataPack > 4 )
  {
  if (dataPack[4]==1)
  {
    cp5.getController("littleO").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("littleO").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[5]==1)
  {
    cp5.getController("littleC").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("littleC").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[6]==1)
  {
    cp5.getController("ringO").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("ringO").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[7]==1)
  {
    cp5.getController("ringC").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("ringC").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[8]==1)
  {
    cp5.getController("middleO").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("middleO").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[9]==1)
  {
    cp5.getController("middleC").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("middleC").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[10]==1)
  {
    cp5.getController("indexO").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("indexO").setColorForeground(color(0, 0, 255));
  }

  if (dataPack[11]==1)
  {
    cp5.getController("indexC").setColorForeground(color(255, 0, 0));
  } else
  {
    cp5.getController("indexC").setColorForeground(color(0, 0, 255));
  }

  }

}

void serialEvent(Serial arduino) {
  if (arduino != null && arduino.available() > 0) {
    String[] data = arduino.readString().trim().split(",");
    if (data.length > 0) {
      for (int i = 0; i < dataPack.length; i++) {
        String valor = data[i].trim();
        try {
          dataPack[i] = Integer.parseInt(valor);  // Converte os valores em inteiros
        }
        catch (NumberFormatException e) {
          println("Erro ao converter valor: " + valor);
          e.printStackTrace();
        }
      }
    }
  }
}
