#ifndef BRACO_H
#define BRACO_H

#include <Adafruit_PWMServoDriver.h>
#include "motor.h"
#include "memoria.h"

class Braco {
  private:
    Adafruit_PWMServoDriver pca9685;
    Motor motores[6];
    Memoria memoria;

  public:
    Braco();
    void iniciar();
    void desligar();
    void set_anguloMotor(int motor, int angulo, int velocidade);
    int get_anguloMotor(int motor);
    void set_posicaoBraco(int posicao, int velocidade);
    void set_posicaoBracoArray(const std::array<int, 6> &posicao, int velocidade);
    std::array<int, 6> get_posicaoBraco();
    void fechar_garra();
    void abrir_garra();
    Memoria& getMemoria();
};

#endif 
