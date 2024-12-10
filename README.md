# Jogo da Forca com Arduino 🎮

## Descrição

O **Jogo da Forca com Arduino** é um projeto divertido e interativo desenvolvido para a matéria de Sistemas Embarcados. Ele utiliza um display gráfico para exibir a interface do jogo, botões físicos para navegação no teclado digital e um buzzer para sons interativos. O jogador deve adivinhar a palavra antes de cometer 6 erros, acompanhando o progresso diretamente no display.

---

## Componentes Utilizados

- **Arduino UNO** (ou compatível)
- **Display Gráfico TFT LCD 1.8" ST7735 (128x160)**
- **5 Botões Físicos** (para movimentação e seleção no teclado)
  - Botão para cima
  - Botão para baixo
  - Botão para esquerda
  - Botão para direita
  - Botão de seleção
- **Buzzer** (para sons interativos)
- **Jumpers e Protoboard** (para conexões)

---

## Como Utilizar

### 1. Preparar o Hardware

Monte os componentes conforme o esquema abaixo:

- **Display TFT ST7735 (128x160)**
  - **CS (Chip Select):** Pino 10
  - **RST (Reset):** Pino 8
  - **DC (Data/Command):** Pino 9
- **Botões Físicos**
  - **Botão UP:** Pino 3
  - **Botão DOWN:** Pino 4
  - **Botão LEFT:** Pino 5
  - **Botão RIGHT:** Pino 6
  - **Botão SELECT:** Pino 7
- **Buzzer:** Pino 2

### 2. Configurar o Ambiente

Certifique-se de que o **Arduino IDE** está instalado em seu computador.

- Baixe e instale as bibliotecas necessárias:
  - `Adafruit_GFX`
  - `Adafruit_ST7735`

### 3. Carregar o Código

- Abra o arquivo `jogodaforca.ino` no Arduino IDE.
- Conecte o Arduino ao computador via USB.
- Selecione a porta e a placa correta no Arduino IDE.
- Clique em **Upload** para carregar o código no Arduino.

---

## Funcionalidades

- **Teclado Digital:** O jogador utiliza botões para navegar pelo teclado digital no display e selecionar letras.
- **Feedback Sonoro:** Sons interativos são emitidos pelo buzzer, indicando acertos, erros e vitória/derrota.
- **Display Gráfico:** Exibe a palavra oculta, erros cometidos, e o esqueleto da forca de forma clara e intuitiva.

---

## Como o Código Funciona

1. **Inicialização:**  
   O display, botões e buzzer são configurados na função `setup()`. Uma palavra é escolhida aleatoriamente para o jogador adivinhar.

2. **Interação do Jogador:**  
   No `loop()`, os botões são lidos para movimentar o cursor no teclado ou selecionar uma letra.

3. **Atualização do Jogo:**  
   A letra selecionada é processada:

   - Se correta, aparece na palavra oculta.
   - Se errada, incrementa o contador de erros e desenha uma parte do boneco no display.

4. **Finalização:**  
   O jogo termina com vitória (todas as letras adivinhadas) ou derrota (6 erros).

---

## Resultado

O jogador pode acompanhar seu progresso diretamente no display, com feedback visual e sonoro, tornando a experiência mais dinâmica e imersiva.

---

## Licença

Este projeto está disponível sob a [Licença MIT](LICENSE).
