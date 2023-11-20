#pragma once
#include <cstdint>
#include <string>

using namespace std;
namespace ConfigNet
{
  extern string ssid;
  extern string pass;
  void setSSID(string ssid_);
  void setPASS(string pass_);
}

namespace ConfigServer
{
  extern uint16_t port;
}