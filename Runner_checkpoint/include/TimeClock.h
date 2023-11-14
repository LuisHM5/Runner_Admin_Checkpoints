#pragma once
#include <Wire.h>
#include <RTClib.h>
#include <string>
#include <sstream>
#include <iomanip>
class TimeClock
{
private:
  static RTC_Millis rtc;
  static DateTime now;
  static bool isInRace;

public:
  TimeClock(){};
  ~TimeClock(){};

  static void Init()
  {
    rtc.begin(DateTime(2023, 1, 1, 0, 0, 0));
  }

  static void Start()
  {
    Clear();
    now = rtc.now();
    isInRace = true;
  }

  static void Stop()
  {
    Clear();
    isInRace = false;
  }

  static std::string GetTime()
  {
    // Obtén la hora actual del RTC interno.
    now = rtc.now();

    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << now.hour() << ":"
        << std::setw(2) << std::setfill('0') << now.minute() << ":"
        << std::setw(2) << std::setfill('0') << now.second();
    return oss.str();
  }

  static bool GetStatus()
  {
    return isInRace;
  }

private:
  static void Clear()
  {
    // Reinicia el RTC interno a la fecha y hora predeterminadas.
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }
};
