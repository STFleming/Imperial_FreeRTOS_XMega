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
	xTaskCreate( vKeypadTask, "Keypad", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
}

static void vKeypadTask( void *pvParameters )
{   uint8_t In,j=0;
    uint8_t Row, Col=0, i;
	
	//lcd_putsp(PSTR("Keypad task is running."));
	
for(;;)
{	
	In = PORTD.IN; //May need to debounce this read.
	
	switch(In) //Check the rows, and assign the row variable.
	{
		case 0x07 : Row=3;
		case 0x0B : Row=2;
		case 0x0D : Row=1;
		case 0x0E : Row=0;
		default : //No keys or more than one key pressed
					key = 0;
					return;
	}
	//----------Rows Processed now cols----------------
	PORTD.DIR = 0x0F; //Rows as output.
	In = PORTD.IN;
	PORTD.DIR = 0xF0; //Rows as inputs.	
	
		switch(In) //Check the cols, and assign the col variable.
	{
		case 0x70 : Col=3;
		case 0xB0 : Col=2;
		case 0xD0 : Col=1;
		case 0xE0 : Col=0;
		default : //No keys or more than one key pressed
					key = 0;
					return;
	}
	
	key = Row*4+Col+1;
	
	vTaskDelay(10); //Delay the task for 10ms
}
	
}