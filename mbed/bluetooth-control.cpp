#include "mbed.h"
#define RX_PIN A1
#define TX_PIN A0 
RawSerial computer(USBTX, USBRX, 9600);
RawSerial CC2541(TX_PIN, RX_PIN, 9600);
DigitalOut led(LED1);

int main()
{
    computer.printf("Bluetooth terminal using Mbed for CC2541 is started at 9600 baud.\n");
    led = 0;

    while(true) {
        char bt_data;

        if(CC2541.readable()) {
            bt_data = CC2541.getc();
            computer.putc(bt_data);
        }
        if(computer.readable()) {
            wait_ms(10);
            CC2541.putc(computer.getc());
        }

        if (bt_data == 'X') {
            computer.printf("LED is off");
            led = 0;
            wait_ms(500);
        } else if (bt_data == 'O') {
            computer.printf("LED is on");
            led = 1;
        }
    }
}
