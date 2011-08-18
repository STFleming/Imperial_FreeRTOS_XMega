/*
 * hardware_config.h
 *
 * Created: 15/07/2011 16:38:19
 *  Author: sf306
 */ 

#include "avr_compiler.h"

#ifndef HARDWARE_CONFIG_H_
#define HARDWARE_CONFIG_H_

//---------------------------------------------------------------------------
//		PERIPHERAL SWITCHES
//---------------------------------------------------------------------------
//These switches determine whether the corresponding peripheral is on or off.
//---------------General Internal Peripherals -------------------------------
#define AES_SWITCH 0 //This switches the AES encrypti1 module on or off.
#define EBI_SWITCH 0 //This switches the external bus interface on or off.
#define RTC_SWITCH 0 //This switches the real time clock module on or off.
#define EVSYS_SWITCH 0 //This switches the event system on or off.
#define DMA_SWITCH 0 //This switches the DMA module on or off.

//--------------Internal Port Peripherals ----------------------------------
#define PORTA_DAC_SWITCH 1 //Switches the DAC PORTA 1 of off. //This is the DAC used for the DAC DRIVER on the XMEGA LAB
#define PORTB_DAC_SWITCH 0 //Switches the DAC PORTB on or off.
#define PORTA_ADC_SWITCH 0 //Switches the ADC PORTA on or off.
#define PORTB_ADC_SWITCH 0 //Switches the ADC PORTB on or off.
#define PORTA_AC_SWITCH 0 //Switches the analog comparators PORTA on or off.
#define PORTB_AC_SWITCH 0 //Switches the analog comparators PORTB on or off.

#define PORTC_TWI_SWITCH 0 //Switches the Two wire interface PORTC on or off.
#define PORTD_TWI_SWITCH 0 //Switches the Two wire interface PORTD on or off.
#define PORTE_TWI_SWITCH 0 //Switches the Two wire interface PORTE on or off.
#define PORTF_TWI_SWITCH 0 //Switches the Two wire interface PORTF on or off.

#define PORTC_USART1_SWITCH 0 //Switches USART1 PORTC on or off.
#define PORTD_USART1_SWITCH 0 //Switches USART1 PORTD on or off.
#define PORTE_USART1_SWITCH 0 //Switches USART1 PORTE on or off.
#define PORTF_USART1_SWITCH 0 //Switches USART1 PORTF on or off.

#define PORTC_USART0_SWITCH 0 //Switches USART0 PORTC on or off.
#define PORTD_USART0_SWITCH 0 //Switches USART0 PORTD on or off.
#define PORTE_USART0_SWITCH 1 //Switches USART0 PORTE on or off. This is the USART used by the Serial Driver.
#define PORTF_USART0_SWITCH 0 //Switches USART0 PORTF on or off.

#define PORTC_SPI_SWITCH 0 //Switches serial peripheral interface PORTC on or off.
#define PORTD_SPI_SWITCH 0 //Switches serial peripheral interface PORTD on or off.
#define PORTE_SPI_SWITCH 0 //Switches serial peripheral interface PORTE on or off.
#define PORTF_SPI_SWITCH 0 //Switches serial peripheral interface PORTF on or off.

#define PORTC_HIRES_SWITCH 0 //Switches HIRES PORTC on or off.
#define PORTD_HIRES_SWITCH 0 //Switches HIRES PORTD on or off.
#define PORTE_HIRES_SWITCH 0 //Switches HIRES PORTE on or off.
#define PORTF_HIRES_SWITCH 1 //Switches HIRES PORTF on or off. REQUIRED FOR ADC OPERATION

#define PORTC_TC1_SWITCH 0 //Switches timer/counter1 PORTC on or off.
#define PORTD_TC1_SWITCH 0 //Switches timer/counter1 PORTD on or off.
#define PORTE_TC1_SWITCH 0 //Switches timer/counter1 PORTE on or off.
#define PORTF_TC1_SWITCH 1 //Switches timer/counter1 PORTF on or off. THIS IS THE ADC CLOCK SOURCE

#define PORTC_TC0_SWITCH 1 //PORT C T/C is used for the RTOS tick, IN ORDER FOR RTOS OPERATION KEEP ON.
#define PORTD_TC0_SWITCH 0 //Switches timer/counter0 PORTD on or off.
#define PORTE_TC0_SWITCH 0 //Switches timer/counter0 PORTE on or off.
#define PORTF_TC0_SWITCH 0 //Switches timer/counter0 PORTF on or off.


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
void prvPowerReduction(void);
void CCPWrite( volatile uint8_t * address, uint8_t value );
void prvLEDSetup(void);
void GRNLEDON(void);
void REDLEDON(void);
void GRNLEDOFF(void);
void REDLEDOFF(void);
void prvBuzzerSetup(void);
void vPulseBuzzer(void);



#endif /* HARDWARE_CONFIG_H_ */