
/**
the 16th god help me draft of trying to kludge together a nixie clock program (works for both 4 & 6 digits)
b gilder
25jul2021
**/

/******* rev log ************************************************/
/* this is the same basic code as rev 16-2
the last thing fixed was the hour animation section. now has fancy roll, swipe left, swipe right.
currently the "production" version of the legacy code
*/

/*
leftover ideas:
i want to introduce a "drift" section of the menu to dynamically scale the time (even just subtract seconds after a week or whatever) if it drifts over the course of, say, a week
*/





/****************PROGRAMMER DEFINED*****************/
/**/					
/**/	const int clockDigitSize = 6;   			//4 or 6
/**/ //	const long flicker_interval = 20; 			//measured in millisec, duration each digit is on. we will need to experiment with this 	//not currently used but maybe for built in pwm stuff?
/**/	const int brightness_max = 128; 		//50% duty cycle. would it be advisable to allow going over that?		//it seemed like making it higher made the 7segs beat but idk if tubes would do that.
/**/	const int brightness_min = 8; 			//this is totally a made up number. we will have to experiment with this as well.
/**/	int brightness = 128; 				//0-255, controls built-in pwm scheme. could be what’s changed by user brightness selection. initially half bright
/**/	const int automatedPWM = 0;			//enable if letting arduino flicker lamps for us (what if you enable both? it might still be legal..  interesting)
/**/	const int bruteForce = 1;				//enable if using custom time delays to flicker lamps
/**/	const int includeBrightness = 0;	//for the setup menu, if 1 will include brightness setting, if 0 will force user to use default brightness
/**/	float onTime = 3.5;				//in ms, for brute force. will need to experiment 
/**/	float offTime = .08;				//ditto	//protip can't be constants if we set them again below lulz
/**/	bool hourType = 0;				// 0 for 12-hr, 1 for 24-hr	//toggled by a physical throw switch? therefore variable	//// or in menu. still variable

/**/	const int digitSetBlinkMult = 50;	//the number of times to run mux for each idle 0s/8s displayed before initial time setting //multiply by 5 = roughly 1sec
/**/ //	const int buttonQuarterDebounce = 50;	//in ms, for debounce	//don't remember what this is for. don't think we use it anymore and just use straight debounce
/**/	const int menuButtDelay = 1500;			//in ms, time to hold the menu button before entering settings menu	
/**/	const float debounce = 10;			//ms. don't know how long to wait
/**/	const int changeHourSwitch = 0;		//1 if using toggle switch, 0 if using button menu	
/**/	const int muxMenuDayOption = 1;			//how to display am/pm choice in menu. 0 is 9 in hr0 and 6 in min1. 1 is 0s in front for am and middle for pm
/**/	const int legacyCounter = 1;		//if 1, will do the old school subtraction math. if not, will divide the clock from millis()
/**/	const int setWaitType = 2;			//0 for alternating 8s and 0s, 1 for flashing 0s, 2 for 0-9
/**/ //	const int setWaitInterval = 1000;	//ms, for setWaitType 2. how often to switch numerals on all digits	///might not use with new structure
/**/	const int animationsTotal = 3;		//number of hour change animations we've baked in
/**/					
/*************************************************************/



/** constants **/
unsigned long sec_interval = 984;	//1 second	///ooooooo! maybe this is a subtle way to adjust for drift!!! ///EYYY!!!!!! /////but if so it can't be a const

const int digitPin1 = 3;   	// MSB, hr1 	//this is what switches on/off the digit muxes
const int digitPin2 = 5;		//conveniently, there are exactly 6 digital pwm pins
const int digitPin3 = 6;
const int digitPin4 = 9; 		//4-digit LSB, min0
const int digitPin5 = 10;
const int digitPin6 = 11;	 	// 6-digit LSB, sec0
const int digit[6] = {digitPin1,digitPin2,digitPin3,digitPin4,digitPin5,digitPin6};	//all 6 included, potentially not utilized. can cannibalize for pins in 4-digit if leftovers are needed
const int secDotPin = 13; 	// pin controlling mux for sec-blink dot 
const int switchPin = A5;	 //!!!! !!!! !!!! or whatever it actually should be // don’t currently have switch
const int pmDotPin = A4;

const int buttLesserPin = A0;	//if these pins don't exist we can use analog pins as inputs and initialize them as high with digitalWrite to set pullups
const int buttGreaterPin = A1;
const int buttMenuPin = A2;		//should these be on pullup pins so we can make them active low and not have to reset them high? yes.

const int driverPin1 = 2; 	//these might be changed so that the driver and buffer and stuff can just go straight across	//have they been already?	//not yet
const int driverPin2 = 4;
const int driverPin3 = 7;	
const int driverPin4 = 8;		//LSB
const int driverArray[4] = {driverPin1,driverPin2,driverPin3,driverPin4}; 
/*hour holders are variable to switch 12/24 mode,
see below, in variables */
const int min1_max = 5;
const int min0_max = 9;
const int sec1_max = 5;
const int sec0_max = 9;
const int output_bin[10][4] = 
	{		//2-d array OKAY #MICHAELRIPLEY jeesh
	{0,0,0,0},  	//0	
	{0,0,0,1},
	{0,0,1,0},
	{0,0,1,1},
	{0,1,0,0},
	{0,1,0,1},
	{0,1,1,0},
	{0,1,1,1},
	{1,0,0,0},
	{1,0,0,1}		//9
	};					///might change this to be convenient for driver pins

	
/** variables **/
int hr1_max = 2;  	// change to 1 if 12-hr mode	///or it changes automatically now? idk whatevs
int hr0_max = 3;  	// I think this can be 2 or 3 without it messing up normal clock progression.. I think I took care of that down below (I’m basically a genius)	//yeah except it hasn't been working, genius
int hr0_min = 0;  	// will automatically change to 1 if 12-hr mode
int hr1 = 1; 		//hour holders, 1 is MSB,   /**/ initial setting is 12:34:56 /**/
int hr0 = 2; 		
int min1 = 0; 		//minute holders
int min0 = 0; 
int sec1 = 0; 		//sec holders
int sec0 = 0; 
int timecard[6] = {hr1,hr0,min1,min0,sec1,sec0};		//pointers would make this easier. sigh. but then pointers. yuckers.
int hr1_old = hr1;	//for hour roll i think?
int hr0_old = hr0;
//int sec1div;			//for clock divider to work with ripple, might not be used anymore.
int sec0div;			//for clock divider to work with ripple
int secDotState = LOW;		// used to set the sec-blink dot 
int digitState = LOW; 		//used to set the currently displaying digit
int buttMenu = LOW;
int buttLesser = LOW;
int buttGreater = LOW;
int mux_enable[6] = {1,1,1,1,1,1};	//tells the mux which lamps to shine
int mux_enable_default[6] = {0,0,0,0,0,0};
int amPm = 0;			//0 is am, 1 is pm	 //allows changing 12/24 on the fly (because I’m a badass)
unsigned long previousSec = 0; 
unsigned long currentSec = 0;
// unsigned long previousTime = 0;  //for old mux? maybe? can we get rid of this?	//i think maybe? i'm commenting it out for now
int hourTypePrev = hourType;		// for changing 12/24 on the fly
int gatekeeper = 0;		//I figure we can keep track of whether we’ve done the initial setup phase or whatever. becomes 1 when finished initializing
int timeChange = 0;		//to prevent recursion in changing the time once the clock as already been set at least once

/***************************************************************************************/



