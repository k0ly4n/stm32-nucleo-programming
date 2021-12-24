#include <SoftwareSerial.h>
#define RX_PIN A1
#define TX_PIN A0
SoftwareSerial CC2541(RX_PIN, TX_PIN);

void setup()
{
  Serial.begin(9600); // start serial port
  Serial.println("Bluetooth terminal using STM32duino for CC2541 is started at 9600 baud.");
  CC2541.begin(9600); // start Bluetooth-module
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // LED is off
}

void loop()
{
  String str_data = "";
  char bt_data;  
  CC2541.listen(); // listen for Bluetooth module
  while (CC2541.available() > 0) {
    bt_data = CC2541.read();
    str_data = String(bt_data);
    Serial.println(bt_data);
  }
  if (Serial.available()) {
    delay(10);
    CC2541.write(Serial.read());
  }
  if ( str_data == "X") {
    Serial.println("LED is off");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  } else if ( str_data == "O") {
    Serial.println("LED is on");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
