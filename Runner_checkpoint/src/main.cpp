// Runner transmitter
// Principal program libraries
#include <Arduino.h>
#include <iostream>
#include <unordered_map>
#include <array>
#include <iomanip>
#include <sstream>

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

// Map of UID's
uint8_t UID_Lecture[4];
unordered_map<std::string, std::string> UID_Database = {
    {"73A8EAFC", "Tarjeta 1"},
    {"538C1BF5", "Tarjeta 2"},
    {"B3530712", "Tarjeta 3"}};
unordered_map<std::string, std::string> Cards_Readed;
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

  // Buzzer setup
  pinMode(BUZZER_PIN, OUTPUT);
  // ledcSetup(channel, frequency, resolution): Config pwm
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
}

void loop()
{
  if (!radio.isChipConnected())
  {
    cout << "chip not connected" << endl;
  }

  if (radio.available())
  {
    string newMessage;
    radio.read(&newMessage, sizeof(newMessage));
    cout << newMessage << endl;
    string UID_Tag(newMessage); // Tamaño suficiente para los datos que se esperan recibir

    if (UID_Tag.length() > 0)
    {
      Serial.print("Datos recibidos:");
      cout << UID_Tag << endl;
    }
  }

  // Check if the lector is connected
  if (!mfrc522.PCD_PerformSelfTest())
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
  auto newUid = mfrc522.uid;

  mfrc522.PICC_HaltA();

  string UID_Tag = uidToString(newUid.uidByte, newUid.size);

  if (UID_Database.find(uidToString(newUid.uidByte, newUid.size)) != UID_Database.end())
  {
    cout << "UID " << UID_Tag << endl;
  }
  else
  {
    cout << "UID no encontrado en la base de datos: " << UID_Tag << endl;
  }

  // delay(100);
}
