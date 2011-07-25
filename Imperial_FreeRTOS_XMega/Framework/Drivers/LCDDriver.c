/*
 * LCDDriver.c
 *
 * Created: 22/07/2011 15:36:09
 *  Author: sf306
 */ 
//Include the header file:
#include "LCDDriver.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

//LCD Include file
#include "sed1335.h"


static volatile char new_screen[16][16] = {{0}}; //Set the screen arrays initially blank.
static volatile char current_screen[16][16] = {{0}};

void vStartLCD(void)
{
	//Starts the lcd driver task which is used to compare the two arrays at a regular interval and update.
	xTaskCreate( vLCDTask, ( signed char * ) "LCDTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
}

static void vLCDTask( void *pvParameters )
{
	//This task cycles through both screen arrays and checks for differences
	//For every difference found it updates the screen using print commands
	//After that it updates the current screen array. 
	int i, j;
	
	for(;;)
	{
		for(i=0; i<=16; i++)
		{
			for(j=0;j<=16;j++)
			{
				if(new_screen[i][j] != current_screen[i][j]) //We are going to require a mutex lock on the arrays.
				{
					lcd_goto(i,j); put_char(new_screen[i][j]);
					current_screen[i][j] = new_screen[i][j];
				}
			}
		}
		vTaskDelay(100);
	}
}

void vPrintChar(int x, int y, char input)
{
	//Add the character to the new_screen array.
	new_screen[x][y] = input;
}