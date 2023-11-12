#pragma once

#include <WiFi.h>
#include "config.h"
class WifiSetup
{
private:
public:
  WifiSetup();
  static void Connect();
  static void ScanNet();
};
