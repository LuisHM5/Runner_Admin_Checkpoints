#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include "WifiSetup.h"
#include "Server.hpp"
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include "config.h"
#include "rgbChanger.h"
ServerHTTP *serverhttp;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  SPIFFS.begin(true);
  RgbChanger::setupPins(2, 4, 16);
  WifiSetup::ScanNet();
  WifiSetup::Connect();
  serverhttp = new ServerHTTP();
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println((String) ":" + ConfigServer::port);
  // Serial.println((String) "http://" + WiFi.localIP() + ":" + ConfigServer::port);
  // serverhttp = new ServerHTTP(ConfigServer::port);
}

void loop()
{
  // put your main code here, to run repeatedly:

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Disconnected from WiFi");
    //  Realizar acciones adicionales si se desconecta
    WifiSetup::ScanNet();
    WifiSetup::Connect();
    // delete serverhttp;
    // serverhttp = new ServerHTTP();
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println((String) ":" + ConfigServer::port);
  }
  // delay(300);
}
