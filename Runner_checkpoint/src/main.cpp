// Runner checkpoint receiver
#include <Arduino.h>
#include <iostream>
#include <SPI.h>
#include <RF24.h>
#include <MFRC522.h>
// SPI bus pins for NRF24L01
#define VSPI_CE 4
#define VSPI_CSN 2
#define VSPI_MISO MISO // 19
#define VSPI_MOSI MOSI // 23
#define VSPI_SCLK SCK  // 18
#define RF24_IRQ 5
// SPI bus pins for RC522
#define HSPI_MISO 12
#define HSPI_MOSI 13
#define HSPI_SCLK 14
#define HSPI_CS 15 // SDA
#define RST_PIN 27

// SPI bus instances
SPIClass *vspi = NULL;
SPIClass *hspi = NULL;

// RFID instance
RF24 radio(VSPI_CE, VSPI_CSN);
// MFRC522 instance
MFRC522 mfrc522(HSPI_CS, RST_PIN);
// Dirección de destino
const uint64_t pipeAddress = 0xE8E8F0F0E1LL; // Debe ser la misma en el receptor

using namespace std;

void setup()
{
  Serial.begin(9600);
  hspi = new SPIClass(HSPI);
  vspi = new SPIClass(VSPI);
  vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_CSN);
  hspi->begin(HSPI_SCLK, HSPI_MISO, HSPI_MOSI, HSPI_CS);
  // if (!radio.begin())
  // {
  //   Serial.println(F("radio hardware is not responding!!"));
  //   while (1)
  //   {
  //   } // hold in infinite loop
  // }
  // Radio setup
  radio.begin(vspi);
  radio.openReadingPipe(1, pipeAddress); // Abre un tubo de lectura en el mismo canal que el transmisor
  radio.startListening();                // Inicia la escucha
  radio.setPALevel(RF24_PA_MIN);         // Establece la potencia de salida del transmisor

  // Lector RFID setup
  mfrc522.PCD_Init();
  mfrc522.PCD_DumpVersionToSerial();
}

void loop()
{
  if (!radio.isChipConnected())
  {
    cout << "chip not connected" << endl;
  }
  if (radio.available())
  {
    // char newMessage[32];
    // string newMessage(10, ' ');
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
}