void setup()			//let's get rrready to rrrrrumblllllle
{
	pinMode(switchPin, INPUT);	//don’t currently have switch
	pinMode(secDotPin, OUTPUT);
	pinMode(driverPin1, OUTPUT);
	pinMode(driverPin2, OUTPUT);
	pinMode(driverPin3, OUTPUT);
	pinMode(driverPin4, OUTPUT);
	for(int p = 0; p < clockDigitSize; p++)		///did loops in setup end up working? i hope so. 
	{	pinMode(digit[p], OUTPUT);
	}												
	pinMode(buttMenuPin, INPUT);
	pinMode(buttLesserPin, INPUT);
	pinMode(buttGreaterPin, INPUT);
	digitalWrite(buttMenuPin, INPUT_PULLUP);			//can we digitalWrite in the setup routine?		//writing high to an analog input should cause the analog pins to pull up	///right?? i sure hope so. that's sort of Really Important. if not, we can use analog to write to the driver or smthg
	digitalWrite(buttLesserPin, INPUT_PULLUP);
	digitalWrite(buttGreaterPin, INPUT_PULLUP);
	
  	randomSeed(analogRead(A3));		/////change this to an analog pin that isn't being used. resets the pRNG even though that isn't really important
	Serial.begin(9600);
	if(hourType == 0)
	{
		hr1_max = 1;  	// change to 1 if 12-hr mode
		hr0_max = 2;  	// I think this can be 2 or 4 without it messing up normal clock progression.. I think I took care of that down below (I’m basically a genius)
		hr0_min = 1; 
		//Serial.println("setup shenanigans 1");	//for debug
	}

	if(hr1 > hr1_max || hr0 > hr0_max || hr0 < hr0_min)
	{
		if(hr0<hr0_min)
		{	amPm = 0;
		}
		hr1=0;
		hr0=1;
		//Serial.println("setup shenanigans 2");	//for debug
	}
  
	if(hourType == 1 && hr1 > 0 && hr0 > 1 )           
	{
		amPm = 1;
		//Serial.println("forcing pm");
		changeHourType();
		//Serial.println("setup shenanigans 3");	//for debug
	}
	bool hourOverflow = hr1>=2 || (hr1>=2 && hr0 >=4);	//does this need to be 'boolean' instead of 'bool'?
	if(hourOverflow)
	{ 
			//Serial.println("input hour overflow");	//for debug
			hr1 = 0;
			hr0 = 1;
			amPm = 0;
    }
	for(int g = 0; g < clockDigitSize; g++)		//can arduino handle this in the setup routine?
	{	mux_enable_default[g] = 1;
	}
	
}//end setup fxn


void loop()   /*what if we made this a dependent function and had the mux be main? <-this might be a good idea <-maybe not. need mux during initialization*/
{

if(!gatekeeper)
  {
	//initialization routine here	//if its own function, bring the brightness cap thing along
	settings();
	gatekeeper = 1;	//could might delete if waitForSet() is a thing	///it is but i'll leave it just in cases
  }

  if(legacyCounter)
  {  clockCounter();
  }
  else
  {	clockDivide();
  }

  mux(timecard,mux_enable_default); 		//?? does this go here??? how do??? //no, if mux is main function //but it probably won’t be, so maybe it goes here?	/// seems to work
 
 /* if(!digitalRead(buttMenuPin))
  {	checkButtons();
  }*/	//this causes the lamps to go dark after the first mid-steam menu access. wonder if there's some weird loop nesting

}//end main loop fxn


void settings()
{

//fun math for pwm and on/off conversion. //will need to determine the max digit cycle length (in milliseconds i guess) -> “maxTime”
 if(bruteForce) 		//if the modes aren’t exclusive, these will override programmer defined values (may be incorrect math here)
 {	brightness = 255*onTime/(onTime+offTime); //unless we determine a max flash time, then the denom would be maxTime or w/e
 }
 else if(automatedPWM)	//will elseif prevent double dipping if both modes are enabled?
  {
	onTime = ((onTime+offTime)*brightness)/255;	//obvs replace with maxTime if we end up using it
	//offTime = maxTime-onTime;	//because i'm lazy	//don’t currently have a maxTime var. inb4 it fails cuz it says we don’t have a maxTime var. hi future ppl	////YEAH IT DID YOU SUCK PAST ME
  }//endelseif

if (brightness > brightness_max)	//to keep max at max (we might decide to go above 50% duty) 
  {	brightness = brightness_max;
  }

bool earlyMorning = !hr1 || (hr1 == 1 && (hr0==0 || hr0==1));	//if hr1=0 or 10,11	///does this need to be 'boolean' instead of 'bool'?
//Serial.println("early morning run");	//for debug
if(hourTypePrev && earlyMorning) //if 24-hr and that thing
  {
	//Serial.println("early morning accepted");
	//Serial.println("forcing am");  		//for debug
	amPm = 0;
  }

   waitForSet();
  
return ;	//does this need to be return(void); or some shit? return c; down below seems to compile
}//end settings fxn


void clockCounter()
{
				 	//metronome on the seconds
 currentSec=millis();
if((currentSec - previousSec >= sec_interval) || (currentSec - previousSec < 0)) 	//this would be so much easier if we used the 555. probs less accurate tho	////LOL ACCURACY HERE WHAT	///this would be an easy adjustment opportunity in sec_interval
  {//count another sec!
	previousSec = currentSec;   	// save the last time you blinked the sec-dot LED
	if (secDotState == LOW)		// if LED is off turn it on and vice-versa
	{	secDotState = 1;
	}
	else
	{	secDotState = LOW;
	}
	clockRipple();	

//	hourType = analogRead(switchPin);	//or whatever the switch pin poll function is. don’t currently have switch
	
	if(changeHourSwitch)	//idk i'm just guessing here. previously there wasn't a conditional, just the next line in brackets and idk why.
  	  {	changeHourType();	//if shit just went down	//if there is an arduino switch flip checker, use that instead of this Doesn’t Match nonsense
	  }

  }//endif currentSec

}//end routine clockCounter


void clockDivide()	//divide the dang clock. millis()/1000 or some shit idk you're the genius you figure it out
{
	unsigned long	timeNow = millis();
	int sec0New = ((timeNow/1000)%10);	//is that seriously it?		//mod brings it down to one digit		//is this where multiplying by an adjustment factor would happen? hhhhhh idk. it couldn't be an int in that case
	//sec1New = (timeNow/10000)%10);
	if(sec0 != sec0New)
	{	
		if (secDotState == LOW)		// if LED is off turn it on and vice-versa
		{	secDotState = HIGH;
		}
		else
		{	secDotState = LOW;
		}
		clockRipple();					// i don't think this will work anymore. 	////ooo it might now!	///except now it is still the same type of time check ripple adder instead of keeping track behind the scenes
	}
	
	
	
}//end routine clockDivide


void newClockDivide()
{	//using actual clock division	
	
//will this break if there is overflow? what if the clock hasn't been set for a while? like...
//just doing an offset won't eliminate the several minutes etc it's been running. current millis + offset or whatever will not work if millis has been counting for a long time. 
//we could just have an initial time we subtract from the total but then what happens if the counter overflows?
	
/*	time_t nowTime = millis();
	
	sec0 = ((nowTime/1000)%10);
	sec1 = ((nowTime/10000)%10);
	
*/	
	
	
}//end routine newClockDivide

