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

const int FLASH_DELAY_MS = 1;
const int DIGIT_DISPLAY_MS = 100;

char DIGIT_SEGMENTS[10][7] = {
     {'A', 'B', 'C', 'D', 'E', 'F'},            // 0
     {'B', 'C'},                                // 1
     {'A', 'B', 'D', 'E', 'G'},                 // 2
     {'A', 'B', 'C', 'D', 'G'},                 // 3
     {'B', 'C', 'F', 'G'},                      // 4
     {'A', 'C', 'D', 'F', 'G'},                 // 5
     {'A', 'C', 'D', 'E', 'F', 'G'},            // 6
     {'A', 'B', 'C'},                           // 7
     {'A', 'B', 'C', 'D', 'E', 'F', 'G',},      // 8
     {'A', 'B', 'C', 'F', 'G',}                 // 9
};

int DIGIT_NUM_SEGMENTS[] = {
    6, // 0
    2, // 1
    5, // 2
    5, // 3
    4, // 4
    5, // 5
    6, // 6
    3, // 7
    7, // 8
    5  // 9
};



#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 100 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define led_delay() _delay_ms(100) // LED flash delay

#define START_BIT_CHECK_TIMEOUT 2 // 5 ms

#define serial_pins PINB
#define serial_pin_in (1 << PB1)

void get_char(volatile unsigned char *pins, unsigned char pin, char *rxbyte) {
   //
   // read character into rxbyte on pins pin
   //    assumes line driver (inverts bits)
   //
   *rxbyte = 0;
   
   int ctr = 0;
   while (pin_test(*pins,pin)) {
      //
      // wait for start bit
      //
      _delay_ms(1);
      ctr++;
      if (ctr >= START_BIT_CHECK_TIMEOUT) {
        return;
      }
   }
   
   //
   // delay to middle of first data bit
   //
   half_bit_delay();
   bit_delay();
   //
   // unrolled loop to read data bits
   //
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 0);
   else
      *rxbyte |= (0 << 0);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 1);
   else
      *rxbyte |= (0 << 1);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 2);
   else
      *rxbyte |= (0 << 2);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 3);
   else
      *rxbyte |= (0 << 3);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 4);
   else
      *rxbyte |= (0 << 4);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 5);
   else
      *rxbyte |= (0 << 5);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 6);
   else
      *rxbyte |= (0 << 6);
   bit_delay();
   if pin_test(*pins,pin)
      *rxbyte |= (1 << 7);
   else
      *rxbyte |= (0 << 7);
   //
   // wait for stop bit
   //
   bit_delay();
   half_bit_delay();
   }

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

void high_impedance(pin) {
    input(pin);
    clear(pin);
}

void flash(char led) {
    int anode_pin = anode_pin_for(led);
    int led_pin = led_pin_for(led);
    
    output(anode_pin);    
    output(led_pin);
        
    set(anode_pin);
    clear(led_pin);

    _delay_ms(FLASH_DELAY_MS);
    
    high_impedance(anode_pin);    
    high_impedance(led_pin);
}

void flash_segments(char segments[], int len) {
    int i;
    for (i = 0; i < len; i++) {
        flash(segments[i]);
    }
}

void display(int digit, int duration_ms) {
    int i;
    char *segments = DIGIT_SEGMENTS[digit];
    int num_segments = DIGIT_NUM_SEGMENTS[digit];
    int cycles = duration_ms / (FLASH_DELAY_MS * num_segments);
    for (i = 0; i < cycles; i++) {
        flash_segments(segments, num_segments);
    }
}

int main(void) {
    static char chr = 0;
    
    while(1) {
        get_char(&serial_pins, serial_pin_in, &chr);
        display(chr, DIGIT_DISPLAY_MS);
    }
    
    return 0;
}

/*
int main(void) {
    int i = 0;
    while(1) {
        display(i, DIGIT_DISPLAY_MS);
        _delay_ms(1000);
        if (++i >= 10)
            i = 0;
    }
    
    return 0;
}
*/

/*

int main(void) {
    char leds[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
    int i = 0;
    
    while(1) {
        flash(leds[i++]);
        _delay_ms(1000);
        if (i >= 7)
            i = 0;
    }
    
    return 0;
}
*/
