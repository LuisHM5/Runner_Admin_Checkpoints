#include <Wire.h>
#include <RTClib.h>

class TimeClock
{
private:
  static RTC_Millis rtc;
  static DateTime now;

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
  }

  static std::string GetTime()
  {
    // Obtén la hora actual del RTC interno.
    now = rtc.now();

    // Concatena las cadenas.
    return std::to_string(now.hour()) + ":" + std::to_string(now.minute()) + ":" + std::to_string(now.second());
  }

private:
  static void Clear()
  {
    // Reinicia el RTC interno a la fecha y hora predeterminadas.
    rtc.adjust(DateTime(2023, 1, 1, 0, 0, 0));
  }
};

RTC_Millis TimeClock::rtc;
DateTime TimeClock::now;
