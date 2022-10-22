#include <Ultrasonic.h>

// --------------------------------------------- BIBLIOTECAS --------------------------------------------- //


// Controle de Motor de Passo - Modo Passo Completo alto torque (Full step)
// Motor 28BYJ48/5V com Módulo ULN20023 - Arduino UNO / IDE 2.0.0
// Uma volta no eixo = 4096 pulsos / 512 x 8 = 4096

// Referencias
// http://renatoaloi.blogspot.com/2015/09/arduino-manipulacao-direta-de-portas.html
// https://blog.eletrogate.com/guia-completo-do-motor-de-passo-28byj-48-driver-uln2003/
// https://www.youtube.com/watch?v=SkIy7oqaKnk
// https://www.youtube.com/watch?v=UhTRrjYXqPU

// --------------------------------------------- DEFINIÇÕES --------------------------------------------- //

byte HOR[4] = { 0x09, 0x03, 0x06, 0x0C };  // Matriz dos bytes das Fases do Motor - sentido Horário Full Step
byte AHO[4] = { 0x0C, 0x06, 0x03, 0x09 };  // Matriz dos bytes das Fases do Motor - sentido Anti-Horário Full Step
int AFL = 4;                               // Atraso de fase lento - 4ms
int AFR = 2;                               // Atraso de fase rapido - 2ms (mínimo para full step)
int intervalo = 1000;                      // Intervalo de tempo entre os movimentos do motor em ms
int start = 0;
float seno;
int freq;
int tst = 0;

// --------------------------------------------- FUNÇÕES --------------------------------------------- //

void Motor_AHO(int voltas)  // Movimento no sentido anti-horário
{
  for (int i = 0; i < 512 * voltas; i++)  // incrementa o contador i de 0 a 511 - uma volta

    for (int j = 0; j < 4; j++)  // incrementa o contador j de 0 a 3
    {
      PORTB = AHO[j];  // Carrega bytes da Matriz AHO na Porta B
      PORTD = AHO[j];  // Carrega bytes da Matriz AHO na Porta D
      delay(AFR);      // Atraso de tempo entre as fases em milisegundos
    }
}
void Motor_HOR()  // Movimento no sentido horário
{
  for (int i = 0; i < 15; i++)  // incrementa o contador i de 0 a 511 - uma volta

    for (int j = 0; j < 4; j++)  // incrementa o contador j de 0 a 3
    {
      PORTB = HOR[j];  // Carrega bytes da Matriz HOR na Porta B
      PORTD = HOR[j];  // Carrega bytes da Matriz HOR na Porta D
      delay(AFR);      // Atraso de tempo entre as fases em milisegundos
    }
}

// --------------------------------------------- SETUP --------------------------------------------- //

void setup() {
  DDRB = DDRB | B00001111;  // B00001111; || 0x0F;    // Configura Portas D8 até D11 como saída
  PORTB = 0x00;             // Reset dos bits da Porta B (D08 a D15) ou nível lógico baixo
  DDRD = DDRD | B01001111;  // B00001111; || 0x0F;    // Configura Portas D0 a D3 como saída
  PORTD = 0x00;             // Reset dos bits da Porta D (D00 a D7) ou nível lógico baixo
}

// --------------------------------------------- LOOP --------------------------------------------- //

void loop() {
  start = digitalRead(4);
  if (start == 1) {
    do {
      tst = digitalRead(5);
      if (tst == 1) {
        // função de injetar, reconhecer cor e injetar qtd certa
        //dar 11 passos
      } else {
        Motor_HOR();
      }
    } while (true);  // Fim de curso
    daley(1000);
    Motor_AHO(7);    // 7 para inicio
  }
}