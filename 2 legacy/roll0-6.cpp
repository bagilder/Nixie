
/*
draft six swinging relatively sightedly in the dark to do hour change animations
bgilder
4jan2017 
*/


/******* rev log ********
6
needed to change the final timecard exit setting for different type of counter.
also, why is the set looper commented completely out? was that just for erase testing? i'll have to run it to be sure

5
don't remember lol

4
got the erase roll to work. hooray. still need to figure out the set roll
changed mux_hax() calls (which were used to display the roll routine) to regular mux() calls, but passes roll_buffer to it. that is, now backwards capatible to mux()
deleted the explicitly defined non-loops. go back to a previous rev if you want to try those
deleted the example table because it was a monster. ditto ditto ^
incorporated hr1_old and hr0_old so it doesn't display the hour having changed before we change the numbers.
i was considering just putting this into the main file, but i don't want to do that until it works. documentation is a handy thing
roll_reset_time now sets sec0 to whatever we determine is the total length of our roll routine.
-- originally had it set sec0 to variable/2 with the idea that if we started MSB->LSB, we could start it with two or three seconds before the hour change so 5959 wouldn't hang out unchanging until the roll got to it.
---- needed to be even number because int
-- now sets to full variable value
commented out the driver set part of the t-loop in the erase looper and it still works totally fine outsourcing all that to the mux routine

3
		erased the first attempt at looping. can still be found in rev 2 file
		added millis checker and math compared to numeral_change_time to initiate loop stuff. so now it should actually, like, go.
		changed driver[ to driverArray[
			NOTE: arrays cannot be accessed by array[x:y] so you'll have to do the zeros thing explicitly. also, can't do array1[] = array2[] so you'll have to loop that piecewise
! ! ! !  this currently doesn't work. if we set the lowest 4 digits to 5959, we can use this routine to test the extent of hour rollovers round the clock, so to speak
-- since it'll just count from 59 to the new hour every two seconds. which itself is kinda neat, but not what we're going for.
		will need to include its own version of mux, ie copypasta, since we are using _roll instead of pure hr vars. so passing to mux_hax will give the "correct" hour
		-- maybe we could have an independent mux function or something, that will do a special type mux for our purposes here

2 
		so i had a neat little nested thing to automate the change, but it made no sense to me upon review so i started over.
		-- i think we should try to run the explicit version too. since apparently that will run exponentially faster.
		----or something.
(i think) i rebuilt the erase looper. i have tried to build the set looper but going from left to right and not scrolling things after we set them is starting to freak my brain out.


1a.5
this is currently msb -> lsb rolling erase to zeroes and msb -> lsb set. 
		there is a skeleton below for lsb->msb set that can be fleshed out and adapted for erasure too, i'm sure	//2- but not if i started over because the original nested stuff was jank
		also is driver[] already a variable? because i use it. maybe i mean driverPin[]? idk. check.

~~~~~~~~~~~~~~~~~~~~~~~~~
future notes:
fix the set looper reboot!
can have a programmer-defined thing at the start to toggle between msb->lsb or vice versa. 
-- just do an if and then copy pasta with diff variable names
---- copypasta never seems to be a good idea. except if it is going opposite directions, it might sort of have to be copypasta
		NOTE: add hr_old variable to the counter routine or something!!	(see below)
*************************/



