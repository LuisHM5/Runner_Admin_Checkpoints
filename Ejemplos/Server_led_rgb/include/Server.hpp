#pragma once

#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "config.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "rgbChanger.h"
class ServerHTTP
{
private:
  /* data */
  static AsyncWebServer server;

public:
  ServerHTTP();

  static void init();
};
