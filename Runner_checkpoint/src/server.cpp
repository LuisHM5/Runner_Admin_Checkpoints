#include "Server.hpp"
// WEB API REST SERVER WITH SOCKETS
AsyncWebServer ServerHTTP::server(ConfigServer::port);
AsyncWebSocket ServerHTTP::ws("/ws");

ServerHTTP::ServerHTTP()
{
  ServerHTTP::init();
}

void ServerHTTP::notifyClients(String info)
{
  ServerHTTP::ws.textAll(info);
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void ServerHTTP::initWebSocket()
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

AsyncCallbackJsonWebHandler *ServerHTTP::SaveNET = new AsyncCallbackJsonWebHandler(
    "/save-network", [](AsyncWebServerRequest *request, JsonVariant &json)
    { 
      Serial.println("Guardando configuracion de red...");
      Serial.println(json.as<String>());
      if(json["ssid"].isNull() || json["pass"].isNull()){
        request->send(400, "application/json", "{\"config\":false}");
        return;
      }

      if(json.isNull()){
        request->send(400, "application/json", "{\"config\":false}");
        return;
      }
      if(ConfigManager::saveConfig(json.as<String>())){
        request->send(200, "application/json", "{\"config\":true}");
      }else{
        request->send(500, "application/json", "{\"config\":false}");
      }

                                                                        
    delay(3000);
    ESP.restart(); },
    1024);

void ServerHTTP::init()
{

  server.addHandler(ServerHTTP::SaveNET);
  ServerHTTP::initWebSocket();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (SPIFFS.exists("/index.html")) {
      request->send(SPIFFS, "/index.html", "text/html");
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
              if (!TimeClock::GetStatus())
              {
                Serial.println("Iniciando carrera...");
                TimeClock::Start();
                ServerHTTP::notifyClients(TimeClock::getDataTimeJson());
                request->send(200, "application/json", TimeClock::getDataTimeJson());
              }
              return request->send(500); });
  server.on("/data-race", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (TimeClock::GetStatus())
              {
                CardsManager cardsManager;
                ServerHTTP::notifyClients(cardsManager.GetAllCardsJson());
                request->send(200, "application/json", cardsManager.GetAllCardsJson());
              }
              return request->send(500); });
  server.on("/stop-race", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              if (TimeClock::GetStatus())
              {
                Serial.println("Deteniendo carrera...");
                TimeClock::Stop();
                CardsManager cardsManager;
                cardsManager.ClearAllCards();
                ServerHTTP::notifyClients("{\"status\":false}");
                return request->send(200, "text/plain", "ok");
              }
              request->send(500, "text/plain", "error"); });

  server.on("/status-race", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (TimeClock::GetStatus())
              {
                return request->send(200, "application/json", TimeClock::getDataTimeJson());
              }
              else
              {
                return request->send(200, "application/json", "{\"status\":false}");
              } });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });
  // add favicon
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/favicon.ico", "image/x-icon"); });
  // static files
  server.serveStatic("/", SPIFFS, "/");
  server.begin();
  Serial.println("HTTP Server is listening on port: ");
  Serial.println(ConfigServer::port);
}