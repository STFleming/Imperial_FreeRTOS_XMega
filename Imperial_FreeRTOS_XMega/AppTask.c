/*
 * LEDTask.c
 *
 * Created: 14/07/2011 14:58:56
 *  Author: sf306
 */ 
/* Compiler definitions include file. */
#include "avr_compiler.h"

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "queue.h"

//Driver Include files.
#include "sed1335.h" //So that the LCD can be used.
#include "portmacro.h" //So that calls to critical sections for tasks can be made

#include "AppTask.h"

extern xSemaphoreHandle xMutexPrinting;

//----------------------------------------------
//	Helper Functions used in printing task info
//----------------------------------------------

void positionLCDPrint(void)
{
	static unsigned int Prow = 0;
	xSemaphoreTake(xMutexPrinting, portMAX_DELAY);
	{
		lcd_goto((Prow++ % 256), 0); //Since this function automatically wraps onto the next line.
	}
	xSemaphoreGive(xMutexPrinting);
}
//-------------------------------------------------
//Define the task implementation code.
void vTaskOne( void *pvParameters )
{
	/* The parameters are not used. */
	( void ) pvParameters;

	for(;;)
	{		
			lcd_putsp(PSTR("1")); //Prints the tasks number to screen
			positionLCDPrint(); //reentrant function uses a mutex lock.
			vTaskDelay(300);
	}
}

void vTaskTwo(void *pvParameters)
{
	/*Parameters are unused in this task*/
	(void)pvParameters;

	for(;;)
	{
		lcd_putsp(PSTR("2"));		
		positionLCDPrint();
		PORTA.OUT = 0x10;
		vTaskDelay(200);
	}
	
}

void vTaskThree(void *pvParameters)
{
	/*Parameters are unused in this task*/
	(void)pvParameters;

	for(;;)
	{
		lcd_putsp(PSTR("3"));
		positionLCDPrint();
		vTaskDelay(100);
	}
	
}

void CreateTaskOne(void)
{
	xTaskCreate(vTaskOne, ( signed char * ) "TASK1", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
}

void CreateTaskTwo(void)
{
	xTaskCreate(vTaskTwo, ( signed char * ) "TASK2", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
}

void CreateTaskThree(void)
{
	xTaskCreate(vTaskThree, (signed char *) "TASK3", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
}