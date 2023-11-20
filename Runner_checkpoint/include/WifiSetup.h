#pragma once
#include <WiFi.h>
#include "AccessPoint.h"
#include "config.h"
#include "ConfigManager.h"

class WifiSetup
{
private:
public:
  WifiSetup();
  static void Connect();
  static void ScanNet();
};