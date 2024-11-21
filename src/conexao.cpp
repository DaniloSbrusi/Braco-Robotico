#include "conexao.h"
#include "credenciais.h"

const char* Conexao::rede = REDE;
const char* Conexao::usuario = USUARIO;
const char* Conexao::senha = SENHA;
TimerHandle_t Conexao::wifiReconnectTimer;

void Conexao::iniciar() {
    WiFi.onEvent(evento);
    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(conectar));
}

bool Conexao::conectar() {
    Serial.print("Conectando a rede Wi-Fi ");
    Serial.println(rede);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);

    // WiFi.begin(rede, WPA2_AUTH_PEAP, usuario, usuario, senha);
    WiFi.begin(rede, senha);

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        i++;
        if (i >= 60) 
            return 1;
    }
    return 0;
    Serial.println();
}

void Conexao::evento(arduino_event_id_t event) {
    switch (event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.println("WiFi conectado");
            Serial.print("IP: ");
            Serial.println(WiFi.localIP());
            break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("Conexão WiFi perdida");
            WiFi.disconnect(true);
            xTimerStart(wifiReconnectTimer, 0); 
            break;
        default:  
            break;   
    }
}