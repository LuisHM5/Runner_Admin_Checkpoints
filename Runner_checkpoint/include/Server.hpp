#pragma once
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <SPIFFS.h>
#include <ArduinoJson.h>
#include <fstream>
#include <sstream>
#include "config.h"
#include "TimeClock.h"
#include "FileUtility.hpp"
class ServerHTTP
{
private:
  /* data */
  static AsyncWebServer server;

public:
  static AsyncWebSocket ws;

public:
  ServerHTTP();

  static void init();
  static void notifyClients(String info);

private:
  static void initWebSocket();
};