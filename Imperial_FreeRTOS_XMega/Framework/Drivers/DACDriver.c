/*
 * DACDriver.c
 *
 * Created: 28/07/2011 11:41:46
 *  Author: sf306
 */ 
/*AVR official header files */
#include <avr/io.h>

//Include files.
#include "DACDriver.h"
#include "LCDDriver.h" //Just to print out some debugging and testing information.

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

//Define task stack size.
#define dacSTACK_SIZE	( ( unsigned short ) 94)


//Variables used within driver.
uint16_t dac_out = 0;

//Prototype for task code body.
static void vDACTask(void *pvParameters);

void vStartDAC(portTickType conversion_time)
{
	portDACconfiguration(); //sets up the appropriate registers to use the DAC.
	xTaskCreate(vDACTask, (signed char*) "DAC", dacSTACK_SIZE, (void *) conversion_time, tskIDLE_PRIORITY+2, NULL);
}

void portDACconfiguration(void)
{
	//This function includes the configuration setup for the DAC registers.
	//------------------PORTA Config---------------------------------------
	//The AWG output is connected to PORTA pin 2 we need to setup this pin and the 2V reference pin.
	PORTA.DIR |= (0x01 << 3); //Set the pin directions for PORTA.
	//------------------DACA Config-----------------------------------------
	DACA.CTRLA = 0x05; //This selects channel 0, and enables the entire DAC block.
	DACA.CTRLB = 0x00; //Set the DACA into single channel operation.
	DACA.CTRLC = 0x10; //This sets up AREF the reference voltage from PORTA, we are using a 2V reference.
}

static void vDACTask(void *pvParameters)
{
	for(;;)
	{
		//Place data in the high and low bytes of the DAC data registers.
		DACA.CH0DATA = dac_out;  //0x0FFF = 4095 -> -2V, 0x07FF = 2047 -> 0v, 0x000 = 0 -> 2v.
		
		vTaskDelay((portTickType)pvParameters);
		//vPrintNumber(0,2, uxTaskGetStackHighWaterMark(NULL)); //Debugging for finding stack high water mark.
	}
}

void SetDACOut(uint16_t input)
{
	dac_out = input;
}