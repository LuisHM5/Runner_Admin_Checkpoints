#include "rgbChanger.h"
uint8_t RgbChanger::redPin = 0;
uint8_t RgbChanger::greenPin = 0;
uint8_t RgbChanger::bluePin = 0;

void RgbChanger::setupPins(uint8_t redPin, uint8_t greenPin, uint8_t bluePin)
{
  RgbChanger::redPin = redPin;
  RgbChanger::greenPin = greenPin;
  RgbChanger::bluePin = bluePin;
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}
void RgbChanger::changeColors(uint8_t red, uint8_t green, uint8_t blue)
{
  // Serial.println((String) "redPin: " + RgbChanger::redPin + " GreenPin:" + RgbChanger::greenPin + " BluePin:" + RgbChanger::bluePin);
  // Serial.println((String) "red: " + red + " greenPin:" + green + " BluePin:" + blue);
  analogWrite(RgbChanger::redPin, red);
  analogWrite(RgbChanger::greenPin, green);
  analogWrite(RgbChanger::bluePin, blue);
}