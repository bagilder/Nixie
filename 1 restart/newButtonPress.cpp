
/* 
newButtonPress.cpp
brian gilder did a thing 23dec2016
this will hopefully allow us to press buttons on interrupt for our clock
*/

/*
* the uno can only use pins 2&3 as external interrupt pins. so maybe we could just have a single button
act as like the select button and have the user hold it down for two or three seconds before listening for
the other buttons. since we can't attach all three buttons to the interrupt pins. we can have the select button
interrupt the unset blink loop and then we can listen for the other buttons using normal digital read or whatever.
*be sure to make variables passed between interrupt and non-interrupt things volatile or dynamic or whatever.
*will we be able to interrupt on select button even during timekeeping? we might need to ditch time.h if it uses
millis() since i think only microseconds() works inside an interrupt loop. i don't remember.
*/

/**** variables ****/
const byte interruptPin = 2;
volatile byte initialButtonPress = 0;
/******setup*******/

attachInterrupt(digitalPinToInterrupt(interruptPin,buttonWake, RISING); // to button1 press or whatever. 


/****end setup****/


/******loop*********/


/***** end loop ****/

void buttonWake()
{
	initialButtonPress = 1;
	delayMicroseconds(1000);	//delayMicroseconds still works inside an interrupt function, so can debounce i hope?
}

