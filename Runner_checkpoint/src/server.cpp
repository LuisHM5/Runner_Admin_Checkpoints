
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

}