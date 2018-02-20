

/**
the 12th god help me draft of trying to kludge together a nixie clock program (for both 4 & 6 digits)
b gilder
22aug2015 424am
**/

/******* rev log ************************************************
12
all about 24/12 hr conversions. added serial output for troubleshooting purposes.   [[see sketch_aug21b. actually ran on chip]]
added stuff to settings to fix initial condition contradictions
added bunch of stuff to change hour type because it was pretty jank
if input for 24-hr is greater than 24, forcing down to 00: ALSO ditto for 12-hr down to 01: but also if less than hr0_min

11
in changehourtype fxn, used the += and -= operators. seems like they work in arduino
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
/**/							//
/**/	const int clockDigitSize = 6;   			//4 or 6
//	const long flicker_interval = 20; 			//measured in millisec, duration each digit is on. we will need to experiment with this 	//not currently used but maybe for built in pwm stuff?
/**/	const int brightness_max = 128; 		//50% duty cycle. would it be advisable to allow going over that?
/**/	const int brightness_min = 10; 			//this is totally a made up number. we will have to experiment with this as well.
/**/	int brightness = 128; 				//0-255, controls built-in pwm scheme. could be what’s changed by user brightness selection. initially half bright
/**/	const int automatedPWM = 0;			//enable if letting arduino flicker lamps for us (what if you enable both? it might still be legal..  interesting)
/**/	const int bruteForce = 1;				//enable if using custom time delays to flicker lamps
/**/	float onTime = 3.5;				//in ms, for brute force. will need to experiment 
/**/	float offTime = .1;				//ditto	//protip can't be constants if we set them again below lulz
/**/	int hourType = 0;				// 0 for 12-hr, 1 for 24-hr	//toggled by a physical throw switch? therefore variable
/**/ 	const float numeral_change_time = 90; 		//in ms, for fancy roll
/**/	const int roll_reset_time = 6;			//in seconds, at the moment must be even number, total amount of time fancy roll animation takes
/**/							//
/*************************************************************/



/** constants **/
const unsigned long sec_interval = 1000;	//1 second
const int digitPin1 = 11;   	// MSB, hr1 	//this is what switches on/off the digit muxes
const int digitPin2 = 10;		//conveniently, there are exactly 6 digital pwm pins
const int digitPin3 = 9;
const int digitPin4 = 6; 		//4-digit LSB, min0
const int digitPin5 = 5;
const int digitPin6 = 3;	 	// 6-digit LSB, sec0
const int digit[] = {digitPin1,digitPin2,digitPin3,digitPin4,digitPin5,digitPin6};	//all 6 included, potentially not utilized. can cannibalize for pins in 4-digit if needed
const int secDotPin = 13; 	// pin controlling mux for sec-blink dot 
const int switchPin = A5;	 //!!!! !!!! !!!! or whatever it actually should be // don’t currently have switch
const int driverPin1 = 8; 	//these might be changed so that the driver and buffer and stuff can just go straight across	//have they been already?	//not yet
const int driverPin2 = 7;
const int driverPin3 = 4;	
const int driverPin4 = 2;		//LSB
const int driverArray[] = {driverPin1,driverPin2,driverPin3,driverPin4}; 
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
	};

/** variables **/
int hr1_max = 2;  	// change to 1 if 12-hr mode
int hr0_max = 3;  	// I think this can be 2 or 3 without it messing up normal clock progression.. I think I took care of that down below (I’m basically a genius)
int hr0_min = 0;  	// will automatically change to 1 if 12-hr mode
int hr1 = 1; 		//hour holders, 1 is MSB,   /**/ initial setting is 12:34:56 /**/
int hr0 = 2; 		
int min1 = 3; 		//minute holders
int min0 = 4; 
int sec1 = 5; 		//sec holders
int sec0 = 6; 
int timecard[] = {hr1,hr0,min1,min0,sec1,sec0};		//pointers would make this easier. sigh. but then pointers. yuckers.
int hr1_old = hr1;
int hr0_old = hr0;
int secDotState = LOW;		// used to set the sec-blink dot 
int digitState = LOW; 		//used to set the currently displaying digit
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
  }
 Serial.begin(9600);
  if(hourType == 0)
  {
	hr1_max = 1;  	// change to 1 if 12-hr mode
	hr0_max = 2;  	// I think this can be 2 or 4 without it messing up normal clock progression.. I think I took care of that down below (I’m basically a genius)
	hr0_min = 1; 
  	Serial.println("setup shenanigans 1");	//for debug
  }

  if(hr1 > hr1_max || hr0 > hr0_max || hr0 < hr0_min)
   {
    	if(hr0<hr0_min)
      	{amPm = 0;
      	}
    	hr1=0;
    	hr0=1;
    	Serial.println("setup shenanigans 2");	//for debug
    }
  
