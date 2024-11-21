#ifndef MOTOR_H
#define MOTOR_H

#include <Adafruit_PWMServoDriver.h>

class Motor {
  private:  
    int conector;
    int servo_min;
    int servo_max;
    int angulo;

  public:
    Motor(int conector, int servo_min, int servo_max);
    void set_angulo(Adafruit_PWMServoDriver &pca9685, int angulo);
    int get_angulo();
};

#endif // MOTOR_H
