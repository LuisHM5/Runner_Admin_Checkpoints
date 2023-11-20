#pragma once
#include <Arduino.h>
#include <string>
#include <iomanip>
#include <sstream>
namespace Parser
{
  // UID to string converter
  std::string uidToString(const byte *uidBytes, byte bufferSize);
}