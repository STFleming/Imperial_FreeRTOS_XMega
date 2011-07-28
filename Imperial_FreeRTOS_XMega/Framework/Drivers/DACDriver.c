/*
 * DACDriver.c
 *
 * Created: 28/07/2011 11:41:46
 *  Author: sf306
 */ 

//Include files.
#include "DACDriver.h"
#include "LCDDriver.h" //Just to print out some debugging and testing information.

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

/*AVR official header files */
#include <avr/io.h>

//Define task stack size.
#define dacSTACK_SIZE	( ( unsigned short ) 120)

//Prototype for task code body.
static void vDACTask(void *pvParameters);

void vStartDAC(portTickType conversion_time)
{
	portDACconfiguration(); //sets up the appropriate registers to use the DAC.
	xTaskCreate(vDACTask, (signed char*) "DAC", dacSTACK_SIZE, (void *) &(conversion_time), tskIDLE_PRIORITY, NULL);
}

void portDACconfiguration(void)
{
	//This function includes the configuration setup for the DAC registers.
	//------------------PORTA Config---------------------------------------
	//The AWG output is connected to PORTA pin 2 we need to setup this pin and the 2V reference pin.
	PORTA.DIR = 0x30; //Set the pin directions for PORTA.
	//------------------DACA Config-----------------------------------------
//	DACA.CTRLA = 0x05; //This selects channel 0, and enables the entire DAC block.
//	DACA.CTRLB = 0x00; //Set the DACA into single channel operation.
//	DACA.CTRLC = 0x11; //This sets up AREF the reference voltage from PORTA, we are using a 2V reference.
//	DACA.TIMCTRL = 0x50; //Minimum 32 CLK between conversions.
//	DACA.CH0DATAH = 0x80; //Initialize the DAC to 0V. High Bytes.


	DACA.CTRLC = (DACA.CTRLC & ~DAC_REFSEL_gm) | DAC_REFSEL_AVCC_gc; //Use AVCC as Voltage Reference.
	DACA.CTRLB = (DACA.CTRLB & ~DAC_CHSEL_gm) | DAC_CHSEL_SINGLE_gc; //Use single conversion mode.
	DACA.CTRLA = DAC_CH0EN_bm | DAC_ENABLE_bm; //Enable CH0 and enable the DAC module.
}

static void vDACTask(void *pvParameters)
{
	for(;;)
	{
		vPrintString(0,5, "DAC task.");
		//Place data in the high and low bytes of the DAC data registers.
		DACA.CH0DATA = 0x0FFF; 
		
		vTaskDelay((portTickType)pvParameters);
	}
}