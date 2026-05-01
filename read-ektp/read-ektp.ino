#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// --- 1. DEFINISI PIN
const uint8_t PIN_SDA  = 5;   
const uint8_t PIN_SCK  = 16; 
const uint8_t PIN_MOSI = 23; 
const uint8_t PIN_MISO = 19; 
const uint8_t PIN_RST  = 21; 

// --- 2. INISIALISASI DRIVER V2 ---
MFRC522DriverPinSimple ss_pin(PIN_SDA);
MFRC522DriverSPI driver{ss_pin}; 
MFRC522 mfrc522{driver};

void setup() {
  Serial.begin(115200);
  while (!Serial);
  SPI.begin(PIN_SCK, PIN_MISO, PIN_MOSI, PIN_SDA);

  mfrc522.PCD_Init(PIN_RST); 

  Serial.println(F("--- Sistem Absensi e-KTP Siap ---"));
  MFRC522Debug::PCD_DumpVersionToSerial(mfrc522, Serial);
}

void loop() {
  // Cek jika tidak ada kartu, balik ke awal loop
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // --- 3. PROSES PEMBACAAN UID ---
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();

  Serial.print(F("Terdeteksi UID: "));
  Serial.println(uidString);

  // --- 4. LOGIKA PENCCOKAN (Ganti XX dengan UID-mu) ---
  if (uidString == "A1B2C3D4") { 
    Serial.println(F(">>> AKSES DITERIMA! Halo OhmBase."));
    // Tambahin code nyalain relay/LED di sini
  } else {
    Serial.println(F(">>> AKSES DITOLAK! Kartu tidak dikenal."));
  }

  Serial.println(F("--------------------------------"));

  // Stop pembacaan agar tidak spamming
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  delay(2000); // Jeda 2 detik tiap scan
}