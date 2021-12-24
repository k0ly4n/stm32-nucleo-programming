#include "mbed.h"
#define RX_PIN A1
#define TX_PIN A0 
RawSerial computer(USBTX, USBRX, 9600);
RawSerial CC2541(TX_PIN, RX_PIN, 9600);
 
int main()
{
    computer.printf("Bluetooth terminal using Mbed for Bluetooth module is started.\n");

    while(true) {
        if(CC2541.readable()) {
            computer.putc(CC2541.getc());
        }
        if(computer.readable()) {
            CC2541.putc(computer.getc());
        }
    }
}
