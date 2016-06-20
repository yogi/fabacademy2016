#include <avr/io.h>
#include <SoftwareSerial.h>

const int rx=10;
const int tx=13;

SoftwareSerial mySerial(rx,tx);
int i=0;
char buf[12];

void setup() { 
  pinMode(rx,INPUT);
  pinMode(tx,INPUT);
  mySerial.begin(9600);
} 

static int sleep = 1000;

void loop() {
  pinMode(tx,OUTPUT);

  // framing
  mySerial.write(6);
  mySerial.write(5);
  mySerial.write(4);
  mySerial.write(3);

  // time
  mySerial.write(1);
  mySerial.write(2);
  mySerial.write(3);
  mySerial.write(4);

  pinMode(tx,INPUT);
  delay(sleep);
} 
