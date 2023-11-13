#include "Server.hpp"

AsyncWebServer ServerHTTP::server(ConfigServer::port);

ServerHTTP::ServerHTTP()
{
  ServerHTTP::init();
}

// Se deja para en caso de que no quedemos sin memoria flash
/* const char webPage[] PROGMEM = R"rawliteral(
  <!doctype html>
  <html lang=en>
  <head>
  <meta charset=utf-8>
  <meta name=viewport content="width=device-width,initial-scale=1">

  </head>
  <body>
  <h2>Wemos Server</h2>
)rawliteral"; */

void ServerHTTP::init()
{
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (SPIFFS.exists("/index.html")) {
          // Leer el contenido del archivo index.html
        string content = FileUtility::readFile("/index.html");

        // Evaluar condiciones y ajustar el contenido
        bool inRace = TimeClock::GetStatus();
        if (inRace) {
            size_t start_pos = content.find("%ifInRace%");
            size_t end_pos = content.find("%endifInRace%", start_pos);
            if (start_pos != std::string::npos && end_pos != std::string::npos) {
                content.erase(start_pos, end_pos - start_pos + std::strlen("%endifInRace%"));
            }

            start_pos = content.find("%ifNotInRace%");
            end_pos = content.find("%endifNotInRace%", start_pos);
            if (start_pos != std::string::npos && end_pos != std::string::npos) {
                content.erase(start_pos, end_pos - start_pos + std::strlen("%endifNotInRace%"));
            }
        } else {
            size_t start_pos = content.find("%ifInRace%");
            size_t end_pos = content.find("%endifInRace%", start_pos);
            if (start_pos != std::string::npos && end_pos != std::string::npos) {
                content.erase(start_pos, end_pos - start_pos + std::strlen("%endifInRace%"));
            }

            start_pos = content.find("%ifNotInRace%");
            end_pos = content.find("%endifNotInRace%", start_pos);
            if (start_pos != std::string::npos && end_pos != std::string::npos) {
                content.erase(start_pos, end_pos - start_pos + std::strlen("%endifNotInRace%"));
            }
        }

        // Enviar la página con las condiciones evaluadas al cliente
        request->send(200, "text/html", content.c_str());

        // request->send(SPIFFS, "/index.html", "text/html");
    } else {
        request->send(404, "text/plain", "Archivo no encontrado");
    } });

  server.on("/formulario.html", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (SPIFFS.exists("/formulario.html")) {
        request->send(SPIFFS, "/formulario.html", "text/html");
    } else {
        request->send(404, "text/plain", "Archivo no encontrado");
    } });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "application/javascript"); });

  server.on("/start-race", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              Serial.println("Iniciando carrera...");
              TimeClock::Start();
              request->redirect("/"); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(400, "text/plain", "Not found"); });
  server.begin();
  Serial.println("HTTP Server is listening on port: ");
  Serial.println(ConfigServer::port);
}