
/**
the 13th god help me draft of trying to kludge together a nixie clock program (for both 4 & 6 digits)
b gilder
3jan2017
**/

/******* rev log ************************************************
13
minor cosmetic changes.
finally added button functionality. menu is pretty slick. basically a state machine inside a switchcase. yes, it is beautiful.
will try eventually to change clock counting from a simple comparison to a clock divider of some sort for accuracy.
	-- or could add adjustment to offset for drift by adding a second or two every so often at 430am or something
	--could use a counter to see how many times 430 comes around before adding or subtracting some time.
	--if subtracting, will need to set a flag so there is not an infinite subtraction at every immediate 430
	--or if there are multiple 430s per counter, can add 1 to total necessary count
	--wait we know which direction it will go. if subtracting, just add 1 to a counter or something before resetting counter
if we use RTC, we can use time.h to simplify timekeeping.
or, you know, just literally divide the clock. but what do i know, amirite
24/12 is now in the menu. so that pretty much eliminates the need to change hourType on the fly. we can eliminate a lot of that clunkiness.
	--although, maybe we can change it in menu once it is already set, so it can be correct new time even if the clock is already running

12
all about 24/12 hr conversions. added serial output for troubleshooting purposes.   [[see sketch_aug21b. actually ran on chip]]
added stuff to settings to fix initial condition contradictions
added bunch of stuff to change hour type because it was pretty jank
if input for 24-hr is greater than 24, forcing down to 00: ALSO ditto for 12-hr down to 01: but also if less than hr0_min

11
in changehourtype fxn, += and -= operators seem to work in arduino
changed return void; to just return ;  but anecdotal evidence suggests including return has slowed down the compiler considerably
tried killing hr1 display being 0 on 12-hr type. AAAAND IT TOTALLY WORKS i'm amazing. !!and! it still shows the roll so there isn't a blank second of nothing happening at :59:59
put change hour type fxn call within the old outermost if that was formerly within the fxn itself, so now we can skip that fxn call completely until something changes. it used to call the fxn first and then check the condition in the fxn
changed notation to include brackets around single line conditional statements OKAY? HAPPY #MICHAELRIPLEY? but i still kept them all squished together because it helps me see how stuff is associated. can't win em all
changed conversion_error fxn call within change_type to assign a value to a variable in case the if statement was improperly handling the error fxn value return
changed output_bin to 2-d array OKAY #MICHAELRIPLEY jeez

10
the updated mux fxn now part of this file
auto-sets hr1_max and hr0_max and hr0_min depending on hourType
changed on and off times to floats to allow for decimals.
added hr1_old and hr0_old
added roll_reset_time, used to set sec0 upon exiting the animation.
-- currently (roll0-4.c) sets sec0 to reset/2, was thinking i could have the animation start at like :57 or something so 59 isnt hanging out waiting for the roll to get to it. (if going MSB->LSB)
----maybe that is a good reason to do it from lsb -> msb instead of how i'm doing it.
changed mux() calls to mux(timecard) to pass the array to be displayed, so can use for fancy roll as well without having copypasta in the form of mux_hax(). also %10 within mux
flicker_interval isn't doing anything so it has been commented out
ditto previousTime

8.5
incorporated the mux function & wow is it simple
now able to switch 12/24 hr mode on the fly (badass) -> will need physical switch
circumscribed possible hr0_max issues with 12->13 o clock and 02->03 o clock by using hr1 state
added more programmer defined vars for flashing the lamps “manually”
preemptively added gatekeeper var to restrict initialization setting function or phase to only the first run
---will need to figure out how to mux and initial setup simultaneously
---initial setup is going to strongly resemble clock function and 12/24 switch as far as ripple is concerned
outsourced clockCount to its own function
---MAYBE COULD BE HIJACKED FOR FANCY ROLL since we aren’t waiting for it to be on the seconds! maybe unnecessary tho


~~~~~~~~~future notes~~~~~~~~~~
13
just divide millis() by 1000 you dope. and then use an offset in the menu. can still use the ripple adder to get correct wraparound.
will need to fix changeHourType to work with the new menu hour type change type thing instead of using a switch
might want to make this automatically 24 hour initially so that everything is zeros. i don't remember how changeHourType initializes everything.
	--we might just make changeHourType do what it normally does but then initialize everything to 0s in 24 hour mode.
11
make sure hourType respects amPm (or vice versa?)
also, make 00: when initialized in 12-hr change automatically to 12: (altho if we have initial settings at 12:xx:xx it shouldn't matter as long as it rolls correctly...)
-- wait so hang on if in 24hr mode and initial setting is 12: then that means it is pm. should it autochange to 00:?
--maybe can put a compensation assignment in the settings loop to check what needs to be initial value or whatever (esp after we incorporate a physical switch)
~!~!~ Still don't want to include the fancy roll in this main file until it is completely independently functional
pointers would make timecard a lot easier.. but then wouldn't i have to restructure stuff for pointer arrays and shit? idk. pointers would make it easier but also way way harder

10
could kill hr1 when it is 0 if we are on 12-hr mode
should figure out some way to call fancy_hour_roll a second or two before :59 so :59 isn't just hanging out waiting for roll to get to it
-- have roll_reset_time var to shift the clock after roll finishes, currently (roll0-4.c) sets sec0 to reset/2, thought the other half of reset could "jump the gun" at like :57 or something
---- if we do this, would need to keep incrementing the sec0 for two or three seconds even as things start rolling
can change output_bin to a 2-d array like bin[n][h] instead of 4*n etc		
24 hour mode doesn't currently respect amPm state. even if amPm = 0, goes to 13 (although if it is already in 24, it would only be 12 if going to 13. maybe force amPm to update in that case?)

8.5
could maybe have hr1 digit have its own dot to display am/pm in 12hr mode
---will need its own transistor set and output pin etc. can mimic secdot code
	still don’t know whether mux should be main or its own function
	---if its own function we can call it while initially setting up!
still need fancy hour roll
***********************************************************/