void clockRipple()
{
//reinventing the wheel like a boss
//do the time counting

if(sec0 < sec0_max)	
  {	sec0++;
  }
else if(sec1 < sec1_max)	//double digits
  {
	sec0 = 0;
  	sec1++;
  }
else				//both are full
  {//min rollover
  	sec0=0;
  	sec1=0;
  	if(min0 < min0_max)	
  	{	min0++;
  	}
  	else if(min1 < min1_max)	//double digits
  	{
		min0 = 0;
		min1++;
  	}
  	else			//both are full
  	{//hour rollover
		min0=0;
		min1=0;
		hr1_old = hr1;
    		hr0_old = hr0;
		if(hr1 < hr1_max)	//this might account for 14 -> 15, etc
		{
			if(hr0 < 9)
			{	hr0++;
			}
			else		//double digits
			{
				hr0 = 0;
				hr1++;
			}
		}
		else			//if hr1 is 2 (in 24-hr) or 1 (in 12-hr)
		{
			if(hr0 < hr0_max)	//still has room to grow
			{	hr0++;
			}
			else		//if at 2 for 12-hr or 4 for 24-hr	// does this account for 14 -> 15 etc??
			{
				hr0 = hr0_min;	//sets to 1 for 12-hr, 0 for 24-hr
				hr1 = 0;
			}
		}
		//timecard[0] = hr1;	//this might break animations that rely on old information	//i'm taking these out so that 
		//timecard[1] = hr0;
		bool amPmFlop = (timecard[0] == 1 && timecard[1] == 2) || ( timecard[0] == 0 && timecard[1] == 0);
		if (amPmFlop)
		{
			if(amPm == 0)
			{	amPm = 1;  //now pm
			}
			else
			{	amPm = 0;  //now am
			}
		}
		hour_animate();	// we don’t have this built yet	//now we half do..	//these comments originally refer to fancy_hour_roll
	}//endelse hour rollover
  }//endelse min rollover
	digitalWrite(secDotPin, secDotState);	//blink sec dot
	timecard[0]={hr1};	//implementing pointers would erase my need to do this. super duper handy. ugh except then pointers	////plus then i couldn't hijack the timecard to do my bidding muahaha
	timecard[1]={hr0};
	timecard[2]={min1};
	timecard[3]={min0};
	timecard[4]={sec1};
	timecard[5]={sec0};

	if(!timeChange)
	{
		buttMenu = digitalRead(buttMenuPin);
		if(buttMenu == LOW)
		{	
			timeChange = 1;
			checkButtons();
			timeChange = 0;
		}
	}
}//end clock count fxn


void changeHourType()
{
//changing 12/24 on the fly	
////now also compatable with the button menu
//Serial.print("I'm in changeHourType\n");
	if(changeHourSwitch)
	{	hourType = digitalRead(switchPin);
	}
	if(hourType == 0)	//if 12-hr, at this very second				////why is this not after the prev check?
	{
		hr1_max = 1;   
		hr0_max = 2;    
		hr0_min = 1; 
		//Serial.println("hourType ==0, 12hr\n");
		/*if(hr1 > hr1_max || hr0 > hr0_max)	//if stuff is too big 	//just trust in the magic, bro
	{
			hr1=1;				//default to a troubleshoot time combo
			hr0=2;
			min1=1;
			min0=2;
			Serial.println("hr1 OR hr0 too big");
		}*/
	}
	else 		//else if hourType ==1, aka 24-hr, at this very second
	{
		hr1_max = 2;    
		hr0_max = 3;    
		hr0_min = 0;
		//Serial.println("else hourType ==1\n");
	
		if(!amPm && hr1 == 1 && hr0 == 2)	// if am, and currently 12:xx
		{
			hr1=0;				// then let's correct that biznazz
			hr0=0;
			//Serial.println("forcing 00:");
		}
	}


	if(hourTypePrev != hourType)	// if the switch has been flipped ///or if changed in menu
	{
		//Serial.println("switch has been flipped");
		if(hr0 == 0 && hr1 == 0 && hourTypePrev)		//if 00:xx and formerly 24-hr (therefore currently 12-hr)
		{
			hr1 = 1;
			hr0 = 2;
			amPm = 0;	//deffo am, in that case
		}
		
		else if(hr1 == 1 && hr0 == 2 && !hourTypePrev && !amPm)
		{
			//	Serial.println("just recently 12: am, midnight");
			hr1=0;
			hr0=0;
			}
		else
		{
			//Serial.println("the switch doesn't matter");
		}
	
	
	
		if(amPm == 1)		//if pm
		{//purple
			if(hourTypePrev == 0)	//if just recently 12-hr mode and pm 
			{
				hr1_max = 2;
				hr0_max = 3;	//24-hr mode biotch	// IS THIS VERY WRONG? what about 14 -> 15 ??? did I already account for this in the counter?	//looks like it. damn I’m good. THIS IS NOT ACTUALLY WRONG probably 
				bool noontime = hr1 == 1 && hr0 == 2 && amPm;
					if(!noontime)//this should just leave noon where it is right? bc it’ll just skip it 	//brian, don’t take out hr0
				{	//if not 12:xx
					int error_check = conversion_error();	//to make sure a value is properly accounted for in the if statement below
					if(error_check)	//conv_error apparently returns values. good news.	
					{
						hr0 -= 8;	//does arduino understand these operators?	///yes
						hr1 += 2;
					}
					else
					{
						hr1++;
						hr0 += 2;	
					}
				}
			}
			//will the normal hour counter make midnight go to 00:00 automatically already? //yup. looks like it. cuz I’m a genius
		
			if(hourTypePrev)	// if just recently 24-hr mode and pm	
			{
				hr1_max = 1;
				hr0_max = 2;	//12-hr mode what whaaat	// IS THIS VERY WRONG? what about 02 -> 03 ??? did I already account for this in the counter	//looks like it. damn I’m good. THIS IS NOT ACTUALLY WRONG probably
				bool noontime = hr1 == 1 && hr0 == 2 && amPm;
				if(!noontime)//this should just leave noon where it is right? bc it’ll just skip it	//brian, don’t take out hr0 		//this is causing problems for anything where hr1 is 1. crap.
				{
					int error_check = conversion_error();	//to make sure a value is properly accounted for in the if statement below
					if(error_check)	
					{
						hr0 += 8;
						hr1 -= 2;
					}
					else
					{
						hr1--;
						hr0 -= 2;
					}
				}
			}
		}//endpurple
		timecard[0] ={hr1};	//pointers would eliminate the need for this, but then pointers. yuckers.
		timecard[1]={hr0};
		timecard[2]={min1};
		timecard[3]={min0};
		timecard[4]={sec1};
		timecard[5]={sec0};
		hourTypePrev = hourType;	//may be unnecessary if arduino has built in switch flip checker
	}
  //i'm about to do something stupid	//idk what the stupid thing was supposed to be. maybe adding the return?
return ;
}//end change hour type fxn


int conversion_error()	//helper routine for changeHourType
{
	int c = 0;
	bool eightOclock12 = !hr1 && hr0==8 && !hourTypePrev;
	bool nineOclock12 = !hr1 && hr0==9 && !hourTypePrev; 
	bool eightOclock24 = hr1==2 && !hr0 && hourTypePrev;
	bool nineOclock24 = hr1==2 && hr0==1 && hourTypePrev;
	
	if(eightOclock12 || nineOclock12 || eightOclock24 || nineOclock24)
	{	c=1;
	}
	
	return c;
}//end conv error fxn


void mux(int array[], int enable[])		//this is a mux function
{
//the way i see it we could do analogWrite or we could do on/off times
//this is its own function so that fancy roll can also use it

unsigned long entranceTime = millis(); //don't currently have use for this but it might be handy eventually at some point maybe idk

	for(int brown = 0; brown < clockDigitSize; brown++)
	{	digitalWrite(digit[brown],LOW); 		//forces all digits to be off before we turn (potentially) only some of them on wnkbr
	}
	for(int w = 0; w < clockDigitSize; w++) 
	{			
		if(enable[w])	//is it really this simple?	//will it just skip the digit entirely and leave off if that position has a 0 enable?
		{
			int x = array[w]%10; 
			for(int i = 0; i < 4; i++) 	// THIS SETS THE DRIVER OUTPUTS		//there will always be 4 driver pins
			{	digitalWrite(driverArray[i],output_bin[x][i]);
			}
		
			if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
			{	analogWrite(digit[w], brightness); //is that how you pwm? 
			}
		//maybe we could give it the normal duty cycle and then an Extra wait time after that to set the driver again
		//physical brightness will be supes reduced bc pwm (suggests >50% duty?). is inrush current a thing? we should scope it. if it isn't maybe we could current push these tubes. if it is, we might explode them
		//because overdriving these may (or may not) be a good idea. o/d with current or voltage? //i'd say current bC voltage just made sure they turned on. but maybe turning on fully is a ~relative~ brightness thing too
		
			if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
			{
				delay(offTime/2);//letting the driver settle. completing the second half of off time
				digitState = HIGH;
				if(w == 0 && hourType == 0 && !array[0] && gatekeeper)	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
				{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
				}
				digitalWrite(digit[w],digitState);  
				delay(onTime);
				
				digitState = LOW;
				digitalWrite(digit[w],digitState);	
				delay(offTime/2);// here is the first half of off time
				}//endif
		}//endif enable
	}//endfor
} //end mux fxn


