#include <SoftwareSerial.h>
#define RX_PIN A1
#define TX_PIN A0
SoftwareSerial CC2541(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("Последовательный терминал STM32duino для работы с модулем Bluetooth запущен.");
  CC2541.begin(9600);
}

void loop() { // 
  if (CC2541.available()) {
    Serial.write(CC2541.read());
  }
  if (Serial.available()) {
    CC2541.write(Serial.read());
  }
}