//#include whateverthefuck
//delay(500); 	// if there is weird power transience do we want to make stuff wait a tick?


/****************PROGRAMMER DEFINED*****************/
/**/					//
/**/	const int clockDigitSize = 6;   			//4 or 6
/**/  //const long flicker_interval = 20; 			//measured in millisec, duration each digit is on. we will need to experiment with this 	//not currently used but maybe for built in pwm stuff?
/**/	const int brightness_max = 128; 		//50% duty cycle. would it be advisable to allow going over that?
/**/	const int brightness_min = 8; 			//this is totally a made up number. we will have to experiment with this as well.
/**/	int brightness = 128; 				//0-255, controls built-in pwm scheme. could be what’s changed by user brightness selection. initially half bright
/**/	const int automatedPWM = 0;			//enable if letting arduino flicker lamps for us (what if you enable both? it might still be legal..  interesting)
/**/	const int bruteForce = 1;				//enable if using custom time delays to flicker lamps
/**/	float onTime = 3.5;				//in ms, for brute force. will need to experiment 
/**/	float offTime = .1;				//ditto	//protip can't be constants if we set them again below lulz
/**/	int hourType = 0;				// 0 for 12-hr, 1 for 24-hr	//toggled by a physical throw switch? therefore variable	//// or in menu. still variable
/**/ 	const float numeral_change_time = 90; 		//in ms, for fancy roll	//float for decimal math later
/**/	const int roll_reset_time = 6;			//in seconds, at the moment must be even number, total amount of time fancy roll animation takes
/**/	const int digitSetBlinkMult = 200;	//the number of times to run mux for each idle 0s/8s displayed before initial time setting //multiply by 5 = roughly 1sec
/**/	const int buttonQuarterDebounce = 50;	//in ms, for debounce
/**/	const int menuButtDelay = 3000;			//in ms, time to hold the menu button before entering settings menu	
/**/	const float debounce = 40;			
/**/	const int changeHourSwitch = 0;		//1 if using toggle switch, 0 if using button menu	
/**/	const int muxMenuDayOption = 0;			//how to display am/pm choice in menu. 0 is 9 in hr0 and 6 in min1. 1 is 0s in front for am and middle for pm
/**/					//
/*************************************************************/



/** constants **/
const unsigned long sec_interval = 1000;	//1 second	///////ooooooo! maybe this is a subtle way to adjust for drift!!!
const int digitPin1 = 11;   	// MSB, hr1 	//this is what switches on/off the digit muxes
const int digitPin2 = 10;		//conveniently, there are exactly 6 digital pwm pins
const int digitPin3 = 9;
const int digitPin4 = 6; 		//4-digit LSB, min0
const int digitPin5 = 5;
const int digitPin6 = 3;	 	// 6-digit LSB, sec0
const int digit[6] = {digitPin1,digitPin2,digitPin3,digitPin4,digitPin5,digitPin6};	//all 6 included, potentially not utilized. can cannibalize for pins in 4-digit if leftovers are needed
const int secDotPin = 13; 	// pin controlling mux for sec-blink dot 
const int switchPin = A5;	 //!!!! !!!! !!!! or whatever it actually should be // don’t currently have switch

const int buttMenuPin = 14;		//should these be on pullup pins so we can make them active low and not have to reset them high?
const int buttLesserPin = 15;
const int buttGreaterPin = 16;

const int driverPin1 = 8; 	//these might be changed so that the driver and buffer and stuff can just go straight across	//have they been already?	//not yet
const int driverPin2 = 7;
const int driverPin3 = 4;	
const int driverPin4 = 2;		//LSB
const int driverArray[4] = {driverPin1,driverPin2,driverPin3,driverPin4}; 
/*hours are variable to switch 12/24 mode,
see below */
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
int hr0_max = 3;  	// I think this can be 2 or 3 without it messing up normal clock progression.. I think I took care of that down below (I’m basically a genius)
int hr0_min = 0;  	// will automatically change to 1 if 12-hr mode
int hr1 = 1; 		//hour holders, 1 is MSB,   /**/ initial setting is 12:34:56 /**/
int hr0 = 2; 		
int min1 = 3; 		//minute holders
int min0 = 4; 
int sec1 = 5; 		//sec holders
int sec0 = 6; 
int timecard[6] = {hr1,hr0,min1,min0,sec1,sec0};		//pointers would make this easier. sigh. but then pointers. yuckers.
int hr1_old = hr1;
int hr0_old = hr0;
int secDotState = LOW;		// used to set the sec-blink dot 
int digitState = LOW; 		//used to set the currently displaying digit
int buttMenu = LOW;
int buttLesser = LOW;
int buttGreater = LOW;
int amPm = 0;			//0 is am, 1 is pm	 //allows changing 12/24 on the fly (because I’m a badass)
unsigned long previousSec = 0;  //for metronome
// unsigned long previousTime = 0;  //for old mux? maybe? can we get rid of this?	//i think maybe? i'm commenting it out for now
int hourTypePrev = hourType;		// for changing 12/24 on the fly
int gatekeeper = 0;		//I figure we can keep track of whether we’ve done the initial setup phase or whatever. becomes 1 when finished initializing

