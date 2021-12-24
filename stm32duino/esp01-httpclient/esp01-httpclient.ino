#include <ESP8266_AT_WebServer.h>
#include <SoftwareSerial.h>
#define RX_PIN A1
#define TX_PIN A0

ESP8266_AT_Client client;

SoftwareSerial EspSerial(RX_PIN, TX_PIN);

char ssid[] = "******";        // SSID сети (имя)
char pass[] = "******";        // пароль (ключ) сети
char server[] = "ident.me";

int status = WL_IDLE_STATUS;     // статус Wi-Fi

void printMacAddress()
{
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf_P(buf, PSTR( "%02X:%02X:%02X:%02X:%02X:%02X"), mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print(F("MAC адрес: "));
  Serial.println(buf);
}

void listNetworks()
{
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println(F("Не удается установить Wi-Fi соединение"));
    while (true);
  }

  Serial.print(F("Количество найденных сетей Wi-Fi - "));
  Serial.println(numSsid);

  for (int thisNet = 0; thisNet < numSsid; thisNet++) 
  {
    Serial.print(thisNet);
    Serial.print(F(") "));
    Serial.print(WiFi.SSID(thisNet));
    Serial.print(F("\tСигнал: "));
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(F(" дБм"));
    Serial.print(F("\tШифрование: "));
    printEncryptionType(WiFi.encryptionType(thisNet));
  }
}

void printEncryptionType(int thisType)
{
  switch (thisType)
  {
    case ENC_TYPE_WEP:
      Serial.print(F("WEP"));
      break;
    case ENC_TYPE_WPA_PSK:
      Serial.print(F("WPA_PSK"));
      break;
    case ENC_TYPE_WPA2_PSK:
      Serial.print(F("WPA2_PSK"));
      break;
    case ENC_TYPE_WPA_WPA2_PSK:
      Serial.print(F("WPA_WPA2_PSK"));
      break;
    case ENC_TYPE_NONE:
      Serial.print(F("None"));
      break;
  }
  Serial.println();
}

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

void setup()
{
  Serial.begin(115200);
  EspSerial.begin(115200);

  Serial.println(F("\nWiFi пример для ESP-01 и STM32duino "));
  WiFi.init(&EspSerial);
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("Ошибка: Wi-Fi модуль не найден!"));
    while (true);
  }

  // ищем сети
  Serial.println();
  Serial.println(F("Ищем доступные сети..."));
  listNetworks();

  // подключаемся к заданной сети
  while ( status != WL_CONNECTED)
  {
    Serial.print(F("Подключаемся к точке доступа с SSID: "));
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.print(F("Подключились к сети, получили IP = "));
  Serial.println(WiFi.localIP());

  printMacAddress();

  // делаем GET-запрос
  httpRequest();
}

void loop()
{
  printoutData();  
}
