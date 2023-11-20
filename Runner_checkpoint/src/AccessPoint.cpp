#include "AccessPoint.h"

void SetupAccessPoint::Created()
{
  // WiFi.disconnect();
  // WiFi.begin();
  WiFi.mode(WIFI_AP);
  // Setup Access Point
  WiFi.softAP(ConfigNetAP::ssid, ConfigNetAP::pass);

  // Config IP Address
  IPAddress IP;
  IP.fromString(ConfigNetAP::ip);
  // init AP
  WiFi.softAPIP();
  WiFi.softAPConfig(IP, IP, IPAddress(255, 255, 255, 0));

  // Print IP Address
  Serial.print("Access Point IP address: ");
  Serial.println(IP);
}