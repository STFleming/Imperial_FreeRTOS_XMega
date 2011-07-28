/*
 * DACDriver.h
 *
 * Created: 28/07/2011 11:41:59
 *  Author: sf306
 */ 

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#ifndef DACDRIVER_H_
#define DACDRIVER_H_


//void vStartDAC(portTickType conversion_time);
void portDACconfiguration(void);
void vStartDAC(portTickType conversion_time);
void SetDACOut(uint16_t input);

#endif /* DACDRIVER_H_ */