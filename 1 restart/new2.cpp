/* 
new2.cpp
brian gilder did a thing 30aug2016
this is a new attempt to nixie clock it up, this time with 60% more ions. 
uses the time.h library and internal arduino oscillator
*/



#include <time.h>
#include <TimerOne.h>
#include <stdio.h>	//this isn't a thing in arduino is it?. arduino is meant to be io

//**********programmer defined***************
const digitLength = 6; 	//either 6 or 4 depending on whether there are seconds digits or not
const PowerMuxTop = 10;		//number of pin at the top of the power mux series of output pins

//*******************************************


int isSet = 0;
int hourAnimate = 0;
int current_h = 0;
int current_m = 0;
int current_s = 0;
int disp_hHi = 0;
int disp_hLo = 0;
int disp_mHi = 0;
int disp_mLo = 0;
int disp_sHi = 0;
int disp_sLo = 0;
char portHax[5] = 0;
long time_t = 0;
char portBin[9] = {B0000,B0001,B0010,B0011,B0100,B0101,B0110,B0111,B1000,B1001};	//reorganize depending on how we pin driver	

void setup() {
	delay(500);
	Serial.begin(9600);
	
	/*for(int i = 0, i<digit_length; i++)		//did we determine whether loops were allowed in setup?
	{
		pinMode(PowerMuxTop - i, OUTPUT);	//make + if going in different direction on port
	}
	*/
	DDRD = DDRD | B11111100;	//sets port D as outputs. erase this and uncomment above loop if not port haxing
	
}

void loop() {
	
	/*
	while(!isSet) 
	{
	setBlink();
	}
	*/
	
time_t t = now();		//is time_t a thing in the time.h library? or does it need a type declaration?
current_h = hour(t);
current_m = minute(t);
current_s = second(t);
disp_hHi = current_h/10;
disp_hLo = current_h%10;
disp_mHi = current_m/10;
disp_mLo = current_m%10;
disp_sHi = current_s/10;
disp_sLo = current_s%10;
disp_send = {disp_hHi,disp_hLo,disp_mHi,disp_mLo,disp_sHi,disp_sLo};

displayUpdate();

}

void displayUpdate(){
	if(hourAnimate)
		hourRoll();
	else
	{
		for(int j = 0; j <s 6; i++)
		{
			portHax[j] = 
			
			
		}
		
		for(int i = 0; i < digitLength; i++)	//this immediately sets the output binary for lamp driver
		{
			switch(disp_send[i]) {		//there is surely a more elegant way to do this
			case 0:
				PORTD = B00000000;		
				break;
			case 1:
				PORTD = B00010000;
				break;
			case 2:
				PORTD = B00100000;
				break;
			case 3:
				PORTD = B00110000;
				break;
			case 4:
				PORTD = B01000000;
				break;
			case 5:
				PORTD = B01010000;
				break;
			case 6:
				PORTD = B01100000;
				break;
			case 7:
				PORTD = B01110000;
				break;
			case 8:
				PORTD = B10000000;
				break;
			case 9:
				PORTD = B10010000;
				break;
			}
			digitalWrite(PowerMuxTop - i, HIGH);
			delay(muxDelay/2);
			digitalWrite(PowerMuxTop - i, LOW);
			delay(muxDelay/2);
		}
	
	}
	
}

// do we need to do what follows with pointers or can we return an array at the end?
//	do we even need to do this or should we have a lookup table? i'll make a table anyway
//	especially since this returns an array. would i need to concatinate it to make it a number again?
int dec2bin(int a) {		
	int c, k, b[4];
	for(c = 4; c>=0; c--)
	{
		k = a >> c;
		if (k & 1)
			b[c] = 1;
		else
			b[c] = 0;
	}
	return b;
}
// is itoa(value,outputstring,base) an actual thing? output string suggests more type conversion but w/e
	
void hourRoll() {		//fancy hour animations
	
	
}

// setBlink should probably be eliminated in favor of flashing a string of 0s on and off until the 
//	time is set. that way, there isn't accidentally a 888888 starting point that is being adjusted
void setBlink() {		
	while(!is_set) 
	{
		disp_string = {0,0,0,0,0,0};
		delay(1000);
		//some_phantom_button_check_or_something();	//use an interrupt if arduino can handle it
		disp_string = {8,8,8,8,8,8};
		delay(1000);
		//some_phantom_button_check_or_something();
	}
}