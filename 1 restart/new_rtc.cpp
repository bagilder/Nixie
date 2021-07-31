
/**
frankensteining a kludged nixie clock program with some i2c rtc (RX8025) interrupts for timekeeping (works for both 4 & 6 digits)
b gilder
30jul2021
**/


/*notes on timekeeping:
-set to always be 24hour mode within rtc, and converting to 12hour mode in software.
-a 1hz interrupt from the rtc is the metronome for our clock.
-every 10 seconds (on the 5s) we ask the rtc what it thinks the time actually is and update our shit to agree.
-rtc gives back BCD values for each nybble of the time register bytes, so {BCDH}{BCDL} for each byte. can leave this and mask stuff if we want, or bcd2bin() if that's easier
*/


#include <Wire.h>


//#define hourmode12 0b00000011  //control register E, sets up periodic iterrupt at 1hz
#define hourmodereg 0xe0
#define hourmode24  0b00100011  //control register E, sets up periodic interrupt at 1hz
#define startupreg 0xf0
#define startupreset 0b00100000 //control register F, 2.2v dropout mode
#define writeaddr 0b0110010  //i2c slave address in write configuration
#define secreg 0x00
#define minreg 0x10
#define hrreg 0x20
#define clockadjreg 0x70

uint8_t seconds=56;
uint8_t minutes=59;
uint8_t hours=21;
uint8_t rtc_time[3]={0,0,0};	//rtc begins readback from register f before wrapping around to seconds, so ignore index 0 //jk we're trying to trash a read and only using 3 spaces in this array instead of 4
volatile uint8_t rtcPing = 0;

uint8_t bcd2bin (uint8_t val) 
{ return val - 6 * (val >> 4); 
}

uint8_t bin2bcd (uint8_t val) 
{ return val + 6 * (val / 10); 
}




setup()
{
	uint8_t i2cSendData[16]={seconds,minutes,hours,0,0,0,0,0,0,0,0,0,0,0,bcd2bin(hourmode24),bcd2bin(startupreset)};

	pinMode(2,INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(2),isr,RISING);

	//usually takes about a second for rtc to fire up, so. keep that in mind.
	Wire.begin();
	Wire.beginTransmission(writeaddr);
	Wire.write(hourmodereg); //WHY THE FUCK ISN'T THIS WRITING TO THE E REGISTER????????????????? //it was. i just wasn't displaying it.
	Wire.write(hourmode24);
	Wire.endTransmission();

	Wire.beginTransmission(writeaddr);
	Wire.write(startupreg);
	Wire.write(startupreset);
	Wire.endTransmission();
  
    Wire.beginTransmission(writeaddr);
	Wire.write(0x00);	//seconds register, lowest register in the rtc
	for(byte f=0;f<16;f++)
	{
	  Wire.write(bin2bcd(i2cSendData[f]));
	}
	Wire.endTransmission();


}

isr()
{	rtcPing++;
}

read_rtc()
{
	uint8_t i=0;
	Wire.beginTransmission(writeaddr);
	Wire.write(0x00);
	Wire.requestFrom(writeaddr,4);
	Wire.read();	//this trashes the first read, which should be register f (and not what we need)
	while(Wire.available())
	{	  rtc_time[i++]= Wire.read();	//be advised, these are BCD values
	}
	Wire.endTransmission();//

	seconds=rtc_time[0];
	minutes=rtc_time[1];
	hours=rtc_time[2];
}

