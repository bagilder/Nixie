/***********/
// 74hc595 shift register nixie clock redesign
// bg 
// 31july2025
//
// i2c rtc timekeeping
// paired digits - one soviet decoder per pair, anode demuxed. 
// low nybble transmits clock data on SPI bus to 595 SR, high nybble controls power demux
// long string of cascading SR data
// an unholy frankenstein of SR demo code & rtc hack from og design as well as some nice bespoke flourishes
/***********/



// uno spi pinout:
//      MOSI          MISO          SCK         SS (slave)  SS (master)
//  11 or ICSP-4  12 or ICSP-1  13 or ICSP-3        10          -

// uno i2c pinout:
//  clock    data        
//   A5      A4

// rtc pinout
//  clock    data    intA    5v    gnd    
//    2       13      10     6     11


#include <Wire.h>
#include "SPI.h"
#define leftPower 0b000100000001000000010000  //current assumption is that two of these outputs can drive transistor pair to power demux the digits
#define rightPower 0b100000001000000010000000 //so these will be OR'd with the nybble corresponding to the data for each digit. data channels are bussed together between digits
#define leftPower1 0b00010000
#define rightPower1 0b10000000

// i think this is i2c shenanigans for rtc
#define hourmodereg 0xE0
#define hourmode24  0b00100011  //control register E, sets up periodic interrupt at 1hz
#define hourmode12 0b00000011  //control register E, sets up periodic iterrupt at 1hz
#define startupreg 0xF0
#define startupreset 0b00100000 //control register F, 2.2v dropout mode
#define writeaddr 0b0110010  //i2c slave address in write configuration
#define secreg 0x00
#define minreg 0x10
#define hrreg 0x20
#define clockadjreg 0x70

// spi settings for SR
uint8_t bussedData = 1; //0= fully directly driven digits; 1= single decoder, bussed data, power demuxed from higher 595 nybble outputs
const uint8_t latchPin = 4; //Pin connected to pin 12 of 74HC595
const uint8_t clockPin = 13;  //Pin connected to pin 11 of 74HC595
const uint8_t dataPin = 11; //Pin connected to pin 14 of 74HC595
const uint8_t spiModePin = 10;  //SS pin must be output (high) for master mode
const uint8_t blankPin = 5; //Pin connected to pin 13 of 74HC595 (active low) -- output enable
const uint8_t clearPin = 6; //Pin connected to pin 10 of 74HC595 (active low)
const uint8_t muxPin0 = 8;  //lowest bits of port B, for muxing latch pin 
const uint8_t muxPin1 = 9;

uint8_t clockDigitSize = 6; //number of digits in the clock lol. hrs/min/sec would be 6, hrs/min would be 4
uint8_t digitSets = 3;  //number of digit pairs in our dang arrangement (must be 4 or less without adding an additional mux output pin)
int blankingTime = 106; //microseconds
int latchTime = 207;  //microseconds
int8_t dimLevel = 0;  //scalar. when multiplied but some amount of blanking interval, acts as pwm brightness control. 0 = full brightness (since 0*interval is no delay)

const int LEDblinkPin = 3;
bool LEDblinkState = LOW;

const int buttLesserPin = A0; //if these pins don't exist we can use analog pins as inputs and initialize them as high with digitalWrite to set pullups
const int buttGreaterPin = A1;
const int buttMenuPin = A2;   //should these be on pullup pins so we can make them active low and not have to reset them high? yes.
uint8_t buttMenu = LOW;
uint8_t buttLesser = LOW;
uint8_t buttGreater = LOW;

// i2c pins
// sclPin = A5;  these are just here for reference
// sdaPin = A4;
const uint8_t rtcInterruptPin = 2;  //catches the interrupt from RTC module to hear the metronome for our seconds counting
uint8_t hourMode = 0;   //0 is 24h mode, 1 is 12h mode. we don't have a 12 hour mode so this won't be used.
uint8_t seconds=00;
uint8_t minutes=00;
uint8_t hours=00;
int8_t hr1 = 0;
int8_t hr0 = 0;
int8_t min1 = 0;
int8_t min0 = 0;
int8_t sec1 = 0;
int8_t sec0 = 0;

