#include <avr/io.h>
#include <util/delay.h>

#define SCK_PIN PB2
#define MOSI_PIN PB0

#define ABC_ANODE   SCK_PIN
#define A           PB4
#define B           PB3
#define C           MOSI_PIN

#define DEF_ANODE   MOSI_PIN
#define D           PB4
#define E           PB3
#define F           SCK_PIN

#define G_ANODE     PB3
#define G           PB4

#define DIR_REG   DDRB

#define output(pin) (DIR_REG |= (1 << pin)) // set port direction for output
#define input(pin) (DIR_REG &= (~ (1 << pin))) // set port direction for input
#define set(pin) (PORTB |= (1 << pin)) // set port pin
#define clear(pin) (PORTB &= (~ (1 << pin))) // clear port pin

const int DELAY = 300;


int anode_pin_for(char led) {
    if (led == 'A' || led == 'B' || led == 'C') 
        return ABC_ANODE;
    else if (led == 'D' || led == 'E' || led == 'F') 
        return DEF_ANODE;
    else  
        return G_ANODE;
}

int led_pin_for(char led) {
    if (led == 'A' || led == 'D' || led == 'G')
        return PB4;
    else if (led == 'B' || led == 'E')
        return PB3;
    else if (led == 'C')
        return MOSI_PIN;
    else // 'F'
        return SCK_PIN;
}

void led_delay() {
    _delay_ms(DELAY);
}

void high_impedance(pin) {
    input(pin);
    clear(pin);
}

void flash(char led) {
    high_impedance(PB0);
    high_impedance(PB1);
    high_impedance(PB2);
    high_impedance(PB3);
    high_impedance(PB4);

    int anode_pin = anode_pin_for(led);
    int led_pin = led_pin_for(led);
    
    output(anode_pin);    
    output(led_pin);
        
    set(anode_pin);
    clear(led_pin);

    led_delay();
    
    high_impedance(anode_pin);    
    high_impedance(led_pin);
}

void display(int n) {
    if (n == 0) {
        flash(A);
    }
}

int main(void) {
    char leds[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    int i = 0;
    
    while(1) {
        flash(leds[i++]);
        _delay_ms(100);
        if (i >= 7)
            i = 0;
    }
    
    return 0;
}
