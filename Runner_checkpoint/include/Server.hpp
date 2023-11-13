#pragma once
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <fstream>
#include <sstream>
#include "config.h"
#include "ArduinoJson.h"
#include "AsyncJson.h"
#include "TimeClock.h"
#include "FileUtility.hpp"
class ServerHTTP
{
private:
  /* data */
  static AsyncWebServer server;

public:
  ServerHTTP();

  static void init();
};