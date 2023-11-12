#pragma once
#include <RTClib.h>
#include <string>
class TimeClock
{
private:
  static RTC_DS3231 rtc;
  static DateTime now;

public:
  TimeClock(){};
  ~TimeClock(){};
  static void Init()
  {
    if (!rtc.begin())
    {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }
  static void Start()
  {
    Clear();
    now = rtc.now();
  }
  static std::string GetTime()
  {
    // Get time from RTC
    now = rtc.now();

    // Concatenate the strings
    return std::to_string(now.hour()) + ":" + std::to_string(now.minute()) + ":" + std::to_string(now.second());
  }

private:
  static void Clear()
  {
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }
};

RTC_DS3231 TimeClock::rtc;
DateTime TimeClock::now;
