#include "TimeClock.h"

RTC_Millis TimeClock::rtc;
DateTime TimeClock::now;
bool TimeClock::isInRace = false;
DynamicJsonDocument TimeClock::doc_time(24);
DynamicJsonDocument TimeClock::status_race(24);
