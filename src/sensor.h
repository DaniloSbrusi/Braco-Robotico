#ifndef SENSOR_H
#define SENSOR_H

#include "src/sensor.h"

class Sensor {
    private:
        int pino_digital;
        int pino_analogico;
    public:
        Sensor(int pino_digital, int pino_analogico);
        void inicializar();
        bool detectar_presenca();
        int ler_distancia();
};

#endif