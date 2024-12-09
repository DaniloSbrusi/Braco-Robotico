#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "src/braco.h"
#include "src/conexao.h"
#include "src/servidor.h"
#include "src/sensor.h"
#include "src/gpio.h"

Braco braco;
Conexao conexao;
Servidor servidor(80);
int modo_operacao = 0;

Sensor sensor1(SENSOR_1_DIGITAL, SENSOR_1_ANALOGICO);
Sensor sensor2(SENSOR_2_DIGITAL, SENSOR_2_ANALOGICO);
Sensor sensor3(SENSOR_3_DIGITAL, SENSOR_3_ANALOGICO);

// Variáveis de interrupção
volatile unsigned long lastInterruptTime1 = 0;
volatile unsigned long lastInterruptTime2 = 0;
volatile unsigned long lastInterruptTime3 = 0;
volatile unsigned long lastInterruptTime4 = 0;
volatile unsigned long lastInterruptTime5 = 0;

volatile bool comandoPosicao1 = false;
volatile bool comandoPosicao2 = false;
volatile bool comandoPosicao3 = false;
volatile bool comandoGarra = false;
volatile bool comandoHabilita = false;

// Funções de interrupção
void IRAM_ATTR comando1ISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime1 > 1000) {
    comandoPosicao1 = true;
    lastInterruptTime1 = interruptTime;
  }
}

void IRAM_ATTR comando2ISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime2 > 1000) {
    comandoPosicao2 = true;
    lastInterruptTime2 = interruptTime;
  }
}

void IRAM_ATTR comando3ISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime3 > 1000) {
    comandoPosicao3 = true;
    lastInterruptTime3 = interruptTime;
  }
}

void IRAM_ATTR comando4ISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime4 > 1000) {
    comandoGarra = !comandoGarra;
    lastInterruptTime4 = interruptTime;
  }
}

void IRAM_ATTR comando5ISR() {
  unsigned long interruptTime = millis();
  if (interruptTime - lastInterruptTime5 > 1000) {
    comandoHabilita = !comandoHabilita;
    lastInterruptTime5 = interruptTime;
  }
}

void setup() {
  Serial.begin(115200);
  
  braco.iniciar();
  conexao.iniciar();
  conexao.conectar();
  servidor.iniciar(braco.getMemoria(), braco, modo_operacao);

  sensor1.inicializar();
  sensor2.inicializar();
  sensor3.inicializar();

  pinMode(ATIVA_SINAL_1, OUTPUT);
  pinMode(ATIVA_SINAL_2, OUTPUT);
  pinMode(ATIVA_SINAL_3, OUTPUT);

  pinMode(COMANDO_1, INPUT_PULLDOWN);
  pinMode(COMANDO_2, INPUT_PULLDOWN);
  pinMode(COMANDO_3, INPUT_PULLDOWN);
  pinMode(COMANDO_4, INPUT_PULLDOWN);
  pinMode(COMANDO_5, INPUT_PULLDOWN);

  attachInterrupt(digitalPinToInterrupt(COMANDO_1), comando1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(COMANDO_2), comando2ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(COMANDO_3), comando3ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(COMANDO_4), comando4ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(COMANDO_5), comando5ISR, CHANGE);

  digitalWrite(DRIVER_PWM_OE, HIGH);
  braco.getMemoria().add_posicao({44,63,11,9,0,77});
  braco.getMemoria().add_posicao({21,78,40,9,17,77});
  braco.getMemoria().add_posicao({7,67,30,10,22,77});
  braco.set_posicaoBraco(0,100);

}

void loop() {

  if(modo_operacao == 0) {

    digitalWrite(DRIVER_PWM_OE, !comandoHabilita);

    if(comandoGarra) {
      braco.fechar_garra();
    } else {
      braco.abrir_garra();
    }
    
    if (comandoPosicao1) {
      Serial.println("Comando 1 ativado");
      comandoPosicao1 = false;
      braco.set_posicaoBraco(0, 50);
    }

    if (comandoPosicao2) {
      Serial.println("Comando 2 ativado");
      comandoPosicao2 = false;
      braco.set_posicaoBraco(1, 50);
    }
    
    if (comandoPosicao3) {
      Serial.println("Comando 3 ativado");
      comandoPosicao3 = false;
      braco.set_posicaoBraco(2, 50);
    }
    
  }
  

  if (sensor1.detectar_presenca() == 0) {
    digitalWrite(ATIVA_SINAL_1, HIGH);
    Serial.println("Sensor 1 ativado");
  } else {
    digitalWrite(ATIVA_SINAL_1, LOW);    
  }

  if (sensor2.detectar_presenca() == 0) {
    digitalWrite(ATIVA_SINAL_2, HIGH);
    Serial.println("Sensor 2 ativado");
  } else {
    digitalWrite(ATIVA_SINAL_2, LOW);    
  }

  if (sensor3.detectar_presenca() == 0) {
    digitalWrite(ATIVA_SINAL_3, HIGH);
    Serial.println("Sensor 3 ativado");
  } else {
    digitalWrite(ATIVA_SINAL_3, LOW);    
  }
}
