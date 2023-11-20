#pragma once
#include <ESPAsyncWebServer.h>
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <fstream>
#include <sstream>
#include "config.h"
#include "TimeClock.h"
#include "FileUtility.hpp"
#include "CardsManager.h"
#include "ConfigManager.h"

class ServerHTTP
{
private:
  /* data */
  static AsyncWebServer server;
  static AsyncCallbackJsonWebHandler *SaveNET;

public:
  static AsyncWebSocket ws;

public:
  ServerHTTP();

  static void init();
  static void notifyClients(String info);

private:
  static void initWebSocket();
};