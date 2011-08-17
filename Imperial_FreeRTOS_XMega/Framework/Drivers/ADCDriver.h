/*
 * ADCDriver.h
 *
 * Created: 01/08/2011 11:32:55
 *  Author: sf306
 */ 

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"

#ifndef ADCDRIVER_H_
#define ADCDRIVER_H_


void vStartADC(portTickType adcPeriod);
uint16_t getADCchannelAdata(void);
uint16_t getADCchannelBdata(void);


#endif /* ADCDRIVER_H_ */