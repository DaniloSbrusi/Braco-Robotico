#include "braco.h"
#include "gpio.h"

Braco::Braco() : 
  pca9685(Adafruit_PWMServoDriver(0x40)), 
  motores{
    Motor(MOTOR_1, 50, 600), 
    Motor(MOTOR_2, 0, 400),
    Motor(MOTOR_3, 0, 500),
    Motor(MOTOR_4, 100, 400),
    Motor(MOTOR_5, 150, 400),
    Motor(MOTOR_6, 100, 300)} 
  {}

void Braco::iniciar() { 
  pca9685.begin();
  pca9685.setPWMFreq(50);
  pinMode(DRIVER_PWM_OE, OUTPUT);
}

void Braco::desligar() {
  digitalWrite(DRIVER_PWM_OE, HIGH);
}

void Braco::set_anguloMotor(int motor, int angulo, int velocidade) {

  // Serial.print("Motor ");
  // Serial.print(motor);
  // Serial.print(" -> angulo ");
  // Serial.print(angulo);
  // Serial.print("%, delay ");
  // Serial.print(velocidade);
  // Serial.println("ms");

  int cont = motores[motor].get_angulo();
  if(cont != -1) { 
    while(cont != angulo){
      if(motor<4)
        vTaskDelay(velocidade);
      else
        vTaskDelay(velocidade/2);
      if (cont < angulo) 
        cont++;
      else if (cont > angulo)
        cont--;
      motores[motor].set_angulo(pca9685, cont);
    }
  } else {
    motores[motor].set_angulo(pca9685, angulo); // Na inicialização, vai direto para o ângulo
  }
}

int Braco::get_anguloMotor(int motor){
  return motores[motor].get_angulo();
}

struct MotorParams {
  int motor;
  int angulo;
  int velocidade;
  Braco *braco;
};

void tarefaMoverMotor(void *param) {
  MotorParams *params = (MotorParams *)param;
  params->braco->set_anguloMotor(params->motor, params->angulo, params->velocidade);
  delete params;  // Libera a memória alocada 
  vTaskDelete(NULL);  // Finaliza a tarefa após o movimento completo
}

void Braco::set_posicaoBraco(int indicePosicao, int velocidade) {
  Serial.print("Para posicao ");
  Serial.println(indicePosicao);
  std::array<int, 6> posicao = memoria.get_posicao(indicePosicao);
  for (int i=0; i<6; i++) {
    MotorParams *params = new MotorParams;  // Aloca memória para os parâmetros
    params->motor = i;
    params->angulo = posicao[i];
    params->velocidade = velocidade;  
    params->braco = this;  
    xTaskCreate(tarefaMoverMotor, "tarefaMoverMotor", 2048, (void *)params, 1, NULL);
  }
}

void Braco::set_posicaoBracoArray(const std::array<int, 6> &posicao, int velocidade) {
  Serial.println("Para posicao costumizada");
  for (int i=0; i<6; i++) {
    MotorParams *params = new MotorParams;  // Aloca memória para os parâmetros
    params->motor = i;
    params->angulo = posicao[i];
    params->velocidade = velocidade;  
    params->braco = this;  
    xTaskCreate(tarefaMoverMotor, "tarefaMoverMotor", 2048, (void *)params, 1, NULL);
  }
}

std::array<int, 6> Braco::get_posicaoBraco(){
  std::array<int, 6> posicaoAtual;
  for(int i=0; i<6; i++){
    posicaoAtual[i] = get_anguloMotor(i);
  }
  return posicaoAtual;
}

void Braco::fechar_garra() {
  set_anguloMotor(5, 0, 0);
}

void Braco::abrir_garra(){
  set_anguloMotor(5, 100, 0);
}

Memoria& Braco::getMemoria() {
  return memoria;
}