/***************************************************************************************/



void setup()			//let's get rrready to rrrrrumblllllle
{
  pinMode(switchPin, INPUT);	//don’t currently have switch
  pinMode(secDotPin, OUTPUT);
  pinMode(driverPin1, OUTPUT);
  pinMode(driverPin2, OUTPUT);
  pinMode(driverPin3, OUTPUT);
  pinMode(driverPin4, OUTPUT);
  for(int p = 0; p < clockDigitSize; p++)
  {	pinMode(digit[p], OUTPUT);
  }												///did this end up working? i hope so. 
  pinMode(buttMenuPin, INPUT);
  pinMode(buttLesserPin, INPUT);
  pinMode(buttGreaterPin, INPUT);
  
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
      	{amPm = 0;
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

}//end setup fxn


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


void loop()   /*what if we made this a dependent function and had the mux be main? <-this might be a good idea <-maybe not. need mux during initialization*/
{

if(!gatekeeper)
  {
	//initialization routine here	//if its own function, bring the brightness cap thing along
	settings();
	gatekeeper = 1;	//could might delete if waitForSet() is a thing
  }

			 	//metronome on the seconds
unsigned long currentSec=millis();
if((currentSec - previousSec >= sec_interval) || (currentSec - previousSec < 0)) 	//this would be so much easier if we used the 555. probs less accurate tho	////LOL ACCURACY WHAT
  {//count another sec!
	previousSec = currentSec;   	// save the last time you blinked the sec-dot LED
	if (secDotState == LOW)		// if LED is off turn it on and vice-versa
	{	secDotState = 1;
	}
	else
	{	secDotState = LOW;
	}
	clockCount();	//will all my variables be updated globally? because this would totally defeat the purpose if not

	digitalWrite(secDotPin, secDotState);	//blink sec dot
	timecard[0]={hr1};	//implementing pointers would erase my need to do this. super duper handy. ugh except then pointers
	timecard[1]={hr0};
	timecard[2]={min1};
	timecard[3]={min0};
	timecard[4]={sec1};
	timecard[5]={sec0};
//	hourType = analogRead(switchPin);	//or whatever the switch pin poll function is. don’t currently have switch
	
	if(changeHourSwitch)	//idk i'm just guessing here. previously there wasn't a conditional, just the next line in brackets and idk why.
  	  {	changeHourType();	//if shit just went down	//if there is an arduino switch flip checker, use that instead of this Doesn’t Match nonsense
	  }

  }//endif currentSec
mux(timecard); 		//?? does this go here??? how do??? //no, if mux is main function //but it probably won’t be, so maybe it goes here?

}//end main loop fxn


void clockCount()
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
		timecard[0] = hr1;
		timecard[1] = hr0;
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
		fancy_hour_roll();	// we don’t have this built yet	//now we half do..
	}//endelse hour rollover
  }//endelse min rollover

}//end clock count fxn


