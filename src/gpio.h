#ifndef GPIO_H
#define GPIO_H

// Entradas dos sensores ópticos
#define SENSOR_1_DIGITAL 16
#define SENSOR_2_DIGITAL 17
#define SENSOR_3_DIGITAL 19
#define SENSOR_1_ANALOGICO 4
#define SENSOR_2_ANALOGICO 5
#define SENSOR_3_ANALOGICO 18

// Sinais enviados para o CLP
#define ATIVA_SINAL_1   14  
#define ATIVA_SINAL_2   12  
#define ATIVA_SINAL_3   13

// Sinais recebidos do CLP
#define COMANDO_5 33
#define COMANDO_4 32
#define COMANDO_3 35
#define COMANDO_2 34
#define COMANDO_1 39

// Driver PWM PCA9685
#define DRIVER_PWM_OE   23
#define DRIVER_PWM_SDA  21
#define DRIVER_PWM_SCL  22

// Conectores no driver PWM PCA9685
#define MOTOR_1 0
#define MOTOR_2 1
#define MOTOR_3 3
#define MOTOR_4 4
#define MOTOR_5 5
#define MOTOR_6 6

#endif