int8_t rtc_time[3]={0,0,0};  //rtc begins readback from register f before wrapping around to seconds, so ignore index 0 //jk we're trying to trash a read and only using 3 spaces in this array instead of 4
volatile uint8_t rtcPing = 0;
uint8_t timeUpdated = 0;

uint8_t gatekeeper = 1;


uint8_t bin2bcd (uint8_t val) 
{ return val + 6 * (val / 10); 
}

int8_t bcd2bin (uint8_t val) 
{ return val - 6 * (val >> 4); 
}


void isr()  //catch interrupt from rtc
{ 
  rtcPing++;
  //Serial.println("ping");
  if (rtcPing > 60)
  { rtcPing = 1;
  }
} //end "isr"


void setup()
{
  //Serial.begin(115200);

  uint8_t i2cSendData[16]={seconds,minutes,hours,0,0,0,0,0,0,0,0,0,0,0,bcd2bin(hourmode24),bcd2bin(startupreset)};

    //SR
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

    //INPUT
  pinMode(buttMenuPin, INPUT_PULLUP);
  pinMode(buttLesserPin, INPUT_PULLUP);
  pinMode(buttGreaterPin, INPUT_PULLUP);

    //RTC
  pinMode(rtcInterruptPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(rtcInterruptPin),isr,RISING);

  Wire.begin();
  Wire.beginTransmission(writeaddr);
  Wire.write(hourmodereg); 
  Wire.write(hourmode24);
  Wire.endTransmission();

  Wire.beginTransmission(writeaddr);
  Wire.write(startupreg);
  Wire.write(startupreset);
  Wire.endTransmission();
  
  Wire.beginTransmission(writeaddr);
  Wire.write(0x00); //seconds register, lowest register in the rtc
  for(byte f=0;f<16;f++)
  {  Wire.write(bin2bcd(i2cSendData[f]));
  //{  Wire.write(i2cSendData[f]);
  }
  Wire.endTransmission();

  //initial_test(); //set_menu();// 
  wait_for_set();

} //end "setup"

/*void button_test()
{
     while(!buttMenu)
    {   
        delay(20);      //kill time while button is still being held down
        buttMenu = digitalRead(buttMenuPin);
    }

    while(1){

    buttMenu = digitalRead(buttMenuPin);
    buttLesser = digitalRead(buttLesserPin);
    buttGreater = digitalRead(buttGreaterPin);
       
    
    if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                        write_things(1,1);
                    }
                    
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);     
                        write_things(5,5);                   
                    }
                    
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                        write_things(9,9);
                    }
                    
                }
    
    }
}

void initial_test()
{
    sec0 = 6;
    sec1 = 5;
    min0 = 9;
    min1 = 5;
    hr0 = 2;
    hr1 = 1;

    write_to_rtc();
}*/

void read_rtc()
{
  //Serial.println("reading rtc");
  uint8_t hello=0;
  Wire.beginTransmission(writeaddr);
  Wire.write(0x00);
  Wire.requestFrom(writeaddr,4);
  Wire.read();  //this trashes the first read, which should be register f (and not what we need)
  while(Wire.available())
  {  rtc_time[hello++]= Wire.read(); //be advised, these are BCD values
  }
  Wire.endTransmission();//

  seconds=rtc_time[0];
  minutes=rtc_time[1];
  hours=rtc_time[2];

  //Serial.println(bcd2bin(hours));
  //Serial.println(bcd2bin(minutes));
  //Serial.println(bcd2bin(seconds));

  hr1=hours>>4; //implementing pointers would erase my need to do this. super duper handy. ugh except then pointers ////plus then i couldn't hijack the timecard to do my bidding muahaha
  hr0=hours&0x0F;
  min1=minutes>>4;
  min0=minutes&0x0F;
  sec1=seconds>>4;
  sec0=seconds&0x0F;

} //end "read_rtc"


