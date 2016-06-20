//
// Neil Gershenfeld
// 10/25/12
//
// (c) Massachusetts Institute of Technology 2012
// This work may be reproduced, modified, distributed,
// performed, and displayed for any purpose. Copyright is
// retained and must be preserved. The work is provided
// as is; no warranty is provided, and users accept all
// liability.
//

#include <avr/io.h>
#include <util/delay.h>

#define output(directions,pin) (directions |= pin) // set port direction for output
#define input(directions,pin) (directions &= (~pin)) // set port direction for input
#define set(port,pin) (port |= pin) // set port pin
#define clear(port,pin) (port &= (~pin)) // clear port pin
#define pin_test(pins,pin) (pins & pin) // test for port pin
#define bit_test(byte,bit) (byte & (1 << bit)) // test for bit set
#define bit_delay_time 102 // bit delay for 9600 with overhead
#define bit_delay() _delay_us(bit_delay_time) // RS232 bit delay
#define half_bit_delay() _delay_us(bit_delay_time/2) // RS232 half bit delay
#define char_delay() _delay_ms(10) // char delay

#define serial_port PORTA
#define serial_direction DDRA
#define serial_pin_out (1 << PB0)


void put_char(volatile unsigned char *port, unsigned char pin, char txchar) {
    //
    // send character in txchar on port pin
    //    assumes line driver (inverts bits)
    //
    // start bit
    //
    clear(*port,pin);
    bit_delay();
    //
    // unrolled loop to write data bits
    //
    if bit_test(txchar,0)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,1)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,2)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,3)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,4)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,5)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,6)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    if bit_test(txchar,7)
    set(*port,pin);
    else
    clear(*port,pin);
    bit_delay();
    //
    // stop bit
    //
    set(*port,pin);
    bit_delay();
    //
    // char delay
    //
    bit_delay();
}

char n = 2;

int main(void) {
    //
    // set clock divider to /1
    //
    CLKPR = (1 << CLKPCE);
    CLKPR = (0 << CLKPS3) | (0 << CLKPS2) | (0 << CLKPS1) | (0 << CLKPS0);
    //
    // initialize output pins
    //
    output(serial_direction, serial_pin_out);
    
    set_time()
    
    
    //
    // main loop
    //
    while (1) {
        // get time
        
        // send framing
        put_char(&serial_port, serial_pin_out, 6);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, 5);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, 4);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, 3);
        char_delay();
        
        // send time
        put_char(&serial_port, serial_pin_out, 1);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, 2);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, 4);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, n);
        char_delay();
        
        if (++n >= 10) n = 0;
        
        _delay_ms(1000);
    }
}