#include "TimeClock.h"

RTC_Millis TimeClock::rtc;
DateTime TimeClock::now;
bool TimeClock::isInRace = false;