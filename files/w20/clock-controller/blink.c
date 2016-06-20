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
#include "usi_i2c_master.c"

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

// Convert normal decimal numbers to binary coded decimal
char decToBcd(char val) {
  return( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
char bcdToDec(char val) {
  return( (val/16*10) + (val%16) );
}

#define RTC_I2C_ADDRESS 0x68

void set_time(char second, char minute, char hour, char dayOfWeek, char dayOfMonth, char month, char year) {
    // sets time and date data to DS3231
    char data_len = 9;
    char data[data_len]; 
        
    data[0] = (RTC_I2C_ADDRESS << 1);
    data[1] = 0;                        // set next input to start at the seconds register
    data[2] = decToBcd(second);         // set seconds
    data[3] = decToBcd(minute);         // set minutes
    data[4] = decToBcd(hour);           // set hours
    data[5] = decToBcd(dayOfWeek);      // set day of week (1=Sunday, 7=Saturday)
    data[6] = decToBcd(dayOfMonth);     // set date (1 to 31)
    data[7] = decToBcd(month);          // set month
    data[8] = decToBcd(year);           // set year (0 to 99)

    USI_I2C_Master_Start_Transmission(data, data_len);
}

void get_time(char *hour_tens, char *hour_units, char *minute_tens, char *minute_units) {
    {   // using explicit scopes to ensure the correct arrays get used
     
        // position the internal address for subsequent reads 
        char data_len = 2;
        char data[data_len]; 
            
        data[0] = ((RTC_I2C_ADDRESS << 1) | 0);
        data[1] = 0;                        // set DS3231 register pointer to 00h
        USI_I2C_Master_Start_Transmission(data, data_len);
    }
    {
        // read 7 bytes of time values from current position
        char time_len = 8;
        char time[time_len]; 
    
        time[0] = ((RTC_I2C_ADDRESS << 1) | 1);
        USI_I2C_Master_Start_Transmission(time, time_len);
    
        char minute, hour;
        char second, dayOfWeek, dayOfMonth, month, year;
        
        second = bcdToDec(time[1] & 0x7f);
        minute = bcdToDec(time[2]);
        hour = bcdToDec(time[3] & 0x3f);
        dayOfWeek = bcdToDec(time[4]);
        dayOfMonth = bcdToDec(time[5]);
        month = bcdToDec(time[6]);
        year = bcdToDec(time[7]);
        
        *hour_tens = hour / 10;
        *hour_units = hour % 10;
        *minute_tens = minute / 10;
        *minute_units = minute % 10;
    }
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
    
    set_time(15,    // sec
             25,    // min
             1,     // hour
             4,     // day of week
             22,    // day
             4,     // month
             16);   // year 
    
    char hour_tens, hour_units, minute_tens, minute_units;
    
    //
    // main loop
    //
    while (1) {
        // get time
        get_time(&hour_tens, &hour_units, &minute_tens, &minute_units);
        
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
        put_char(&serial_port, serial_pin_out, hour_tens);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, hour_units);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, minute_tens);
        char_delay();
        
        put_char(&serial_port, serial_pin_out, minute_units);
        char_delay();
        
        if (++n >= 10) n = 0;
        
        _delay_ms(1000);
    }
}