void fancy_hour_roll()		//this is one animation among (hopefully) several	//MUST be less than 9 seconds total
{

	/***** programmer defined *****************/
	/**/					
	/**/ 	const float numeral_change_time = 18; 		//in ms, for fancy roll	//float for decimal math later
	/**/	const int roll_reset_time = 3;			//in seconds, totalish amount of time fancy roll animation takes	///only used if legacyCounter
	/**/			
	/******************************************/
		// with 36ms, this module takes 4.827 seconds to complete.
		// with 19ms, this module takes 4.172 seconds to complete.
		// with 18ms, this module takes 3.042 seconds to complete somehow????? but it is also way fast so the numbers look random.
		//dealer's choice. whatever you think looks best
		
	timecard[0]={hr1};	
	timecard[1]={hr0};
	timecard[2]={min1};
	timecard[3]={min0};
	timecard[4]={sec1};
	timecard[5]={sec0};
	
	unsigned long currentSec = millis();
	
	/*check to see if the count routine increments the hour before calling this fxn or if this should do it at the end. if it does it in the counter, we will need to assign a "-1" thing UGH but then that brings in so many exceptions ughhh lame lame lame. 
	maybe we can have the counter store an hr_old or something so that we don't have to display the new hr numeral until after the fancy roll ooo good idea.*/		//done. don't worry your precious little head.
	/*dude we can just put the function above the actual math in the counter. dude.
	or we could incorporate hr1_old and then*/
	for(int blue=0; blue<6; blue++)
	{	mux_enable[blue] = 0;
	}
	for(int a=0; a<clockDigitSize; a++)
	{	mux_enable[a] = 1;
	}
	int hr1_roll = hr1_old;
	int hr0_roll = hr0_old;
	min1=5; //should i do this? am i allowed? does it matter? //this can be used to check proper hour rollover, since it sets everything to 5959 and keep rolling over etc whatever
	min0=9;
	sec1=5;
	sec0=9;
	int roll_buffer[] = {hr1_roll, hr0_roll, min1, min0, sec1, sec0}; //maybe for iterating hmmmm?
	int magicNumber[] = {10+hr0_roll,15,19,15,19,19};// change the 19's to 9 if you want to pick up the next digit without going a full rotation
	
	
	//hey dude include a single full roll for the hr1 digit so it won't get caught on hr0 the first time	//that sounds like magic number stuff. i'll worry about that later. you're welcome, future me!	////you're a jerk
	
	/** ERASE LOOPER **/
	for(int y=0; y < clockDigitSize; y++)	
	{
		while(roll_buffer[y] < magicNumber[y])	//whatever. sort out magic number later	
		{
			mux(roll_buffer, mux_enable_default);
			currentSec = millis();
			if((currentSec - previousSec >= numeral_change_time) || (currentSec - previousSec < 0))
			{
				previousSec = currentSec;
				for(int t=0; t<=y; t++)	//this sets as many positions in roll_buffer as that particular step should allow (wrt which digit you've gotten to)
				{
					/*int n = (roll_buffer[t]%10);
					for(int h=0;h<4;h++)
						{
							digitalWrite(driverArray[h],output_bin[n][h]);
						//THERE IS CURRENTLY NOTHING TURNING THE LAMPS ON TO DISPLAY THESE DRIVER CHANGES IT IS ALL HAPPENING IN THE MUXX
							//so does that mean i can comment out everything within t-loop except the increment?	//YUP totally works.
						}	*/
						roll_buffer[t]++;	
				}
			}
		}
	}
	/*** end ERASE LOOPER ***/
	
	/*** now the SET LOOPER ***/	
	magicNumber[0] = {hr1_roll+10};
	magicNumber[1] = {hr0_roll+20};
	magicNumber[2] = {30};
	magicNumber[3] = {40};
	magicNumber[4] = {50};
	magicNumber[5] = {60-roll_reset_time}; 	// will this allow the digits to stop rolling hr1 and hr0 once they hit their mark? there is a mod 10 in some stuff down there. i'm counting to 60 etc without resetting since i can just mod it back to a single digit. idk if that will work but i'm gonna try it.
		
	//here's what i imagine should be happening:
	//go through each digit index. if the roll buffer is greater than the magic number, display the roll buffer. if not, display the magic number%10. at the end, subtract 1 from all magic number indexes.
		for(int goo=60-roll_reset_time;goo>0;goo--)
	{
		for(int bleep=0;bleep<clockDigitSize;bleep++)
			{			
				if(timecard[bleep]>magicNumber[bleep])
				{	roll_buffer[bleep]=timecard[bleep];
				}
				else
				{roll_buffer[bleep]=magicNumber[bleep]%10;	//note this causes the numbers to go 9->0 but at speed it probably won't be noticeable
				}
				magicNumber[bleep]--;
			}
			currentSec = millis();
			previousSec = currentSec;
			while((currentSec - previousSec <= numeral_change_time) )//|| (currentSec - previousSec > 0))
			{
				mux(roll_buffer,mux_enable_default);
				currentSec = millis();
			}
	}
	/*** end SET LOOPER ***/
	
	timecard[0]={hr1}; 
	timecard[1]={hr0};
	min1={0};
	min0={0};
	sec1={0};
	if(legacyCounter)
	{	sec0={roll_reset_time};
	}
	else
	{	clockDivide();
	}
	
}//end hour_roll fxn

void hour_animate()
{
	int choose = random()%((2*animationsTotal)-1);	//animationsTotal should be the number of unique programs, not the number of cases here
	
	switch(choose)		//expand this to always have a fancy_hour_roll after any new animations. because we really want it to clean off those filaments
	{
		case 0:
		{
			fancy_hour_roll();
		}
		break;
		
		case 1:
		{	
			swipe_left();	
		}
		break;
		
		case 2:
		{
			fancy_hour_roll();
		}
		break;
		
		case 3:
		{
			swipe_right();
		}
		break;
		
		case 4:
		{
			fancy_hour_roll();
		}
	}	
	//add more cases as needed, but remember to update animationsTotal
	
}//end routine hour_animate

void swipe_left()		//flies old time out the left of the clock and new time in from the right
{
	
	/**** programmer defined ******/
	/**/		//
	/**/	const float	shift_time = 250;	//ms, how fast numerals change to different digits as they fly out and back in the sides
	/**/	const int reset_time = 2;			//in seconds, at the moment must be even number (does it still? for this one?), total amount of time animation takes to set sec0	///only used if legacyCounter	
	/**/		//
	/******************************/
	
	int canDo = 0;
	int roll_buffer[6] = {hr1_old,hr0_old,5,9,5,9};
	
	for(int j=0; j<clockDigitSize; j++)		//start with all digits on
	{
		mux_enable[j]=mux_enable_default[j];
	}
	
	for(int thing = 0; thing<clockDigitSize; thing++)	//this makes the old numbers fly out to the left
	{			
		canDo=0;
		while(!canDo)	
		{
			mux(roll_buffer, mux_enable);
			currentSec = millis();
			if((currentSec - previousSec >= shift_time) || (currentSec - previousSec < 0))
			{
				previousSec = currentSec;
				for(int k=1; k<clockDigitSize;k++)
				{
					roll_buffer[k-1] = roll_buffer[k];
				}
			mux_enable[clockDigitSize-1 - thing] = 0;	//should blank them out from right to left one at a time, right?	
			canDo=1;
			}
			
		}
	
	}
	
	if(legacyCounter)
	{
		clockCounter();
	}
	else
	{
		clockDivide();
	}
	
	timecard[0]=hr1;
	timecard[1]=hr0;
	timecard[2]=min1;
	timecard[3]=min0;
	if(clockDigitSize > 4)
	{
		timecard[4] = sec1;
		timecard[5] = sec0;
	}
	
	for(int go=clockDigitSize; go>0; go--)	//this makes the new numbers fly in from the right	//should this be go > 1 so it ends a frame early and has the regular mux do the normal ending frame?
	{	//protip if you cut the animation right before the final frame it will allow the regular function to put everything where it should be to show the correct new time	///yeah but that might be causing problems
		canDo=0;
		while(!canDo)	
		{
			mux(roll_buffer, mux_enable);
			currentSec = millis();
			if((currentSec - previousSec >= shift_time) || (currentSec - previousSec < 0))
			{
				previousSec = currentSec;
				for(int m=clockDigitSize-1; m>0; m--)	
				{
					roll_buffer[m]=roll_buffer[m-1];
				}
				roll_buffer[clockDigitSize-1]=timecard[clockDigitSize-go];		//will this do it? 	//i think so
				mux_enable[go-1] = 1;	//should light them up from right to left one at a time, right?	because the previous part should have made them all zeros, right?
				canDo=1;	//is this premature? is it going to jump us out of the loop too quickly?
			}
		}
	}

	/*
	for(int re=0; re<clockDigitSize; re++)
	{	mux_enable[re] = 1;
	}
	*/
	
	if(legacyCounter)
	{	sec0={reset_time};
	}
	else
	{
		clockDivide();
	}
	
}//end routine swipe_left