void changeHourType()
{
//changing 12/24 on the fly	
////now also compatable with the button menu

  if(changeHourSwitch)
  {	hourType = digitalRead(switchPin);
  }
  if(hourType == 0)	//if 12-hr, at this very second				////why is this not after the prev check?
  {
  	hr1_max = 1;   
  	hr0_max = 2;    
  	hr0_min = 1; 
    	//Serial.println("hourType ==0, 12hr");
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
  //	Serial.println("else hourType ==1");
  
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


void mux(int mux_array[])		//this is a mux function
{
//the way i see it we could do analogWrite or we could do on/off times
//this is its own function so that fancy roll can also use it

unsigned long entranceTime = millis(); //don't currently have use for this but it might be handy eventually at some point maybe idk

for(int w = 0; w < clockDigitSize; w++) 
  {		

				// THIS SETS THE DRIVER OUTPUTS		
	int x = mux_array[w]%10; 
	for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
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
	  	if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
	  	{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
	  	}
		digitalWrite(digit[w],digitState);  
  		delay(onTime);
		
		digitState = LOW;
		digitalWrite(digit[w],digitState);	
		delay(offTime/2);// here is the first half of off time
    	}//endif

  }//endfor

} //end mux fxn




//////////////// new stuff in rev13 //////////
void waitForSet()
{	//another option for doing this is only sending 0s to mux for certain duration and then not muxing for duration to blink numbers
	
	const int setDisplay[2][6] = {{0,0,0,0,0,0},{8,8,8,8,8,8}};
	int setDisplaySend[6] = {0,0,0,0,0,0};
	int currentIdleMuxCount = 0;
	int displayNum = 0;
	
	while(!gatekeeper){
		mux(setDisplaySend);
		buttMenu = digitalRead(buttMenuPin);
		if(buttMenu == HIGH){
			checkButtonsFirst();
		}//endif
		else{
			for(int b = 0; b<6; b++){
				setDisplaySend[b] = setDisplay[displayNum][b];
			}//endfor
			while(currentIdleMuxCount < digitSetBlinkMult){
				mux(setDisplaySend);
				delay(5);
				currentIdleMuxCount++;
			}//endwhile
			if(displayNum ==0){
				displayNum = 1;
			}//endif
			else{
				displayNum = 0;
			}//endelse
			currentIdleMuxCount = 0;	
		}//endelse
		
	}//endwhile
	
	
	
	gatekeeper=1;
	
}//end waitForSet


void checkButtonsFirst()
{

	int buttAlert = millis();
	int buttAlertOld = buttAlert;
	
	if(!gatekeeper){	//first time setting the clock
		delay(debounce);	//debounce
		for(int i = 0; i<6;i++)
		{	timecard[i] = 0;	//sets all digits to zero for first time setting the thing
		}
		buttMenu = digitalRead(buttMenuPin);
		if(buttMenu == HIGH){
			
			while(buttAlert - buttAlertOld < menuButtDelay){	//check to see if button has been held long enough to enter menu
				mux(timecard);
				delay(20);
				buttAlert = millis();			
			}//endwhile
			buttMenu = digitalRead(buttMenuPin);
			if(buttMenu == HIGH){
				menuGo();
			}//endif still HIGH
		}//endif first HIGH
	
	
	}//endif !gatekeeper
	
}//end checkButtonsFirst


void menuGo()
{		//this is the setup menu that triggers if the menu button is held for three-ish seconds
	
	int hold = 1;
	int entrance = 1;
	int step = 0;
	int moveStep = 0;
	
	const int exitCase = 9;			//case number to exit the menu. might change if we expand what stuff does, ie add more cases
	const int start24hr = 1;		//case number to set 24hr numbers
	const int start12hr = 3;		//case number to set 12hr numbers
	const int minSet = 5;			//case number to set minutes
	const int brightCase = 8;		//case number to set brightness
	const int amPmSet = 7;			//case number to set am/pm in 12-hr mode
	
	while(hold)
	{	//while still in menu mode
		
	buttMenu = digitalRead(buttMenuPin);
	buttLesser = digitalRead(buttLesserPin);
	buttGreater = digitalRead(buttGreaterPin);
	
	while(entrance && buttMenu)
	{
	delay(100);	//kill time while menu button is still being held from menu mode initialization. will blank out the display while held
	buttMenu = digitalRead(buttMenuPin);
	}
	entrance = 0;
	
	switch(step)		//we could eventually add functionality to hold down the up/down buttons to quick scroll hr/min set instead of individual presses
		{								//^that could be annoying for 24 hour mode. that's a lot of numbers. maybe quick scrolling is good enough to fix it but each digit might be preferable for 24hr
		case 0:		// set hourType
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				hourTypePrev = hourType;
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						if(hourType == 0){
							hourType = 1;
						}//endif hourType
						else
						{
							hourType = 0;
						}//endelse hourType
					}//endif lesser still
				}//endif lesser first
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						if(hourType == 0)
						{
							hourType = 1;
						}//endif hourType
						else
						{
							hourType = 0;
						}//endelse hourType
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
						{
							delay(debounce);
							buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
						}
						if(gatekeeper)
						{	changeHourType;
						}
						moveStep = 1;	//exits the while
					}//endif menu
				}//endelseif menu
			}//endwhile !moveStep
			moveStep = 0;
			if(gatekeeper)	//if this isn't the first time we've done this, we might as well automate the hour conversion from 12->24 or viceversa
			{
				if(hourType != hourTypePrev)
				{	changeHourType;		//!!!!!!!! !!!!! !!!! WE NEED TO CHANGE THIS TO WORK WITH THE MENU METHOD INSTEAD OF TOGGLE SWITCH
				}
			}//end gatekeeper
			if(hourType == 1)
			{	step = start24hr;	//go to 24-hour mode hr0
			}
			else
			{ step = start12hr;	// go to 12-hour mode hr0
			}
		}//end case0, set hourType
			break;
			
