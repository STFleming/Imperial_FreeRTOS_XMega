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
 FreeRTOS to ATXMega port
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

/*Task Header files and implementation files */
#include "AppTask.h"

//-------------------------------------------------------------
//				Function Prototypes
//-------------------------------------------------------------
void vApplicationIdleHook( void );
//-------------------------------------------------------------

//-------------------------------------------------------------
// Global variables
//-------------------------------------------------------------
xSemaphoreHandle xMutexPrinting; //Handle for the Mutex
//-------------------------------------------------------------

//-------------------------------------------------------------
//					Main Function 
//-------------------------------------------------------------
short main( void )
{
	prvSetupHardware();
	
	//LCD printing tests to tryout drivers.
	lcd_init(GRAPHTEXT);
	//lcd_putsp(PSTR("Trying out printing"));
	
	//-----------------------------------------------------
	// Creating MUTEXs and Semaphores before Task Creation.
	//-----------------------------------------------------
	xMutexPrinting = xSemaphoreCreateMutex();
	//-----------------------------------------------------
	
	//--------------------------------------
	//Create Tasks before starting scheduler
	//--------------------------------------
	CreateTaskOne();
	CreateTaskTwo();
	CreateTaskThree();
	//--------------------------------------
	
	
	vTaskStartScheduler();
	
	for(;;){
		lcd_putsp(PSTR("ERROR Scheduler unexpected exit"));
	}
	return;
}

//-------------------------------------------------------------
//					Function Specifications
//-------------------------------------------------------------

void vApplicationIdleHook( void )
{
	//This function is called when no other tasks are running apart from the idle task.
	lcd_init(GRAPHTEXT);
	lcd_putsp(PSTR("Hook test"));
	//printN(xTaskGetTickCount());
}