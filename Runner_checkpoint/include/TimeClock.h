#pragma once
#include <Wire.h>
#include <RTClib.h>
#include <string>
#include <ArduinoJson.h>
#include <sstream>
#include <iomanip>
class TimeClock
{
private:
  static RTC_Millis rtc;
  static DateTime now;
  static bool isInRace;
  static DynamicJsonDocument doc_time;
  static DynamicJsonDocument status_race;

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
    oss << std::setw(2) << std::setfill('0') << std::to_string(now.hour()) << ":"
        << std::setw(2) << std::setfill('0') << std::to_string(now.minute()) << ":"
        << std::setw(2) << std::setfill('0') << std::to_string(now.second());
    return oss.str();
  }

  static bool GetStatus()
  {
    return isInRace;
  }

  static void SetStatus(bool status)
  {
    isInRace = status;
  }

  static String getDataTimeJson()
  {
    doc_time["time"] = TimeClock::GetTime().c_str();

    // Serialize JSON to string
    return doc_time.as<String>();
  }

  static String getStatusRaceJson()
  {
    status_race["status"] = TimeClock::GetStatus() ? "true" : "false";

    // Serialize JSON to string
    return status_race.as<String>();
  }

private:
  static void Clear()
  {
    // Reinicia el RTC interno a la fecha y hora predeterminadas.
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }
};
