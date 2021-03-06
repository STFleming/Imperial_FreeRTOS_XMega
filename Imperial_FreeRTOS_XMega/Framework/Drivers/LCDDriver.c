/*
 * LCDDriver.c
 *
 * Created: 22/07/2011 15:36:09
 *  Author: sf306
 */ 
//Include the header file:
#include "LCDDriver.h"
#include "DACDriver.h"
#include "CommPortDriver.h"

//General Header files for string operations
#include <string.h>
#include <stdlib.h>	// for itoa() call for print number.

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//LCD Include file
#include "sed1335.h"

#define lcdSTACK_SIZE	( ( unsigned short ) 86)

//TODO
//----

//-----------------------------
//	Variables
//-----------------------------
xSemaphoreHandle xMutexLCD;
static volatile char new_screen[16][16] = {{'\0'}}; //Set the screen arrays initially blank.
static volatile char current_screen[16][16] = {{'\0'}};

//-----------------------------
//  Task Prototype
//-----------------------------
static void vLCDTask( void *pvParameters );

void vStartLCD(void)
{
	//Starts the lcd driver task which is used to compare the two arrays at a regular interval and update.
	prvLCDPortSetup(); //Performs the PORT configuration for the LCD.
	lcd_init(GRAPHTEXT);
	xMutexLCD = xSemaphoreCreateMutex();
	xTaskCreate( vLCDTask, ( signed char * ) "LCDTask", lcdSTACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
}

void prvLCDPortSetup(void)
{
		PORTF.DIR |= (0x01 << 7); //LCD WR
		PORTF.DIR |= (0x01 << 6); //LCD RD
		PORTF.DIR |= (0x01 << 1); //LCD A0
		
		PORTK.DIR      = 0xFF;  // LCD data
		PORTK.OUT      = 0x00;
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
				xSemaphoreTake(xMutexLCD, portMAX_DELAY); //Take the mutex for the new_screen array.
				if(new_screen[i][j] != current_screen[i][j]) //We are going to require a mutex lock on the new_screen array.
				{
					portENTER_CRITICAL(); //Enter critical section to make the lcd function safe.
					lcd_goto(i,j); put_char(new_screen[i][j]);
					portEXIT_CRITICAL(); //Exit critical section now that lcd function calls are completed.
					
					current_screen[i][j] = new_screen[i][j];
				}
				xSemaphoreGive(xMutexLCD); //give back the mutex for the new_screen array.
			}
		}
		vTaskDelay(10);
		//vPrintNumber(0,1, uxTaskGetStackHighWaterMark(NULL)); //Debugging for finding stack high water mark.
	}
}

void vPrintChar(int x, int y, char input)
{
	xSemaphoreTake(xMutexLCD, portMAX_DELAY); //Take the new_screen array mutex.
	//Add the character to the new_screen array.
	new_screen[x][y] = input;
	xSemaphoreGive(xMutexLCD); //Give back the new_screen array mutex.
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
		vPrintChar(k,l,input[i]);
		
		if(k < 15) {k++;}  //This if statement is used to check if the string has reached
		else {k = 0; l++;} //the end of the current line. If this is the case keep printing the 
						   //string on the next line.
	}
	
}

void vPrintNumber(int x, int y, int number)
{
	char *strNumber = NULL;
	portENTER_CRITICAL();
	itoa(number, strNumber, 10); //This function converts the number to a string.
	portEXIT_CRITICAL();
	vPrintString(x,y,strNumber); //We then use the already implemented PrintString function
								 //to print our new string.
}

void vClearScreen(void)
{
		for(int i=0; i<=15; i++) //Cycles through the entire 2D screen arrays checking for differences.
		{
			for(int j=0;j<=15;j++)	//If a difference is found then we print the difference to the screen.
			{
				vPrintChar(i,j, '\0');
			}
		}
}

void vClearPosition(int x, int y)
{
	vPrintChar(x,y,'\0');
}
