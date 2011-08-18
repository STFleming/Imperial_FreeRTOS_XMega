/*
 * hardware_config.c
 *
 * Created: 15/07/2011 16:38:05
 *  Author: Shane Fleming
 */ 

#include "hardware_config.h"
#include "sed1335.h"

void prvSetupHardware(void)
{
	prvPowerReduction();
	prvHoldPowerOn();
	prvClockSetup();
	prvLEDSetup();
	prvBuzzerSetup();
	
	//Setup the virtual port registers.
	prvSetupVirtualPortA(VPPORTB, VPPORTJ);
	prvSetupVirtualPortB(VPPORTF, VPPORTK);
}

//--------------------------------------//
//		LED Controller Functions		//
//--------------------------------------//
//-----------------------------------------------------------------------------
void prvLEDSetup(void)
{
	PORTA.DIR |= (0x01 << 5); //Enable the RED LED.
	PORTA.DIR |= (0x01 << 4); //Enable the Green LED.
}

void GRNLEDON(void){PORTA.OUT |= (0x01 << 4);} //Switches the green led on.
void REDLEDON(void){PORTA.OUT |= (0x01 << 5);} //Switches the red led on.
void GRNLEDOFF(void){PORTA.OUT &= (0x00 << 4);} //Switches the green led off.
void REDLEDOFF(void){PORTA.OUT &= (0x00 << 5);} //Switches the red led off.
//-----------------------------------------------------------------------------

//--------------------------------------//
//		Buzzer Controller Functions		//
//--------------------------------------//
//-----------------------------------------------------------------------------
void prvBuzzerSetup(void)
{
	PORTC.DIR |= (0x01 << 2);
	PORTC.DIR |= (0x01 << 3);
	
	PORTC.OUT = 0x00;	
}

void vPulseBuzzer(void)
{
	PORTC.OUTTGL |= (0x01 << 2);
}
//-----------------------------------------------------------------------------

void prvHoldPowerOn(void)
{
	//This function is used to configure the ports so that the Board power stays on
	PORTQ.DIR      = 0x04;  // On switch, voltage regulator control
    PORTQ.OUT      = 0x04;  // Power ON voltage regulators
}

//----------------Virtual Port Setup Functions-------------------------------
void prvSetupVirtualPortA(uint8_t VPone, uint8_t VPtwo)
{
	uint8_t temp = ((VPone << 4) | VPtwo ); //combine the two nibbles containing the ports into a single byte.
	PORTCFG.VPCTRLA = temp;
}

void prvSetupVirtualPortB(uint8_t VPone, uint8_t VPtwo)
{
	uint8_t temp = ((VPone << 4) | VPtwo ); //combine the two nibbles containing the ports into a single byte.
	PORTCFG.VPCTRLB = temp;
}
//---------------------------------------------------------------------------

