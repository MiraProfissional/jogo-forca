#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ST7735.h"

// Configuração da tela TFT
#define TFT_CS     10
#define TFT_RST    8
#define TFT_A0     9  // A0 no lugar do DC
#define buzzerPin  2

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_A0, TFT_RST);

// Configuração dos pinos dos botões
#define UP_BUTTON 3
#define DOWN_BUTTON 4
#define RIGHT_BUTTON 6
#define LEFT_BUTTON 5
#define SELECT_BUTTON 7

bool alreadySelected[26] = {false};

// Configuração do teclado QWERTY
char keyboard[3][10] = {
  {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
  {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ' '},
  {'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' ', ' ', ' '}
};

// Array de palavras para o jogo
const char *wordList[] = {"HELLO", "WORLD", "CARECA", "FORCA", "JOGO", "CODIGO"};
const int wordCount = sizeof(wordList) / sizeof(wordList[0]);
const char *targetWord; // Palavra a ser adivinhada
char guessedWord[10]; // Palavra oculta (tamanho ajustável)
int mistakes = 0; // Contador de erros

// Posição do cursor no teclado
int row = 0, col = 0;

void setup() {
  tft.initR(INITR_BLACKTAB);  // Deve ser ajustado conforme o modelo do display (GREENTAB, REDTAB, BLACKTAB), BLACKTAB no caso
  tft.setRotation(2);  // Definir orientação do display
  tft.fillScreen(ST7735_BLACK);

  // Configurar pinos dos botões como entrada
  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(SELECT_BUTTON, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Escolher palavra aleatória e inicializar o jogo
  randomSeed(analogRead(0)); // Inicializar aleatoriedade
  targetWord = wordList[random(wordCount)];
  initializeGuessedWord();

  // Exibir a palavra oculta
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setCursor(10, 20);
  tft.print("Palavra: ");
  tft.print(guessedWord);

  // Exibir o contador de erros
  displayMistakes();

  // Desenhar teclado QWERTY
  drawKeyboard();

  // Destaque inicial da letra
  highlightLetter(row, col, ST7735_YELLOW);
}

void loop() {
  // Ler o estado dos botões e mover o cursor
  if (digitalRead(UP_BUTTON) == HIGH) { moveCursor(-1, 0); delay(200); }
  if (digitalRead(DOWN_BUTTON) == HIGH) { moveCursor(1, 0); delay(200); }
  if (digitalRead(LEFT_BUTTON) == HIGH) { moveCursor(0, -1); delay(200); }
  if (digitalRead(RIGHT_BUTTON) == HIGH) { moveCursor(0, 1); delay(200); }
  if (digitalRead(SELECT_BUTTON) == HIGH) { selectLetter(); delay(200); }
}

// Função para inicializar a palavra oculta
void initializeGuessedWord() {
  int len = strlen(targetWord);
  for (int i = 0; i < len; i++) {
    guessedWord[i] = '_';
  }
  guessedWord[len] = '\0'; // Finalizar a string
}

// Função para desenhar o teclado
void drawKeyboard() {
  tft.setTextSize(1);
  int yOffset = 90; // Posição vertical do teclado (ajustada para 128x160)
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 10; j++) {
      if (keyboard[i][j] != ' ') {
        tft.setCursor(10 + j * 12, yOffset + i * 20); // Ajuste do espaçamento
        tft.setTextColor(ST7735_WHITE);
        tft.print(keyboard[i][j]);
      }
    }
  }
}

// Função para mover o cursor
void moveCursor(int dRow, int dCol) {
  highlightLetter(row, col, ST7735_WHITE); // Apaga o destaque anterior

  row = constrain(row + dRow, 0, 2);
  col = constrain(col + dCol, 0, 9);

  highlightLetter(row, col, ST7735_YELLOW); // Novo destaque
}

// Função para destacar uma letra
void highlightLetter(int r, int c, uint16_t color) {
  char letter = keyboard[r][c];
  if(alreadySelected[letter - 'A']) {
    color = ST7735_RED;  
  }
  tft.setCursor(10 + c * 12, 90 + r * 20);
  tft.setTextColor(color);
  tft.print(keyboard[r][c]);
}

// Função para exibir o contador de erros
void displayMistakes() {
  tft.fillRect(10, 50, 100, 20, ST7735_BLACK); // Limpa a área do contador
  tft.setCursor(10, 50);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.print("Erros: ");
  tft.print(mistakes);
}

void lostSound() {
  // Frequências em Hertz e durações em milissegundos
  tone(buzzerPin, 440, 300); // A4
  delay(350);
  tone(buzzerPin, 349, 300); // F4
  delay(350);
  tone(buzzerPin, 523, 300); // C5
  delay(350);
  tone(buzzerPin, 440, 300); // A4
  delay(350);
  tone(buzzerPin, 349, 300); // F4
  delay(350);
  tone(buzzerPin, 440, 300); // A4
  delay(350);
}

void victorySound() {
  // Frequências em Hertz e durações em milissegundos
  tone(buzzerPin, 523, 300); // C5 (Dó)
  delay(350);
  tone(buzzerPin, 659, 300); // E5 (Mi)
  delay(350);
  tone(buzzerPin, 784, 300); // G5 (Sol)
  delay(350);
  tone(buzzerPin, 523, 300); // C5 (Dó)
  delay(350);
  tone(buzzerPin, 784, 300); // G5 (Sol)
  delay(350);
  tone(buzzerPin, 1047, 300); // C6 (Dó)
  delay(350);
}

void errorSound() {
    tone(buzzerPin, 500);   // Frequência de 500 Hz
    delay(500);               // Duração de 500 ms
    noTone(buzzerPin);        // Desliga o som  
}

void hitSound() {
    tone(buzzerPin, 1000);   // Frequência inicial de 1000 Hz
    delay(100);               // Duração de 100 ms
    tone(buzzerPin, 1500);   // Frequência de 1500 Hz
    delay(100);               // Duração de 100 ms
    tone(buzzerPin, 2000);   // Frequência final de 2000 Hz
    delay(100);               // Duração de 100 ms
    noTone(buzzerPin);        // Desliga o som
}

// Função para processar a letra selecionada
void selectLetter() {
  char selectedLetter = keyboard[row][col];

  // Verifica se a 
  if(alreadySelected[selectedLetter - 'A']) {
    return;  
  }
  
  // selectedLetter - 'A'
  alreadySelected[selectedLetter - 'A'] = true;
  // Marcar a letra como usada, alterando sua cor no teclado
  highlightLetter(row, col, ST7735_BLACK);

  // Verificar se a letra faz parte da palavra
  bool found = false;
  for (int i = 0; i < strlen(targetWord); i++) {   
    if (targetWord[i] == selectedLetter) {
      guessedWord[i] = selectedLetter;
      found = true;
    }
  }

  // Atualizar a palavra oculta na tela
  tft.fillRect(10, 20, 100, 20, ST7735_BLACK); // Limpa a área da palavra
  tft.setCursor(10, 20);
  tft.setTextColor(ST7735_WHITE);
  tft.print("Palavra: ");
  tft.print(guessedWord);
  
  // Atualizar o contador de erros se a letra não foi encontrada
  if (!found) {
    mistakes++;
    errorSound();
    displayMistakes();
    drawHangmanPart();
  }
  else {
    hitSound();
    }
  
  // Condição de vitória/derrota
  if (strcmp(guessedWord, targetWord) == 0) {
    tft.setCursor(30, 60);
    tft.setTextColor(ST7735_GREEN);
    tft.print("Venceu!");
    victorySound();
    delay(2000);
    resetGame();
  } else if (mistakes >= 6) {
    tft.setCursor(30, 60);
    tft.setTextColor(ST7735_RED);
    tft.print("Perdeu!"); 
    lostSound();
    delay(2000);
    resetGame();
  }
}

// Função para desenhar as partes do boneco da forca
void drawHangmanPart() {
  int x = 110; // Posição central do boneco
  int y = 50;  // Posição inicial do boneco
  
  switch (mistakes) {
    case 1: 
      tft.drawCircle(x, y, 8, ST7735_WHITE); // Cabeça simplificada
      break; 
    case 2: 
      tft.drawLine(x, y + 8, x, y + 25, ST7735_WHITE); // Corpo
      break; 
    case 3: 
      tft.drawLine(x, y + 15, x - 10, y + 25, ST7735_WHITE); // Braço esquerdo
      break; 
    case 4: 
      tft.drawLine(x, y + 15, x + 10, y + 25, ST7735_WHITE); // Braço direito
      break; 
    case 5: 
      tft.drawLine(x, y + 25, x - 10, y + 40, ST7735_WHITE); // Perna esquerda
      break; 
    case 6: 
      tft.drawLine(x, y + 25, x + 10, y + 40, ST7735_WHITE); // Perna direita
      break;
  }
}


// Função para reiniciar o jogo
void resetGame() {
  mistakes = 0;
  row = 0; col = 0;
  for(int i = 0; i < 26; i++) {
    alreadySelected[i] = false;
  }
  targetWord = wordList[random(wordCount)]; // Escolher nova palavra aleatória
  initializeGuessedWord();

  tft.fillScreen(ST7735_BLACK);
  setup(); // Recomeça o setup do jogo
}
