#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include "pagina.h"
#include "memoria.h"
#include "braco.h"

class Servidor {
    private:
        AsyncWebServer servidor;

    public:
        Servidor(int porta);
        void iniciar(Memoria& memoria, Braco& braco, int& modo_operacao);
};

#endif

