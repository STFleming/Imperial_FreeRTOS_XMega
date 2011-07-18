/*
 * hardware_config.c
 *
 * Created: 15/07/2011 16:38:05
 *  Author: Shane Fleming
 */ 

#include "hardware_config.h"

void prvSetupHardware(void)
{
	prvHoldPowerOn();
	prvPortSetup();
}

void prvPortSetup(void)
{	
//-----------------------------------------------------------------------------------
// Peripheral and port setup
//-----------------------------------------------------------------------------------	
    PORTA.DIR      = 0x30;  // Encoder, LEDS, Volt Monitors, AWG, 2.0V
    PORTA.PIN6CTRL = 0x18;  // Pull up on pin PA6, sense both edges
    PORTA.PIN7CTRL = 0x18;  // Pull up on pin PA7, sense both edges
    PORTA.OUT      = 0x10; // Turn off both LEDs   
    PORTB.DIR      = 0x00;  // JTAG, CHA, CHB, Ext Trig, 2.0V
    PORTB.OUT      = 0x00;
    PORTC.DIR      = 0xBC;  // SPI, Buzzer, Encoder
    PORTC.PIN0CTRL = 0x18;  // Pull up on pin PC0, sense both edges
    PORTC.PIN1CTRL = 0x18;  // Pull up on pin PC1, sense both edges
    PORTC.OUT      = 0x14;  // C-SS high, pulse buzzer
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

