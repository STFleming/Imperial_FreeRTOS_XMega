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

//LCD Include file
#include "sed1335.h"

//---------------Variables-----------------
extern int key;
//-----------------------------------------

//--------------Prototypes-----------------
static void vKeypadTask( void *pvParameters );
//-----------------------------------------

void vStartKeypadTask(void)
{
	//This function starts the keypad task with a configurable priority, should generally be given
	//a low priority as it will infrequently check the keypad.
	xTaskCreate( vKeypadTask, ( signed char * ) "Keypad", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
}

static void vKeypadTask( void *pvParameters )
{   uint8_t In,j=0;
    uint8_t Row, Col=0, i;
	
for(;;)
{	

	PORTD.DIR = 0xF0;
	for(i=3; i>0; i--) {
		vTaskDelay(1);
		In = PORTD.IN;            	// Read PORTD
		if(j!=In) { j=In; i++; }
	}
//---------------Process Rows-----------------------------------
	if	    (In==(0x07)) Row=3;
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
	     if	(In==(0xB0)) Col=2;
	else if	(In==(0xD0)) Col=1;
	else if	(In==(0xE0)) Col=0;
	else {Col=4;}						// No keys or more than 1 key pressed

//---------------------------------------------------------------
	
	key = Row*3+Col+1; 
	lcd_goto(0,2); printN(key);	
	vTaskDelay(10); //Delay the task for 10ms
}
	
}