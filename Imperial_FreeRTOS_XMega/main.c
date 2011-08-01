/*
    FreeRTOS V7.0.1 - Copyright (C) 2011 Real Time Engineers Ltd.
	

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS tutorial books are available in pdf and paperback.        *
     *    Complete, revised, and edited pdf reference manuals are also       *
     *    available.                                                         *
     *                                                                       *
     *    Purchasing FreeRTOS documentation will not only help you, by       *
     *    ensuring you get running as quickly as possible and with an        *
     *    in-depth knowledge of how to use FreeRTOS, it will also help       *
     *    the FreeRTOS project to continue with its mission of providing     *
     *    professional grade, cross platform, de facto standard solutions    *
     *    for microcontrollers - completely free of charge!                  *
     *                                                                       *
     *    >>> See http://www.FreeRTOS.org/Documentation for details. <<<     *
     *                                                                       *
     *    Thank you for using FreeRTOS, and thank you for your support!      *
     *                                                                       *
    ***************************************************************************


    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    >>>NOTE<<< The modification to the GPL is included to allow you to
    distribute a combined work that includes FreeRTOS without being obliged to
    provide the source code for proprietary components outside of the FreeRTOS
    kernel.  FreeRTOS is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
    or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
 FreeRTOS port for the XMegaLab which uses an atxmega128A1 chip.
	Written by Shane Fleming
	Imperial College London.
 */


#include <stdlib.h>
#include <string.h>

#ifdef GCC_MEGA_AVR
	/* EEPROM routines used only with the WinAVR compiler. */
	#include <avr/eeprom.h> 
#endif

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "sed1335.h"

/* Hardware Drivers */
#include "hardware_config.h"
#include "Keypad.h"
#include "LCDDriver.h"
#include "DACDriver.h"
#include "ADCDriver.h"

/*Demo application header files */
#include "integer.h" //Tests math operations
#include "PollQ.h"	//Tests Queues
#include "regtest.h" //Register tests 

//-------------------------------------------------------------
//		Priority Definitions
//-------------------------------------------------------------
#define mainQUEUE_POLL_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define mainERROR_TSK_PRIORITY			( tskIDLE_PRIORITY + 3 )

//-------------------------------------------------------------
//				Function Prototypes
//-------------------------------------------------------------
void vApplicationIdleHook( void );

/*
 * The task function for the "Check" task.
 */
static void vErrorChecks( void *pvParameters );

//Function prototype used to describe test task for printing out functions.
static void vPrintOutStuff(void *pvParameters);

/*
 * Checks the unique counts of other tasks to ensure they are still operational.
 * Flashes an LED if everything is okay. 
 */
static void prvCheckOtherTasksAreStillRunning( void );

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName );
	/* This function is called immediately after task context is saved into stack. This is the case
	 * when stack contains biggest amount of data. Hook function checks if there is a stack overflow
	 * for the current (switched) task. Parameters could be used for debug output.
	 * configCHECK_FOR_STACK_OVERFLOW should be defined as 1 to use StackOverflowHook.
	 */
//-------------------------------------------------------------

//-------------------------------------------------------------
// Global variables
//-------------------------------------------------------------

//-------------------------------------------------------------

//-------------------------------------------------------------
//					Main Function 
//-------------------------------------------------------------
short main( void )
{
	prvSetupHardware();
	
	//--------------------------------------
	//Create Tasks before starting scheduler
	//--------------------------------------
	
	/* Create all the Demo test tasks */
	//xTaskCreate( vErrorChecks, ( signed char * ) "Check", configMINIMAL_STACK_SIZE, NULL, mainERROR_TSK_PRIORITY, NULL );
	//vStartIntegerMathTasks( tskIDLE_PRIORITY ); //Test tasks used to determine reliability of mathematical calculations.
	//vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
	//vStartRegTestTasks(); //Starts tasks used to test registers.
	
	/*Create the keypad task*/
	vStartKeypadTask();
	vStartLCD();
	vStartDAC(5);
	vStartADC(100);
	
	/* Create tasks to test LCD printing and keypad */
	xTaskCreate(vPrintOutStuff, (signed char * )"Printing", 256, NULL, tskIDLE_PRIORITY+1, NULL);
	//--------------------------------------
		
	PMIC.CTRL = 0x87; //Enable all three interrupt levels with round robin scheduling.
	
	vTaskStartScheduler();
	
	lcd_putsp(PSTR("ERROR Scheduler unexpected exit"));
	for(;;){
	}
	return 0;
}

//-------------------------------------------------------------
//					Function Specifications
//-------------------------------------------------------------

void vApplicationIdleHook( void )
{
	//This function is called when no other tasks are running.
}

