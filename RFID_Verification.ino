#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

byte saved_UID[10] = {0xF4, 0x4C, 0x31, 0x00};
byte card_UID[10] = {};
uint8_t reader = 0;

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  Serial.println(F("Scan PICC to unlock door."));
}

void loop() {
  // Look for new cards
  if ( mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
      //Read UID
      Serial.print(F("Card UID:"));
      for (byte i = 0; i < mfrc522.uid.size; i++) {
        card_UID[i] = mfrc522.uid.uidByte[i];
        Serial.print(card_UID[i] < 0x10 ? " 0" : " ");
        Serial.print(card_UID[i], HEX);
      }
      Serial.println();
  
  //Compare card UID with the stored/saved UID
  if ( card_UID[0] == saved_UID[0] && card_UID[1] == saved_UID[1] &&
       card_UID[2] == saved_UID[2] && card_UID[3] == saved_UID[3] ){
        Serial.println("Door is unlocked!!");
        analogWrite(5, 255);
        delay(2000); //wait for 2 seconds
        digitalWrite(5, LOW);
        Serial.println();
  }
  else {
    Serial.println("Card is not recognised. Please try a different card.");
    Serial.println();
    analogWrite(7, 255);
    analogWrite(6, 255);
    delay(2000); //wait for 2 seconds
    digitalWrite(7, LOW);
    digitalWrite(6, LOW);
  }

  }
}
