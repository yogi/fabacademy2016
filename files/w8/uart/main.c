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

