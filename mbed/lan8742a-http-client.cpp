#include "mbed.h"
#include "EthernetInterface.h"

EthernetInterface net;
Serial computer(USBTX, USBRX, 9600);

int main() {
    computer.printf("Ethernet example for Mbed OS\n");
    net.connect();

    const char *ip = net.get_ip_address();
    computer.printf("Connected to network, got IP = %s\n", ip ? ip : "error, no IP!");
    computer.printf("MAC: %s\n", net.get_mac_address()); 

    computer.printf("Connecting...\n");
    TCPSocket socket;
    socket.open(&net);
    socket.connect("ident.me", 80);

    char sbuffer[] = "GET / HTTP/1.1\r\nHost: ident.me\r\n\r\n";
    int scount = socket.send(sbuffer, sizeof sbuffer);
    char rbuffer[256];
    int rcount = socket.recv(rbuffer, sizeof rbuffer);
    if (rcount > 0)
        computer.printf("%s", rbuffer);

    socket.close();
    net.disconnect();
}
