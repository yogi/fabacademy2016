---
layout: page
title: Week 20 - Final Project Development
permalink: w20-final.html
---

--- 

# Thu, May 12th 

Given the plan for the final project, which is documented on the [Applications & Implications page](w18-applications.html), I'm starting
prototyping some of the key pieces that are required for the clock.

The first is the Motion Detector. I need a way to detect a hand-wave about 1 foot above or in front of the clock.
  
However I like working outside-in, i.e. starting with the end product and working backwards to the details. Therefore, I'm starting off
    creating the design in Rhino.
    
## Digit Test Cuts 

First step was to design the digits and try out a test cut to get the tolerances correct for the acrylic segments to fit 
    into the craft plywood outlines. 
    
Here is the design in Rhino. 

<img src="images/w20-rhino-test.jpg"/>

The original digit design is on the left, this is used for the plywood cut. 

The one on the right is used for the acrylic cut, it is increased in size by 1.15 mm (using Offset command).
 
The ones in the middle are the different sizes (0.1mm, 0.15mm, 0.2mm offset) to get the acrylic fit just right.

0.15mm increase had the best fit, here are the cut pieces:

<img src="images/w20-digit-test-cut.jpg"/>

"2" being displayed with the test digit on the 7-segment board:

<img src="images/w20-digit-2-test.jpg"/>

Looks like I might have to add 2 LEDs instead of 1 for each segment.

---

# Fri, May 13th 

Today I went to the local electronics market to buy a bunch of things I needed for the project. It turned out to be very 
    productive as I was able to get pretty much everything I needed:
    
<img src="images/w20-shopping.jpg"/>

* RTC DS3231 module 
* HCSR04 Ultrasonic sensor module
* IR Transceiver
* Bright LEDs
* Arduino Uno for prototyping
* Jumper cables
* Push buttons with longer shaft

For motion detection I plan to try the ultrasonic sensor first since I want detection over at least 1+ ft, which the IR sensor isn't 
   capable of.
   
### Ultrasonic sensor test

As per the [datasheet](http://www.micropik.com/PDF/HCSR04.pdf) and [user-guide](https://www.mpja.com/download/hc-sr04_ultrasonic_module_user_guidejohn.pdf) 
    the sensor requires a 10µs pulse on it's Trigger pin, which causes it to send out 8 40khz sound pulses and waits for the echo back. 
    The distance to the object can be calculated by multiplying the time taken by the speed of sound. 
 
I used an Arduino sketch from [here](http://playground.arduino.cc/Code/NewPing).

<pre>
#include &lt;NewPing.h&gt;
 
#define TRIGGER_PIN  12
#define ECHO_PIN     11
#define MAX_DISTANCE 50
 
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  delay(500);
  int uS = sonar.ping();
  Serial.print("Ping: ");
  Serial.print(uS / US_ROUNDTRIP_CM);
  Serial.println("cm");
}
</pre>
   
Here is a video showing the test: 

<video controls>
  <source src="images/w20-ultrasonic-test.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>


### Real Time Clock Test 

Next step is to test the RTC module. Here is the [DS3231 datasheet](https://datasheets.maximintegrated.com/en/ds/DS3231.pdf).

I used this [guide](http://tronixstuff.com/2014/12/01/tutorial-using-ds1307-and-ds3231-real-time-clock-modules-with-arduino/) and sketch
    to test the module. 
    
I hooked up the I2C SDA (data) and SCL (clock) to pins A4 and A5 respectively. 

Here's the sketch from the article:
 
<pre>
#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
// Convert normal decimal numbers to binary coded decimal
byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year)
{
  // sets time and date data to DS3231
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register
  Wire.write(decToBcd(second)); // set seconds
  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours
  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month
  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();
}

void setup()
{
  Wire.begin();
  Serial.begin(9600);
  // set the initial time here:
  // DS3231 seconds, minutes, hours, day, date, month, year
  setDS3231time(0,14,20,6,13,5,16);
}

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);
  *minute = bcdToDec(Wire.read());
  *hour = bcdToDec(Wire.read() & 0x3f);
  *dayOfWeek = bcdToDec(Wire.read());
  *dayOfMonth = bcdToDec(Wire.read());
  *month = bcdToDec(Wire.read());
  *year = bcdToDec(Wire.read());
}

