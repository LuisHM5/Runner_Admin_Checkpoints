#include "ConfigManager.h"

String ConfigManager::serializeJsonConfig()
{
  DynamicJsonDocument doc(1024);
  JsonObject root = doc.to<JsonObject>();
  root["ssid"] = ConfigNet::ssid;
  root["pass"] = ConfigNet::pass;
  root["port"] = ConfigServer::port;
  String json;
  serializeJson(root, json);
  return json;
}

DynamicJsonDocument ConfigManager::parseJsonConfig(const String &jsonConfig)
{

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonConfig);
  JsonObject root = doc.as<JsonObject>();
  if (root["ssid"].isNull() || root["pass"].isNull())
  {
    Serial.println("Invalid config file");
    return DynamicJsonDocument(0);
  }
  String ssid = root["ssid"];
  String pass = root["pass"];
  if (ssid.length() == 0 || pass.length() == 0)
  {
    Serial.println("Invalid config file");
    return DynamicJsonDocument(0);
  }

  return doc;
}

bool ConfigManager::saveConfig(const String &jsonConfig)
{
  if (!SPIFFS.exists("/config.json"))
  {
    // Create file
    File file = SPIFFS.open("/config.json", FILE_WRITE);
    if (!file)
    {
      Serial.println("Failed to create file");
      return false;
    }
    return false;
  }

  File file = SPIFFS.open("/config.json", FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return false;
  }
  if (file.print(jsonConfig))
  {
    Serial.println("File written");
    return true;
  }
  else
  {
    Serial.println("Write failed");
    return false;
  }
}

bool ConfigManager::setupConfig()
{
  // FileUtility
  if (!SPIFFS.begin(true))
  {
    Serial.println("SPIFFS Mount Failed");
    return false;
  }
  if (!SPIFFS.exists("/config.json"))
  {
    Serial.println("Config file not found");
    return false;
  }

  String jsonConfig = FileUtility::readFile(SPIFFS, "/config.json");
  DynamicJsonDocument docDataConfig(1024);
  docDataConfig = parseJsonConfig(jsonConfig);

  if (docDataConfig.isNull() || docDataConfig.size() == 0)
  {
    Serial.println("no config data");
    return false;
  }

  Serial.println("Config file found");
  ConfigNet::setSSID(docDataConfig["ssid"]);
  ConfigNet::setPASS(docDataConfig["pass"]);
  Serial.println("Config data loaded:");
  Serial.println((String) "SSID: " + ConfigNet::ssid.c_str());
  Serial.println((String) "PASS: " + ConfigNet::pass.c_str());
  Serial.println("=====================================");
  return true;
}

void ConfigManager::initWifi()
{
  WifiSetup::ScanNet();
  WifiSetup::Connect();
  if (WiFi.localIP() == IPAddress(0, 0, 0, 0))
  {
    Serial.println("No se pudo conectar a la red Wi-Fi red fuera de acceso o credenciales incorrectas");
    cout << "Configuración guardada" << endl;
    cout << "Red: " << ConfigNet::ssid << endl;
    cout << "Contraseña: " << ConfigNet::pass << endl;
    initAP();
    return;
  }

  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println((String) ":" + ConfigServer::port);
}
void ConfigManager::initAP()
{
  SetupAccessPoint::Created();
  Serial.println((String) "Access Point created: " + ConfigNetAP::ssid);
  Serial.print("http://");
  Serial.print(ConfigNetAP::ip);
  Serial.println((String) ":" + ConfigServer::port);
}

void ConfigManager::begin()
{
  if (ConfigManager::setupConfig())
  {
    initWifi();
  }
  else
  {
    initAP();
  }
}