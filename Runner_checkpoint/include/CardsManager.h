#pragma once
#include <unordered_map>
#include <string>
#include <iostream>
#include <ArduinoJson.h>
using namespace std;
using CheckpointData = std::unordered_map<std::string, std::string>;
using CardDataMap = std::unordered_map<std::string, CheckpointData>;

class CardsManager
{
public:
  CardsManager(){};
  ~CardsManager(){};
  static std::unordered_map<std::string, std::string> UID_Database_local;
  static CardDataMap Cards_Readed;
  bool AddCard(const std::string &uid, const std::string &time, const std::string &punto)
  {

    if (!findCardInLocal(uid))
      return false;

    // Verificar si la tarjeta ya está en el mapa
    if (Cards_Readed.find(uid) != Cards_Readed.end())
    {
      if (Cards_Readed[uid].size() > 0 && Cards_Readed[uid].find(punto) != Cards_Readed[uid].end())
      {
        cout << "Punto de control ya registrado" << endl;
        return false;
      }
      std::pair<std::string, std::string> data = {punto, time};
      Cards_Readed[uid].insert(data);

      cout << "Tiempo registrado en " << punto << endl
           << uid << ": " << time << endl;
      return true;
    }
    else
    {
      // Si la tarjeta no existe, creamos una nueva entrada en el mapa con el nuevo punto de control y tiempo
      CheckpointData data;
      std::pair<std::string, std::string> data_pair = {punto, time};
      data.insert(data_pair);
      Cards_Readed.insert({uid, data});
      cout << "Tiempo registrado en " << punto << endl
           << uid << ": " << time << endl;
      return true;
    }

    return false;
  }

  void PrintAllCards()
  {
    cout << "====Tarjetas registradas=====" << endl;
    for (auto &card : Cards_Readed)
    {
      cout << "Tarjeta: " << card.first << endl;
      for (auto &data : card.second)
      {
        cout << "Punto: " << data.first << endl;
        cout << "Tiempo: " << data.second << endl;
      }
    }
  }

  String GetAllCardsJson()
  {
    DynamicJsonDocument doc(1024);
    JsonObject cards = doc.createNestedObject("cards");
    for (auto &card : Cards_Readed)
    {
      JsonObject card_data = cards.createNestedObject(card.first.c_str());
      for (auto &data : card.second)
      {
        card_data[data.first.c_str()] = data.second.c_str();
      }
    }
    return doc.as<String>();
  }

  void ClearAllCards()
  {
    Cards_Readed.clear();
  }

private:
  bool findCardInLocal(const std::string &uid)
  {
    if (UID_Database_local.find(uid) != UID_Database_local.end())
      return true;
    std::cout << "UID no encontrado en la base de datos: " << uid << std::endl;
    return false;
  }
};
