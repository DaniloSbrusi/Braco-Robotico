#include "servidor.h"
#include <ArduinoJson.h>

Servidor::Servidor(int porta)
    : servidor(porta) {}

void Servidor::iniciar(Memoria& memoria, Braco& braco, int &modo_operacao) {

    servidor.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    servidor.on("/getModoOperacao", HTTP_GET, [&modo_operacao](AsyncWebServerRequest *request) {
        // Criar um objeto JSON
        DynamicJsonDocument doc(1024);
        doc["modo"] = modo_operacao;

        // Serializar o JSON para uma string
        String response;
        serializeJson(doc, response);

        // Enviar a resposta JSON
        request->send(200, "application/json", response);
    });


    servidor.on("/atualizarModoOperacao",
        HTTP_POST,
        [](AsyncWebServerRequest* request) {
            // Esta função é chamada ao término do processamento
            request->send(200, "application/json", "{\"status\":\"success\", \"message\":\"Modo atualizado\"}");
        },
        nullptr,
        [&modo_operacao](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {

            Serial.println("Requisicao para alterar modo de operacao");

            // Processar o corpo da requisição
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, data, len);

            if (error) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid JSON\"}");
                return;
            }

            // Recuperar o índice e os valores editados
            int modo = doc["modo"];

            if (modo == 0){
                modo_operacao = 0;
            } else if (modo == 1) {
                modo_operacao = 1;
            } else {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Modo invalido\"}");
                return;
            }
            Serial.print("Modo de operacao: ");
            Serial.println(modo_operacao);
        }
    );

    servidor.on("/obterPosicoes", HTTP_GET, [&memoria](AsyncWebServerRequest *request) {
        // Criar um objeto JSON
        DynamicJsonDocument doc(1024);
        JsonArray positions = doc.to<JsonArray>();

        for (int i = 0; i < memoria.tamanho(); i++) {
            JsonObject position = positions.createNestedObject();
            auto posicao = memoria.get_posicao(i);  // Obter a posição pelo índice
            position["motor1"] = posicao[0];
            position["motor2"] = posicao[1];
            position["motor3"] = posicao[2];
            position["motor4"] = posicao[3];
            position["motor5"] = posicao[4];
            position["motor6"] = posicao[5];
        }

        // Serializar o JSON para uma string
        String response;
        serializeJson(doc, response);

        // Enviar a resposta JSON
        request->send(200, "application/json", response);
    });

    servidor.on(
        "/salvarPosicao",
        HTTP_POST,
        [](AsyncWebServerRequest* request) {
            // Esta função é chamada ao término do processamento
            request->send(200, "application/json", "{\"status\":\"success\"}");
        },
        nullptr,
        [&memoria](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {

            Serial.println("Requisicao para salvar posicao");

            // Processar o corpo da requisição
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, data, len);

            if (error) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid JSON\"}");
                return;
            }

            // Recuperar valores do JSON
            int motor1 = doc["motor1"];
            int motor2 = doc["motor2"];
            int motor3 = doc["motor3"];
            int motor4 = doc["motor4"];
            int motor5 = doc["motor5"];
            int motor6 = doc["motor6"];

            // Construir um std::array<int, 6> para salvar na memória
            std::array<int, 6> novaPosicao = {motor1, motor2, motor3, motor4, motor5, motor6};
            memoria.add_posicao(novaPosicao);
        }
    );

    servidor.on(
        "/editarPosicao",
        HTTP_PUT,
        [](AsyncWebServerRequest* request) {
            // Esta função é chamada ao término do processamento
            request->send(200, "application/json", "{\"status\":\"success\"}");
        },
        nullptr,
        [&memoria](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            
            Serial.println("Requisicao de edicao de posicao");

            // Processar o corpo da requisição
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, data, len);

            if (error) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid JSON\"}");
                return;
            }

            // Recuperar o índice e os valores editados
            int indexPosicaoEditar = doc["index"];
            JsonObject posicao = doc["posicao"];
            if (!posicao || indexPosicaoEditar < 0 || indexPosicaoEditar >= memoria.tamanho()) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid index or position\"}");
                return;
            }

            // Atualizar a posição na memória
            std::array<int, 6> posicaoEditada = {
                posicao["motor1"],
                posicao["motor2"],
                posicao["motor3"],
                posicao["motor4"],
                posicao["motor5"],
                posicao["motor6"]
            };

            memoria.set_posicao(indexPosicaoEditar, posicaoEditada);

            Serial.println('Posicao editada com sucesso');
            // Enviar resposta de sucesso
            request->send(200, "application/json", "{\"status\":\"success\", \"message\":\"Position updated\"}");
        }
    );

    servidor.on(
        "/moverBraco",
        HTTP_POST,
        [](AsyncWebServerRequest* request) {
            // Enviar a resposta ao final do processamento
            request->send(200, "application/json", "{\"status\":\"success\", \"message\":\"Request processed\"}");
        },
        nullptr,
        [&braco](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {

            Serial.println("Requisicao para movimentar braco");

            // Processar o corpo da requisição
            StaticJsonDocument<1024> doc;
            DeserializationError error = deserializeJson(doc, data, len);

            if (error) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid JSON format\"}");
                return;
            }

            String jsonString;
            serializeJson(doc, jsonString);
            Serial.println(jsonString);

            std::array<int, 6> posicoesArray = {
                doc["motor1"].as<int>(),
                doc["motor2"].as<int>(),
                doc["motor3"].as<int>(),
                doc["motor4"].as<int>(),
                doc["motor5"].as<int>(),
                doc["motor6"].as<int>()
            };

            braco.set_posicaoBracoArray(posicoesArray, 50);

            Serial.println("Braço movido com sucesso");
        }
    );

    servidor.on("/obterPosicaoAtual", HTTP_GET, [&braco](AsyncWebServerRequest *request) {
        // Obter a posição atual do braço
        std::array<int, 6> posicaoAtual = braco.get_posicaoBraco();  

        // Criar o objeto JSON
        DynamicJsonDocument doc(1024);
        JsonArray posicoes = doc.to<JsonArray>();

        for (int i = 0; i < 6; i++) {
            posicoes.add(posicaoAtual[i]);
        }

        // Serializar o JSON para uma string
        String response;
        serializeJson(doc, response);

        // Enviar a resposta JSON
        request->send(200, "application/json", response);
    });

    servidor.on(
        "/deletarPosicao",
        HTTP_DELETE,
        [](AsyncWebServerRequest* request) {
            // Enviar uma resposta de sucesso após o processamento
            request->send(200, "application/json", "{\"status\":\"success\", \"message\":\"Position deleted\"}");
        },
        nullptr,
        [&memoria](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            // Processar o corpo da requisição
            StaticJsonDocument<256> doc;
            DeserializationError error = deserializeJson(doc, data, len);

            if (error) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid JSON\"}");
                return;
            }

            // Recuperar o índice da posição a ser excluída
            int indexToDelete = doc["index"];
            if (indexToDelete < 0 || indexToDelete >= memoria.tamanho()) {
                request->send(400, "application/json", "{\"status\":\"error\", \"message\":\"Invalid index\"}");
                return;
            }

            // Excluir a posição da memória
            memoria.deletar_posicao(indexToDelete);

            // Log e resposta
            Serial.printf("Posição %d excluída com sucesso\n", indexToDelete);
            request->send(200, "application/json", "{\"status\":\"success\", \"message\":\"Position deleted\"}");
        }
    );

    servidor.begin();
}
