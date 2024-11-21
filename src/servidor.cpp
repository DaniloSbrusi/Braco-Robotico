#include "servidor.h"

Servidor::Servidor(int porta)
    : servidor(porta) {}

void Servidor::iniciar() {

    servidor.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    servidor.begin();
}