if(hourType == 1 && hr1 > 0 && hr0 > 1 )           
  {
	amPm = 1;
	Serial.println("forcing pm");
	changeHourType();
	Serial.println("setup shenanigans 3");	//for debug
  }
bool hourOverflow = hr1>=2 || (hr1>=2 && hr0 >=4);	//does this need to be 'boolean' instead of 'bool'?
if(hourOverflow)
  { 
    	Serial.println("input hour overflow");	//for debug
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
	offTime = maxTime-onTime;	//because i'm lazy	//don’t currently have a maxTime var. inb4 it fails cuz it says we don’t have a maxTime var. hi future ppl
  }//endelseif

if (brightness > brightness_max)	//to keep max at max (we might decide to go above 50% duty) 
  {	brightness = brightness_max;
  }

bool earlyMorning = !hr1 || (hr1 == 1 && (hr0==0 || hr0==1));	//if hr1=0 or 10,11	///does this need to be 'boolean' instead of 'bool'?
Serial.println("early morning run");	//for debug
if(hourTypePrev && earlyMorning) //if 24-hr and that thing
  {
	Serial.println("early morning accepted");
	Serial.println("forcing am");  
	amPm = 0;
  }

return ;	//does this need to be return(void); or some shit? return c; down below seems to compile
}//end settings fxn


void loop()   /*what if we made this a dependent function and had the mux be main? <-this might be a good idea <-maybe not. need mux during initialization*/
{

if(!gatekeeper)
  {
	//initialization routine here	//if its own function, bring the brightness cap thing along
	settings();
	gatekeeper = 1;
  }

			 	//metronome on the seconds
unsigned long currentSec=millis();
if((currentSec - previousSec >= sec_interval) || (currentSec - previousSec < 0)) 	//this would be so much easier if we used the 555. probs less accurate tho
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

hourType = digitalRead(switchPin);

  if(hourType == 0)	//if 12-hr, at this very second
  {
  	hr1_max = 1;   
  	hr0_max = 2;    
  	hr0_min = 1; 
    	Serial.println("hourType ==0");
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
  	Serial.println("else hourType ==1");
  
  	if(!amPm && hr1 == 1 && hr0 == 2)	// if am, and currently 12:xx
  	{
    		hr1=0;				// then let's correct that biznazz
    		hr0=0;
    		Serial.println("forcing 00:");
    	}
  }



if(hourTypePrev != hourType)	// if the switch has been flipped
{
	Serial.println("switch has been flipped");
	if(hr0 == 0 && hr1 == 0 && hourTypePrev)		//if 00:xx and formerly 24-hr (therefore currently 12-hr)
	{
		hr1 = 1;
		hr0 = 2;
		amPm = 0;	//deffo am, in that case
	}
	
  	else if(hr1 == 1 && hr0 == 2 && !hourTypePrev && !amPm)
  	{
  	           	Serial.println("just recently 12: am, midnight");
 		hr1=0;
    		hr0=0;
    	}
  	else
  	{
  	  	Serial.println("the switch doesn't matter");
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
					hr0 -= 8;	//does arduino understand these operators?
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




/******* a mux ***********/


//the way i see it we could do analogWrite or we could do on/off times
//this is its own function so that fancy roll can also use it


void mux(int mux_array[])		//this is a mux function
{

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
