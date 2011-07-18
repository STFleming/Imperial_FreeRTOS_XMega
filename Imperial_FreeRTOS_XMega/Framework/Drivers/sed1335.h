/***********************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics C.A.
May 2009

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com

Gabriel Anzziani - gabriel@gabotronics.com
based on library from: Knut Baardsen @ Baardsen Software, Norway

*****************************************************************************/

/***********************************************************************
Port, PINS and LCD defines 
***********************************************************************/

#ifndef _SED1335_H
#define _SED1335_H

#define setbit(port, bit) ((port) |= (uint8_t)_BV(bit))
#define setbits(port, mask) ((port) |= (uint8_t)(mask))
#define clrbit(port, bit) ((port) &= (uint8_t)~_BV(bit))
#define clrbits(port, mask) ((port) &= (uint8_t)~(mask))
#define testbit(port, bit) (uint8_t)(((uint8_t)port & (uint8_t)_BV(bit)))
#define togglebit(port, bit) (port ^= (uint8_t)_BV(bit))
#define	hibyte(x) (uint8_t)(x>>8)
#define	lobyte(x) (uint8_t)(x&0x00FF)

#include "LCDFonts.h"
#include <avr/pgmspace.h>
#include <stdint.h>

// Data port
#define LCDDATAPIN  VPORT2.IN
#define LCDDATADDR  VPORT2.DIR
#define LCDDATAPORT VPORT2.OUT
#define LCDDATASET  VPORT2.OUTSET
#define LCDDATACLR  VPORT2.OUTCLR
// Control port
#define LCDCTLPIN   VPORT3.IN
#define LCDCTLDDR   VPORT3.DIR
#define LCDCTLPORT  VPORT3.OUT
//#define LCDCTLSET   PORTF.OUTSET
//#define LCDCTLCLR   PORTF.OUTCLR
// Control pins
#define RD          6
#define WR          7
#define A0          1
// LCD display info, specific to the display used:
// DMF5008N: 128x128 pixels, 8kB RAM
#define LCD_X_SIZE  128
#define LCD_Y_SIZE  128
#define DISPMEM     8192
#define LCD_XTAL    6000000
#define TEXT        0       // Overlay is OR
#define GRAPHTEXT   3       // Overlay is priority OR
#define GRAPHICS    0x0C    // Overlay os OR

#define IV          1       // 0: Screen top-line correction
                            // 1: No screen top-line correction
#define WS          1       // 0: Single-panel drive 1: Dual-panel drive
#define M2          0       // 0: 8-pixel character height
                            // 1: 16-pixel character height
#define M1          1       // 0: CG RAM1 and CG RAM2 address spaces are not
                            // contiguous, the CG RAM1 address space is
                            // treated as character generator RAM, and the
                            // CG RAM2 address space is treated as character
                            // generator ROM.
                            // 1: The CG RAM1 and CG RAM2 address spaces are
                            // contiguout and are both treated as character
                            // generator RAM.
#define M0          0       // 0: Internal CG ROM 1: External CG ROM
#define WF          1       // 0: 16-line AC drive 1: two-frame AC drive

#define FY          7       // Vertical character size - 1
                            
#define TCR         ((LCD_XTAL / 70 / LCD_Y_SIZE) / 9)
#define LF          (LCD_Y_SIZE - 1)
#define APL         (CR+1)
#define APH         0
#define SL1         (LCD_Y_SIZE>>WS)
#define SL2         (LCD_Y_SIZE>>WS)

#define SAGL        0x00    // CGRAM Address
#define SAGH        0x0A

// LCD Commands: COMMAND    // (# of bytes after command)
#define SYSTEM_SET  0x40    // (8) Initialize Device
#define SLEEP_IN    0x53    // (0) Enter stand-by mode
#define DISP_ON     0x59    // (1) Enable display flashing
#define DISP_OFF    0x58    // (1) Disable display flashing
#define SCROLL      0x44    // (10) Set display start address and display regions
#define CSRFORM     0x5D    // (2) Set cursor type
#define CGRAM_ADR   0x5C    // (2) Set start address of character generator RAM
#define CSR_RIGHT   0x4C    // (0) Set direction of cursor movement: right
#define CSR_LEFT    0x4D    // (0) Set direction of cursor movement: left
#define CSR_UP      0x4E    // (0) Set direction of cursor movement: up
#define CSR_DOWN    0x4F    // (0) Set direction of cursor movement: down
#define HDOT_SCR    0x5A    // (1) Set horizontal scroll position
#define OVLAY       0x5B    // (1) Set display overlay format
#define CSRW        0x46    // (2) Set cursor address
#define CSRR        0x47    // (2) Read cursor address
#define MWRITE      0x42    // Write to display memory
#define MREAD       0x43    // Read from display memory

/***********************************************************************
Prototypes
***********************************************************************/
// General LCD functions
uint8_t lcd_read_data(void);
void lcd_write_data(uint8_t data);
void lcd_write_command(uint8_t command);
void lcd_init(uint8_t mode);
void lcd_scroll(int8_t S1, int8_t S2, int8_t S3, int8_t S4);
void lcd_delay(void);


// Text functions
void lcd_clear_text(void);
void lcd_goto(uint8_t row, uint8_t col); // text char position on screen
void lcd_set_cursor_addr(uint16_t addr);
uint16_t lcd_read_cursor_addr(void);
void NewLine(void);
void lcd_putsp(const char *ptr);
void lcd_puts(const char*);
void put_char(char c);
void lcd_show_cursor(uint8_t rate);
void printN(int32_t Data);
void lcd_cursor_type(uint8_t type);
// Graphic functions
void lcd_clear_graphics(void);
void lcd_clear_graphics2(void);
void set_pixel(uint8_t x, uint8_t y);
void clr_pixel(uint8_t x, uint8_t y);
//uint8_t read_pixel(uint8_t x, uint8_t y);
void lcd_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void lcd_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void lcd_circle(uint8_t x, uint8_t y, uint8_t radius);
void lcd_degree_line(uint8_t degree, uint8_t inner_radius, uint8_t outer_radius);
void put_2char(uint8_t c1, uint8_t c2);
void put_2charinv(uint8_t c1, uint8_t c2, uint8_t invert);
uint8_t EENibble(uint16_t adr);

/* x,y are pixel position on screen */
void tiny_print(uint8_t x, uint8_t y, const char *ptr);
void tiny_printp(uint8_t x, uint8_t y, const char *ptr);
uint8_t tiny_printpinv(uint8_t x, uint8_t y, const char *ptr, uint8_t invert);
void print_timer(uint8_t x, uint8_t y);

/* x,y are text char position on screen */
void printF(uint8_t x, uint8_t y, int32_t Data);
void printB(uint8_t y, int32_t Data);


void lcd_or_layers(void);
void bitmap(uint8_t x, uint8_t y, const uint8_t *BMP);

extern uint16_t SAD1; // Screen layer 1 pointer
extern uint16_t SAD2; // Screen layer 2 pointer (Graphics)
extern uint16_t SAD3; // Screen layer 3 pointer
extern uint16_t SAD4; // Screen layer 4 pointer (Graphics)
extern const char emptyline[];

#endif
