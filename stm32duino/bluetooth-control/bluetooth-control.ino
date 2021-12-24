#include <SoftwareSerial.h>
#define RX_PIN A1
#define TX_PIN A0
SoftwareSerial CC2541(RX_PIN, TX_PIN);

void setup()
{
  Serial.begin(9600); // активация последовательного порта
  Serial.println("Bluetooth терминал STM32duino для CC2541 запущен на скорости 9600.");
  CC2541.begin(9600); // активация Bluetooth-модуля
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW); // светодиод выключен
}

void loop()
{
  String str_data = "";
  char bt_data;  
  CC2541.listen(); // следим за модулем Bluetooth
  while (CC2541.available() > 0) {
    bt_data = CC2541.read();
    str_data = String(bt_data); // сохраняем в тип String
    Serial.println(bt_data);
  }
  if (Serial.available()) {
    delay(10);
    CC2541.write(Serial.read());
  }
  if ( str_data == "X") {
    Serial.println("Светодиод выключен");
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  } else if ( str_data == "O") {
    Serial.println("Светодиод включен");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}
