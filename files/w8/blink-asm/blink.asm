; blink.asm
;
; Code from Fab Academy
; February 2016
; by Francisco Sanchez
; MIT License
;
;
; This program blinks the LED of a helloworld board

.include "tn44def.inc"

.org 0              ; sets the programs origin

sbi DDRA, 7
; sbi(reg,bit): Sets a bit of a register.  
; DDRA is the data direction register A
; Setting DDRA bit 7 makes pin PA7 a (digital) output
; A digital output can be switched ON/OFF for 5V or 0V

loop:       
; label for main loop
; labels must start with a letter and end with a colon

sbi PORTA,7 
; Sets bit 7 of register PORTA
; Turns pin PA7 to 5V

; Here it would come delay code

cbi PORTA,7
; Clears bit 7 of register PORTA
; Turns pin PA7 to 0V

; Here it would come delay code

rjmp loop
; relative jump to label called loop