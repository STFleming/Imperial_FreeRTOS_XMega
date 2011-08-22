/*
 * demo.c
 *
 * Created: 22/08/2011 15:15:11
 *  Author: sf306
 */ 
#include "demo.h"
#include "LCDDriver.h"

/*Demo application header files */
#include "integer.h" //Tests math operations
#include "PollQ.h"	//Tests Queues
#include "regtest.h" //Register tests 

//-------------------------------------------------------------
//		Priority Definitions
//-------------------------------------------------------------
#define mainQUEUE_POLL_PRIORITY			( tskIDLE_PRIORITY + 2 )
#define mainERROR_TSK_PRIORITY			( tskIDLE_PRIORITY + 3 )

static void vErrorChecks( void *pvParameters );

void vStartDemoApplication(void)
{
	/* Create all the Demo test tasks */
	xTaskCreate( vErrorChecks, ( signed char * ) "Check", configMINIMAL_STACK_SIZE, NULL, mainERROR_TSK_PRIORITY, NULL );
	vStartIntegerMathTasks( tskIDLE_PRIORITY ); //Test tasks used to determine reliability of mathematical calculations.
	vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
	vStartRegTestTasks(); //Starts tasks used to test registers.
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

void prvCheckOtherTasksAreStillRunning( void )
{
static portBASE_TYPE xErrorHasOccurred = pdFALSE;

	if( xAreIntegerMathsTaskStillRunning() != pdTRUE ) //if statement used to report math test errors.
	{
		xErrorHasOccurred = pdTRUE;
		vPrintString(0,0, "Math: FAIL");
	}
	else
	{
		vPrintString(0,0, "Math: PASS");
	}

	if( xArePollingQueuesStillRunning() != pdTRUE ) //if statement used to report queue errors.
	{
		xErrorHasOccurred = pdTRUE;
		vPrintString(0,1, "Queue: FAIL");
	}
	else
	{
		vPrintString(0,1, "Queue: PASS");
	}
	
	if( xAreRegTestTasksStillRunning() != pdTRUE ) //If statement used to report regtest errors.
	{
		xErrorHasOccurred = pdTRUE;
		vPrintString(0,2, "RegTest: FAIL");
	}
	else
	{
		vPrintString(0,2, "RegTest: PASS");		
	}
	
	if( xErrorHasOccurred == pdFALSE )
	{
		vPrintString(0,5, "Result: PASS");
	}
	else
	{
		vPrintString(0,5, "Result: FAIL");
	}
}