
int numero = 0;       // Número inicial
int valorVariavel1 = 0; // Variável para controlar o incremento/decremento baseado na primeira variável
float valorVariavel2 = 0; // Variável para controlar o "scroll" do número

void setup() {
  size(400, 300);
  textSize(100);
}

void draw() {
  background(255);
  
  // Atualiza o número com base na primeira variável
  numero += valorVariavel1;
  
  // Atualiza o número com base na segunda variável (scroll)
  numero += int(valorVariavel2);
  
  // Mostra o número grande no centro da tela
  textAlign(CENTER, CENTER);
  text(numero, width/2, height/2);
}

// Função para receber os valores da primeira variável (0-180) e da segunda variável (scroll)
void receberDados(int valor1, float valor2) {
  valorVariavel1 = valor1;
  valorVariavel2 = valor2;
}
