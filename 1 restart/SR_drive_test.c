/*
bgilder
nixie clock display direct drive shift register method
uses 595 SR to interface with a pair of k155id1 to control 2 total tubes (currently)
spi code adapted from https://forum.arduino.cc/t/spi-transfer-on-74hc595-problem-solved/252061
31aug2021
*/

// uno spi pinout
//    MOSI      MISO      SCK     SS (slave)  SS (master)
//  11 or ICSP-4  12 or ICSP-1  13 or ICSP-3    10      -


#include <SPI.h>


uint8_t latchPin = 4; //Pin connected to pin 12 of 74HC595
uint8_t clockPin = 13;  //Pin connected to pin 11 of 74HC595
uint8_t dataPin = 11; //Pin connected to pin 14 of 74HC595
uint8_t spiModePin = 10;  //SS pin must be output (high) for master mode
uint8_t blankPin = 5; //Pin connected to pin 13 of 74HC595 (active low) -- output enable
uint8_t clearPin = 6; //Pin connected to pin 10 of 74HC595 (active low)

uint8_t theData = 0;  //the data to be written. each nybble is BCD for a tube
unsigned long currentms = 0;
unsigned long currentsec = 0;


uint8_t bin2bcd (uint8_t val) 
{ return val + 6 * (val / 10); 
}


void setup(){
 
 // Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(spiModePin, OUTPUT);
  pinMode(blankPin, OUTPUT);
  pinMode(clearPin, OUTPUT);

  digitalWrite(blankPin,LOW); //allows data
  digitalWrite(clearPin,HIGH);  //enables output


  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);  //8MHz clock

 
}


void loop() {

currentms = (millis()/10)%10;
currentsec  = (millis()/1000)%10;

writeThings();


}


void writeThings(){

  digitalWrite(latchPin,LOW);
  theData = (bin2bcd(currentms)<<4) | bin2bcd(currentsec);   //due to current wiring and MSB-first transmission, [right digit | left digit]
/*  for (byte h=0;h<8;h++)
  { Serial.print(0x1&(theData>>(7-h)));}
  Serial.print("\n");*/
  SPI.transfer(theData);
  digitalWrite(latchPin,HIGH);
  delayMicroseconds(707);
  digitalWrite(latchPin,LOW);
}

