/*
 * demo.h
 *
 * Created: 22/08/2011 15:15:22
 *  Author: sf306
 */ 

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "sed1335.h"

#ifndef DEMO_H_
#define DEMO_H_


void vStartDemoApplication(void);
void prvCheckOtherTasksAreStillRunning( void );

#endif /* DEMO_H_ */