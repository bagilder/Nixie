/*
bgilder
nixie clock display direct drive shift register method
uses 595 SR to interface with a pair of k155id1 to control 2 total tubes (currently)
set up to allow a 3-to-8 mux (but only 2-to-4 connected) to distribute the latch pin 
spi code adapted from https://forum.arduino.cc/t/spi-transfer-on-74hc595-problem-solved/252061
3sept2021
*/

// uno spi pinout:
//    MOSI      MISO      SCK     SS (slave)  SS (master)
//  11 or ICSP-4  12 or ICSP-1  13 or ICSP-3    10      -



#include <SPI.h>
#define leftPower 0b00010000  //current assumption is that two of these outputs can drive transistor pair to power demux the digits
#define rightPower 0b10000000 //so these will be OR'd with the nybble corresponding to the data for each digit. data channels are bussed together between digits


uint8_t bussedData = 1; //0= fully directly driven digits; 1= single decoder, bussed data, power demuxed from higher 595 nybble outputs


uint8_t latchPin = 4; //Pin connected to pin 12 of 74HC595
uint8_t clockPin = 13;  //Pin connected to pin 11 of 74HC595
uint8_t dataPin = 11; //Pin connected to pin 14 of 74HC595
uint8_t spiModePin = 10;  //SS pin must be output (high) for master mode
uint8_t blankPin = 5; //Pin connected to pin 13 of 74HC595 (active low) -- output enable
uint8_t clearPin = 6; //Pin connected to pin 10 of 74HC595 (active low)
uint8_t muxPin0 = 8;  //lowest bits of port B, for muxing latch pin 
uint8_t muxPin1 = 9;


uint8_t digitSets = 4;  //number of digit pairs in our dang arrangement (must be 4 or less without adding an additional mux output pin)
int blankingTime = 106; //microseconds
int latchTime = 207;  //microseconds

uint8_t theData = 0;  //the data to be written. each nybble is BCD for a tube
unsigned long currentms = 0;
unsigned long currentsec = 0;


uint8_t bin2bcd (uint8_t val) 
{ return val + 6 * (val / 10); 
}


void setup()
{
  //Serial.begin(115200);

  pinMode(latchPin, OUTPUT);
  pinMode(spiModePin, OUTPUT);
  pinMode(blankPin, OUTPUT);
  pinMode(clearPin, OUTPUT);
  pinMode(muxPin0,OUTPUT);
  pinMode(muxPin1,OUTPUT);



  digitalWrite(blankPin,LOW); //allows data
  digitalWrite(clearPin,HIGH);  //enables output

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV2);  //8MHz clock

}


void loop()
{

  currentms = (millis()/100)%10;
  currentsec  = (millis()/1000)%10;
  
  for(uint8_t d=0; d<digitSets; d++)
  {
      digitalWrite(latchPin,LOW);
      PORTB &= 0b11111100;
      PORTB |= d; //does this need to be bin2bcd(d)? no if it's less than 8 why would it? do we need bin2bcd for the others, even??
      writeThings(currentsec, currentms);
  }

}


void writeThings(uint8_t leftData, uint8_t rightData)
{

  //digitalWrite(latchPin,LOW); //add back if eliminating this is the calling for loop above

  if(bussedData)
  {
    theData = rightPower | bin2bcd(rightData);
    SPI.transfer(theData);
    digitalWrite(latchPin,HIGH);
    delayMicroseconds(latchTime);
    digitalWrite(latchPin,LOW);

    SPI.transfer(0x0F);   //blanks to avoid ghosting
    digitalWrite(latchPin,HIGH);
    delayMicroseconds(latchTime);
    digitalWrite(latchPin,LOW);
    //delayMicroseconds(blankingTime);

    theData = leftPower | bin2bcd(leftData);
  }
  else
  {
    theData = (bin2bcd(rightData)<<4) | bin2bcd(leftData);   //due to current wiring and MSB-first transmission, [right digit | left digit]  ///hey if these are low enough, do we even need bin2bcd?
    /*for (byte h=0;h<8;h++)
      { Serial.print(0x1&(theData>>(7-h)));}
    Serial.print("\n");*/
  }
  SPI.transfer(theData);
  digitalWrite(latchPin,HIGH);
  delayMicroseconds(latchTime);
  digitalWrite(latchPin,LOW);

  SPI.transfer(0x0F);   //blanks at the end so that if it gets hung, one digit isn't brighter than the other
  digitalWrite(latchPin,HIGH);
  delayMicroseconds(latchTime);
  digitalWrite(latchPin,LOW);
  delayMicroseconds(blankingTime);

}