void vApplicationStackOverflowHook( xTaskHandle *pxTask, signed portCHAR *pcTaskName )
{
	/* stop execution and report error */
	lcd_putsp(PSTR("STACK OVERFLOW"));
	
	//infinite loop to halt the execution.
	for(;;){}
}

static void vPrintOutStuff(void *pvParameters)
{
	uint16_t increment = 0;
	for(;;)
	{
		vPrintNumber(0,0,GetLastKeyPressed()*310);
		
		SetDACOut(GetLastKeyPressed()*310); //Vary the DAC output for debugging the ADC.
		
		vTaskDelay(2);
	}
}


static void vErrorChecks( void *pvParameters )
{
static volatile unsigned long ulDummyVariable = 3UL;

	/* The parameters are not used. */
	( void ) pvParameters;

	/* Cycle for ever, delaying then checking all the other tasks are still
	operating without error. */
	for( ;; )
	{
		vTaskDelay(300);

		/* Perform a bit of 32bit maths to ensure the registers used by the 
		integer tasks get some exercise. The result here is not important - 
		see the demo application documentation for more info. */
		ulDummyVariable *= 3;
		
		prvCheckOtherTasksAreStillRunning();
	}
}

static void prvCheckOtherTasksAreStillRunning( void )
{
static portBASE_TYPE xErrorHasOccurred = pdFALSE;

	if( xAreIntegerMathsTaskStillRunning() != pdTRUE ) //if statement used to report math test errors.
	{
		xErrorHasOccurred = pdTRUE;
		
		portENTER_CRITICAL();
		lcd_goto(0,0);
		lcd_putsp(PSTR("Math: FAIL"));
		portEXIT_CRITICAL();
	}
	else
	{
		portENTER_CRITICAL();
		lcd_goto(0,0);
		lcd_putsp(PSTR("Math: PASS"));
		portEXIT_CRITICAL();
	}

	if( xArePollingQueuesStillRunning() != pdTRUE ) //if statement used to report queue errors.
	{
		xErrorHasOccurred = pdTRUE;
		
		portENTER_CRITICAL();
		lcd_goto(0,1);
		lcd_putsp(PSTR("Queue: FAIL"));
		portEXIT_CRITICAL();
	}
	else
	{
		portENTER_CRITICAL();
		lcd_goto(0,1);
		lcd_putsp(PSTR("Queue: PASS"));
		portEXIT_CRITICAL();
	}
	
	if( xAreRegTestTasksStillRunning() != pdTRUE ) //If statement used to report regtest errors.
	{
		xErrorHasOccurred = pdTRUE;
		
		portENTER_CRITICAL();
		lcd_goto(0,2);
		lcd_putsp(PSTR("RegTest: FAIL"));
		portEXIT_CRITICAL();
	}
	else
	{		
		portENTER_CRITICAL();
		lcd_goto(0,2);
		lcd_putsp(PSTR("RegTest: PASS"));
		portEXIT_CRITICAL();
	}
	
	if( xErrorHasOccurred == pdFALSE )
	{
		/* Toggle the LED if everything is okay so we know if an error occurs even if not
		using console IO. */
		portENTER_CRITICAL();
		lcd_goto(0,5);
		lcd_putsp(PSTR("Result: PASS"));
		portEXIT_CRITICAL();
	}
	else
	{
		portENTER_CRITICAL();
		lcd_goto(0,5);
		lcd_putsp(PSTR("Result: FAIL"));
		portEXIT_CRITICAL();

	}
}
/*-----------------------------------------------------------*/

// POWER button - This is configured as a low level interrupt
ISR(PORTQ_INT0_vect) {
    uint8_t i=0,j;
    _delay_ms(10);
    if(testbit(PORTQ.IN, ONBUTTON)) {
        PORTC.OUTTGL = _BV(BUZZER1);    // Buzzer sound
        setbit(Signals, update);         // Valid key
        setbit(Signals, userinput);
    }
    while(testbit(PORTQ.IN, ONBUTTON)) {
        _delay_ms(10);
        i++;
        if(i==200) {    // Key pressed for 2 seconds
            cli();  // permanently disable interrupts
            PORTC.OUT = _BV(BUZZER1);
            // Shut down sound
            for(i=0; i<40; i++) {
		        for(j=i; j; j--) {
                    _delay_ms(1);
                }
		        PORTC.OUTTGL = _BV(BUZZER1); // Toggle pin
		        PORTC.OUTTGL = _BV(BUZZER2); // Toggle pin
	        }
            for(;;) PORTQ.OUT = 0;      // TURN OFF SYSTEM!!!
        }
    }
}	