/** 24-hour mode **/
		case 1:		//hr0 set for 24-hour mode		//!! !! !! i might prefer to make this hr1 set with hr0 overflow rather than each digit
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						/* lesser meat for hr0 in 24 hour mode (hourType ==1)*/
						/*decrement hr0, valid 0,1,2*/
						/* 
						0->2
						2->1
						1->0
						*/
						// starts at hr0 = 0
						// 00;00;00
						if(hr0 == 0)
						{	hr0 = 2;
						}
						else if(hr0 == 2)
						{	hr0 = 1;
						}
						else
						{	hr0 = 0;
						}
						/* end lesser meat*/
					}//endif lesser
				}//endif
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*greater meat for hr0 in 24 hour mode (hourType ==1)*/
						/*increment hr0, valid 0,1,2*/
						/* 
						0->1
						1->2
						2->0
						*/
						// starts at hr0 = 0
						// 00;00;00
						if(hr0 == 0)
						{	hr0 = 1;
						}
						else if(hr0 == 1)
						{	hr0 = 2;
						}
						else
						{	hr0 = 0;
						}
						/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
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
		}//end case1, hr0 set for 24hr 
			break;

		case 2:		//hr1 set for 24-hour mode		//must follow hr0 set for 24-hr
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*lesser meat for hr1 in 24 hour mode (hourType ==1)*/
						/*decrement hr1, valid 0,1,2,3,4,5,6,7,8,9*/
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
						0->9	UNLESS 00 and then hr0=2 and hr1=3
						*/
						// starts at hr1 = 0
						// x0;00;00
						if( hr1 > 0)	//if hr1 is 1 to 9 (or broken)
						{	hr1--;
						}
						if(hr1 < 1)		//if hr1 is 0 or broken
						{
							if(hr0 < 1)	//if hr0 is 0 (ie midnight) or broken	
							{
								hr0 = 2;
								hr1 = 3;
							}
							else	//if hr0 is 1 or 2 or broken
							{	
								hr0--;
								hr1 = 9;
							}
						}//end if hr=0
						/* end lesser meat*/
					}//endif lesser
				}//endif
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*greater meat for hr1 in 24 hour mode (hourType ==1)*/
						/*increment hr1, valid 0,1,2,3,4,5,6,7,8,9*/
						/*must consider rollover*/
						/* 
						0->1
						1->2
						2->3
						3->4 UNLESS 23, then hr0 = 0 and hr1 = 0
						4->5	
						5->6
						6->7
						7->8
						8->9
						9->0 AND hr0 UP	(there will never be a 29 case because it is caught by the 24 case above. edgecase'd anyway)
						*/
						// starts at hr1 = 0
						// x0;00;00
						if(-1< hr1<3 || 3<hr1<9)	//if hr is not 3 or 9
						{	hr1++;
						}
						if(hr1 == 3)
						{
							if(hr0 > 1)
							{
								hr0 = 0;
								hr1 = 0;
							}
							else
							{	hr1++;
							}
						}//end if hr=3
						if(hr1 > 8)			//if hr1 is 9 or broken
						{
							if(hr0 < 2)
							{
								hr0++;
								hr1 = 0;
							}
							else if(hr0 > 1)	//if 2 (or edgecase)
							{
								hr0=0;
								hr1=0;
							}
							else				//edgecase
							{	hr1 = 0;
							}
						}//end if hr1=9 or broken;
						/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
						{
							delay(debounce);
							buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
						}
						moveStep = 1;	//exits the while
					}//endif menu
				}//endelseif menu
			}//endwhile !moveStep
			moveStep = 0;
			step = minSet;	//go to min0
		}//end case2, hr1 set for 24hr mode
			break;		
/** end 24-hour mode **/
			
/** 12-hour mode **/			
		case 3:		//hr0 set for 12-hour mode		
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*lesser meat for hr0 in 12 hour mode (hourType ==0)*/
						/*decrement hr0, valid 0,1*/
						/* 
						0->1
						1->0	
						yes, that's it. awesome. 12hr mode is the best
						*/
						// starts at hr0 = 0
						// 00;00;00
						if(hr0 < 1)	//if 0 or broken
						{	hr0 = 1;
						}
						else
						{	hr0 = 0;
						}		
						/* end lesser meat*/
					}//endif lesser
				}//endif
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*greater meat for hr0 in 12 hour mode (hourType ==0)*/
						/*increment hr0, valid 0,1*/
						/* 
						0->1
						1->0	
						literally the same as the lesser meat.
						*/
						// starts at hr0 = 0
						// 00;00;00
						if(hr0 < 1)	//if 0 or broken
						{	hr0 = 1;
						}
						else
						{	hr0 = 0;
						}
						/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
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
			
		case 4:		//hr1 set for 12-hr mode	//must follow hr0 12-hr set
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*lesser meat for hr1 in 12 hour mode (hourType ==0)*/
						/*decrement hr1, valid 0,1,2,3,4,5,6,7,8,9*/
						/*must consider rollover*/
						/* 
						1->0	CONSIDER UNDERFLOW
						2->1
						3->2
						4->3
						5->4
						6->5
						7->6
						8->7
						9->8
						0->9	CONSIDER UNDERFLOW
						*/
						// starts at hr1 = 0
						// x0;00;00
						if( hr1 > 1)	//if hr1 is 2 to 9 (or broken)
						{	hr1--;
						}
						if(hr1 < 1)		//if hr1 is 0	(so, 10 o'clock is the only time this will happen)
						{
							hr0 = 0;
							hr1 = 9;
						}
						if(hr1 == 1)		//if hr1 is 1
						{
							if(hr0 < 1)		//if hr0 is 0 or broken
							{
							hr0 = 1;
							hr1 = 2;
							}
							else		//if 11 o'clock
							{
								hr1 = 0;	//becomes 10 o'clock
							}
						}//end if hr1=1
						/* end lesser meat*/
					}//endif lesser
				}//endif
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*greater meat for hr1 in 12 hour mode (hourType ==0)*/
						/*increment hr1, valid 0,1,2,3,4,5,6,7,8,9*/
						/*must consider rollover*/
						/* 
						0->1
						1->2	
						2->3	UNLESS hr0==1
						3->4 
						4->5	
						5->6
						6->7
						7->8
						8->9
						9->0 AND hr0 UP	
						*/
						// starts at hr1 = 0
						// x0;00;00
						if(hr1 < 2 || 2 > hr1 > 9)		//if 0 or 1, or 3 to 8
						{	hr1++;
						}
						if(hr1 == 2)
						{
							if(hr0 == 1)	//if 12:00
							{
								hr0 = 0;
								hr1 = 1;
							}
							else
							{	hr1++;
							}
						}
						if(hr1 == 9)	//if 09:00, will never be 19:00 in 12-hr mode
						{
							hr0 = 1;
							hr1 = 0;
						}
						/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
						{
							delay(debounce);
							buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
						}
						moveStep = 1;	//exits the while
					}//endif menu
				}//endelseif menu
			}//endwhile !moveStep
			moveStep = 0;			
			step = minSet;
		}
			break;
