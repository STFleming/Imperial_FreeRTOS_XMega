/*
 * LCDDriver.c
 *
 * Created: 22/07/2011 15:36:09
 *  Author: sf306
 */ 
//Include the header file:
#include "LCDDriver.h"

//General Header files for string operations
#include <string.h>
#include <stdlib.h>	// for itoa() call for print number.

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

//LCD Include file
#include "sed1335.h"

//TODO
//----
//Write clear screen and delete position function.
//Add mutex locks around arrays.


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
		for(i=0; i<=15; i++) //Cycles through the entire 2D screen arrays checking for differences.
		{
			for(j=0;j<=15;j++)	//If a difference is found then we print the difference to the screen.
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

//Void vPrintString is used to place an entire string in the new_screen array
//This array is then compared with the current_screen array and differences
//are printed.It works by simply cycling through the character array adding
//each character to the new_screen array.
void vPrintString(int x, int y, char *input)
{
	int k=x; int l=y;
	for(int i=0; i<=strlen(input); i++)
	{
		new_screen[k][l] = input[i];
		
		if(k < 15) {k++;}  //This if statement is used to check if the string has reached
		else {k = 0; l++;} //the end of the current line. If this is the case keep printing the 
						   //string on the next line.
	}
	
}

void vPrintNumber(int x, int y, int number)
{
	char *strNumber;
	itoa(number, strNumber, 10); //This function converts the number to a string.
	vPrintString(x,y,strNumber); //We then use the already implemented PrintString function
								 //to print our new string.
}

