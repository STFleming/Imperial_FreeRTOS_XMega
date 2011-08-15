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

//C include files
#include <string.h>

//Other driver includes
#include "LCDDriver.h"
#include "Keypad.h"
#include "DACDriver.h" //For debugging.

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//Define driver task stack size.
#define commSTACK_SIZE	( ( unsigned short ) 97)


//-----------------------------------------------
//				Global Variables.
//-----------------------------------------------
char recieved_char = '\0';
xQueueHandle xCommPortQueue; //Handle for the queue used to buffer information to the serial port dispatcher task.


//Prototype for driver task implementation code.
static void vCommTask(void *pvParameters);

void vStartCommPort(void)
{
	const unsigned portBASE_TYPE uxBufferSize = 50;
	
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
	//Start the queue for the comm port.
	xCommPortQueue = xQueueCreate(uxBufferSize, ( unsigned portBASE_TYPE ) sizeof(char));
}

static void vCommTask(void *pvParameters)
{ //This function implements the CommTask
char tempCharacter; //Used for passing the character to the output from the buffer.

	for(;;)
	{	
		if ( (USARTE0.STATUS & (1 << 7)) == 128) //Check the RXCIF flag to see if any data is ready to be read.
		{
			recieved_char = USARTE0.DATA;
		}

		while( uxQueueMessagesWaiting( xCommPortQueue ) != 0 )
		{
			if ((USARTE0.STATUS & (1 << 5)) == 32) //There is room to transmit another character.Checking the DREIF flag
			{
				xQueueReceive(xCommPortQueue, &tempCharacter, 0);
				USARTE0.DATA = tempCharacter;	
			}
		}			
		
		vTaskDelay(10);
		//vPrintNumber(0,4, uxTaskGetStackHighWaterMark(NULL)); //Debugging for finding stack high water mark.
	}
}

void SendCommString(char* input)
{
	for (int i = 0; i <= strlen(input); i++)
	{
		xQueueSendToBack(xCommPortQueue, (void *)&input[i], 100); //Adds the characters to the queue in order, waits up to 100 ticks if the queue is full.
	}
}