/** end 12-hour mode **/
			
		case 5:		//min0 set		//don't have it overflow into hours. that would complicate things and be annoying
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*lesser meat for min0 for either hour mode*/
						/*decrement min0, valid 0,1,2,3,4,5*/
						/* 
						5->4	
						4->3
						3->2
						2->1
						1->0
						0->5
						*/
						// starts at min0 = 0
						// xx;00;00
						if(min0 > 0)	//if min0 is 1 to 5 (or broken higher)
						{	min0--;
						}
						if(min0 < 1)	//if min0 is 0 (or broken lower)
						{	min0 = 5;
						}				//golly. not having to account for rollover is great
						/* end lesser meat*/
					}//endif lesser
				}//endif
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*greater meat for min0 for either hour mode*/
						/*increment min0, valid 0,1,2,3,4,5*/
						/* 
						0->1
						1->2
						2->3
						3->4 
						4->5	
						5->0
						*/
						// starts at min0 = 0
						// xx;00;00
						if(min0 < 5)	//if min0 is 0 to 4 (or broken lower)
						{	min0++;
						}
						if(min0 > 4)	//if min0 is 5 (or broken higher)
						{	min0 = 0;
						}
						/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
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
			
		case 6:		//min1 set		//must follow min0 set
		{
			while(!moveStep)
			{
				muxMenu(step);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
						{
						delay(debounce);
						buttLesser = digitalRead(buttLesserPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*lesser meat for min1 in either hour mode*/
						/*decrement min1, valid 0,1,2,3,4,5,6,7,8,9*/
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
						// starts at min1 = 0
						// xx;x0;00
						if(min1 > 0)	//if min1 is 1 to 9 (or broken higher)
						{	min1--;
						}
						if(min1 < 1)	//if min1 is 0 (or broken lower)
						{	min1 = 9;
						}
						/* end lesser meat*/
					}//endif lesser
				}//endif
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
						{
							delay(debounce);
							buttGreater = digitalRead(buttGreaterPin);	// waits for release. prevents multiple spam input by holding button
						}
						/*greater meat for min1 for either hour mode*/
						/*increment min1, valid 0,1,2,3,4,5,6,7,8,9*/
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
						// starts at min1 = 0
						// xx;x0;00
						if(min1 < 9)	//if min1 is 0 to 8 (or broken lower)
						{	min1++;
						}
						if(min1 > 8)	//if min1 is 9 (or broken higher)
						{	min1 = 0;
						}
						/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
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
			{	step = amPmSet;
			}
			else
			{	step = brightCase;
			}
		}
			break;
			
		case 7:		//am/pm for 12-hr
		{	//am is amPm=0, pm is amPm=1
				//i'm thinking 9 for am and 6 for pm? like where the sun goes. up for morning down for night? on opposite ends of the display
			if(!gatekeeper)
			{	amPm = 0;			//forces auto am on startup even though we mess with it like ten times before this
			}
			while(!moveStep)
			{
				muxMenuDay();
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
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
				else if(buttGreater){
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
						if(buttGreater){
							while(buttGreater)
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
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
						{
							delay(debounce);
							buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
						}
						moveStep = 1;	//exits the while
					}//endif menu
				}//endelseif menu
			}//endwhile !moveStep
			moveStep = 0;
			step = brightCase;
		}
			break;
			
		case 8:		//brightness set. maybe could do both pwm and brute force simultaneously so one doesn't break if we change our minds. backwards compatibility!
		{
			int brightDispTop = 16; //just for occasional convenient math
			int brightDispBottom = 2;
			int brightDispStep = 2;
			float numerator = (brightDispTop-brightDispBotttom)*(brightness - brightness_min);
			float denom = (brightness_max-brightness_min);
			int brightDisp = round(numerator/denom + brightDispBotttom);	//does arduino do rounding?
			
			int oddCheck = brightDisp%2;
			if(evenCheck)		//this isn't needed if top=16,bottom=2,max=128,min=8 but i'll leave it just in case those change
			{	brightDisp++;
			}
			if(brightDisp > brightDispTop)
			{	brightDisp = brightDispTop;
			}
			if(brightDisp < brightDispBottom)
			{	brightDisp = brightDispBottom;
			}
			
			while(!moveStep)
			{
				int mux_array[] = {0,brightDisp/10,brightDisp%10,0,0,0};
				muxMenuBright(mux_array);
				buttLesser = digitalRead(buttLesserPin);
				buttGreater = digitalRead(buttGreaterPin);
				buttMenu = digitalRead(buttMenuPin);
				if(buttLesser)
				{
					delay(debounce);	
					buttLesser = digitalRead(buttLesserPin);
					if(buttLesser){
						while(buttLesser)
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
				else if(buttGreater)
				{
					delay(debounce);	
					buttGreater = digitalRead(buttGreaterPin);
					if(buttGreater){
						while(buttGreater)
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
							float bigup = ((brightDisp-brightDispBottom)*denom)
							float bigdown = (brightDispTop-brightDispBottom)+brightness_min
							brightness = round(bigup/bigdown);		//does arduino do rounding?
							if(brightness > brightness_max)
							{	brightness = brightness_max;
							}
						}	//endif PWM mode
							/*end greater meat*/
					}
				}//endelseif greater
				else if(buttMenu)
				{
					delay(debounce);	
					buttMenu = digitalRead(buttMenuPin);
					if(buttMenu)
					{
						while(buttMenu)
						{
							delay(debounce);
							buttMenu = digitalRead(buttMenuPin);	// waits for release. prevents multiple spam input by holding button
						}
						moveStep = 1;	//exits the while
					}//endif menu
				}//endelseif menu
			}//endwhile !moveStep
			moveStep = 0;			
			step = exitCase;
		}
			break;
		
		case 9:		// exit menu routine
		{
			gatekeeper = 1;		//we've succesfully set the clock for at least the first time
			hold = 0;
		}
		break;
		}
	
	//can add more cases as needed to expand menu. just be sure to change switch numbers is the muxes below

	}//endwhile hold
	
}//end routine menuGo
	
	
/****** specialty muxes ******/
void muxMenu(int step)
{		//these are hardcoded. change them if you change the menu switchcase numbering
	
	//if in 24 mode, only muxes hr1 and hr0 digits with 24 and kills it on the lower two,
	//if in 12 mode, only muxes min1 and min0 digits with 12 and kills it on the upper two
	
	
	int mux_array[] = {0,0,0,0,0,0};	//array to be used to display stuff in menu mode. included the last 0s in case a shorter array breaks it
	
	if(step == 0)
	{	//hourType set
		if(hourType ==0){	//if 12-hour mode, only muxes middle digits with 12 and kills the other digits
		
		mux_array[2] = 1;
		mux_array[3] = 2;
		muxMenuMiddle(mux_array);
			/*for(int b = 0; b < clockDigitSize; b++)
			{	digitalWrite(digit[b],LOW); 			//forces all digits to be off before we turn only some of them on wnkbr
			}
			
			for(int w = 2; w < 4; w++) 									//only sets and illuminates the middle two digits
			{						// THIS SETS THE DRIVER OUTPUTS		
				int x = mux_array[w]%10; 
				for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
				{	digitalWrite(driverArray[i],output_bin[x][i]);
				}
				
				
				if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
				{	analogWrite(digit[w], brightness); //is that how you pwm? 
				}
				
				if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
				{
					delay(offTime/2);//letting the driver settle. completing the second half of off time
					digitState = HIGH;
					if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
					{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
					}
					digitalWrite(digit[w],digitState);  
					delay(onTime);
					digitState = LOW;
					digitalWrite(digit[w],digitState);	
					delay(offTime/2);// here is the first half of off time
					}//endif
				
			}//endfor*/
		}//endif hourType 0
		
		else if(hourType ==1){	//if 24-hour mode, only muxes front digits with 24 and kills the other digits
		
		mux_array[0] = 2;
		mux_array[1] = 4;
		muxMenuFront(mux_array);
		
			/*for(int b = 0; b < clockDigitSize; b++)
			{	digitalWrite(digit[b],LOW); 		//forces all digits to be off before we turn only some of them on wnkbr
			}
			
			for(int w = 0; w < 2; w++) 									//only sets and illuminates the first two digits
			{						// THIS SETS THE DRIVER OUTPUTS		
				int x = mux_array[w]%10; 
				for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
				{	digitalWrite(driverArray[i],output_bin[x][i]);
				}
				
				
				if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
				{	analogWrite(digit[w], brightness); //is that how you pwm? 
				}
				
				if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
				{
					delay(offTime/2);//letting the driver settle. completing the second half of off time
					digitState = HIGH;
					if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
					{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
					}
					digitalWrite(digit[w],digitState);  
					delay(onTime);
					digitState = LOW;
					digitalWrite(digit[w],digitState);	
					delay(offTime/2);// here is the first half of off time
					}//endif
				
			}//endfor*/
		}//endif hourType 1
	}//endif step == 0
	
	if(0 < step < 5)		
	{	//hours set. only muxes front digits with hours and kills the other digits
		mux_array[0] = hr0;
		mux_array[1] = hr1;
		muxMenuFront(mux_array);
	}
	
	if(step == 5 || step == 6)
	{	//minutes set. only muxes middle digits with minutes and kills the other digits
		mux_array[2] = min0;
		mux_array[3] = min1;
		muxMenuMiddle(mux_array);
	}
}//end routine muxMenu

