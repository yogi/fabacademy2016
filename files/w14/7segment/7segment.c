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

const int DELAY = 1000;


int anode_for(int led) {
    if (led == A || led == B || led == C) 
        return ABC_ANODE;
    else if (led == D || led == E || led == F) 
        return DEF_ANODE;
    else  
        return G_ANODE;
}

void led_delay() {
    _delay_ms(DELAY);
}

void high_impedance(pin) {
    input(pin);
    clear(pin);
}

void flash(int led) {
    int anode = anode_for(led);
    
    output(anode);    
    output(led);
        
    set(anode);
    clear(led);

    led_delay();
    
    high_impedance(anode);    
    high_impedance(led);
}

void display(int n) {
    if (n == 0) {
        flash(A);
    }
}

int main(void) {
    high_impedance(A);
    high_impedance(B);
    high_impedance(C);
    high_impedance(D);
    high_impedance(E);
    high_impedance(F);
    high_impedance(G);

    while(1) {
        int i;
        for (i = 0; i < 10; i++) {
            display(i);
            _delay_ms(100);
        }
    }
    
    return 0;
}
//        output(DIR_REG, ABC_ANODE);    
//        output(DIR_REG, A);
//            
//        set(ABC_ANODE);
//        clear(A);
        
//        _delay_ms(500);

//        clear(DEF_ANODE);

//        input(DIR_REG, DEF_ANODE);    
//        input(DIR_REG, D);
//    }
    
//    while(1) {
//        DDRB |= (1 << SCK_PIN); // sets the data direction to output
//        PORTB |= (1 << SCK_PIN);  // high value to common-anode for group 1
//
//        PORTB &= ~(1 << LEDA_PIN); // write a low value
//         
//        _delay_ms(delay);
//    }
