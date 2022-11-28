// --------------------------------------------- BIBLIOTECAS --------------------------------------------- //


// Controle de Motor de Passo - Modo Passo Completo alto torque (Full step)
// Motor 28BYJ48/5V com Módulo ULN20023 - Arduino UNO / IDE 2.0.0
// Uma volta no eixo = 4096 pulsos / 512 x 8 = 4096

// Referencias
// http://renatoaloi.blogspot.com/2015/09/arduino-manipulacao-direta-de-portas.html
// https://blog.eletrogate.com/guia-completo-do-motor-de-passo-28byj-48-driver-uln2003/
// https://www.youtube.com/watch?v=SkIy7oqaKnk
// https://www.youtube.com/watch?v=UhTRrjYXqPU
#include <NewPing.h>
#define MAX_DISTANCE 200
// --------------------------------------------- DEFINIÇÕES --------------------------------------------- //

byte HOR[4] = { 0x09, 0x03, 0x06, 0x0C };  // Matriz dos bytes das Fases do Motor - sentido Horário Full Step
byte AHO[4] = { 0x0C, 0x06, 0x03, 0x09 };  // Matriz dos bytes das Fases do Motor - sentido Anti-Horário Full Step
int AFL = 4;                               // Atraso de fase lento - 4ms
int AFR = 2;                               // Atraso de fase rapido - 2ms (mínimo para full step)
int intervalo = 1000;                      // Intervalo de tempo entre os movimentos do motor em ms
int start = 0;
float seno;
int freq;
int mag = 0;
int cont = 0;
int PinTrigger = 13;  // Pino usado para disparar os pulsos do sensor
int PinEcho = 12;     // Pino usado para ler a saida do sensor
NewPing sonar(PinTrigger, PinEcho, MAX_DISTANCE);
float CM = 0;
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
void Motor_HOR_DESLOC(int voltas)  // Movimento no sentido horário
{
  for (int i = 0; i < 15 * voltas; i++)  // incrementa o contador i de 0 a 511 - uma volta

    for (int j = 0; j < 4; j++)  // incrementa o contador j de 0 a 3
    {
      PORTB = HOR[j];  // Carrega bytes da Matriz HOR na Porta B
      PORTD = HOR[j];  // Carrega bytes da Matriz HOR na Porta D
      delay(AFR);      // Atraso de tempo entre as fases em milisegundos
    }
}
// --------------------------------------------- SETUP --------------------------------------------- //

void setup() {
  DDRB = DDRB | B00001111;  // B00001111; || 0x0F;    // Configura Portas D8 até D11
  PORTB = 0x00;             // Reset dos bits da Porta B (D08 a D15) ou nível lógico baixo
  DDRD = DDRD | B01001111;  // B00001111; || 0x0F;    // Configura Portas D0 a D7
  PORTD = 0x00;             // Reset dos bits da Porta D (D00 a D7) ou nível lógico baixo
  //Serial.begin(9600);
}

// --------------------------------------------- LOOP --------------------------------------------- //

void loop() {
  start = digitalRead(4);
  if (start == 1) {
    cont = 0;
    do {
      mag = digitalRead(5);
      if (mag == 1) {
        Motor_HOR_DESLOC(9);
          CM = (sonar.ping_cm());
          //Serial.println(CM);
          if (CM > 4.5 && CM < 5.5) { // molde maior
            delay(1000);
            digitalWrite(6,1);
            delay(9000);
            digitalWrite(6,0);
            delay(1000);
          }
          else if (CM > 5.5 && CM < 6.5) { // medio
            delay(1000);
            digitalWrite(6,1);
            delay(7000);
            digitalWrite(6,0);
            delay(1000);
          }
          else if (CM > 6.5 && CM < 7.5){ // menor
            delay(1000);
            digitalWrite(6,1);
            delay(4500);
            digitalWrite(6,0);
            delay(1000);
          }
          else{
            // sem molde
          }
        delay(3000);
        Motor_HOR_DESLOC(20);
        cont = cont + 1;
      } else {
        Motor_HOR();
      }
    } while (cont < 3);  // Fim de curso
    delay(1000);
    Motor_AHO(8);  // 5 para inicio
    //Serial.println("FIM");
  }
}
