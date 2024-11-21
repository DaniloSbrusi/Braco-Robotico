#include "motor.h"

Motor::Motor(int conector, int servo_min, int servo_max) {
  this->conector = conector;
  this->servo_min = servo_min;
  this->servo_max = servo_max;
  this->angulo = -1;
}

void Motor::set_angulo(Adafruit_PWMServoDriver &pca9685, int angulo) {
    if (angulo>100) angulo = 100;
    else if (angulo<0) angulo = 0;
    this->angulo = angulo;
    int pwm = map(angulo, 0, 100, servo_min, servo_max);
    pca9685.setPWM(conector, 0, pwm);
}

int Motor::get_angulo() {
    return angulo;
}