void fancy_hour_roll()		//this is one animation among (hopefully) several	//MUST be less than 9 seconds total
{
	
	
	/***** programmer defined *****************
	/**					
	/* float numeral_change_time=20; //in ms
	/* int roll_reset_time = 6; //in sec. sets sec0 at the end of routine	
	/*					/*
	/******************************************/
		//we will need to tweak the timing. like what looks good
	
	
						timecard[0]={hr1};	//i don't end up using timecard explicitly, do i? can something be eliminated and have this instead?
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
	
	
	
	
	int hr1_roll = hr1_old;
	int hr0_roll = hr0_old;
	min1=5; //should i do this? am i allowed? does it matter? //this can be used to check proper hour rollover, since it sets everything to 5959 and keep rolling over etc whatever
	min0=9;
	sec1=5;
	sec0=9;
	int roll_buffer[] = {hr1_roll, hr0_roll, min1, min0, sec1, sec0}; //maybe for iterating hmmmm?
	int magicNumber[] = {10+hr0_roll,15,19,15,19,19};// change the 19's to 9 if you want to pick up the next digit without going a full rotation
	
	
	//hey dude include a single full roll for the hr1 digit so it won't get caught on hr0 the first time	//that sounds like magic number stuff. i'll worry about that later. you're welcome, future me!
	
	for(int y=0; y < clockDigitSize; y++)	//this is an ** ERASE LOOPER ** (i think)	//the set looper should basically be copypasta but with real hr1 and hr0
	{
		while(roll_buffer[y] < magicNumber[y])	//whatever. sort out magic number later	//for set, this can just sit at 10, right? or maybe not for hr1 and hr0 hm. maybe at the end of this, we reset magic number to h1,hr0,10,10,10,10
		{
			mux(roll_buffer);
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
	
	
	/*** end ERASE LOOPER reboot ***/
	/*
	magicNumber[0] = {hr1_roll+10};
	magicNumber[1] = {hr0_roll+20};
	magicNumber[2] = {30};
	magicNumber[3] = {40};
	magicNumber[4] = {50};
	magicNumber[5] = {60}; 	// will this allow the digits to stop rolling hr1 and hr0 once they hit their mark? there is a mod 10 in some stuff down there. i'm counting to 60 etc without resetting since i can just mod it back to a single digit. idk if that will work but i'm gonna try it.
	*/
	/*** now the SET LOOPER ***/	//which is basically the same thing. but uses hrr1 and hr0. oh crap we need to not have it roll the leading digits now. crap.
	/*
	for(int y=0; y < clockDigitSize; y++)	//this is a ** SET LOOPER ** (i think)	
	{
		while(roll_buffer[y] < magicNumber[y])	//for set, this can just sit at 10, right? or maybe not for hr1 and hr0 hm. maybe at the end of this, we reset magic number to h1,hr0,10,10,10,10
		{
			mux(roll_buffer);
			currentSec = millis();
			if((currentSec - previousSec >= numeral_change_time) || (currentSec - previousSec < 0))
			{
				previousSec = currentSec;
				for(int t=clockDigitSize; t>=0; t--)	//this sets as many positions in roll_buffer as that particular step should allow (wrt which digit you've gotten to)
				{
					int digitOffset = clockDigitSize-t;
					int n = (roll_buffer[digitOffset]%10);	//i put the mod back thinking i could have rollbuffer go {hr1, 10+hr0, 0,0,0,0} or something but it won't reset hr0 to 0, right? my thought is that hr0 will get set immediately as hr1 is set if we have the buffer be {hr1,hr0, 0,0,0,0} right? it'll jump to hr0 instead of sticking there
					for(int h=0;h<4;h++)
							digitalWrite(driverArray[h],output_bin[n][h]);
					//digit[digitOffset] = (roll_buffer[digitOffset]%10); 
					roll_buffer[digitOffset]++;	
	
				}//for t
	
			}//if numeral
	
		}//while roll
	//	roll_buffer[] = {hr1,hr0,0,0,0,0};	//this needs to happen somewhere but the second digit needs to scroll an extra time	//WAIT WHAT IF WE DON'T NEED TO RESET THE BUFFER it'll just stick to that digit and keep rolling the Rest of the digits from there! so we can have magic number be {hr1, hr0+10, 20,30,40,50,60} and the conditions can be magicNumber%10 !!! or something
	
	}//for y
	*/
	/*** end SET LOOPER reboot ***/
	
	
	/* notes about this set looper:
	i'm having a hard time figuring out how to get the subsequent digits to roll but not influence the preceding digits.
	i think clockDigitSize-t will take care of that... but what about resetting the buffer to 0 at the end of the cycle?
	
	wait can i just make it count to 10 instead of 9 and then just mod 10 everything?
	
	yeah but what does 'everything' mean????? you are right on the edge of a good idea but it isn't quite materializing.
	
	at the beginning of the loop i mention sitting magic number at 10 but that isn't a good idea for some reason.
	
	//digit[] has to come into play somehow.
	*/
	
	
	
	
	
	
	//}//BIG
	
	
	
	timecard[0]={hr1}; //i don't end up using timecard explicitly, do i? can something be eliminated and have this instead?
	timecard[1]={hr0};
	min1={0};
	min0={0};
	sec1={0};
	if(legacyCounter)
	{	sec0={roll_reset_time};
	}
	else
	{
		clockDivide();
	}
	
	
	
	
	/*
	for(int j = 0; j<6;j++)
	timecard[j] = roll_buffer[j]; //maybe?  //when returning, need to make sure that sec0 is whatever offset it should be to be back on track like normal
	*/
	
	return ;
}//end fxn








/*
void mux_hax()		//this is a mux_hax function
{

unsigned long entranceTime = millis(); //don't currently have use for this but it might be handy eventually at some point maybe idk

for(int w = 0; w < clockDigitSize; w++) 
{		

				// THIS SETS THE DRIVER OUTPUTS		
int x = roll_buffer[w]; 
for(int i = 0; i < 4; i++) 	//there will always be 4 driver pins
	digitalWrite(driverArray[i],output_bin[x][i]);


if(automatedPWM) 	//if using analogwrite, will changing on the rising edge cause problems?
	analogWrite(digit[w], brightness); //is that how you pwm? 
//maybe we could give it the normal duty cycle and then an Extra wait time after that to set the driver again
//physical brightness will be supes reduced bc pwm (suggests >50% duty?). is inrush current a thing? we should scope it. if it isn't maybe we could current push these tubes. if it is, we might explode them
//because overdriving these may (or may not) be a good idea. o/d with current or voltage? //i'd say current bC voltage just made sure they turned on. but maybe turning on fully is a ~relative~ brightness thing too


if(bruteForce) //i didn't do an else so we could run them both together. i wonder what would happen
{
	delay(offTime/2);//letting the driver settle. completing the second half of off time
	digitState = HIGH;
	digitalWrite(digit[w],digitState);	
	delay(onTime);
	
	digitState = LOW;
	digitalWrite(digit[w],digitState);	
	delay(offTime/2);//here is the first half of off time

}//endif


}//endfor



} //end fxn
*/

