#include "config.h"

namespace ConfigNet
{
  string ssid = "";
  string pass = "";

  void setSSID(string ssid_)
  {
    ssid = ssid_;
  };
  void setPASS(string pass_)
  {
    pass = pass_;
  };
}

namespace ConfigServer
{
  uint16_t port = 3001;
}