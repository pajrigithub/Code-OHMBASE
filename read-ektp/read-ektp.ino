#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

// Konfigurasi Pin SS (Slave Select)
MFRC522DriverPinSimple ss_pin(5);
MFRC522DriverSPI driver{ss_pin}; 
MFRC522 mfrc522{driver};         

void setup() {
  Serial.begin(115200);
  while (!Serial); 

  // Inisialisasi MFRC522
  mfrc522.PCD_Init();
  
  Serial.println(F("========================================"));
  Serial.println(F("MFRC522 Ready! Silakan tempelkan KTP..."));
  Serial.println(F("========================================"));
}

void loop() {
  // Cek apakah ada kartu baru
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Mengambil UID dan menyimpannya ke dalam String
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      uidString += "0";
    }
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  
  // Ubah ke huruf kapital agar lebih rapi (optional)
  uidString.toUpperCase();

  // Print hasil sesuai permintaan
  Serial.print(F("UID KTP ANDA: "));
  Serial.println(uidString);

  // Berhenti membaca kartu yang sama
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
  
  delay(1000); // Delay singkat agar tidak terbaca berulang kali secara cepat
}