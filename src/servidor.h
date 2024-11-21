#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "pagina.h"

class Servidor {
    private:
        AsyncWebServer servidor;

    public:
        Servidor(int porta);
        void iniciar();
};

#endif