void write_to_rtc()
{
  //Serial.println("writing to rtc\n");
  /*seconds = (bin2bcd(sec1)<<4) | bin2bcd(sec0); //build our bytes
  minutes = (bin2bcd(min1)<<4) | bin2bcd(min0);
  hours = (bin2bcd(hr1)<<4) | bin2bcd(hr0);*/
  seconds = (sec1<<4) | sec0; //build our bytes
  minutes = (min1<<4) | min0;
  hours = (hr1<<4) | hr0;
 /* 
 if(!hourType) //if 12-hour mode
  {
    if(amPm) //if 12-hour mode and PM
    {   hours |= 1<<5;
    }
    else    //if 12-hour mode and AM
    {   hours &= ~(1<<5);
    }
  }
 */

  //Serial.println("beginning transmission");
  Wire.beginTransmission(writeaddr);
  Wire.write(0x00); //seconds register, lowest register in the rtc
  Wire.write(seconds);
  Wire.write(minutes);
  Wire.write(hours);
  Wire.endTransmission();
/*
Serial.println("\nseconds write =");
Serial.print(seconds);
Serial.println("\nminutes write =");
Serial.print(minutes);
Serial.println("\nhours write =");
Serial.print(hours);
*/
  //Serial.println("rtc written");

} //end "write_to_rtc"


void loop()
{

    if(rtcPing)
    {
        if(bcd2bin(minutes) == 59 && bcd2bin(seconds) == 59)
        {   hour_animate();
        }
        read_rtc();
        rtcPing--;
        if(rtcPing < 0)
        {   rtcPing = 0;
        }
        LEDblinkState != LEDblinkState;
        digitalWrite(LEDblinkPin, LEDblinkState);
    }
    uint32_t temp1=hr1;
    uint32_t temp0=hr0;
    //uint32_t sendLeft = (temp1<<16)|(min1<<8)|sec1;
    uint32_t sendLeft = hr1;
    sendLeft <<= 16;
    sendLeft |= min1<<8|sec1;
    //sendLeft |= sec1;
    uint32_t sendRight = hr0;//(temp0<<16)|(min0<<8)|sec0;
    sendRight <<=16;
    sendRight |= min0<<8|sec0;
    write_things(sendLeft,sendRight);
    check_buttons();

} //end "loop"


void wait_for_set()
{
    uint8_t displayDigit = 0;
    uint32_t sendthings = 0;
    unsigned long currentSec = millis();
    unsigned long previousSec = currentSec;
    unsigned long flipRate = 750;

    while(gatekeeper)
    {
      currentSec = millis();
      if((currentSec - previousSec >= flipRate) || (currentSec - previousSec < 0))
      {
        previousSec = currentSec;
        displayDigit++;
        if(displayDigit > 9)
        {   displayDigit = 0;
        }
      }
        //displayDigit = millis()<<10;
       // displayDigit = bin2bcd(displayDigit);
        uint32_t intermediate = (displayDigit<<8)|displayDigit;
        sendthings = intermediate;
        sendthings <<=16;
        sendthings |= (intermediate<<8)|intermediate;
        write_things(sendthings,sendthings);
        sendthings = 0;
        check_buttons();
    }

} //end "wait_for_set"



