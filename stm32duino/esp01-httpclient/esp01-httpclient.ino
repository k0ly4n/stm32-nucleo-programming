#include <ESP8266_AT_WebServer.h>
#include <SoftwareSerial.h>
#define RX_PIN A1
#define TX_PIN A0

ESP8266_AT_Client client;

SoftwareSerial EspSerial(RX_PIN, TX_PIN);

char ssid[] = "******";        // Wi-Fi SSID (name)
char pass[] = "******";        // password (key)
char server[] = "ident.me";

int status = WL_IDLE_STATUS;     // Wi-Fi status

void printMacAddress()
{
  byte mac[6];
  WiFi.macAddress(mac);
  char buf[20];
  sprintf_P(buf, PSTR( "%02X:%02X:%02X:%02X:%02X:%02X"), mac[5], mac[4], mac[3], mac[2], mac[1], mac[0]);
  Serial.print(F("MAC address: "));
  Serial.println(buf);
}

void listNetworks()
{
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1)
  {
    Serial.println(F("Can't establish Wi-Fi connection"));
    while (true);
  }

  Serial.print(F("The number of found Wi-Fi networks - "));
  Serial.println(numSsid);

  for (int thisNet = 0; thisNet < numSsid; thisNet++) 
  {
    Serial.print(thisNet);
    Serial.print(F(") "));
    Serial.print(WiFi.SSID(thisNet));
    Serial.print(F("\tSignal: "));
    Serial.print(WiFi.RSSI(thisNet));
    Serial.print(F(" dBm"));
    Serial.print(F("\tEncryption: "));
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
    Serial.println(F("Connecting..."));
    client.println(F("GET / HTTP/1.1"));
    client.println(F("Host: ident.me"));
    client.println(F("Connection: close"));
    client.println();
  }
  else
  {
    Serial.println(F("Error: connection failed!"));
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

  Serial.println(F("\nWiFi example for ESP-01 using STM32duino "));
  WiFi.init(&EspSerial);
  if (WiFi.status() == WL_NO_SHIELD)
  {
    Serial.println(F("Error: Wi-Fi module is not found!"));
    while (true);
  }

  // search for networks
  Serial.println();
  Serial.println(F("Searching networks..."));
  listNetworks();

  // connecting to network
  while ( status != WL_CONNECTED)
  {
    Serial.print(F("Connecting to the network with SSID: "));
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  Serial.print(F("Connected to network, got IP = "));
  Serial.println(WiFi.localIP());

  printMacAddress();

  // run GET-request
  httpRequest();
}

void loop()
{
  printoutData();
}
