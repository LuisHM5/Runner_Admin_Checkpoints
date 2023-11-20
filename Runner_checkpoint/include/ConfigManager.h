#pragma once
#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPIFFS.h>
#include "FileUtility.hpp"
#include "Config.h"
#include "WifiSetup.h"
#include "AccessPoint.h"
namespace ConfigManager
{
  void begin();
  bool setupConfig();
  bool saveConfig(const String &jsonConfig);
  void initWifi();
  void initAP();
  String serializeJsonConfig();
  DynamicJsonDocument parseJsonConfig(const String &jsonConfig);
}