void write_things(uint32_t leftData, uint32_t rightData)
{
    /*digitalWrite(blankPin,HIGH);
    digitalWrite(clearPin,LOW);
    delayMicroseconds(blankingTime);
    digitalWrite(clearPin,HIGH);
   // digitalWrite(blankPin,LOW);
    delayMicroseconds(latchTime);*/
    /*uint32_t theData = 0x0000;
    theData = leftPower | leftData;*/
    uint8_t byte2=leftData>>16 | leftPower1;
    uint8_t byte1=leftPower1 | leftData>>8;
    uint8_t byte0=leftPower1 | leftData;
    SPI.transfer(byte0);
    SPI.transfer(byte1);
    SPI.transfer(byte2);
    //SPI.transfer(theData);
    digitalWrite(latchPin,HIGH);
    delayMicroseconds(latchTime);
    digitalWrite(latchPin,LOW); 

   /* SPI.transfer(0x0F0F0F0F);   //blanks to avoid ghosting
    digitalWrite(latchPin,HIGH);
    delayMicroseconds(latchTime);
    digitalWrite(latchPin,LOW);
    digitalWrite(clearPin,LOW);
    digitalWrite(clearPin,HIGH);
    //delayMicroseconds(blankingTime);*/

   /* theData = 0x0000;
    theData = rightPower | rightData;*/
    byte2 =  rightPower1 | rightData>>16;
    byte1 = rightPower1 | rightData>>8;
    byte0 = rightPower1 | rightData;
    SPI.transfer(byte0);
    SPI.transfer(byte1);
    SPI.transfer(byte2);
    //SPI.transfer(theData);
    digitalWrite(latchPin,HIGH);
    delayMicroseconds(latchTime);
    digitalWrite(latchPin,LOW);

    /*SPI.transfer(0x0F0F0F0F);   //blanks at the end so that if it gets hung, one digit isn't brighter than the other
    digitalWrite(latchPin,HIGH);
    delayMicroseconds(latchTime);
    digitalWrite(latchPin,LOW);
    digitalWrite(clearPin,LOW);
    digitalWrite(clearPin,HIGH);
    delayMicroseconds(blankingTime);*/


    ///////////////////
    //
    //
    //  can include some blanking interval to dim the displays pwm style. menu settable maybe. built-in blanking with invalid inputs, perhaps? send all FFFF and delay?
    //  keep at the end (or the beginning) tho so things aren't imbalanced
    //
    //
    ///////////////////

} //end "write_things"



void check_buttons()
{

  int buttAlert = millis();
  int buttAlertOld = buttAlert;
  int debounce = 20; //i guess?
  
  buttMenu = digitalRead(buttMenuPin);
  if(buttMenu == LOW)
  {
    delay(debounce);  //debounce
    while(buttAlert - buttAlertOld < 1000 || buttAlert - buttAlertOld < 0)
    { //check to see if button has been held long enough to enter menu (in this case, 1000ms = 1sec)
        uint32_t sendLeft = hr1;
        sendLeft <<= 16;
        sendLeft |= min1<<8|sec1;
        uint32_t sendRight = hr0;
        sendRight <<= 16;
        sendRight |= min0<<8|sec0;
        write_things(sendLeft,sendRight);
        buttAlert = millis();     
    }
    buttMenu = digitalRead(buttMenuPin);
    if(buttMenu == LOW)
    {   
        gatekeeper = 1;
        //button_test();
        set_menu();
    }
  }
}//end "check_buttons"


