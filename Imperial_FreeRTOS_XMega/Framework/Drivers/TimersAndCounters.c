/*
 * TimersAndCounters.c
 *
 * Created: 22/08/2011 11:10:21
 *  Author: sf306
 */ 
#include "TimersAndCounters.h"
#include "LCDDriver.h"

//Reads the switches for timers and counters in the TimersAndCounters.h file and sets them up accordingly.
// T/C 0 on PORTD setup
void prvSetupTimers(void)
{
	//----------------------TCD0-----------------------------
	#if TC0_PORTD_SWITCH == 1 //SETUP for T/C 0 on PORTD
		TCD0.CTRLA = TC0_PORTD_PRESCALE;
		TCD0.PER = TC0_PORTD_PERIOD;
		TCD0.INTCTRLA = TC0_PORTD_INTLVL;
	#else
		PR.PRPD |= (0x01 << 0); //Stop the clock going to this timer circuit to reduce power
	#endif
	//--------------------------------------------------------
	
	//----------------------TCE0-----------------------------
	#if TC0_PORTE_SWITCH == 1 //SETUP for T/C 0 on PORTE
		TCE0.CTRLA = TC0_PORTE_PRESCALE;
		TCE0.PER = TC0_PORTE_PERIOD;
		TCE0.INTCTRLA = TC0_PORTE_INTLVL;
	#else
		PR.PRPE |= (0x01 << 0); //Power Reduction.
	#endif
	//-------------------------------------------------------
	
	//----------------------TCF0-----------------------------
	#if TC0_PORTF_SWITCH == 1 //SETUP for T/C 0 on PORTF
		TCF0.CTRLA = TC0_PORTF_PRESCALE;
		TCF0.PER = TC0_PORTF_PERIOD;
		TCF0.INTCTRLA = TC0_PORTF_INTLVL;
	#else
		PR.PRPF |= (0x01 << 0); //Power Reduction.
	#endif
	//-------------------------------------------------------
	
	//----------------------TCC1-----------------------------
	#if TC1_PORTC_SWITCH == 1 //SETUP for T/C 1 on PORTC
		TCC1.CTRLA = TC1_PORTC_PRESCALE;
		TCC1.PER = TC1_PORTC_PERIOD;
		TCC1.INTCTRLA = TC1_PORTC_INTLVL;
	#else	
		PR.PRPC |= (0x01 << 1); //Power Reduction.
	#endif
	//-------------------------------------------------------
	
	//----------------------TCD1-----------------------------
	#if TC1_PORTD_SWITCH == 1 //SETUP for T/C 1 on PORTD
		TCD1.CTRLA =  TC1_PORTD_PRESCALE;
		TCD1.PER =  TC1_PORTD_PERIOD;
		TCD1.INTCTRLA = TC1_PORTD_INTLVL;
	#else	
		PR.PRPD |= (0x01 << 1); //Power Reduction.
	#endif
	//-------------------------------------------------------
	
	//----------------------TCE1-----------------------------
	#if TC1_PORTE_SWITCH == 1 //SETUP for T/C 1 on PORTE
		TCE1.CTRLA = TC1_PORTE_PRESCALE;
		TCE1.PER = TC1_PORTE_PERIOD;
		TCE1.INTCTRLA = TC1_PORTE_INTLVL;
	#else	
		PR.PRPE |= (0x01 << 1); //Power Reduction.
	#endif
	//-------------------------------------------------------
}

//Test out a timer interrupt
ISR(TCD0_OVF_vect)
{
	vPulseBuzzer();
}
