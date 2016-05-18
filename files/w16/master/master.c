#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN PB3
const int delay = 100;

int main(void) {
    DDRB |= (1 << LED_PIN); // sets the data direction of pin 7 in port A to output
    
    while(1) {
        PORTB |= (1 << LED_PIN);  // writes a high value to pin 7
        _delay_ms(delay);
        
        PORTB &= ~(1 << LED_PIN); // write a low value to pin 7
        _delay_ms(delay);
    }

    return 0;
}