void swipe_right()
{
	
	/**** programmer defined ******/
	/**/		//
	/**/	const float	shift_time = 150;	//ms, how fast numerals change to different digits as they fly out and back in the sides
	/**/		//
	/******************************/
	
	int canDo = 0;
	int roll_buffer[6] = {hr1_old,hr0_old,5,9,5,9};
	
	for(int j=0; j<clockDigitSize; j++)		//start with all digits on
	{
		mux_enable[j]=mux_enable_default[j];
	}
	
	for(int thing = 0; thing<clockDigitSize; thing++)	//this makes the old numbers fly out to the right
	{			
		canDo=0;
		while(!canDo)
		{
			mux(roll_buffer, mux_enable);
			currentSec = millis();
			if((currentSec - previousSec >= shift_time) || (currentSec - previousSec < 0))
			{
				previousSec = currentSec;
				for(int k=clockDigitSize; k>1;k--) 		//k>1 so it doesn't go out of array bounds
				{
					roll_buffer[k-1] = roll_buffer[k-2];
				}
			mux_enable[thing] = 0;	//should blank them out from right to left one at a time, right?	
			canDo=1;
			}
			
		}
	
	}
	
	timecard[0]=hr1;
	timecard[1]=hr0;
	timecard[2]=min1;
	timecard[3]=min0;
	if(clockDigitSize > 4)
	{
		timecard[4] = sec1;
		timecard[5] = sec0;
	}
	
	
	for(int go=0; go<clockDigitSize-1; go++)	//this makes the new numbers fly in from the left	//should this be go > cDS-1 so it ends a frame early and has the regular mux do the normal ending frame? //actually i think yeah
	{	//protip if you cut the animation right before the final frame it will allow the regular function to put everything where it should be to show the correct new time		///yeah but that might be causing problems. can't do much about it, since roll_buffer[clockDigitSize}] doesn't exist
		canDo=0;
		while(!canDo)	
		{
			mux(roll_buffer, mux_enable);
			currentSec = millis();
			if((currentSec - previousSec >= shift_time) || (currentSec - previousSec < 0))
			{
				previousSec = currentSec;
				
				for(int m=1; m<clockDigitSize; m++)	
				{
					roll_buffer[m]=roll_buffer[m-1];
				}
				roll_buffer[0] = timecard[clockDigitSize - go];	//do i need to include clockDivide somewhere? so that the time that flies in is the appropriate time?
				mux_enable[go] = 1;	//should light them up from right to left one at a time, right?	because the previous part should have made them all zeros, right?
				canDo=1;	//is this premature? is it going to jump us out of the loop too quickly?
			}
		}
	}
	

	 int chillout = 1;
    while(chillout)		///i know i did this thing but is it necessary?
    { 
      mux(roll_buffer,mux_enable);
      currentSec = millis();
      if((currentSec - previousSec >= shift_time) || (currentSec - previousSec < 0))    //so the last frame that is provided by normal clocking doesn't jump immediately and supercede the penultimate frame
      {
        if(legacyCounter)
        { clockCounter();
        }
        else
        {
          clockDivide();
        }
        chillout = 0;
      }
    }
   for(int re=0; re<clockDigitSize; re++)
  { mux_enable[re] = 1;
  }
  
	
}//end routine swipe right


void waitForSet()
{	//another option for doing this is only sending 0s to mux for certain duration and then not muxing for duration to blink numbers
	
	const int setDisplay[10][6] = 
		{
		{0,0,0,0,0,0},
		{1,1,1,1,1,1},
		{2,2,2,2,2,2},
		{3,3,3,3,3,3},
		{4,4,4,4,4,4},
		{5,5,5,5,5,5},
		{6,6,6,6,6,6},
		{7,7,7,7,7,7},
		{8,8,8,8,8,8},
		{9,9,9,9,9,9} 
		};
	int setDisplaySend[6] = {0,0,0,0,0,0};
	int currentIdleMuxCount = 0;
	int displayNum = 0;
	int rightNow = millis();
	int prevNow = rightNow;
	int rollCount = 0;

	
		for(int j=0; j<clockDigitSize; j++)		//start with all digits on
	{
		mux_enable[j]=mux_enable_default[j];
	}
	
	while(!gatekeeper)
	{
		//mux(setDisplaySend, mux_enable);
		buttMenu = digitalRead(buttMenuPin);
		if(buttMenu == LOW)
		{	checkButtons();
		}
		else
		{
			for(int b = 0; b<6; b++)
			{	setDisplaySend[b] = setDisplay[displayNum][b];
			}
			while((currentIdleMuxCount < digitSetBlinkMult) && !gatekeeper)
			{
				mux(setDisplaySend,mux_enable);
			//	delay(5);			///can't remember why this was here but it made stuff flicker so i took it out
				currentIdleMuxCount++;
				buttMenu = digitalRead(buttMenuPin);
				if(buttMenu == LOW)
				{	checkButtons();
				}
			}//endwhile
			if(!setWaitType)
			{
				if(displayNum < 1)
				{	displayNum = 8;
				}
				else
				{	displayNum = 0;
				}
			}//endif !setWaitType
			else if(setWaitType == 1)		//blinks 0s only.		//this might cause spotting on the other filaments. maybe do the 0-9 roll only
			{	
					displayNum == 0;			
					if(mux_enable[0]==1)
					{
						for(int mauve =0; mauve<clockDigitSize;mauve++)
						{  mux_enable[mauve] = 0;
						}
					}
					else
					{
						for(int caramel =0; caramel<clockDigitSize;caramel++)
						{  mux_enable[caramel] = 1;
						}
				}
				/*buttMenu = digitalRead(buttMenuPin);		//pretty sure this is unnecessary since it checks at the top of every loop
				if(buttMenu == LOW)
				{ checkButtons();
				}*/
			}
			else //if(setWaitType == 2)		//this doesn't fit with the structure of the previous parts of the conditional. oh well. 	///now it does. it should work.
			{//this is where a roll for all-digits-simultaneously are 0-9 continuously. only change every half second or smthn
				if(mux_enable[0]==0)
				{
					for(int jack =0; jack<clockDigitSize;jack++)
					{  mux_enable[jack] = 1;
					}
				}
				if(rollCount>9)
				{	rollCount = 0;
				}
				displayNum = rollCount%10; 	//mod just in cases
				rollCount++;
				
				//commenting out the stuff below because it hasn't worked with this structure. using macro-local rollCount to keep track of which digit to display		//in theory, this should also go directly to the clock on exiting the menu instead of finishing the count
							
				/*int k = 0;
				while(k < 10)		//i'm trying to get it to stick around long enough to display all of 0-9 //this will pull back after checkButtons and re-roll everything before going to the way it should be, i think. no good. //i put a k=10 at the end of check buttons. maybe that'll do it
				{					
					for(int q = 0; q < 10; q++)	//this is just going to skip it if the interval is not enough. it'll never change. right?
					{	
						rightNow = millis();
						if(rightNow - prevNow > setWaitInterval || rightNow - prevNow < 0)
						{
							prevNow = rightNow;
							for(int r = 0; r < clockDigitSize; r++)
							{
								setDisplaySend[r] = q;
							}
							q++;
							k++;	//will this do what i want? or will it exit too early?
						}
						mux(setDisplaySend,mux_enable);
						buttMenu = digitalRead(buttMenuPin);
						if(buttMenu == LOW)
						{	checkButtons();
							k = 10;			//this will reset it to 0 if there is even a single low spot, not even a hold
						}
					}
					
				}*/
			}
			currentIdleMuxCount = 0;	
		}//endelse !buttons
	}//endwhile !gatekeep
	
	
	
	//gatekeeper=1;	//why is this outside of the while !gatekeeper? do i need checkButtons to appoint the gatekeeper? ////it happens in other places i think
					//don't worry the end of the menu does it //also after running settings but that part is dumb
					
}//end waitForSet


