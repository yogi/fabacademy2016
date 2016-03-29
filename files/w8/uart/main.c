#include <avr/io.h>
#include <util/delay.h>

#define CLOCK 1000000
#define BAUD 9600

#define STX_PORT PORTA
#define STX_PIN  0

#define LED_PIN PA7

void sendByte( uint8_t c )
{
  c = ~c;
  
  STX_PORT &= ~(1<<STX_PIN);            // start bit
  
  uint8_t i = 10;
  for( ; i; i-- ){        // 10 bits
//    _delay_us( CLOCK / BAUD );            // bit duration
    _delay_us( 102 );            // bit duration
    if( c & 1 )
      STX_PORT &= ~(1<<STX_PIN);        // data bit 0
    else
      STX_PORT |= 1<<STX_PIN;           // data bit 1 or stop bit
    c >>= 1;
  }
}


volatile char data[32] = {
    0xAA,
    0xC5,
    0x00,
    0x1C,
    0xFD,
    0x60,
    0x00,
    0x00,
    0xAA,
    0xCC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x38,
    0xAB,
    0x81,
    0x9A,
    0x00,
    0x0F,
    0x68,
    0x00,
    0x0D,
    0xA4,
    0xA7,
    0xB0,
    0x3B,
    0xAA,
    0xCA
};

void blinkLED() {
    PORTA |= (1 << LED_PIN);  // writes a high value to pin 7
    _delay_ms(100);
    PORTA &= ~(1 << LED_PIN); // write a low value to pin 7
    _delay_ms(100);
}

int main(void)
{
    
    DDRA |= (1 << LED_PIN);
     
    uint8_t i = 0;      

    for(;;){
        sendByte(65);  // Send byte of data
        i++;                // Add one to the counter
        
        if(i == 32){        // End of Data
            i = 0;              // Reset Counter
            _delay_ms(500);      // Delay a bit before sending another block of data
            blinkLED();
        }
    }
    
    return 0;
}


/**
#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"


#define F_CPU 20000000UL    // CPU clock in Hertz 


volatile char data[32] = {
    0xAA,
    0xC5,
    0x00,
    0x1C,
    0xFD,
    0x60,
    0x00,
    0x00,
    0xAA,
    0xCC,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x38,
    0xAB,
    0x81,
    0x9A,
    0x00,
    0x0F,
    0x68,
    0x00,
    0x0D,
    0xA4,
    0xA7,
    0xB0,
    0x3B,
    0xAA,
    0xCA
};


int main(void)
{
    
    uartInit();         // Init the Uart
    uint8_t i = 0;      // Counter Varible
    
    for(;;){
        sendByte(data[i]);  // Send byte of Data
        i++;                // Add one to are Counter
        
        if(i == 32){        // End of Data
            i = 0;              // Reset Counter
            _delay_ms(20);      // Delay a bit before sending another block of data
        }
    }
    
    return 0;
}
**/