void muxMenuFront(int mux_array[])
{		//this is for setting the hours or showing 24hr mode. sometimes uses global hr0 and hr1
	
	/**
	this is for setting the clock hours or 24hr option
	have this only mux hr1 and hr0 and kill it on the lower ones
	**/
	
	
	for(int b = 0; b < clockDigitSize; b++)
	{	digitalWrite(digit[b],LOW); 		//forces all digits to be off before we turn only some of them on wnkbr
	}
	
	for(int w = 0; w < 2; w++) 									//only sets and illuminates the first two digits
	{						// THIS SETS THE DRIVER OUTPUTS		
		int x = mux_array[w]%10; 
		for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
		{	digitalWrite(driverArray[i],output_bin[x][i]);
		}
		
		
		if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
		{	analogWrite(digit[w], brightness); //is that how you pwm? 
		}
		
		if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
		{
			delay(offTime/2);//letting the driver settle. completing the second half of off time
			digitState = HIGH;
			if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
			{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
			}
			digitalWrite(digit[w],digitState);  
			delay(onTime);
			digitState = LOW;
			digitalWrite(digit[w],digitState);	
			delay(offTime/2);// here is the first half of off time
			}//endif
		
	}//endfor

	
}//end routine muxMenuFront

void muxMenuMiddle(int mux_array[])
{		//this is for setting the minutes and 12hr option. uses globals min0 and min1
	
	/**
	this is for setting the clock minutes and showing 12hr format option
	only muxes min1 and min0 and kills it on the lower and higher ones
	**/
	
	for(int b = 0; b < clockDigitSize; b++)
		{	digitalWrite(digit[b],LOW); 			//forces all digits to be off before we turn only some of them on wnkbr
		}
	
	for(int w = 2; w < 4; w++) 									//only sets and illuminates the middle two digits
	{						// THIS SETS THE DRIVER OUTPUTS		
		int x = mux_array[w]%10; 
		for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
		{	digitalWrite(driverArray[i],output_bin[x][i]);
		}
		
		
		if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
		{	analogWrite(digit[w], brightness); //is that how you pwm? 
		}
		
		if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
		{
			delay(offTime/2);//letting the driver settle. completing the second half of off time
			digitState = HIGH;
			if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
			{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
			}
			digitalWrite(digit[w],digitState);  
			delay(onTime);
			digitState = LOW;
			digitalWrite(digit[w],digitState);	
			delay(offTime/2);// here is the first half of off time
			}//endif
		
	}//endfor
	
	
}//end routine muxMenuMiddle