void prvPowerReduction(void)
{
	//statements that check if peripheral switches in hardware_config.h
	//are on and sets the appropriate register.
	
	//General internal peripherals.
	PR.PRGEN = 0x00;
	if(AES_SWITCH != 1){PR.PRGEN |= (0x01 << 4);} //AES encryption module.
	if(EBI_SWITCH != 1){PR.PRGEN |= (0x01 << 3);} //External Bus Interface.
	if(RTC_SWITCH != 1){PR.PRGEN |= (0x01 << 2);} //Real Time Clock.
	if(EVSYS_SWITCH != 1){PR.PRGEN |= (0x01 << 1);} //Event System.
	if(DMA_SWITCH != 1){PR.PRGEN |= (0x01 << 0);} //DMA
	
	//PORT specific peripherals PORTS A and B.
	PR.PRPA = 0x00; PR.PRPB = 0x00;
	if(PORTA_DAC_SWITCH != 1){DACA.CTRLA &= (0x00 << 0); PR.PRPA |= (0x01 << 2);} //We need to disable the DAC before stopping the clock to the peripheral.
	if(PORTB_DAC_SWITCH != 1){DACB.CTRLA &= (0x00 << 0); PR.PRPB |= (0x01 << 2);} //Disable PORTB DAC and Stop the clock to the DAC.
	if(PORTA_ADC_SWITCH != 1){ADCA.CTRLA &= (0x00 << 0); PR.PRPA |= (0x01 << 1);} //Disable the ADC on PORTA before stopping the clock.
	if(PORTB_ADC_SWITCH != 1){ADCB.CTRLA &= (0x00 << 0); PR.PRPB |= (0x01 << 1);} //Disable the ADC on PORTA before stopping the clock.
	if(PORTA_AC_SWITCH != 1){ACA.AC0CTRL &= (0x00 << 0); ACA.AC1CTRL &= (0x00 <<0); PR.PRPA |= (0x01 << 0);} //Disable the analog comparator on PORTA
	if(PORTA_AC_SWITCH != 1){ACB.AC0CTRL &= (0x00 << 0); ACB.AC1CTRL &= (0x00 <<0); PR.PRPB |= (0x01 << 0);} //Disable the analog comparator on PORTB
		
	//Port specific peripherals PORTS C, D, E and F.
	//---------------------------------------------------------------
	PR.PRPC = 0x00; PR.PRPD = 0x00; PR.PRPE = 0x00; PR.PRPF = 0x00;
	
	//Two Wire Interface.
	if(PORTC_TWI_SWITCH != 1){PR.PRPC |= (0x01 << 6);} //TWI on PORTC
	if(PORTD_TWI_SWITCH != 1){PR.PRPD |= (0x01 << 6);} //TWI on PORTD
	if(PORTE_TWI_SWITCH != 1){PR.PRPE |= (0x01 << 6);} //TWI on PORTE
	if(PORTF_TWI_SWITCH != 1){PR.PRPF |= (0x01 << 6);} //TWI on PORTF
	
	//USART1
	if(PORTC_USART1_SWITCH != 1){PR.PRPC |= (0x01 << 5);} //USART1 on PORTC
	if(PORTD_USART1_SWITCH != 1){PR.PRPD |= (0x01 << 5);} //USART1 on PORTD
	if(PORTE_USART1_SWITCH != 1){PR.PRPE |= (0x01 << 5);} //USART1 on PORTE
	if(PORTF_USART1_SWITCH != 1){PR.PRPF |= (0x01 << 5);} //USART1 on PORTF
	
	//USART0
	if(PORTC_USART0_SWITCH != 1){PR.PRPC |= (0x01 << 4);} //USART0 on PORTC
	if(PORTD_USART0_SWITCH != 1){PR.PRPD |= (0x01 << 4);} //USART0 on PORTD
	if(PORTE_USART0_SWITCH != 1){PR.PRPE |= (0x01 << 4);} //USART0 on PORTE
	if(PORTF_USART0_SWITCH != 1){PR.PRPF |= (0x01 << 4);} //USART0 on PORTF
	
	//Serial Peripheral Interface
	if(PORTC_SPI_SWITCH != 1){PR.PRPC |= (0x01 << 3);} //SPI on PORTC
	if(PORTD_SPI_SWITCH != 1){PR.PRPD |= (0x01 << 3);} //SPI on PORTD
	if(PORTE_SPI_SWITCH != 1){PR.PRPE |= (0x01 << 3);} //SPI on PORTE
	if(PORTF_SPI_SWITCH != 1){PR.PRPF |= (0x01 << 3);} //SPI on PORTF
	
	//HIRES
	if(PORTC_HIRES_SWITCH != 1){PR.PRPC |= (0x01 << 2);} //HIRES on PORTC
	if(PORTD_HIRES_SWITCH != 1){PR.PRPD |= (0x01 << 2);} //HIRES on PORTD
	if(PORTE_HIRES_SWITCH != 1){PR.PRPE |= (0x01 << 2);} //HIRES on PORTE
	if(PORTF_HIRES_SWITCH != 1){PR.PRPF |= (0x01 << 2);} //HIRES on PORTF
	
	//Timer/Counter 1
	if(PORTC_TC1_SWITCH != 1){PR.PRPC |= (0x01 << 1);} //TC1 on PORTC
	if(PORTD_TC1_SWITCH != 1){PR.PRPD |= (0x01 << 1);} //TC1 on PORTD
	if(PORTE_TC1_SWITCH != 1){PR.PRPE |= (0x01 << 1);} //TC1 on PORTE
	if(PORTF_TC1_SWITCH != 1){PR.PRPF |= (0x01 << 1);} //TC1 on PORTF
	
	//Timer/Counter 0
	if(PORTC_TC0_SWITCH != 1){PR.PRPC |= (0x01 << 0);} //TC0 on PORTC
	if(PORTD_TC0_SWITCH != 1){PR.PRPD |= (0x01 << 0);} //TC0 on PORTD
	if(PORTE_TC0_SWITCH != 1){PR.PRPE |= (0x01 << 0);} //TC0 on PORTE
	if(PORTF_TC0_SWITCH != 1){PR.PRPF |= (0x01 << 0);} //TC0 on PORTF
	
	//---------------------------------------------------------------	
}

void prvClockSetup(void)
{
	// Clock Settings
    OSC.XOSCCTRL = 0xCB;    // 0.4-16 MHz XTAL - 16K CLK Start Up
    OSC.CTRL = 0x08;        // Enable External Oscillator
    while(!testbit(OSC.STATUS,OSC_XOSCRDY_bp));  // wait until crystal stable
    OSC.PLLCTRL = 0xC2;     // XOSC is PLL Source - 2x Factor (32MHz)
    OSC.CTRL = 0x19;        // Enable PLL & External Oscillator
    while(!testbit(OSC.STATUS,OSC_PLLRDY_bp));  // wait until PLL stable
    CCPWrite(&CLK.CTRL, CLK_SCLKSEL_PLL_gc);    // Switch to PLL clock
    OSC.CTRL = 0x18;        // Disable internal 2MHz
}

// From Application Note AVR1003
void CCPWrite( volatile uint8_t * address, uint8_t value ) {
    uint8_t volatile saved_sreg = SREG;
    cli();

#ifdef __ICCAVR__
	asm("movw r30, r16");
#ifdef RAMPZ
	RAMPZ = 0;
#endif
	asm("ldi  r16,  0xD8 \n"
	    "out  0x34, r16  \n"
#if (__MEMORY_MODEL__ == 1)
	    "st     Z,  r17  \n");
#elif (__MEMORY_MODEL__ == 2)
	    "st     Z,  r18  \n");
#else /* (__MEMORY_MODEL__ == 3) || (__MEMORY_MODEL__ == 5) */
	    "st     Z,  r19  \n");
#endif /* __MEMORY_MODEL__ */

#elif defined __GNUC__
	volatile uint8_t * tmpAddr = address;
#ifdef RAMPZ
	RAMPZ = 0;
#endif
	asm volatile(
		"movw r30,  %0"	      "\n\t"
		"ldi  r16,  %2"	      "\n\t"
		"out   %3, r16"	      "\n\t"
		"st     Z,  %1"       "\n\t"
		:
		: "r" (tmpAddr), "r" (value), "M" (CCP_IOREG_gc), "i" (&CCP)
		: "r16", "r30", "r31"
		);

#endif
	SREG = saved_sreg;
}