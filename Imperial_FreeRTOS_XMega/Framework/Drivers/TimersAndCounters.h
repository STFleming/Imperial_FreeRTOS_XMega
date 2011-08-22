/*
 * TimersAndCounters.h
 *
 * Created: 22/08/2011 11:10:45
 *  Author: sf306
 */ 

#include "hardware_config.h"

#ifndef TIMERSANDCOUNTERS_H_
#define TIMERSANDCOUNTERS_H_

//This header file includes software switches for all the free timers and counters.
//It also has spaces where setup information for each timer/counter can be inputted.

//TCD0
#define TC0_PORTD_SWITCH 1 //Software switch for T/C0 on port D
		#define TC0_PORTD_PRESCALE DIV1024 //Prescale value for T/C0 on PORT D. Uses the defined prescale values.
		#define TC0_PORTD_PERIOD 32000 //Defines the overflow period of the TC
		#define TC0_PORTD_INTLVL INTERRUPTMED  //Determines the interrupt level on overflow, set INTERRUPTOFF to disable interrupts for this T/C.

//TCE0
#define TC0_PORTE_SWITCH 0 // T/C 0 PORTE
		#define TC0_PORTE_PRESCALE DIV1024
		#define TC0_PORTE_PERIOD 32000
		#define TC0_PORTE_INTLVL INTERRUPTOFF //ISR Vector = TCE0_OVF_vect

//TCF0
#define TC0_PORTF_SWITCH 0 // T/C 0 PORTF
		#define TC0_PORTF_PRESCALE DIV1024
		#define TC0_PORTF_PERIOD 32000
		#define TC0_PORTF_INTLVL INTERRUPTOFF //ISR Vector = TCF0_OVF_vect

//TCC1
#define TC1_PORTC_SWITCH 0 // T/C 1 PORTC
		#define TC1_PORTC_PRESCALE DIV1024
		#define TC1_PORTC_PERIOD 32000
		#define TC1_PORTC_INTLVL INTERRUPTOFF //ISR Vector = TCC1_OVF_vect

//TCD1		
#define TC1_PORTD_SWTICH 0 //T/C 1 PORTD
		#define TC1_PORTD_PRESCALE DIV1024
		#define TC1_PORTD_PERIOD 32000
		#define TC1_PORTD_INTLVL INTERRUPTOFF //ISR Vector = TCD1_OVF_vect

//TCE1
#define TC1_PORTE_SWITCH 0 // T/C 1 PORTE
		#define TC1_PORTE_PRESCALE DIV1024
		#define TC1_PORTE_PERIOD 32000
		#define TC1_PORTE_INTLVL INTERRUPTOFF //ISR Vector = TCE1_OVF_vect

//**NOTE** There are two other timers avaliable of PORTC and PORTF, however they are both used internally with the RTOS.
//The timer T/C 0 on PORTC is used for the RTOS tick and should never be modified as this will effect the operation of the RTOS.
//The time T/C 1 on PORT F is used for the ADC and can be modified if the ADC is not operational. 		
 
 //Function Prototypes
 void prvSetupTimers(void);

#endif /* TIMERSANDCOUNTERS_H_ */