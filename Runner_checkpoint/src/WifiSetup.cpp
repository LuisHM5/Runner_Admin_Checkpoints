#include "WifiSetup.h"
unsigned long previousMillis = 0;
const long interval = 10000;

void WifiSetup::Connect()
{
  WiFi.begin(ConfigNet::ssid.c_str(), ConfigNet::pass.c_str());
  int tries = 5;

  unsigned long currentMillis = millis();
  previousMillis = currentMillis;
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED && tries > 0 && currentMillis - previousMillis < interval)
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      WifiSetup::ScanNet();
      tries--;
    }

    if (tries == 0)
    {
      break;
    }
  }

  if (WiFi.status() == WL_NO_SSID_AVAIL)
  {
    Serial.println("\nRed fuera de alcance.");
  }

  // Verificar si las credenciales son incorrectas
  if (WiFi.status() == WL_CONNECT_FAILED)
  {
    Serial.println("\nCredenciales incorrectas.");
  }

  if (WiFi.status() == WL_DISCONNECTED)
  {
    Serial.println("\nDesconectado.");
  }

  if (WiFi.status() == WL_IDLE_STATUS)
  {
    Serial.println("\nIdle status.");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WiFi!");
    Serial.println("Conectado a la red Wi-Fi.");
    Serial.print(" Dirección IP: ");
    Serial.println(WiFi.localIP());
    return;
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