#ifndef CONEXAO_H
#define CONEXAO_H

#include <Arduino.h>
#include <pgmspace.h>
#include <WiFi.h>

class Conexao {
    private:
        static const char* rede;
        static const char* usuario;
        static const char* senha;
        static TimerHandle_t wifiReconnectTimer;

    public:
        static void iniciar();
        static bool conectar();
        static void evento(arduino_event_id_t event);
};


#endif