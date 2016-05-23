#define output(directions, pin) (directions |= (1 << pin)) // set port direction for output
#define input(directions, pin) (directions &= (~(1 << pin))) // set port direction for input
#define set(port, pin) (port |= (1 << pin)) // set port pin
#define clear(port, pin) (port &= (~(1 << pin))) // clear port pin

#define LED_PIN PB3

#define I2C_SLAVE_ADDRESS 0x4 // Address of the slave
 
#include <TinyWireS.h>
 
void setup()
{
    output(DDRB, LED_PIN);
    clear(PORTB, LED_PIN);
    
    TinyWireS.begin(I2C_SLAVE_ADDRESS); // join i2c network
}
 
void loop()
{
  byte recd = 1;
  if(TinyWireS.available()) {
      recd = TinyWireS.receive();
      if(recd == 1) {
          clear(PORTB, LED_PIN);
      } else {
          set(PORTB, LED_PIN);
      }
  }
}
