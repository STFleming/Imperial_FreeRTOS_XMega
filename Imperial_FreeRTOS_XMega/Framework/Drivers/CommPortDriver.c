/*
 * CommPortDriver.c
 *
 * Created: 02/08/2011 13:46:25
 *  Author: sf306
 */ 

/* To connect the board to hyper terminal for comm port testing use the following settings
   baud rate: 115200.
   Parity: None.
   Stop bits: 1bits.
   Data bits: 8 bits.
   Flow Control: None.
     */

//Include files----------------------------------------
#include "CommPortDriver.h"

//Other driver includes
#include "LCDDriver.h"
#include "Keypad.h"
#include "DACDriver.h" //For debugging.

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

//Define driver task stack size.
#define commSTACK_SIZE	( ( unsigned short ) 256)

//-----------------------------------------------
//		Global Variables.
//-----------------------------------------------
char recieved_char = '/0';
char *transmitionString;

//Prototype for driver task implementation code.
static void vCommTask(void *pvParameters);


void vStartCommPort(void)
{
	//Setup the atxmega ports
	PORTE.DIR      = 0xB9;  //  TX, RX
    PORTE.PIN3CTRL = 0x40;  // Invert TX output
	
	// Initialize USART - We are using USARTE0
    USARTE0.BAUDCTRLA = 0x17;   // BSCALE = -6, BSEL = 1047
    USARTE0.BAUDCTRLB = 0xA4;   // ==> 115211 bps (~115.2kbps)
    USARTE0.CTRLC = 0x03;       // Async, No Parity, 1 stop bit, 8 data bits
    USARTE0.CTRLB = 0x18;       // Enable RX and TX
	
	//Start the driver task for the Comm Port.
	xTaskCreate(vCommTask, (signed char*) "COMM", commSTACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
}

static void vCommTask(void *pvParameters)
{ //This function implements the CommTask
	int tempKeyPress = 0;
	for(;;)
	{	
		if ( (USARTE0.STATUS & (1 << 7)) == 128) //Check the RXCIF flag to see if any data is ready to be read.
		{
			recieved_char = USARTE0.DATA;
		}

		if ((USARTE0.STATUS & (1 << 5)) == 32) //There is room to transmit another character.Checking the DREIF flag
		{
			//USARTE0.DATA = 'P'; //Keep transmitting P over and over again for debugging purposes.
			if(GetLastKeyPressed() != tempKeyPress)
			{
				switch(GetLastKeyPressed())
				{
					case 1 :
							USARTE0.DATA = '1';
							break;
					case 2 :
							USARTE0.DATA = '2';
							break;
					case 3 :
							USARTE0.DATA = '3';
							break;
					case 4 :
							USARTE0.DATA = '4';
							break;
					case 13 :
							USARTE0.DATA = 'O'; 
							break;
					default :
							USARTE0.DATA = 'P';     
				}
			}			
		  
		 
		}
		
		tempKeyPress = GetLastKeyPressed();		
		vPrintChar(0,12,recieved_char);
		
		vTaskDelay(10);
	}
}

void SendCommString(char* input)
{
	transmitionString = input;
}