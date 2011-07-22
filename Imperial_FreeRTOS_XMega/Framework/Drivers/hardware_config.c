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
	prvHoldPowerOn();
	prvPortSetup();
	prvClockSetup();
}

void prvPortSetup(void)
{	
//-----------------------------------------------------------------------------------
// Peripheral and port setup
//-----------------------------------------------------------------------------------	
    PORTA.DIR      = 0x30;  // Encoder, LEDS, Volt Monitors, AWG, 2.0V
    PORTA.PIN6CTRL = 0x18;  // Pull up on pin PA6, sense both edges
    PORTA.PIN7CTRL = 0x18;  // Pull up on pin PA7, sense both edges
    PORTA.OUT      = 0x00; // Turn off both LEDs   
    PORTB.DIR      = 0x00;  // JTAG, CHA, CHB, Ext Trig, 2.0V
    PORTB.OUT      = 0x00;
    PORTC.DIR      = 0xBC;  // SPI, Buzzer, Encoder
    PORTC.PIN0CTRL = 0x18;  // Pull up on pin PC0, sense both edges
    PORTC.PIN1CTRL = 0x18;  // Pull up on pin PC1, sense both edges
    PORTC.OUT      = 0x14;  // C-SS high, pulse buzzer
	
	PORTD.DIR      = 0xF0;  // Keypad and switches
	PORTCFG.MPCMASK = 0xFF; // Configure all pins on PORTD the same way
    PORTD.PIN0CTRL = 0x18;  // Pull up on pin PortD (Keypad and K2-K5 buttons)
    PORTD.OUT      = 0x00;
	
    PORTE.DIR      = 0xB9;  // SPI, TX, RX, SD_CD, TEST
    PORTE.PIN3CTRL = 0x40;  // Invert TX output
    PORTE.OUT      = 0x10;  // Deselect SD card
    PORTE.PIN1CTRL = 0x02;  // PE1 will sense falling edges
    PORTE.INTCTRL  = 0x01;  // PORTE will generate low level interrupts
    PORTE.INT0MASK = 0x02;  // PE1 will be the interrupt source
    PORTF.DIR      = 0xFB;  // LCD WR, LCD RD, SRAM CS, ADCCLK, CHSEL, SD_WP, LCD A0, LCD BKLT
    PORTF.PIN3CTRL = 0x80;  // Slew rate control
    PORTF.PIN4CTRL = 0x80;  // Slew rate control
    PORTF.OUT      = 0x21;  // Deselect Serial RAM, backlight on
    PORTH.DIR      = 0xFF;  // PDN, \OE, SELA, SELB
    PORTH.OUT      = 0x80;  // Power Down ADS931
    PORTJ.DIR      = 0x00;  // ADS931 data
    PORTK.DIR      = 0xFF;  // LCD data
    PORTK.OUT      = 0x00;
	PORTQ.DIR      = 0x04;  // On switch, voltage regulator control
    PORTQ.OUT      = 0x04;  // Power ON voltage regulators
    PORTQ.PIN3CTRL = 0x01;  // PQ3 will sense rising edges
    PORTQ.INT0MASK = 0x08;  // PQ3 will be the interrupt 0 source
    PORTQ.INTCTRL  = 0x01;  // PORTQ will generate low level interrupts
    PORTCFG.VPCTRLA = 0x18; // VP1 Map to PORTB, VP0 Map to PORTJ
    PORTCFG.VPCTRLB = 0x59; // VP3 Map to PORTF, VP2 Map to PORT
//-------------------------------------------------------------------------------------------------
}

void prvHoldPowerOn(void)
{
	//This function is used to configure the ports so that the Board power stays on
	PORTQ.DIR      = 0x04;  // On switch, voltage regulator control
    PORTQ.OUT      = 0x04;  // Power ON voltage regulators
    PORTQ.PIN3CTRL = 0x01;  // PQ3 will sense rising edges
    PORTQ.INT0MASK = 0x08;  // PQ3 will be the interrupt 0 source
    PORTQ.INTCTRL  = 0x01;  // PORTQ will generate low level interrupts
	
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