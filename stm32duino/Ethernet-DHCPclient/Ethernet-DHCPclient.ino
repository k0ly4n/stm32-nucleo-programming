#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF }; // MAC-address of an interface

IPAddress ip(192, 168, 3, 177); // fallback IP if DCHP failed
IPAddress myDns(192, 168, 3, 1); // DNS-server address
char server[] = "ident.me";
EthernetClient client;

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

void setup() {
  Serial.begin(9600);
  Serial.println("Initializing Ethernet with DHCP from STM32duino:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Error configuring DHCP");
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("  Etherner cable is not connected.");
    }
    Ethernet.begin(mac, ip, myDns); // static IP if DHCP failed
  } else {
    Serial.print("  DHCP assigned us an IP ");
    Serial.println(Ethernet.localIP());
  }

  // run GET-request
  httpRequest();
}

void loop() {
  printoutData();
}
