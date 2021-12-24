#include "mbed.h"
#define RX_PIN D4
#define TX_PIN D5 
RawSerial computer(USBTX, USBRX, 9600);
RawSerial CC2541(TX_PIN, RX_PIN, 9600);
 
int main()
{
    computer.printf("Последовательный терминал Mbed для работы с модулем Bluetooth запущен.\n");

    while(true) {
        if(CC2541.readable()) {
            computer.putc(CC2541.getc());
        }
        if(computer.readable()) {
            CC2541.putc(computer.getc());
        }
    }
}
