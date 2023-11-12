#pragma once
#include <Arduino.h>
#include <cstdint>

class RgbChanger
{
private:
  static uint8_t redPin, greenPin, bluePin;

public:
  static void setupPins(uint8_t redPin, uint8_t greenPin, uint8_t bluePin);
  static void changeColors(uint8_t red, uint8_t green, uint8_t blue);
};