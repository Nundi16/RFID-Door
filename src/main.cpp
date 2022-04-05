#include <Arduino.h>
#include <MFRC522.h>
#include <SPI.h>

#define SS_PIN 21
#define RST_PIN 22
#define GREEN_LED 25
#define RED_LED 33

// Variables
MFRC522 rfid = MFRC522(SS_PIN, RST_PIN);
byte nuidPICC[4] = {0, 0, 0, 0};
MFRC522::MIFARE_Key key;
String ActualRead = "";

void setup()
{
  Serial.begin(115200);
  Serial.println(F("Initialize System"));

  SPI.begin();
  rfid.PCD_Init();

  Serial.print(F("Reader :"));
  rfid.PCD_DumpVersionToSerial();

  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);

  digitalWrite(RED_LED,HIGH);
  digitalWrite(GREEN_LED,LOW);
}
String printDec(byte *buffer, byte bufferSize) 
{
  String _result = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    _result += buffer[i];
    /* Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC); */
  }
  return(_result);
}
String readRFID(void)
{
  for (byte i = 0; i < 6; i++)
  {
    key.keyByte[i] = 0xFF;
  }
  // Look for new 1 cards
  if (!rfid.PICC_IsNewCardPresent())
    return "";

  // Verify if the NUID has been readed
  if (!rfid.PICC_ReadCardSerial())
    return "";

  // Store NUID into nuidPICC array
  for (byte i = 0; i < 4; i++)
  {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

  return (printDec(rfid.uid.uidByte,rfid.uid.size));
}
void openDoor(){
  digitalWrite(RED_LED,LOW);
  digitalWrite(GREEN_LED,HIGH);

  sleep(5);

  digitalWrite(RED_LED, HIGH);
  digitalWrite(GREEN_LED, LOW);
}
void loop()
{
  ActualRead = readRFID();
  if (ActualRead == "14111414355")
  {
    openDoor();
  }
}