void displayTime()
{
  byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  // retrieve data from DS3231
  readDS3231time(&second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month,
  &year);
  // send it to the serial monitor
  Serial.print(hour, DEC);
  // convert the byte variable to a decimal number when displayed
  Serial.print(":");
  if (minute<10)
  {
    Serial.print("0");
  }
  Serial.print(minute, DEC);
  Serial.print(":");
  if (second<10)
  {
    Serial.print("0");
  }
  Serial.print(second, DEC);
  Serial.print(" ");
  Serial.print(dayOfMonth, DEC);
  Serial.print("/");
  Serial.print(month, DEC);
  Serial.print("/");
  Serial.print(year, DEC);
  Serial.print(" Day of week: ");
  switch(dayOfWeek){
  case 1:
    Serial.println("Sunday");
    break;
  case 2:
    Serial.println("Monday");
    break;
  case 3:
    Serial.println("Tuesday");
    break;
  case 4:
    Serial.println("Wednesday");
    break;
  case 5:
    Serial.println("Thursday");
    break;
  case 6:
    Serial.println("Friday");
    break;
  case 7:
    Serial.println("Saturday");
    break;
  }
}
void loop()
{
  displayTime(); // display the real-time clock data on the Serial Monitor,
  delay(1000); // every second
}
</pre>

Most of the code is for formatting the time for display. The actual interaction is quite simple, and converts data between 
    [Binary Coded Decimal](https://en.wikipedia.org/wiki/Binary-coded_decimal) and Decimal. The RTC uses BCD for each part
     of the time (H:M:S, etc.).
      
Here is a video showing the test the time is printed out every second. 

<video controls>
  <source src="images/w20-rtc-test.mp4" type="video/mp4">
  Your browser does not support the video tag.
</video>


--- 

# Sat, May 14th 

Today I designed the cover for the clock in Rhino and did test cuts to get the press-fit tolerances and flexures correct. 
  
Here is the design for the testing press-fit tolerance. I kept one side constant and increased the size (using Offset command) of the other 
    pieces by 0.1 - 0.2 mm (The laser cutter has a kerf of 0.2 mm). 
   
<img src="images/w20-rhino-test-cuts.jpg"/>

These are the test cuts. 0.175 had a good fit, I'll be going with 0.18. 
    
<img src="images/w20-test-cuts.jpg"/>

I have to remember to add chamfers to make it easier to slide the pieces in.

The flexures took longer than expected and I had to try a number of variations. Here is the design:

<img src="images/w20-rhino-test-flexures.jpg"/>

The one which worked best had 1 mm horizontal spacing and 2 mm gap between each vertical cut, which is the bottom right onw below:
 
<img src="images/w20-test-flexures.jpg"/>


 --- 
 
# Sun, May 15th
  
Today my goal is to fabricate a digit board with the new bright LEDs I've bought.
 
I checked the voltage and current range for the LED using a DC power supply. 

At a steady current of 10mA, the LED is off at 1.4V and brightest at 2.2V. 

I was considering having 2 LEDs for each segment, but that significantly increases the effort to solder so many LEDs and
    resistors (if 2 LEDs for each segment are arranged in parallel with their own resistor). 
    
What I really want is a more diffused light coming from the LED. I found a neat [hack on Instructables](http://www.instructables.com/id/how-to-defuse-an-LED/?ALLSTEPS) 
    to do this by sandpapering the LED. 
    
Here is the sandpapered one next to a normal one:

<img src="images/w20-sandpapered-led.jpg"/>

Here is the output of the 2 when connected to a power source with 2.1V and 10mA:

<img src="images/w20-diffused-led-digit.jpg"/> 
    
The middle horizontal segment is the diffused one. The difference is not pronouned in the photo but the sandpapered one is 
    definitely diffusing the light.  

This is an excellent hack... it saves me a lot of time and effort!    
 
---
 
Next step is to fabricate the digit board with the new LEDs.

I plan to drill vias for the LED leads and solder them to the underside of the board. I don't 
