#pragma once
#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;
class CardsManager
{
public:
  CardsManager(){};
  ~CardsManager(){};
  static std::unordered_map<std::string, std::string> UID_Database_local;
  static std::unordered_map<std::string, std::string> Cards_Readed;

  bool AddCard(const std::string &uid, const std::string &time)
  {
    if (!findCardInLocal(uid))
      return false;

    if (this->Cards_Readed.find(uid) != this->Cards_Readed.end())
      return false;

    this->Cards_Readed[uid] = time;

    cout << "Tiempo registrado" << endl
         << uid << ": " << time << endl;
    return true;
  }

  void PrintAllCards()
  {
    cout << "====Tarjetas registradas=====" << endl;
    for (auto &card : this->Cards_Readed)
    {
      std::cout << "UID: " << card.first << " Time: " << card.second << std::endl;
    }
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
std::unordered_map<std::string, std::string> CardsManager::Cards_Readed;
std::unordered_map<std::string, std::string> CardsManager::UID_Database_local = {
    {"73A8EAFC", "Tarjeta 1"},
    {"538C1BF5", "Tarjeta 2"},
    {"B3530712", "Tarjeta 3"},
    {"9368850D", "Tarjeta 4"}};