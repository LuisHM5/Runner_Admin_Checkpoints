#include "Server.hpp"

AsyncWebServer ServerHTTP::server(ConfigServer::port);

ServerHTTP::ServerHTTP()
{
  // Constructor de la clase

  ServerHTTP::init();
}

// void colorHandler(AsyncWebServerRequest *request, JsonVariant &json)
// {
//   JsonObject jsonObj = json.as<JsonObject>();
//   // ... (realiza las operaciones necesarias con el objeto jsonObj)
// }

void ServerHTTP::init()
{

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    if (SPIFFS.exists("/index.html")) {
        request->send(SPIFFS, "/index.html", "text/html");
    } else {
        request->send(404, "text/plain", "Archivo no encontrado");
    } });
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });

  server.on("/tinycolor.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/tinycolor.js", "application/javascript"); });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "application/javascript"); });

  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
  //           { request->send(200, "text/plain", "Hello, World!"); });

  // server.on("/color", HTTP_POST, [](AsyncWebServerRequest *request) {

  // });

  AsyncCallbackJsonWebHandler *handler = new AsyncCallbackJsonWebHandler("/color", [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                         {
    StaticJsonDocument<200> data;
    if (json.is<JsonArray>())
    {
      data = json.as<JsonArray>();
    }
    else if (json.is<JsonObject>())
    {
      data = json.as<JsonObject>();
      uint8_t red = data["red"];
      uint8_t green = data["green"];
      uint8_t blue = data["blue"];
      RgbChanger::changeColors(red, green, blue);
      // Serial.println((String) red + " " + green + " " + blue);
    }
    String response;
    serializeJson(data, response);
    request->send(200, "application/json", response);
    Serial.println(response); });
  server.addHandler(handler);

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(400, "text/plain", "Not found"); });
  server.begin();
  Serial.println("HTTP Server is listening on port: ");
  Serial.println(ConfigServer::port);
}