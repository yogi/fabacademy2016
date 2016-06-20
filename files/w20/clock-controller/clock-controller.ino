#include <avr/io.h>
#include <SoftwareSerial.h>

const int rx=10;
const int tx=13;

SoftwareSerial mySerial(rx,tx);
int i=0;
char buf[12];

void setup() { 
  pinMode(rx,INPUT);
  pinMode(tx,OUTPUT);
//  DDRA |= (1 << PA0);
  mySerial.begin(9600);
} 

static char ASCII_ZERO = 48;
static char min_unit = 4;
static int sleep = 100;

void loop() {
//  mySerial.write(ASCII_ZERO + 1);
//  mySerial.write(ASCII_ZERO + 2);
//  mySerial.write(ASCII_ZERO + 3);
//  mySerial.write(ASCII_ZERO + min_unit);
//  min_unit++;
//  if (min_unit > 9) {
//    min_unit = 0;
//  }
  mySerial.write(49);
  delay(sleep);
}