void set_menu()
{
    enum
    {
        setHourMode,
        setHourHigh,
        setHourLow,
        setMinHigh,
        setMinLow,
        setSecHigh,
        setSecLow,
        setBrightness,
        finishState
    };

    int state = setHourHigh;    //skipping setHourMode since we don't have 12 hour mode on the rtc

    buttMenu = digitalRead(buttMenuPin);
    buttLesser = digitalRead(buttLesserPin);
    buttGreater = digitalRead(buttGreaterPin);

    write_things(0,0);

    while(!buttMenu)
    {   
        delay(20);      //kill time while button is still being held down
        buttMenu = digitalRead(buttMenuPin);
    }

    while(gatekeeper)
    {
        buttMenu = digitalRead(buttMenuPin);
        buttLesser = digitalRead(buttLesserPin);
        buttGreater = digitalRead(buttGreaterPin);
        
        switch(state)
        {
            case setHourMode:   //we don't have a 12 hour mode so this won't be used
            {
                if(hourMode)
                {    write_things(1<<16, 2<<16);
                }
                else
                {   write_things(2<<8, 4<<8);
                }
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    hourMode = !hourMode;
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    hourMode = !hourMode;
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    state = setHourHigh;
                }
                break;
            }

            case setHourHigh:
            {
                int8_t hr1max = 2;
                if(hourMode)
                {    hr1max = 1;
                }
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    hr1--;
                    if(hr1 < 0)
                    {    hr1 = hr1max;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    hr1++;
                    if(hr1 > hr1max)
                    {    hr1 = 0;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    state = setHourLow;
                }
                break;
            }

            case setHourLow:
            {
                int8_t hr0max = 3;
                if(hourMode)
                {    hr0max = 2;
                }
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    hr0--;
                    if(hr0 < 0)
                    {    hr0 = hr0max;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    hr0++;
                    if(hr0 > hr0max)
                    {    hr0 = 0;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    state = setMinHigh;
                }
                break;
            }

            case setMinHigh:
            {
                int8_t min1max = 5;
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    min1--;
                    if(min1 < 0)
                    {    min1 = min1max;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    min1++;
                    if(min1 > min1max)
                    {    min1 = 0;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    state = setMinLow;
                }
                break;
            }

            case setMinLow:
            {
                int8_t min0max = 9;
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    min0--;
                    if(min0 < 0)
                    {    min0 = min0max;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    min0++;
                    if(min0 > min0max)
                    {    min0 = 0;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    //state = setSecHigh;
                    state = finishState;
                }
                break;
            }

            case setSecHigh:
            {
                int8_t sec1max = 5;
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    sec1--;
                    if(sec1 < 0)
                    {    sec1 = sec1max;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    sec1++;
                    if(sec1 > sec1max)
                    {    sec1 = 0;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    state = setSecLow;
                }
                break;
            }

            case setSecLow:
            {
                int8_t sec0max = 9;
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    sec0--;
                    if(sec0 < 0)
                    {    sec0 = sec0max;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    sec0++;
                    if(sec0 > sec0max)
                    {    sec0 = 0;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }

                    //state = setBrightness;
                    state = finishState;
                }
                break;
            }

            case setBrightness:
            {
                ///increment a certain amount difference each time. this will be the amount of time we linger on a blanking signal after writing each loop.
                ///this is emulating pwm brightness control.
                ///have it display simple numbers to represent "brightness levels" or whatever. set boundaries. 
                ///don't have it wrap around. up goes to up and stops, down goes to down and stops.
               
               uint8_t dimMax = 6;
                if(!buttLesser)
                {
                    while(!buttLesser)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttLesser = digitalRead(buttLesserPin);                        
                    }
                    dimLevel--;
                    if(dimLevel < 0)
                    {    dimLevel = 0;
                    }
                }
                else if(!buttGreater)
                {
                    while(!buttGreater)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttGreater = digitalRead(buttGreaterPin);                        
                    }
                    dimLevel++;
                    if(dimLevel > dimMax)
                    {    dimLevel = dimMax;
                    }
                }
                else if(!buttMenu)
                {
                    while(!buttMenu)
                    {
                        delay(20);      //kill time while button is still being held down
                        buttMenu = digitalRead(buttMenuPin);
                    }
                    state = finishState;
                }

                //write_things(something,idk);  //display the dimming level. or maybe (fake number-dim level) to act as "brightness level"? that might be more intuitive
                
                break;
            }

            case finishState:
            {
                write_to_rtc();
                gatekeeper = 0;
                break;
            }
        }
        uint32_t writeLeft = hr1;
        writeLeft <<= 16;
        writeLeft |= min1<<8|sec1;
        uint32_t writeRight = hr0;
        writeRight <<= 16;
        writeRight |= min0<<8|sec0;
        write_things(writeLeft, writeRight);
    }
} //end "set_menu"


void hour_animate()     //stolen from previous clock code & shoehorned. janky af but if it works idc
{
  /******************************/
  const float numeral_change_time = 23;//18;     //in ms, for fancy roll //float for decimal math later
  const int8_t roll_reset_time = 3;      //in seconds, totalish amount of time fancy roll animation takes  ///only used if legacyCounter
  /******************************/
    // with 36ms, this module takes 4.827 seconds to complete.
    // with 19ms, this module takes 4.172 seconds to complete.
    // with 18ms, this module takes 3.042 seconds to complete somehow????? but it is also way fast so the numbers look random.
    //dealer's choice. whatever you think looks best

  uint8_t timecard[6] = {hr1,hr0,5,9,5,9};
  unsigned long currentSec = millis();
  unsigned long punchIn = millis();
  unsigned long previousSec = 0;
  uint32_t temp0 = 0;
  uint32_t temp1 = 0; //writing data with 4bytes is gumming up the works so this is a kludge that fixes it. i don't have time to become a good programmer

  uint16_t roll_buffer[] = {hr1, hr0, 5, 9, 5, 9}; //maybe for iterating hmmmm?
  int magicNumber[] = {10+hr0,15,19,15,19,19};// change the 19's to 9 if you want to pick up the next digit without going a full rotation
  
  /** ERASE LOOPER **/
  for(uint8_t y=0; y < clockDigitSize; y++) 
  {
    while(roll_buffer[y] < magicNumber[y])  //whatever. sort out magic number later 
    {
      temp1 = roll_buffer[0]%10; //msb
      temp1 <<= 16;
      temp1 |= (roll_buffer[2]%10<<8)| roll_buffer[4]%10;
      temp0 = roll_buffer[1]%10; //lsb
      temp0 <<= 16;
      temp0 |=(roll_buffer[3]%10<<8) | roll_buffer[5]%10;
      write_things(temp1, temp0);
      currentSec = millis();
      if((currentSec - previousSec >= numeral_change_time) || (currentSec - previousSec < 0))
      {
        previousSec = currentSec;
        for(uint8_t t=0; t<=y; t++) //this sets as many positions in roll_buffer as that particular step should allow (wrt which digit you've gotten to)
        { roll_buffer[t]++; 
        }
      }
    }
  }
  /*** end ERASE LOOPER ***/

  read_rtc();
  
  /*** now the SET LOOPER ***/  
  magicNumber[0] = {hr1+10};
  magicNumber[1] = {hr0+20};
  magicNumber[2] = {30};
  magicNumber[3] = {40};
  magicNumber[4] = {50};
  magicNumber[5] = {60-roll_reset_time};  // will this allow the digits to stop rolling hr1 and hr0 once they hit their mark? there is a mod 10 in some stuff down there. i'm counting to 60 etc without resetting since i can just mod it back to a single digit. idk if that will work but i'm gonna try it.
  uint8_t timecard2[6] = {hr1,hr0,min1,min0,sec1,sec0};  
  
  //here's what i imagine should be happening:
  //go through each digit index. if the roll buffer is greater than the magic number, display the roll buffer. if not, display the magic number%10. at the end, subtract 1 from all magic number indexes.

    for(uint8_t goo=60-roll_reset_time;goo>0;goo--)
    {
      for(uint8_t bleep=0;bleep<clockDigitSize;bleep++)
      {     
        if(timecard2[bleep]>magicNumber[bleep])
        { roll_buffer[bleep]=timecard2[bleep];
        }
        else
        {roll_buffer[bleep]=magicNumber[bleep]%10;  //note this causes the numbers to go 9->0 but at speed it probably won't be noticeable
        }
        magicNumber[bleep]--;
      }
      currentSec = millis();
      previousSec = currentSec;
      while((currentSec - previousSec <= numeral_change_time) )//|| (currentSec - previousSec > 0))
      {
        temp1 = roll_buffer[0]; //msb
        temp0 = roll_buffer[1]; //lsb
        write_things((temp1<<16) | (roll_buffer[2]<<8) | roll_buffer[4], (temp0<<16) | (roll_buffer[3]<<8) | roll_buffer[5]);
        currentSec = millis();
      }
  }
  /*** end SET LOOPER ***/
  
  unsigned long punchOut = millis();
  unsigned long rollDuration = punchOut - punchIn;
  sec0 = rollDuration; ////maybeeeeeee??
  temp1 = roll_buffer[0]; //msb
  temp0 = roll_buffer[1]; //lsb
  write_things((temp1<<16) | (roll_buffer[2]<<8) | roll_buffer[4], (temp0<<16) | (roll_buffer[3]<<8) | roll_buffer[5]); ////mayyyybeeeeeeeeeeeeee?????

} //end "hour_animate"
