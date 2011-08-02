/*
 * CommPortDriver.c
 *
 * Created: 02/08/2011 13:46:25
 *  Author: sf306
 */ 

//Include files----------------------------------------
#include "CommPortDriver.h"

//Other driver includes
#include "LCDDriver.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

//Define driver task stack size.
#define commSTACK_SIZE	( ( unsigned short ) 256)

//-----------------------------------------------
//		Global Variables.
//-----------------------------------------------
char recieved_char = '/0';

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
    USARTE0.CTRLA = 0x10;       // RX is low level interrupt
    USARTE0.CTRLC = 0x03;       // Async, No Parity, 1 stop bit, 8 data bits
    USARTE0.CTRLB = 0x18;       // Enable RX and TX
	
	//Start the driver task for the Comm Port.
	xTaskCreate(vCommTask, (signed char*) "COMM", commSTACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL);
}

static void vCommTask(void *pvParameters)
{ //This function implements the CommTask
	for(;;)
	{
		//Debugging, Printing the status bits of the USART flags
		vPrintNumber(4,10, (USARTE0.STATUS & (1 << 7))); //RXCIF
		vPrintNumber(8,10, (USARTE0.STATUS & (1 << 5))); //DREIF
		
		if ( (USARTE0.STATUS & (1 << 7)) == 1 ) //Check the RXCIF flag to see if any data is ready to be read.
		{
			recieved_char = USARTE0.DATA;
			vPrintString(0,11, "Recieving");
		}
		
		if ((USARTE0.STATUS & (1 << 5)) == 1) //There is room to transmit another character.Checking the DREIF flag
		{
			USARTE0.DATA = 'P'; //Keep transmitting P over and over again for debugging purposes.
			vPrintString(0,11, "Transmitting");
		}
		
		vPrintChar(0,12,recieved_char);
		
		vTaskDelay(1);
	}
}
