#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }; // MAC-адрес сетевого адаптера

IPAddress ip(192, 168, 3, 177); // используем этот статический адрес при отказе DHCP
IPAddress myDns(192, 168, 3, 1); // адрес DNS-сервера
char server[] = "ident.me";
EthernetClient client;

void httpRequest()
{
  Serial.println();
  client.stop();
  if (client.connect(server, 80))
  {
    Serial.println(F("Подключаемся..."));
    client.println(F("GET / HTTP/1.1"));
    client.println(F("Host: ident.me"));
    client.println(F("Connection: close"));
    client.println();
  }
  else
  {
    Serial.println(F("Ошибка: подключение не удалось!"));
  }
}

void printoutData(void)
{
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }
  Serial.flush();
}

void setup() {
  Serial.begin(9600);
  Serial.println("Инициализация Ethernet с DHCP из STM32duino:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Ошибка конфигурирования DHCP");
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("  кабель Ethernet отключен.");
    }
    Ethernet.begin(mac, ip, myDns); // статический адрес при отказе DHCP
  } else {
    Serial.print("  DHCP назначил адрес IP ");
    Serial.println(Ethernet.localIP());
  }

  // делаем GET-запрос
  httpRequest();
}

void loop() {
  printoutData();
}
