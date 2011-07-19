/*
 * hardware_config.h
 *
 * Created: 15/07/2011 16:38:19
 *  Author: sf306
 */ 

#include "avr_compiler.h"

#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

// PORT DEFINITIONS
#define ONBUTTON    3   // PORTQ
#define BUZZER1     2   // PORTC
#define BUZZER2     3   // PORTC

// Global variables, using GPIO for optimized access
#define Signals GPIO0   // Signals

// Signal bits (GPIO0)
#define update      0       // Update
#define userinput   3       // User input received

void prvSetupHardware(void);
void prvPortSetup(void);
void prvHoldPowerOn(void);
void prvClockSetup(void);
void CCPWrite( volatile uint8_t * address, uint8_t value );



#endif /* HARDWARE_CONFIG_H_ */