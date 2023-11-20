#include "Parser.h"

std::string Parser::uidToString(const byte *uidBytes, byte bufferSize)
{
  std::stringstream uidStringStream;

  for (byte i = 0; i < bufferSize; i++)
  {
    uidStringStream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(uidBytes[i]);
  }

  return uidStringStream.str();
}