#include "WifiSetup.h"
void WifiSetup::Connect()
{
  WiFi.begin(ConfigNet::ssid, ConfigNet::pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Connecting to WiFi...");
    delay(6000);
    if (WiFi.status() == WL_NO_SSID_AVAIL)
    {
      Serial.println("\nRed fuera de alcance.");
      return;
    }

    // Verificar si las credenciales son incorrectas
    if (WiFi.status() == WL_CONNECT_FAILED)
    {
      Serial.println("\nCredenciales incorrectas.");
      return;
    }

    if (WiFi.status() == WL_DISCONNECTED)
    {
      Serial.println("\nDesconectado.");
      return;
    }

    if (WiFi.status() == WL_IDLE_STATUS)
    {
      Serial.println("\nIdle status.");
      return;
    }
  }

  Serial.println("Connected to WiFi!");
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Conectado a la red Wi-Fi.");
    Serial.print(" Dirección IP: ");
    Serial.println(WiFi.localIP());
  }
}

void WifiSetup::ScanNet()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from any previous connections

  Serial.println("Scanning for Wi-Fi networks...");
  int numNetworks = WiFi.scanNetworks();

  if (numNetworks == 0)
  {
    Serial.println("No networks found.");
  }
  else
  {
    Serial.print("Found ");
    Serial.print(numNetworks);
    Serial.println(" networks:");
    for (int i = 0; i < numNetworks; ++i)
    {
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.print(WiFi.SSID(i));
      Serial.print(" (");
      Serial.print(WiFi.RSSI(i));
      Serial.println(" dBm)");
    }
  }
}