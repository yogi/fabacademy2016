//Code for the Arduino Mega<
#include <Wire.h>
 
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
}
 
byte x = 0;

void loop() {
  Wire.beginTransmission(0x4);
  Wire.write(++x % 2);        
  Wire.endTransmission();    

  delay(1000);
}