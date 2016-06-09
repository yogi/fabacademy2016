---
layout: page
title: Week 17 - Interface & Application Programming
permalink: w17-interface.html
---

[Class Notes](http://academy.cba.mit.edu/classes/interface_application_programming/index.html)   

## Assignment

> write an application that interfaces with an input &/or output device that you made,
    comparing as many tool options as possible
    
Looking at all the interface options, these seem interesting: 

* Javascript: because it seems to be the way all interfaces are going. Also would be interesting to play with serial 
    communications from the browser.
* WebGL, Three.js: 3D programming in the browser seems super interesting.

Given the above, I started researching what I could build. 

I came across a game called the [Aviator](http://tympanus.net/codrops/2016/04/26/the-aviator-animating-basic-3d-scene-threejs/), 
    which is written in Three.js.

I've been wanting to get into game development and physics engines, so this would be a good chance to do so.

So, I'm going to create a simple game that involves a character continuously moving forward, where you can control its vertical movement.
  For this I'll use the ambient reflection detecting sensor to control the up and down movements.
   
## Plan

*  Iteration 1
    * Write a simple Javascript program to accept input from the board and display it on the screen. 
    * Translate the sensor readings to an up or down command.
* Iteration 2
    * Create a simple 3D world and a character moving continuously from left to right. Use the sensor inputs to move the character up and down.
* Iteration 3
    * Add obstacles to avoid and treasures to collect and keep track of the score.
* Iteration 4 (optional)
    * Enrich the world visually. 

## Iteration 1

### Javascript program to accept input from the board
 
I used the code from the input-device week to check that the sensor is working fine. The python app showed the values correctly.
 

 
### Translate the sensor readings to an up or down command

