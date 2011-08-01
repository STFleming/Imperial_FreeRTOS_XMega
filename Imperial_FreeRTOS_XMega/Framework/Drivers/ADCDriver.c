/*
 * ADCDriver.c
 *
 * Created: 01/08/2011 11:32:42
 *  Author: sf306
 */ 


//This file contains the driver tasks used to control the ADC input

//Standard include file for the ADC driver.
#include "ADCDriver.h"

//Other include driver files.
#include "LCDDriver.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

//Define ADC driver task stack size.
#define adcSTACK_SIZE  ( ( unsigned short ) 256)

//-----------------------------------------
//		Global Variables.
//-----------------------------------------
uint16_t dataChannelA = 0; //This is used to store the value collected from channelA.
uint16_t dataChannelB = 0; //This is used to store the value collected from channelB.

//Prototype for task code body.
static void vADCChannelA(void *pvParameters);
static void vADCChannelB(void *pvParameters);

void vStartADC(portTickType adcPeriod)
{
	//Setup the PORT setings.
	//PORTJ is the port used for the ADC input pins.
	PORTJ.DIR = 0x00; //Set all pins on PORTJ as input pins.
	//Port H is used to set the v/div and power for the ADC.
	PORTH.OUT = 0x2D; //This sets the ADC on and sets each channel to 0.5v/div.
	
	//Setup Clock for ADC.
	TCF1.CTRLB = 0x11; //CCAEN override, frequency mode.
	TCF0.CTRLB = 0x83; //CCDEN override, Single Slop PWM
	TCF0.PER = 100;
	TCF0.CCD = 199;
	TCF1.CCA = 99; //ADC clock
	TCF0.CTRLA = 0x01; //Enable Timer, Prescaler: clk/1
	TCF1.CTRLA = 0x01; //Enable Timer, Prescaler: clk/1
	
	//Start the task to periodically read the ADC values and store them.
	xTaskCreate(vADCChannelA, (signed char*) "ADCA", adcSTACK_SIZE, (void *) adcPeriod, tskIDLE_PRIORITY+1, NULL);
	xTaskCreate(vADCChannelB, (signed char*) "ADCB", adcSTACK_SIZE, (void *) adcPeriod, tskIDLE_PRIORITY+1, NULL);
	
	//Setup of ADC completed.
}

//------------------------------------------------------
// Define Task Functionality.
//------------------------------------------------------
static void vADCChannelA(void *pvParameters)
{
	for(;;)
	{
		PORTF.OUT ^= 0x04; //This sets pin 3 on PORTF to 0. Used to select channel B.
		dataChannelB = PORTJ.IN; //Collects the ADC inputs from PORTJ.
		
		//Use LCD Print Number functions to print their output and debug.
		vPrintNumber(0,6, dataChannelA);
		
		//Delay the task based on the value inputted by the user.
		vTaskDelay((portTickType)pvParameters);
	}
}

static void vADCChannelB(void *pvParameters)
{
	for(;;)
	{
		//Check the first Channel and read it's value.
		PORTF.OUT |= 0x04; //This sets Pin 3 on PORTF High used to select channel A.
		dataChannelA = PORTJ.IN; //This collects the ADC inputs from PORTJ.
		
		vPrintNumber(0,7, dataChannelB);
		
		//Delay the task based on the value inputted by the user.
		vTaskDelay((portTickType)pvParameters);
		
	}
}