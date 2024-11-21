#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "src/braco.h"
#include "src/conexao.h"
#include "src/servidor.h"

Braco braco;
Conexao conexao;
Servidor servidor(80);

// const int encoderPinA = 13;  // DT
// const int encoderPinB = 14;  // CLK
// const int buttonPin = 12;    // SW

// volatile int lastStateA = LOW;
// volatile int lastStateB = LOW;

// volatile int encoderValue = 0;

// void IRAM_ATTR encoderISR() {

//   int stateA = digitalRead(encoderPinA);
//   int stateB = digitalRead(encoderPinB);

//   // Detectar a direção de rotação
//   if (stateA != lastStateA) {
//     if (stateB != stateA) {
//       encoderValue++;  // Sentido horário
//     } else {
//       encoderValue--;  // Sentido anti-horário
//     }
//   }

//   lastStateA = stateA;
//   lastStateB = stateB;
// }


void setup() {

  Serial.begin(115200);
  
  braco.iniciar();
  conexao.iniciar();
  conexao.conectar();
  servidor.iniciar();
  
  // pinMode(encoderPinA, INPUT);
  // pinMode(encoderPinB, INPUT);
  // pinMode(buttonPin, INPUT_PULLUP);

  // Serial.println("Teste de leitura do encoder");
  // attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
  
  // braco.set_posicaoBraco({50,50,50,50,50,50}, 70);

}

int motorSelecionado = 0;

void loop() {

  // Rotina para ajuste da posição via serial:

  // int motor, angulo, velocidade;

  // Serial.print("Digite o número do motor (0 a 5): ");
  // while (Serial.available() == 0);
  // motor = Serial.parseInt();   
  // Serial.println(motor);     
  // Serial.read(); 

  // Serial.print("Digite o ângulo (0 a 100): ");
  // while (Serial.available() == 0);   
  // angulo = Serial.parseInt();        
  // Serial.println(angulo);     
  // Serial.read(); 

  // Serial.print("Digite o delay (ms) [velocidade]: ");
  // while (Serial.available() == 0);   
  // velocidade = Serial.parseInt();    
  // Serial.println(velocidade);     
  // Serial.read(); 
  
  // braco.set_anguloMotor(motor, angulo, velocidade);

  // --------------------------------------------------------------

  // Rotina para posição salva em memória:

  // braco.set_posicaoBraco(0, 50);
  // braco.fechar_garra();
  // delay(5000);
  // braco.set_posicaoBraco(1, 50);
  // braco.abrir_garra();
  // delay(5000);
  
  // --------------------------------------------------------------

  // Rotaciona motores de acordo com a rotação do encoder
  // braco.set_anguloMotor(motorSelecionado, encoderValue, 50);

  // // Botão do encoder troca motor a regular
  // if (digitalRead(buttonPin) == LOW) {
  //   motorSelecionado++;
  //   if(motorSelecionado == 6) motorSelecionado = 0;
  //   encoderValue = braco.get_anguloMotor(motorSelecionado);
  //   Serial.println("Botão pressionado!");
  //   delay(200);  // Debounce do botão
  // }

}
