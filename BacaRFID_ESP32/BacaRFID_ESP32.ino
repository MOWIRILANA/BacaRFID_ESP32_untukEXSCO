#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin};
MFRC522 mfrc522{driver};

const int ledPin = 2;

// UID Yg dapat diakses
byte allowedUID[] = {0x63, 0xD2, 0xC6, 0x0D};

void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  mfrc522.PCD_Init();
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
  Serial.println(F("Tempelkan kartu RFID/NFC ke reader..."));
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print(F("UID: "));
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  bool isAllowed = true;
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] != allowedUID[i]) {
      isAllowed = false;
      break;
    }
  }

  if (isAllowed) {
    digitalWrite(ledPin, HIGH);
    Serial.println(F("Akses diterima!"));
    delay(1000);
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, LOW);
    Serial.println(F("Akses ditolak!"));
  }

  mfrc522.PICC_HaltA();
  delay(500);
}