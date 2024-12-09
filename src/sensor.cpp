#include "sensor.h"
#include <Arduino.h>

Sensor::Sensor(int pino_digital, int pino_analogico) {
    this->pino_digital = pino_digital;
    this->pino_analogico = pino_analogico;
}

void Sensor::inicializar(){
    pinMode(pino_digital, INPUT);
    pinMode(pino_analogico, INPUT);
}

bool Sensor::detectar_presenca() {
    return digitalRead(pino_digital);
}

int Sensor::ler_distancia() {
    return analogRead(pino_analogico);
}

