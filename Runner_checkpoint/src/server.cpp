#include "Server.hpp"

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

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  // AwsFrameInfo *info = (AwsFrameInfo *)arg;
  // if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  // {
  //   data[len] = 0;
  //   String message = (char *)data;
  //   // Check if the message is "getReadings"
  //   if (strcmp((char *)data, "getReadings") == 0)
  //   {
  //     // if it is, send current sensor readings
  //     String message = TimeClock::getDataTimeJson();
  //     ServerHTTP::notifyClients(message);
  //   }
  // }
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
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
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

void ServerHTTP::init()
{
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
            {
              request->send(SPIFFS, "/script.js", "application/javascript");
              if(TimeClock::GetStatus()) {
                ServerHTTP::notifyClients(TimeClock::getDataTimeJson());
                Serial.println("Enviando estado de carrera...");
              } });

  server.on("/start-race", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              if (!TimeClock::GetStatus())
              {
                Serial.println("Iniciando carrera...");
                TimeClock::Start();
                ServerHTTP::notifyClients(TimeClock::getDataTimeJson());
                request->send(200, "application/json", TimeClock::getDataTimeJson());
              }
              // request->redirect("/");
            });
  server.on("/stop-race", HTTP_POST, [](AsyncWebServerRequest *request)
            {
              if (TimeClock::GetStatus())
              {
                Serial.println("Deteniendo carrera...");
                TimeClock::SetStatus(false);
                TimeClock::Stop();
                ServerHTTP::notifyClients("{\"status\":false}");
                return request->send(200, "text/plain", "ok");
              }
              request->send(500, "text/plain", "error"); });
  server.on("/status-race", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (TimeClock::GetStatus())
              {
                request->send(200, "application/json", TimeClock::getDataTimeJson());
              }
              else
              {
                request->send(200, "application/json", "{\"status\":false}");
              } });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "Not found"); });

  server.begin();
  Serial.println("HTTP Server is listening on port: ");
  Serial.println(ConfigServer::port);
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