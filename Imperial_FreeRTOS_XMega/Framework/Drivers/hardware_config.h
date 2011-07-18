/*
 * hardware_config.h
 *
 * Created: 15/07/2011 16:38:19
 *  Author: sf306
 */ 

#include "avr_compiler.h"

#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

void prvSetupHardware(void);
void prvPortSetup(void);
void prvHoldPowerOn(void);
void prvClockSetup(void);
void CCPWrite( volatile uint8_t * address, uint8_t value );



#endif /* HARDWARE_CONFIG_H_ */