/*
 * Keypad.c
 *
 * Created: 21/07/2011 15:40:54
 *  Author: sf306
 */ 

#include "Keypad.h"
#include <stdlib.h>

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

//LCD & DAC Include files for debugging
#include "LCDDriver.h"
#include "DACDriver.h"

//Define the stack size for the keypad driver task.
#define configKEYPAD_STACK_SIZE  99

//---------------Variables-----------------
int key = 13; //There is no key 13, if this has been returned it indicates that no key has been pressed.
//-----------------------------------------

//--------------Prototypes-----------------
static void vKeypadTask( void *pvParameters );
//-----------------------------------------

void vStartKeypadTask(void)
{
	//This function starts the keypad task with a configurable priority, should generally be given
	//a low priority as it will infrequently check the keypad.
	vKeypadPortSetup();
	xTaskCreate( vKeypadTask, ( signed char * ) "Keypad", configKEYPAD_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
}

static void vKeypadTask( void *pvParameters )
{   uint8_t In,j=0;
    uint8_t Row, Col=0, i;
for(;;)
{	
//Initially start by reading the rows from PORTD.
	PORTD.DIR = 0xF0; //Set the rows as the input.
	for(i=3; i>0; i--) { //For loop used to debounce the switch.
		vTaskDelay(1);
		In = PORTD.IN;            	// Read PORTD
		if(j!=In) { j=In; i++; }
	}
	
//---------------Process Rows-----------------------------------
	if	    (In==(0x07)) Row=3;	//Assign the values to the row.
	else if	(In==(0x0B)) Row=2;
	else if	(In==(0x0D)) Row=1;
	else if	(In==(0x0E)) Row=0;
	else {Row=4;}						// No keys or more than 1 key pressed

	//----------Rows Processed now cols----------------
	PORTD.DIR = 0x0F; //Rows as output.
	for(i=2; i>0; i--) {
		vTaskDelay(1);
		In = PORTD.IN;            	// Read PORTD
		if(j!=In) { j=In; i++; }
	}
	//-------------------------------------------------

//------------Process Cols---------------------------------------
//Assign the column value.
	     if	(In==(0xB0)) Col=2;
	else if	(In==(0xD0)) Col=1;
	else if	(In==(0xE0)) Col=0;
	else {Col=4;}						// No keys or more than 1 key pressed

//---------------------------------------------------------------
	
	if(!(Row == 4 || Col == 4)) {key = Row*3+Col+1;} //Check to make sure that no more than one key is pressed or that no key is pressed.
	vTaskDelay(10); //Delay the task for 10ms
	
	//vPrintNumber(0,0, uxTaskGetStackHighWaterMark(NULL)); //Debugging for finding stack high water mark.
}
	
}

void vKeypadPortSetup(void)
{
	PORTD.DIR      = 0xF0;  // Keypad and switches
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
    PORTD.PIN0CTRL = 0x18;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
    PORTD.OUT      = 0x00;
}

int GetLastKeyPressed(void)
{
	return key;
}