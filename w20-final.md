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
    
## Clock Case Design 

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
    
* RTC DS3231 module 
* HCSR04 Ultrasonic sensor module
* IR Transceiver
* Jumper cables
* Arduino Uno for prototyping
* Bright LEDs

For motion detection I plan to try the ultrasonic sensor first since I want detection over at least 1+ ft, which the IR sensor isn't 
   capable of.
   
### Ultrasonic sensor test

The sensor requires a 10µs pulse on it's Trigger pin, which causes it to send out 8 40khz sound pulses and waits for the echo back. 
 The distance to the object can be calculated by dividing the time taken by the speed of sound. 
 
I used an Arduino sketch I found [here](http://playground.arduino.cc/Code/NewPing).

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

Next step is to test the RTC DS3231 module.

