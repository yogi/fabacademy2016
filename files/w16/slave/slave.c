#include <avr/io.h>
#include <util/delay.h>
#include "TinyWireS/TinyWireS.h"

#define output(directions, pin) (directions |= (1 << pin)) // set port direction for output
#define input(directions, pin) (directions &= (~(1 << pin))) // set port direction for input
#define set(port, pin) (port |= (1 << pin)) // set port pin
#define clear(port, pin) (port &= (~(1 << pin))) // clear port pin

#define LED_PIN PB3

#define I2C_SLAVE_ADDR 0x4

const int delay = 1000;

int main(void) {
    TinyWireS.begin(I2C_SLAVE_ADDR);
    output(DDRB, LED_PIN);
    
    byte recd = 0;
    
    while(1) {
        if(TinyWireS.available()) {
            recd = TinyWireS.receive();
            if(recd == 1) {
                clear(PORTB, LED_PIN);
            } else {
                set(PORTB, LED_PIN);
            }
        }
    }

    return 0;
}