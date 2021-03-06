/* 
new3.cpp
brian gilder did a thing 22dec2016
this is a new attempt to nixie clock it up, this time with 60% more ions. 
uses the time.h library and internal arduino oscillator
*/


/********** notes ***********
**can implement interrupts to set seconds etc. although if we're using time.h, it doesn't really
**	matter how often we update our output registers since the internal timekeeping is still accurateish without
**	needing our involvement. maybe we don't need interrupts at all. 
**we could use interrupts to update the lamps using whatever is the current register set. so we have consistent timing
**	on lighting them up. but since we are using time.h, we have nothing but free time. timekeeping is basically
**	already finished. we just have to update our registers and then animate the hours. this problem becomes pretty trivial. damn.
**perhaps the hourAnimate can use the interrupt. that way, we don't need to poll for a flag. now i just need to learn interrupts.
**
**
*****************************/

#include <time.h>
#include <TimerOne.h>
#include <stdio.h>	//this isn't a thing in arduino is it?. arduino is meant to be io

//**********programmer defined***************
const digitLength = 6; 	//either 6 or 4 depending on whether there are seconds digits or not
const PowerMuxTop = 10;		//number of pin at the top of the power mux series of output pins. should descend in number from here to use current sign scheme in setup loop

//*******************************************


int isSet = 0;
int hourAnimate = 0;		// need to write the trigger for this. or get rid of it and assign an interrupt for that condition
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
//	long t = 0;			// does this need to exist? idk how the time.h library works
short portBin[9] = {B00000000,B00010000,B00100000,B00110000,B01000000,B01010000,B01100000,B01110000,B10000000,B10010000};	//reorganize depending on how we pin driver	//might not use if we go with switchcase for porthax	//can we use shorts to store B's? are those not considered letters? since it's for encoding schemes?

void setup() {
	delay(500);
	Serial.begin(9600);
	
	/*for(int i = 0, i<digit_length; i++)		
	{										//did we determine whether loops were allowed in setup? if no, just powermuxtop - 1, - 2 etc
		pinMode(PowerMuxTop - i, OUTPUT);	//make + if going in different direction on port
	}
	*/
	DDRD = DDRD | B11111100;	//sets port D as outputs. erase this and uncomment above loop if not port haxing
	
}

void loop() {
	
	/*
	while(!isSet) 	// should we have the while at this level? atm there are two while loops
	{
	setBlink();
	}
	*/
	
time_t t = now();		//is time_t a thing in the time.h library? or does it need a type declaration? // is the t a straggler or is it supposed to be there? if so, instantiate it
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
	if(hourAnimate)		// can eliminate this flag poll (ha get it) if we use an interrupt for new hour condition
		hourRoll();
	else
	{
		for(int j = 0; j <s 6; i++)
		{
//			portHax[j] = 	//don't remember what i was doing here. maybe it's instead of the switchcase?
		}
		
		int q = 0;
		
		for(int i = 0; i < digitLength; i++)	//this immediately sets the output port binary for tube driver
		{
/*		//i'm an idiot. why else would i have portBin if not for this purpose. the indeces even work with the numbers
			switch(disp_send[i]) 
			{		//there is surely a more elegant way to do this		/////SPOILERS THERE IS use portBin you dummy
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
			*/
		
			q = disp_send[i];	//goes through each position of disp_send and gets value for q
			PORTD = portBin[q];	//sets port d to corresponding binary for q value i hope. index already matches values (convenient)
			
			//will i need a delay here before toggling on? i think probably not if i'm port hacking. that's way faster than digital write
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
int dec2bin(int a) {		//this is probably superfluous now that i've made the switchcase
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
	
	//use the old stuff from other versions. they mostly worked alright.
	
	
	
	
	/* more ideas:
	-could have them scroll out one side and then scroll in the other. (like a lame powerpoint transition)
	-could have them roll, but one at a time. like secLow rolls through all 10 numbers, then stops, then secHi goes, etc down the line
	-could have them flash randomish numbers until it lands on the correct one. either in series or randomly
	*/
	
}

// setBlink should probably be eliminated in favor of flashing a string of 0s on and off until the 
//	time is set. that way, there isn't accidentally a 888888 starting point that is being adjusted
void setBlink() {	
	while(!is_set) 		// might be able just to use an interrupt to quit this loop		/////YOU CAN!!!
	{
		disp_string = {0,0,0,0,0,0};	//have not defined disp_string before now. maybe use disp_send
		delay(1000);
		//some_phantom_button_check_or_something();	//use an interrupt if arduino can handle it		/////IT CAN! 
		disp_string = {8,8,8,8,8,8};
		delay(1000);
		//some_phantom_button_check_or_something();
	}
}