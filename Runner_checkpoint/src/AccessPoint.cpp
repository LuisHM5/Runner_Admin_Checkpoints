#include <WiFi.h>
#include "configAP.h"

#include "AccessPoint.h"

void SetupAccessPoint::Created()
{
  Serial.print("Setting AP (Access Point)…");
  WiFi.softAP(ConfigNetAP::ssid, ConfigNetAP::pass);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
}