void checkButtons()
{

	int buttAlert = millis();
	int buttAlertOld = buttAlert;
	
	delay(debounce);	//debounce
	if(!gatekeeper)
	{	//first time setting the clock
		for(int i = 0; i<6;i++)
		{	timecard[i] = 0;		//sets all digits to zero for first time setting the thing
		}
	}//endif !gatekeeper
	buttMenu = digitalRead(buttMenuPin);
	if(buttMenu == LOW)
	{
		while(buttAlert - buttAlertOld < menuButtDelay || buttAlert - buttAlertOld < 0)
		{	//check to see if button has been held long enough to enter menu
			if(gatekeeper)	//this will allow the clock to keep ticking while the button is held. you're right! i am clever!		///wait but the gatekeeper is in a not gatekeeper loop
			{
				if(legacyCounter)
				{  clockCounter();
				}
				else
				{	clockDivide();
				}
			}//endif gatekeeper
			mux(timecard,mux_enable_default);
			//delay(10);		//this won't affect proper timekeeping with the divider because the divider divides and the counter is super inaccurate anyway	///i think it is making stuff look dumb though
			buttAlert = millis();			
		}//endwhile
		buttMenu = digitalRead(buttMenuPin);
		if(buttMenu == LOW)
		{	menuGo();
		}//endif still LOW
	}//endif first LOW
}//end checkButtons


