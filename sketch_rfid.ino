#include <SPI.h>
#include <MFRC522.h>
#include "ArduinoGraphics.h"
#include "Arduino_LED_Matrix.h"


#define RST_PIN 9
#define SS_PIN 10
MFRC522 mfrc522(SS_PIN, RST_PIN);

void show_text(const char *);
bool compareBytes(byte *arr1, byte *arr2, size_t bytes);

ArduinoLEDMatrix matrix;


void setup() {
  Serial.begin(115200);           // Start serial comms

  matrix.begin();
  show_text("   Made by Axel </:)");
  delay(1000);

  SPI.begin();                    // Start SPI Bus
  mfrc522.PCD_Init();             // Start MFRC522


  show_text("   Reading UID");
  delay(3000);
}


// Initialize UID credentials
byte currentUID[4];
byte adminUID[4] = {0xA3, 0x6B, 0x7D, 0xF5};

void loop() {
  // put your main code here, to run repeatedly:

  if( mfrc522.PICC_IsNewCardPresent()){
      if (mfrc522.PICC_ReadCardSerial()){
        // Show UID
        show_text("     CARD DETECTED");
        Serial.print("UID length (bytes): "); Serial.print(mfrc522.uid.size, DEC); Serial.println("");


        for (byte i=0; i< mfrc522.uid.size; i++){
          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(mfrc522.uid.uidByte[i], HEX);

          currentUID[i] = mfrc522.uid.uidByte[i];  
        }

        Serial.println("");

        // Example comparision to validate an NFC UID
        if(compareBytes(currentUID, adminUID, 4)){
          show_text("   AUTHORIZED");
        }else
          show_text("   UNAUTHORIZED");

        // Terminamos la lectura de la tarjeta  actual
        mfrc522.PICC_HaltA();
        show_text("   FINISHED READING");
      }
  }
}

void show_text(const char *text){
  Serial.println(text);
  
  matrix.beginDraw();

  matrix.stroke(0xFFFFFFFF);
  matrix.textScrollSpeed(50);

  matrix.textFont(Font_5x7);
  matrix.beginText(0,1, 0xFFFFFF);
  matrix.println(text);
  matrix.endText(SCROLL_LEFT);

  matrix.endDraw();
}

bool compareBytes(byte *arr1, byte *arr2, size_t bytes){
  for(byte i=0; i<bytes; i++){
    if(arr1[i] != arr2[i])
      return false;
  }
  return true;
}