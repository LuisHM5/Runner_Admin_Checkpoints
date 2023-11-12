#pragma once
#include <RTClib.h>
#include <string>
class Time
{
private:
  static RTC_DS3231 rtc;
  static DateTime now;

public:
  Time(){};
  ~Time(){};
  static void Time::Init()
  {
    if (!rtc.begin())
    {
      Serial.println("RTC is NOT running!");
      // following line sets the RTC to the date & time this sketch was compiled
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    else
    {
      Serial.println("RTC is running!");
    }
  }
  static void Time::StartClock()
  {
    ClearClock();
    now = rtc.now();
  }

  static void Time::ClearClock()
  {
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }
  static std::string Time::GetTime()
  {
    // Get time from RTC
    now = rtc.now();

    // Concatenate the strings
    return std::to_string(now.hour()) + ":" + std::to_string(now.minute()) + ":" + std::to_string(now.second());
  }
};