void muxMenuDay()
{
	int mux_array[] = {0,0,0,0,0,0};
	
	/**
	this is for am/pm selection. 
	goes on far opposites with 9 for am and 6 for pm.
		-- think of it with the loop being like the position of the sun. in the am, sun is high in the sky. in the pm, sun is down
	have this mux hr0 and min1 and kill it on the others.
	**/
	//am is amPm=0, pm is amPm=1
	
	if(muxMenuDayOption == 0)	//if 9 in hr0 digit and 6 in min1 digit
	{
		mux_array[0] = 9;
		mux_array[3] = 6;
		
		for(int b = 0; b < clockDigitSize; b++)
		{	digitalWrite(digit[b],LOW); 			//forces all digits to be off before we turn only some of them on wnkbr
		}
	
	for(int w = 0; w < 4; w+=3) 	//does += work in ifs?			//only sets and illuminates the hr0 and min1 digits
	{						// THIS SETS THE DRIVER OUTPUTS		
		int x = mux_array[w]%10; 
		for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
		{	digitalWrite(driverArray[i],output_bin[x][i]);
		}
		
		
		if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
		{	analogWrite(digit[w], brightness); //is that how you pwm? 
		}
		
		if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
		{
			delay(offTime/2);//letting the driver settle. completing the second half of off time
			digitState = HIGH;
			if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
			{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
			}
			digitalWrite(digit[w],digitState);  
			delay(onTime);
			digitState = LOW;
			digitalWrite(digit[w],digitState);	
			delay(offTime/2);// here is the first half of off time
			}//endif
		
	}//endfor
	}
	else	//if using the 0s in front for am and in middle for pm
	{
		if(amPm == 0)	//if am
		{
			muxMenuFront(mux_array);
		}
		else			//if pm or broken 
		{
			muxMenuMiddle(mux_array);
		}
	}
	
}// end routine muxMenuDay

void muxMenuBright(int mux_array[])
{
	/**
	this is for brightness and should be clearly different than setting the times, hence the hour and minute split
	have this mux hr0 and min1 and kill it on the others.
	**/
	
	for(int b = 0; b < clockDigitSize; b++)
	{	digitalWrite(digit[b],LOW); 			//forces all digits to be off before we turn only some of them on wnkbr
	}
	
	for(int w = 1; w < 3; w++) 									//only sets and illuminates the middle two digits
	{						// THIS SETS THE DRIVER OUTPUTS		
		int x = mux_array[w]%10; 
		for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
		{	digitalWrite(driverArray[i],output_bin[x][i]);
		}
		
		
		if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
		{	analogWrite(digit[w], brightness); //is that how you pwm? 
		}
		
		if(bruteForce) // i didn't do an else so we could run them both together. i wonder what would happen
		{
			delay(offTime/2);//letting the driver settle. completing the second half of off time
			digitState = HIGH;
			if(w == 0 && hourType == 0 && !mux_array[0])	// if toggling on MSB digit and is 12-hr and hr1 is 00	//BRIAN leave this as mux_array instead of timecard so that it will still display the fancy roll
			{	digitState = LOW;	//this totally freaking works i'm a freaking genius like seriously holy crap this is beautiful
			}
			digitalWrite(digit[w],digitState);  
			delay(onTime);
			digitState = LOW;
			digitalWrite(digit[w],digitState);	
			delay(offTime/2);// here is the first half of off time
			}//endif
		
	}//endfor
	
}// end routine muxMenuBright

/*****************************/

