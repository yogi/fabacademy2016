---
layout: page
title: Week 20 - Final Project Development
permalink: w20-final.html
---

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
 
The ones in the middle are the different sizes (1mm, 1.5mm, 2mm offset) to get the acrylic fit just right.

1.15mm increase had the best fit, here are the cut pieces:

<img src="images/w20-digit-test-cut.jpg"/>

"2" being displayed with the test digit on the 7-segment board:

<img src="images/w20-digit-2-test.jpg"/>

Looks like I might have to add 2 LEDs instead of 1 for each segment.

