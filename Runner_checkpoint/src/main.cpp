// Runner transmitter
// Principal program libraries
#include <Arduino.h>
#include <iostream>
#include <unordered_map>
#include <array>
#include <iomanip>
#include <sstream>
// Web server
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "WifiSetup.h"
#include "Server.hpp"
#include "config.h"

// Flash File System
#include <SPIFFS.h>
// Bus SPI
#include <SPI.h>

// Radio NRF24L01
#include <RF24.h>

// RFID libraries
#include <Wire.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Modules
#include "TimeClock.h"
#include "CardsManager.h"

// SPI bus pins for NRF24L01 radio
#define VSPI_CE 4
#define VSPI_CSN 2
#define VSPI_MISO MISO // 19
#define VSPI_MOSI MOSI // 23
#define VSPI_SCLK SCK  // 18
#define RF24_IRQ 5

// SPI bus pins for RC522 RFID lector
#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_CS 15 // SDA
#define RST_PIN 27

// Buzzer
#define BUZZER_PIN 26
// ServerHTTP instance
ServerHTTP *serverhttp;

// Cards manager
CardsManager cardsManager;

// SPI bus instances
SPIClass *vspi = new SPIClass(VSPI);
SPIClass *hspi = new SPIClass(HSPI);

// RADIO NRF24L01 config & instance
RF24 radio(VSPI_CE, VSPI_CSN);

// MFRC522 config & instance
MFRC522DriverPinSimple hspi_ss_pin(HSPI_CS);
MFRC522DriverSPI driver{hspi_ss_pin, *hspi};
MFRC522 mfrc522{driver};

// Address of the pipe that the radio will open
const uint64_t pipeAddress = 0xE8E8F0F0E1LL; // Must be the same in the receiver

using namespace std;

// UID to string converter
std::string uidToString(const byte *uidBytes, byte bufferSize)
{
  std::stringstream uidStringStream;

  for (byte i = 0; i < bufferSize; i++)
  {
    uidStringStream << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(uidBytes[i]);
  }

  return uidStringStream.str();
}

void setup()
{
  Serial.begin(9600);
  SPIFFS.begin(true);
  WifiSetup::ScanNet();
  WifiSetup::Connect();
  serverhttp = new ServerHTTP();
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println((String) ":" + ConfigServer::port);
  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);

  ledcSetup(0, 1000, 10); // Configura el canal LEDC 0
  ledcAttachPin(BUZZER_PIN, 0);

  // SPI bus setup
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_CSN);
  pinMode(vspi->pinSS(), OUTPUT); // VSPI SS
  pinMode(hspi->pinSS(), OUTPUT); // HSPI SS

  // Radio setup
  radio.begin(vspi);
  radio.openReadingPipe(1, pipeAddress); // Abre un tubo de lectura en el mismo canal que el transmisor
  radio.startListening();                // Inicia la escucha
  radio.setPALevel(RF24_PA_MIN);

  // Lector RFID setup
  mfrc522.PCD_Init();
  mfrc522.PCD_AntennaOn();
  mfrc522.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max);

  // Test lector RFID
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  bool result = mfrc522.PCD_PerformSelfTest();
  Serial.println(F("-----------------------------"));
  Serial.print(F("Result: "));
  if (result)
    Serial.println(F("OK"));
  else
    Serial.println(F("DEFECT or UNKNOWN"));
  Serial.println();
  TimeClock::Init();
}

void loop()
{
  string newUID;

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Disconnected from WiFi");
    //  Realizar acciones adicionales si se desconecta
    WifiSetup::ScanNet();
    WifiSetup::Connect();
    // delete serverhttp;
    // serverhttp = new ServerHTTP();
    Serial.print("http://");
    Serial.print(WiFi.localIP());
    Serial.println((String) ":" + ConfigServer::port);
  }

  if (!radio.isChipConnected())
  {
    cout << "chip not connected" << endl;
  }

  if (radio.available())
  {
    string message;

    radio.read(&message, sizeof(message));
    string UID_Tag(message);

    if (UID_Tag.length() > 0)
    {
      newUID.assign(message);
      Serial.print("Datos recibidos:");
      cout << UID_Tag << endl;
    }
    else
    {
      cout << "No se recibieron datos" << endl;
    }
  }

  if (newUID.length() > 0)
  {
    cardsManager.AddCard(newUID, TimeClock::GetTime());
    cardsManager.PrintAllCards();
  }

  // Check if the lector is connected
  if (mfrc522.PCD_PerformSelfTest())
  {
    mfrc522.PCD_Init();
    mfrc522.PCD_AntennaOn();
    mfrc522.PCD_SetAntennaGain(MFRC522::PCD_RxGain::RxGain_max);
  }

  // Check if there is a new card
  if (!mfrc522.PICC_IsNewCardPresent())
    return;

  // Check if the card is readable
  if (!mfrc522.PICC_ReadCardSerial())
    return;

  // Card detected and readable
  cout << "Tarjeta detectada" << endl;

  // Save the UID and the time
  string Time = TimeClock::GetTime();
  auto newUid = mfrc522.uid;

  // halt mr522
  mfrc522.PICC_HaltA();

  // Transform the UID to string and save it
  string UID_Tag = uidToString(newUid.uidByte, newUid.size);
  cardsManager.AddCard(UID_Tag, Time);
  cardsManager.PrintAllCards();
}