void menuGo()
{		//this is the setup menu that triggers if the menu button is held for three-ish seconds
	
	int hold = 1;
	int entrance = 1;
	int step = 0;
	int moveStep = 0;
	hourTypePrev = hourType;

	//i was going to make these costants global so muxMenu could use them but i'd rather hardcode muxMenu in case i rearrange things.
	const int switchStart24hr = 1;		//case number to set 24hr numbers
	const int switchStart12hr = 3;		//case number to set 12hr numbers
	const int switchMinSet = 5;			//case number to set minutes
	const int switchAmPmSet = 7;			//case number to set am/pm in 12-hr mode
	const int switchBrightCase = 8;		//case number to set brightness
	const int switchExitCase = 9;			//case number to exit the menu. might change if we expand what stuff does, ie add more cases
	
	while(hold)
	{	//while still in menu mode
		
		buttMenu = digitalRead(buttMenuPin);
		buttLesser = digitalRead(buttLesserPin);
		buttGreater = digitalRead(buttGreaterPin);
		
		for(int green=0; green<clockDigitSize; green++)		//turn all lamps off and use enable instead of specialized mux functions 
		{	mux_enable[green] = 0;
		}
		
		while(entrance && !buttMenu)
		{
			delay(debounce);	//kill time while menu button is still being held from menu mode initialization. will blank out the display while held, right?
			buttMenu = digitalRead(buttMenuPin);
		}
		entrance = 0;
		
		switch(step)		//we could eventually add functionality to hold down the up/down buttons to quick scroll hr/min set instead of individual presses
		{								//^that could be annoying for 24 hour mode. that's a lot of numbers. maybe quick scrolling is good enough to fix it but each digit might be preferable for 24hr
			case 0:		// set hourType
			{
				//Serial.print("case 0 - set hour type\n");
				hourTypePrev = hourType;
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						//Serial.print("lessbutton detected\n");
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							//Serial.print("lessbutton debounce successful\n");
							while(!buttLesser)
							{
								delay(debounce);
								buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							hourType = !hourType;
							/*Serial.print("lessbutton hour change\n");
							Serial.print("hourType= ");
							Serial.print( hourType);
							Serial.print("\n\n");*/
							/*if(hourType == 0)
							{	hourType = 1;
							}//endif hourType
							else
							{	hourType = 0;
							}//endelse hourType*/
						}//endif lesser still
					}//endif lesser first
					else if(!buttGreater)
					{
						//Serial.print("greatbutton detected");
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							//Serial.print("greatbutton debounce successful\n");
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							hourType = !hourType;
							/*Serial.print("greatbutton hour change\n");
							Serial.print("hourType= \n");
							Serial.print( hourType);
							Serial.print("\n\n");*/
							/*if(hourType == 0)
							{	hourType = 1;
							}//endif hourType
							else
							{	hourType = 0;
							}//endelse hourType*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							if(!gatekeeper)	//should this be !gatekeeper? it wasn't originally
							{	
								//Serial.print("entering changeHourType\n");
								changeHourType();
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;
				if(gatekeeper)	//if this isn't the first time we've done this, we might as well automate the hour conversion from 12->24 or viceversa
				{
					if(hourType != hourTypePrev)
					{	
						changeHourType();		//!!!!!!!! WE NEED TO CHANGE THIS TO WORK WITH THE MENU METHOD INSTEAD OF TOGGLE switch ///it says it has been updated?
						hourTypePrev = hourType;
					}
				}//end gatekeeper
				if(hourType)
				{	step = switchStart24hr;	//go to 24-hour mode hr0
				}
				else
				{	step = switchStart12hr;	// go to 12-hour mode hr0
				}
			}//end case0, set hourType
			break;
				
		/** 24-hour mode **/
			case switchStart24hr:		//hr1 set for 24-hour mode		
			{
				if(!gatekeeper)
				{
					hr1=0;
					hr0=0;
				}
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/* lesser meat for hr1 in 24 hour mode (hourType ==1)*/
							/*decrement hr1, valid 0,1,2*/
							/* 
							0->2
							2->1
							1->0
							*/
							// starts at hr0 = 0
							// 00;00;00
							if(hr1 == 0)
							{	hr1 = 2;
							}
							else if(hr1 == 2)
							{	hr1 = 1;
							}
							else
							{	hr1 = 0;
							}
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*greater meat for hr1 in 24 hour mode (hourType ==1)*/
							/*increment hr1, valid 0,1,2*/
							/* 
							0->1
							1->2
							2->0
							*/
							// starts at hr1 = 0
							// 00;00;00
							if(hr1 == 0)
							{	hr1 = 1;
							}
							else if(hr1 == 1)
							{	hr1 = 2;
							}
							else
							{	hr1 = 0;
							}
							/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;
				step++;
			}//end case1, hr1 set for 24hr 
			break;
		
			case switchStart24hr+1:		//hr0 set for 24-hour mode		//must follow hr1 set for 24-hr
			{
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
								delay(debounce);
								buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for hr0 in 24 hour mode (hourType ==1)*/
							/*decrement hr0, valid 0,1,2,3,4,5,6,7,8,9*/
							/* 
							1->0
							2->1
							3->2
							4->3
							5->4
							6->5
							7->6
							8->7
							9->8
							0->9	(unless hr1 is 2, then ->3)
							*/
							// starts at hr0 = 0
							// x0;00;00
							if( hr0 > 0)	//if hr1 is 1 to 9 (or broken)
							{	hr0--;
							}
							else if(hr0 < 1)		//if hr1 is 0 or broken
							{
								if(hr1 == 2)	
								{
									//hr1 = 2;
									hr0 = 3;
								}
								else	//if hr0 is 1 or 2 or broken
								{	
									//hr1--;
									hr0 = 9;
								}
							}//end if hr=0
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*greater meat for hr0 in 24 hour mode (hourType ==1)*/
							/*increment hr0, valid 0,1,2,3,4,5,6,7,8,9*/
							/* 
							0->1
							1->2
							2->3
							3->4 UNLESS 23, then ->0
							4->5	
							5->6
							6->7
							7->8
							8->9
							9->0 
							*/
							// starts at hr0 = 0
							// x0;00;00
							
							if((hr0 > 8)||((hr1 > 1) && (hr0 > 2)))//if hr0 is 9 or broken, or display is "23"
							{	hr0 = 0;
							}
							else
							{	
								//hr1--;
								hr0 ++;
							}

							/*if(-1< hr0<3 || 3<hr0<9)	//if hr is not 3 or 9
							{	hr0++;
							}
							else if(hr0 == 3)
							{
								if(hr1 > 1)
								{
									hr1 = 0;
									hr0 = 0;
								}
								else
								{	hr0++;
								}
							}//end if hr=3
							else if(hr0 > 8)			//if hr1 is 9 or broken
							{
								if(hr1 < 2)
								{
									//hr1++;
									hr0 = 0;
								}
								else if(hr1 > 1)	//if 2 (or edgecase)
								{
									hr0=0;
									//hr1=0;
								}
								else				//edgecase
								{	hr0 = 0;
								}
							}//end if hr1=9 or broken;*/
							/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep

				if(!hr1 || (hr1==1&&hr0<2))
				{	amPm = 0;
				}	
				else
				{	amPm = 1;
				}
				moveStep = 0;
				step = switchMinSet;	//go to min0
			}//end case2, hr0 set for 24hr mode
			break;		
		/** end 24-hour mode **/
				
		/** 12-hour mode **/			
			case switchStart12hr:		//hr1 set for 12-hour mode		
			{
				if (!gatekeeper)	//if first time through
				{
					hr1=0;
					hr0=1;
				}
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for hr1 in 12 hour mode (hourType ==0)*/
							/*decrement hr1, valid 0,1*/
							/* 
							0->1
							1->0	
							yes, that's it. awesome. 12hr mode is the best
							*/
							// starts at hr1 = 0
							// 00;00;00
							if(hr1 < 1)	//if 0 or broken
							{	hr1 = 1;	//this method was chosen instead of hr1=!hr1; in case of logical NOT encoding variances 
							}
							else
							{	hr1 = 0;
							}		
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*greater meat for hr1 in 12 hour mode (hourType ==0)*/
							/*increment hr1, valid 0,1*/
							/* 
							0->1
							1->0	
							literally the same as the lesser meat.
							*/
							// starts at hr1 = 0
							// 00;00;00
							if(hr1 < 1)	//if 0 or broken
							{	hr1 = 1;
							}
							else
							{	hr1 = 0;
							}
							/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;			
				step++;
			}
			break;
				
			case switchStart12hr+1:		//hr0 set for 12-hr mode	//must follow hr1 12-hr set
			{
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for hr0 in 12 hour mode (hourType ==0)*/
							/*decrement hr0, valid 0,1,2,3,4,5,6,7,8,9*/
							/* 
							1->0	
							2->1
							3->2
							4->3
							5->4
							6->5
							7->6
							8->7
							9->8
							0->9	 UNLESS hr1=1 then ->2
							*/
							// starts at hr0 = 0
							// x0;00;00

							//hr0 = (hr0-1) ? hr0-1 : (hr1 ? 2 : 9);	//this is close to working but doesn't quit get it

							if(hr0 > 0)	
							{	hr0--;
							}
							else
							{
								if(hr1)	
								{	hr0 = 2;
								}
								else
								{	hr0 = 9;
								}
							}
							if(!hr1&&!hr0)
							{	hr0=9;
							}
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*greater meat for hr0 in 12 hour mode (hourType ==0)*/
							/*increment hr0, valid 0,1,2,3,4,5,6,7,8,9*/
							/* 
							0->1
							1->2	
							2->3	UNLESS hr1==1
							3->4 
							4->5	
							5->6
							6->7
							7->8
							8->9
							9->0
							*/
							// starts at hr0 = 0
							// x0;00;00
							if(hr1 && (hr0==2))
							{	hr0=0;								
							}
							else
							{	
								hr0=(hr0+1)%10;
								if(!hr0)
								{	hr0=1;
								}								
							}
							/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;			
				step = switchMinSet;
			}
			break;
		/** end 12-hour mode **/
				
			case switchMinSet:		//min1 set		//don't have it overflow into hours. that would complicate things and be annoying
			{
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for min0 for either hour mode*/
							/*decrement min1, valid 0,1,2,3,4,5*/
							/* 
							5->4	
							4->3
							3->2
							2->1
							1->0
							0->5
							*/
							// starts at min1 = 0
							// xx;00;00
							if(min1 > 0)	//if min1 is 1 to 5 (or broken higher)
							{	min1--;
							}
							else if(min1 < 1)	//if min1 is 0 (or broken lower)
							{	min1 = 5;
							}				//golly. not having to account for rollover is great
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*greater meat for min1 for either hour mode*/
							/*increment min1, valid 0,1,2,3,4,5*/
							/* 
							0->1
							1->2
							2->3
							3->4 
							4->5	
							5->0
							*/
							// starts at min1 = 0
							// xx;00;00
							min1=(min1+1)%6;
							/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;			
				step++;
			}
			break;
				
			case switchMinSet+1:		//min0 set		//must follow min1 set
			{
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for min0 in either hour mode*/
							/*decrement min0, valid 0,1,2,3,4,5,6,7,8,9*/
							/*must consider rollover*/
							/* 
							1->0
							2->1
							3->2
							4->3
							5->4
							6->5
							7->6
							8->7
							9->8
							0->9 							
							*/
							// starts at min0 = 0
							// xx;x0;00
							if(min0 > 0)	//if min0 is 1 to 9 (or broken higher)
							{	min0--;
							}
							else 	//if min0 is 0 (or broken lower)
							{	min0 = 9;
							}
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*greater meat for min0 for either hour mode*/
							/*increment min0, valid 0,1,2,3,4,5,6,7,8,9*/
							/* 
							0->1
							1->2
							2->3
							3->4 
							4->5	
							5->6
							6->7
							7->8
							8->9
							9->0 							
							*/
							// starts at min0 = 0
							// xx;x0;00
							min0=(min0+1)%10;
							/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;	
				if(hourType == 0)	//if 12-hour mode, set am/pm
				{	step = switchAmPmSet;
				}
				else if(includeBrightness)
				{	step = switchBrightCase;
				}
				else
					step = switchExitCase;
			}
			break;
				
			case switchAmPmSet:		//am/pm for 12-hr
			{	//am is amPm=0, pm is amPm=1
					//i'm thinking 9 for am and 6 for pm? like where the sun goes. up for morning down for night? on opposite ends of the display
				if(!gatekeeper)
				{	amPm = 0;			//forces auto am on startup even though we mess with it like ten times before this
				}
				while(!moveStep)
				{
					muxMenu(step);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for am/pm*/
							if(amPm == 0)
							{	amPm = 1;
							}
							else
							{	amPm = 0;
							}	
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater){
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
							if(!buttGreater){
								while(!buttGreater)
								{
									delay(debounce);
									buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
								}
								/*greater meat for am/pm*/
								if(amPm == 0)
							{	amPm = 1;
							}
							else
							{	amPm = 0;
							}	//literally copypasta from the other one. toggle choices are great.
								/*end greater meat*/
							}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;
				if(includeBrightness)
				{	step = switchBrightCase;
				}
				else
				{	step = switchExitCase;
				}
			}
			break;
				
			case switchBrightCase:		//brightness set. maybe could do both pwm and brute force simultaneously so one doesn't break if we change our minds. backwards compatibility!
			{
				int brightDispTop = 16; //just for occasional convenient math
				int brightDispBottom = 2;
				int brightDispStep = 2;
				float numerator = (brightDispTop-brightDispBottom)*(brightness - brightness_min);
				float denom = (brightness_max-brightness_min);
				int brightDisp = round(numerator/denom + brightDispBottom);	//does arduino do rounding?
				
				int oddCheck = brightDisp%2;
				if(oddCheck)		//this isn't needed if top=16,bottom=2,max=128,min=8 but i'll leave it just in case those change
				{	brightDisp++;
				}
				if(brightDisp > brightDispTop)
				{	brightDisp = brightDispTop;
				}
				if(brightDisp < brightDispBottom)
				{	brightDisp = brightDispBottom;
				}
				
				int m_array[6];	//will initializing this in the loop break things? or does it disappear once the loop concludes?
				
				while(!moveStep)
				{
					m_array[1] = brightDisp/10;
					m_array[2] = brightDisp%10;
					mux_enable[1] = 1;
					mux_enable[2] = 1;
					mux(m_array,mux_enable);
					buttLesser = digitalRead(buttLesserPin);
					buttGreater = digitalRead(buttGreaterPin);
					buttMenu = digitalRead(buttMenuPin);
					if(!buttLesser)
					{
						delay(debounce);	
						buttLesser = digitalRead(buttLesserPin);
						if(!buttLesser){
							while(!buttLesser)
							{
							delay(debounce);
							buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
							}
							/*lesser meat for brightness*/
							if(automatedPWM)
							{
								brightDisp -= brightDispStep;	
								if(brightDisp < brightDispBottom)
								{	brightDisp = brightDispBottom;
								}
								float bigup = ((brightDisp-brightDispBottom)*denom);
								float bigdown = (brightDispTop-brightDispBottom)+brightness_min;
								brightness = round(bigup/bigdown);		//does arduino do rounding?
								if(brightness < brightness_min)
								{	brightness = brightness_min;
								}
							}	//endif PWM mode
							if(bruteForce)
							{
								//lol idk. maybe we should stick with pwm for convenience. 
								// OR we can use the pwm math to do the adjustments and then convert it to brute force times after that.
							}
							/* end lesser meat*/
						}//endif lesser
					}//endif
					else if(!buttGreater)
					{
						delay(debounce);	
						buttGreater = digitalRead(buttGreaterPin);
						if(!buttGreater){
							while(!buttGreater)
							{
								delay(debounce);
								buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
							}
						/*greater meat for brightness*/
							if(automatedPWM)
							{
								brightDisp += brightDispStep;	
								if(brightDisp > brightDispTop)
								{	brightDisp = brightDispTop;
								}
								float bigup = ((brightDisp-brightDispBottom)*denom);
								float bigdown = (brightDispTop-brightDispBottom)+brightness_min;
								brightness = round(bigup/bigdown);		//does arduino do rounding?
								if(brightness > brightness_max)
								{	brightness = brightness_max;
								}
							}	//endif PWM mode
								/*end greater meat*/
						}
					}//endelseif greater
					else if(!buttMenu)
					{
						delay(debounce);	
						buttMenu = digitalRead(buttMenuPin);
						if(!buttMenu)
						{
							while(!buttMenu)
							{
								delay(debounce);
								buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
							}
							moveStep = 1;	//exits the while
						}//endif menu
					}//endelseif menu
				}//endwhile !moveStep
				moveStep = 0;			
				step = switchExitCase;
			}
			break;
			
			case switchExitCase:		// exit menu routine
			{
				gatekeeper = 1;		//we've sucessfully set the clock for at least the first time
				hold = 0;
				timeChange = 0;
			}
			break;
		}//end switch
		//can add more cases as needed to expand menu. just be sure to change switch numbers is the muxes below
		
	}//endwhile hold
}//end routine menuGo
	
	
/****** specialty mux preparation ******/
void muxMenu(int step)
{		//these are hardcoded. change them if you change the menu switchcase numbering
	
	//if in 24 mode, only muxes hr1 and hr0 digits with 24 and kills it on the lower two,
	//if in 12 mode, only muxes min1 and min0 digits with 12 and kills it on the upper two
	
	int mux_menu_array[6] = {0,0,0,0,0,0};	//array to be used to display stuff in menu mode. included the last 0s in case a shorter array breaks it
	
	for(int lol=0;lol<clockDigitSize; lol++)
	{	mux_enable[lol] = 0;	//default set all low
	}
	
	for(int b = 0; b < clockDigitSize; b++)
	{	digitalWrite(digit[b],LOW); 		//forces all digits to be off before we turn only some of them on wnkbr
	}
	
	
	if(step == 0)
	{	//hourType set
		if(!hourType)
		{	//if 12-hour mode, only muxes middle digits with 12 and kills the other digits
			//Serial.print("muxMenu 12hour\n");
			mux_menu_array[2] = 1;
			mux_menu_array[3] = 2;
			mux_enable[2] = 1;
			mux_enable[3] = 1;				
		}//endif hourType 0
		
		else
		{	//if 24-hour mode, only muxes front digits with 24 and kills the other digits
			//Serial.print("muxMenu24hour\n");
			mux_menu_array[2] = 2;
			mux_menu_array[3] = 4;		
			mux_enable[2] = 1;		//this puts them on the middle digits so that 12 & 24 are on the same spaces
			mux_enable[3] = 1;		
		}//endif hourType 1

/*		else
		{	//if 24-hour mode, only muxes front digits with 24 and kills the other digits
			//Serial.print("muxMenu24hour\n");
			mux_menu_array[0] = 2;
			mux_menu_array[1] = 4;		
			mux_enable[0] = 1;		//this put them on the front 2 digits, which i thought was confusing for setting the hour immediately after
			mux_enable[1] = 1;		
		}//endif hourType 1*/
		
		mux(mux_menu_array,mux_enable);

	}//endif step == 0
	
	if((0 < step) && (step < 5))		
	{	//hours set. only muxes front digits with hours and kills the other digits
		//Serial.print("muxMenuhourset\n");
		mux_menu_array[0] = hr1;
		mux_menu_array[1] = hr0;
		mux_enable[0] = 1;
		mux_enable[1] = 1;
		mux(mux_menu_array,mux_enable);
	}//endif step 0-4
	
	if(step == 5 || step == 6)
	{	//minutes set. only muxes middle digits with minutes and kills the other digits
		//Serial.print("muxMenuminuteset\n");
		mux_menu_array[0] = hr1;
		mux_menu_array[1] = hr0;
		mux_menu_array[2] = min1;
		mux_menu_array[3] = min0;
		for(byte bleem=0;bleem<4;bleem++)
		{	mux_enable[bleem] = 1;
		}
		mux(mux_menu_array,mux_enable);
	}//endif step 5,6
	
	if(step ==7)
	{
		//Serial.print("muxMenuampm\n");
		if(muxMenuDayOption == 0)	//if 9 in hr1 digit and 6 in min0 digit
		{
			if(amPm == 1)	//if pm
			{
				mux_menu_array[3] = 6;
				mux_enable[3] = 1;
				mux(mux_menu_array,mux_enable);
			}
			else			//if am or broken 
			{
				mux_menu_array[0] = 9;
				mux_enable[0] = 1;
				mux(mux_menu_array,mux_enable);
			}	
		}
		else	//if using the 0s in front for am and in middle for pm
		{
			if(amPm == 1)	//if pm
			{
				mux_enable[2] = 1;
				mux_enable[3] = 1;
				mux(mux_menu_array,mux_enable);
			}
			else			//if am or broken 
			{
				mux_enable[0] = 1;
				mux_enable[1] = 1;
				mux(mux_menu_array,mux_enable);
			}
		}	
	}//endif step 7
	
	//note: no step 8 because it is handled within the menu switchcase above. the enable buffer simplified things a lot
	
}//end routine muxMenu

