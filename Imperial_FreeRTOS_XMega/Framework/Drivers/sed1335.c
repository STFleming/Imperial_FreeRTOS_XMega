/*****************************************************************************

Xmegalab / XMultiKit:
XMEGA Oscilloscope and Development Kit

Gabotronics
July 2010

Copyright 2009 Gabriel Anzziani

This program is distributed under the terms of the GNU General Public License 

www.gabotronics.com
email me at: gabriel@gabotronics.com                                        */

/***********************************************************************
Includes
***********************************************************************/

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "sed1335.h"
#include <avr/pgmspace.h>
//#include "main.h"

uint8_t CR;         // Address range covered by one display line
uint8_t LCDmode;    // Text, GraphText, Graphics
uint16_t SAD1;      // Screen layer 1 pointer
uint16_t SAD2;      // Screen layer 2 pointer (Graphics)
uint16_t SAD3;      // Screen layer 3 pointer
uint16_t SAD4;      // Screen layer 4 pointer (Graphics)
const char emptyline[] PROGMEM = "                     ";

/***********************************************************************
Write specified command to LCD panel.
This is an internal function...
***********************************************************************/
void lcd_write_command(uint8_t command) {
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    setbit(LCDCTLPORT,A0);// LCDCTLSET = A0;
    clrbit(LCDCTLPORT,WR);//LCDCTLCLR = WR;
    LCDDATAPORT = command;
    asm("lpm");
    asm("lpm");
    setbit(LCDCTLPORT,WR);//LCDCTLSET = WR;
    if(testbit(Stat, CPU_I_bp)) sei();
}

/***********************************************************************
Write specified data to LCD panel.
This is an internal function...
***********************************************************************/
void lcd_write_data(uint8_t data) {
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    //clrbit(VPORT3.OUT, A0);
    clrbit(LCDCTLPORT,A0);//LCDCTLCLR = A0;
    clrbit(LCDCTLPORT,WR);//LCDCTLCLR = WR;
    LCDDATAPORT = data;
    asm("lpm");
    asm("nop");
    asm("nop");
    setbit(LCDCTLPORT,WR);//LCDCTLSET = WR;
    if(testbit(Stat, CPU_I_bp)) sei();
}

/***********************************************************************
Read data from LCD panel at position given by lcd_pixel command.
This is an internal function...
************************************************************************/
uint8_t lcd_read_data(void) {
    uint8_t data;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    setbit(LCDCTLPORT,A0);//LCDCTLSET = A0;
    clrbit(LCDCTLPORT,RD);//LCDCTLCLR = RD;
    LCDDATADDR = 0x00;
    asm("lpm");
    asm("nop");
    data = LCDDATAPIN;
    setbit(LCDCTLPORT,RD);//LCDCTLSET = RD;     
    LCDDATADDR = 0xff;
    if(testbit(Stat, CPU_I_bp)) sei();
    return data;
}

/***********************************************************************
Initialize the LCD controller. Read the documentation for the 
controller in use. If any other than SED1335 values can be wrong ! 
***********************************************************************/
void lcd_init(uint8_t mode) {
    uint16_t i;
    uint8_t FX;
    LCDmode=mode;
    if(mode==TEXT) {
        FX = 5;  // Horizontal character size - 1
        CR=(LCD_X_SIZE/(FX+1)-1);       // Address range covered by one display line
        SAD1 = 0; // Screen layer 1 pointer
        SAD2 = 3*(uint16_t)SL1*APL;     // Screen layer 2 pointer (Graphics)
    }
    else {
        FX = 7;
        CR=(LCD_X_SIZE/(FX+1)-1);       // Address range covered by one display line
        SAD1 = (uint16_t)LCD_Y_SIZE*APL; // Screen layer 1 pointer
        SAD2 = 0x00;                    // Screen layer 2 pointer (Graphics)
    }

    if(mode==TEXT || mode==GRAPHTEXT) {
        SAD3 = SAD1 + (APL*(SL1/(FY+1)));
    }
    else {
        SAD3 = 3*(uint16_t)SL1*APL;
    }

    SAD4 = (uint16_t)SL1*APL;

    LCDDATAPORT = 0x00;                 // Setup LCD Data bus port
//    LCDDATADDR = 0xFF;                // Port direction already set at main
    setbit(LCDCTLPORT,WR); setbit(LCDCTLPORT,RD);//LCDCTLSET = WR | RD;                // Setup LCD control bus port
//    LCDCTLDDR  = CS | RST | WR | RD | A0; // already set at main
//    delay_ms(2);                        // Reset the LCD
//    LCDCTLPORT &= ~RST;
    _delay_ms(100);
//    LCDCTLPORT |= RST;
    // System
    lcd_write_command(SYSTEM_SET);                   // C: SYSTEM SET command
    lcd_write_data(0x10|IV<<5|WS<<3|M2<<2|M1<<1|M0); // P1: 0 0 IV 1 W/S M2 M1 M0
    lcd_write_data((WF<<7)+(0x07 & FX));        // P2: WF 0 0 0 0 FX2 FX1 FX0
    lcd_write_data(0x0F & FY);                  // P3: 0 0 0 0 FY3 FY2 FY1 FY0
    lcd_write_data(CR);                         // P4: C/R
    lcd_write_data(TCR);                        // P5: TC/R   
    lcd_write_data(LF);                         // P6: L/F
    lcd_write_data(APL);                        // P7: APL
    lcd_write_data(APH);                        // P8: APH
    lcd_scroll(0,0,0,0);
    // Horizontal scroll
    lcd_write_command(HDOT_SCR);
    lcd_write_data(0x00);     
    // Overlay
    lcd_write_command(OVLAY);
    lcd_write_data(mode);           // mode=TEXT:       Screen 1 & 3 Text,
                                    // mode=GRAPHICS:   Screen 1 & 3 Graphics
                                    // Screen 2 & 4 Graphics only
/*    // CGRAM Address
    lcd_write_command(CGRAM_ADR);
    lcd_write_data(SAGL);           // SAGL
    lcd_write_data(SAGH);           // SAGH*/
    // Display On/Off I
    lcd_write_command(DISP_OFF);    // Display off
    lcd_write_data(0x14);
    // Cursor write  
    lcd_write_command(CSRW);
    lcd_write_data(0x00); 
    lcd_write_data(0x00);
    // Cursor format
    lcd_write_command(CSRFORM); 
    lcd_write_data(0x05);    
    lcd_write_data(0x87);    
    // Curson direction  
    lcd_write_command(CSR_RIGHT);
    // Display On/Off II
    lcd_write_command(DISP_ON);     // Display on
    lcd_write_data(0x54);           // Cursor flash off

    // Clear memory
    lcd_write_command(CSR_RIGHT);
    lcd_write_command(CSRW);    // Set cursor address
    lcd_write_data(0);          
    lcd_write_data(0);      
    lcd_write_command(MWRITE);    // Write to display memory
    for(i=DISPMEM; i; i--) lcd_write_data(0);
    lcd_goto(0,0);
}

void lcd_scroll(int8_t S1, int8_t S2, int8_t S3, int8_t S4) {
    SAD1 += S1;
    SAD2 += S2;
    SAD3 += S3;
    SAD4 += S4;
    // Scroll
    lcd_write_command(SCROLL); 
    lcd_write_data(lobyte(SAD1));       // SAD1L
    lcd_write_data(hibyte(SAD1));       // SAD1H
    lcd_write_data(SL1);                // SL1
    lcd_write_data(lobyte(SAD2));       // SAD2L
    lcd_write_data(hibyte(SAD2));       // SAD2H
    lcd_write_data(SL2);                // SL2
    lcd_write_data(lobyte(SAD3));       // SAD3L
    lcd_write_data(hibyte(SAD3));       // SAD3H
    lcd_write_data(lobyte(SAD4));       // SAD4L
    lcd_write_data(hibyte(SAD4));       // SAD4H
}

/***********************************************************************
Clears the text layer / screen.
***********************************************************************/
void lcd_clear_text(void) {
    uint16_t i;
    if(LCDmode==TEXT) {
        SAD1 = 0;
        SAD3 = APL*(SL1/(FY+1));
    }
    lcd_scroll(0,0,0,0);
    lcd_write_command(CSR_RIGHT);
    lcd_write_command(CSRW);
    lcd_write_data(lobyte(SAD1));
    lcd_write_data(hibyte(SAD1));      
    lcd_write_command(MWRITE);   
    for(i=0; i<APL*(LCD_Y_SIZE/FY); i++) lcd_write_data(' ');
    lcd_goto(0,0);
}

/***********************************************************************
Goto specified column and line. 0,0 is the upper left corner.
***********************************************************************/
void lcd_goto(uint8_t column, uint8_t row) {
    uint16_t addr;
    uint8_t lo;
    uint8_t hi;
    addr = row*APL + column;
    lo = lobyte(SAD1)+lobyte(addr);
    hi = hibyte(SAD1)+hibyte(addr);
    lcd_write_command(CSRW);
    lcd_write_data(lo);
    lcd_write_data(hi);
}

void lcd_set_cursor_addr(uint16_t addr) {
    lcd_write_command(CSRW);   // Write cursor
    lcd_write_data(lobyte(addr));
    lcd_write_data(hibyte(addr));
}

uint16_t lcd_read_cursor_addr(void) {
    uint16_t addr;
    lcd_write_command(CSRR);   // Read cursor
    addr = lcd_read_data();
    addr += lcd_read_data()<<8;
    return addr;
}

void NewLine(void) {
    uint16_t addr;
    addr = lcd_read_cursor_addr();          // Read current address
    addr = addr+21-addr%21;
    lcd_set_cursor_addr(addr);              // next line
/*    while(addr>=SAD1+(21*16) && SAD1<1024) {    // scroll down
        lcd_putsp(emptyline);
        lcd_set_cursor_addr(addr);
        lcd_scroll(21,0,21,0);
    }*/
    if(addr>=SAD1+(21*16)) {    // scroll down
        lcd_putsp(emptyline);
        lcd_set_cursor_addr(addr);
        lcd_scroll(21,0,21,0);
    }
    if (SAD1>1024) lcd_clear_text();
}

/***********************************************************************
Write strings to the text layer. Set position with lcd_goto.
Text will wrap if to long to show on one line.
**********************************************************************
void lcd_write_string(char *ptr) {
    lcd_write_command(MWRITE);
    while (*ptr != 0x00) lcd_write_data(*ptr++);
}*/

/***********************************************************************
Write strings to the text layer. Set position with lcd_goto.
Text will wrap if to long to show on one line. DATA FROM PMEM
***********************************************************************/
void lcd_putsp(const char *ptr) {
    while (pgm_read_byte(ptr) != 0x00) put_char(pgm_read_byte(ptr++));
}

void lcd_puts(const char *ptr) {
    while (*ptr!=0x00) put_char(*ptr++);
}

void put_char(char c) {
    uint16_t addr;
    if(c=='\n') {
        addr = lcd_read_cursor_addr();
        if(addr%21) NewLine();
    }
    else {
        lcd_write_command(MWRITE);
        lcd_write_data(c);
        if(LCDmode==TEXT) {
            addr = lcd_read_cursor_addr();          // Read current address
            if(addr>SAD1+(21*16)) {    // scroll down
                lcd_set_cursor_addr(addr);
                lcd_scroll(21,0,21,0);
            }
            if (SAD1>1024) lcd_clear_text();
        }
    }
}

// Print Number (text mode)
void printN(int32_t Data) {
    uint8_t D1=0,D2=0,D3=0,D4=0,D5=0;
    lcd_write_command(MWRITE);
    if(Data<0) {
        Data=-Data;
        lcd_write_data('-');
    }
    else lcd_write_data(' ');
	while (Data>=10000)	{ D5++; Data-=10000; }
	while (Data>=1000)	{ D4++; Data-=1000; }
	while (Data>=100)	{ D3++; Data-=100; }
	while (Data>=10)	{ D2++; Data-=10; }
	while (Data>=1)		{ D1++; Data-=1; }
    if(D5) lcd_write_data(0x30+D5);
    if(D5 || D4) lcd_write_data(0x30+D4);
    if(D5 || D4 || D3) lcd_write_data(0x30+D3);
    else lcd_write_data(' ');
    lcd_write_data(0x30+D2);
    lcd_write_data(0x30+D1);
    lcd_write_data(' ');
}

/***********************************************************************
Show or hide cursor. 0=Hide, 1=no flashing, 2=2Hz, 3=1Hz
***********************************************************************/
void lcd_show_cursor(uint8_t rate) {
    if(rate>=4) return;
    lcd_write_command(DISP_ON);
    lcd_write_data(0x54+rate);
}

/***********************************************************************
Set cursor type. 0 = Block cursor, 1=Underline cursor
***********************************************************************/
void lcd_cursor_type(uint8_t type) {
    lcd_write_command(CSRFORM);     // Cursor format
    lcd_write_data(0x05);    
    if(type) lcd_write_data(0x07);
    else  lcd_write_data(0x87);
}

/***********************************************************************
Clears the graphic layer
***********************************************************************/
void lcd_clear_graphics(void) {
    uint16_t i;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    lcd_write_command(CSR_RIGHT);
    lcd_write_command(CSRW);    // Set cursor address
    lcd_write_data(lobyte(SAD2));          
    lcd_write_data(hibyte(SAD2));      
    lcd_write_command(MWRITE);    // Write to display memory
    for(i=2048; i; i--) lcd_write_data(0);
    if(testbit(Stat, CPU_I_bp)) sei();
}

/***********************************************************************
Clears the second graphic layer.
***********************************************************************/
void lcd_clear_graphics2(void) {
    uint16_t i;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    lcd_write_command(CSR_RIGHT);
    lcd_write_command(CSRW);        // Set cursor address
    lcd_write_data(0);          
    lcd_write_data(8);      
    lcd_write_command(MWRITE);      // Write to display memory
    for(i=2048; i; i--) lcd_write_data(0);
    if(testbit(Stat, CPU_I_bp)) sei();
}

/***********************************************************************
Write pixel data to the display at specified position.
***********************************************************************/
void set_pixel(uint8_t x, uint8_t y) {
    uint16_t Address;
    uint8_t Offset;
    uint8_t lo;
    uint8_t hi;
    uint8_t data;
    Address = (y<<4) + ((uint8_t)(x>>3));  // Optimized for 128x128 display
//  Address = (y*APL) + (x/(FX+1));     // Generic display
    lo = (uint8_t)  (Address & 0x00ff);
    hi = (uint8_t) ((Address & 0xff00) >> 8);
    lcd_write_command(CSRW);        // Set cursor address
    lcd_write_data(lo);
    lcd_write_data(hi);
    lcd_write_command(MREAD);       // Read data
    Offset = (uint8_t)(0x80 >> (x & 0x07));
    data=lcd_read_data();       // Read LCD data

    if(!(data & Offset)) {      // pixel already on?
        data |= Offset;         // SET
        lcd_write_command(CSRW);            // Set cursor address
        lcd_write_data(lo);    
        lcd_write_data(hi);   
        lcd_write_command(MWRITE);
        lcd_write_data(data); 
    }
}

/***********************************************************************
Write pixel data to the display at specified position.
***********************************************************************/
void clr_pixel(uint8_t x, uint8_t y) {
    uint16_t Address;
    uint8_t Offset;
    uint8_t lo;
    uint8_t hi;
    uint8_t data;
    Address = (y<<4) + ((uint8_t)(x>>3));  // Optimized for 128x128 display
//  Address = (y*APL) + (x/(FX+1));     // Generic display
    lo = (uint8_t)  (Address & 0x00ff);
    hi = (uint8_t) ((Address & 0xff00) >> 8);
    lcd_write_command(CSRW);        // Set cursor address
    lcd_write_data(lo);
    lcd_write_data(hi);
    lcd_write_command(MREAD);       // Read data
    Offset = (uint8_t)(0x80 >> (x & 0x07));
    data=lcd_read_data();       // Read LCD data

    if(data&Offset) {               // Pixel already off?
        data &= ~Offset;            // CLEAR
        lcd_write_command(CSRW);    // Set cursor address
        lcd_write_data(lo);    
        lcd_write_data(hi);   
        lcd_write_command(MWRITE);
        lcd_write_data(data); 
    }
}

/***********************************************************************
Draws a rectangle from x1,y1 to x2,y2.
Set show to 1 to draw pixel, set to 0 to hide pixel.
***********************************************************************/
void lcd_rectangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t i;
    for (i=x1; i<=x2; i++) { set_pixel(i,y1); set_pixel(i,y2); }
    for (i=y1; i<=y2; i++) { set_pixel(x1,i); set_pixel(x2,i); }
}

/***********************************************************************
Draws a circle with center at x,y with given radius.
Set show to 1 to draw pixel, set to 0 to hide pixel.
************************************************************************/
void lcd_circle(uint8_t x, uint8_t y, uint8_t radius) {
    int xc = 0;
    int yc = radius;
    int p = 3 - (radius<<1);
    while (xc <= yc) {
        set_pixel(x + xc, y + yc);
        set_pixel(x + xc, y - yc);
        set_pixel(x - xc, y + yc);
        set_pixel(x - xc, y - yc);
        set_pixel(x + yc, y + xc);
        set_pixel(x + yc, y - xc);
        set_pixel(x - yc, y + xc);
        set_pixel(x - yc, y - xc);
        if (p < 0) p += (xc++ << 2) + 6;
        else p += ((xc++ - yc--)<<2) + 10;
    }
}

void lcd_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    uint8_t i,dxabs,dyabs,x,y;
    int8_t dx,dy,stepx,stepy;
    dx=(int8_t)x2-x1;      // the horizontal distance of the line
    dy=(int8_t)y2-y1;      // the vertical distance of the line
    if(dy<0) { dyabs=-dy; stepy=-1; }
    else { dyabs=dy; stepy=1; }
    if(dx<0) { dxabs=-dx; stepx=-1; }
    else {dxabs=dx; stepx=1; }
    x=(uint8_t)(dyabs>>1);
    y=(uint8_t)(dxabs>>1);
    set_pixel(x1,y1);
    if (dxabs>=dyabs) { // the line is more horizontal than vertical
        for(i=0;i<dxabs;i++) {
            y+=dyabs;
            if (y>=dxabs) {
                y-=dxabs;
                y1+=stepy;
            }
            x1+=stepx;
            set_pixel(x1,y1);
        }
    }
    else {  // the line is more vertical than horizontal
        for(i=0;i<dyabs;i++) {
            x+=dxabs;
            if (x>=dyabs) {
                x-=dyabs;
                x1+=stepx;
            }
            y1+=stepy;
            set_pixel(x1,y1);
        }
    }
}

// Read Nibble from EEPROM
uint8_t EENibble(uint16_t adr) {
    uint8_t a;
    if(adr&1) {
        a=adr>>1;
        return 0x0F & pgm_read_byte(&font[a]);
        
    }
    else {
        a=adr>>1;
        return (uint8_t)(pgm_read_byte(&font[a])>>4);
    }
}

// put_2char function
// Cursor increment has to be set to down
void put_2char(uint8_t c1, uint8_t c2) {
    uint8_t pixels;
    uint16_t a1, a2;
    if(c1) a1=(uint16_t)(c1-0x20)*5;    // Set index to desired character
    else a1=0;
    a2=(uint16_t)(c2-0x20)*5;           // Set index to desired character
    lcd_write_command(CSR_DOWN);        // LCD cursor will move down on each write
    lcd_write_command(MWRITE);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    lcd_write_data(pixels);
    lcd_write_command(CSR_RIGHT);
}

// put_2char function with invert option
// Cursor increment has to be set to down
void put_2charinv(uint8_t c1, uint8_t c2, uint8_t invert) {
    uint8_t pixels;
    uint16_t a1, a2;
    if(c1) a1=(uint16_t)(c1-0x20)*5;    // Set index to desired character
    else a1=0;
    a2=(uint16_t)(c2-0x20)*5;           // Set index to desired character
    lcd_write_command(CSR_DOWN);        // LCD cursor will move down on each write
    lcd_write_command(MWRITE);

    pixels = 0;
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);             // Upper line
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);
    pixels = (uint8_t)(EENibble(a2++)<<4) | EENibble(a1++);
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);
    pixels = 0;
    if(invert) pixels = ~pixels;
    lcd_write_data(pixels);             // Lower line
    lcd_write_command(CSR_RIGHT);
}

// Print small font text from program memory with invert option
// Functions expects that the size of the string is multiple of 2 (not including \0)
// Functions returns the number of characters printed
uint8_t tiny_printpinv(uint8_t x, uint8_t y, const char *ptr, uint8_t invert) {
    uint8_t c1,c2;
    uint16_t addr;
    uint8_t lo;
    uint8_t hi;
    uint8_t n=0;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    while (pgm_read_byte(ptr)) {
        addr = (y<<4) + x++;
        lo = (uint8_t)  (addr & 0x00ff);
        hi = (uint8_t) ((addr & 0xff00) >> 8);
        // If GRAPHICS mode, print on second layer
        if(LCDmode==GRAPHICS) hi += hibyte(SAD1);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(lo);
        lcd_write_data(hi);
        c1 = pgm_read_byte(ptr++);
        c2 = pgm_read_byte(ptr++);
        n+=2;
        put_2charinv(c2, c1, invert);
        if(c2==0) break;
    }
    if(testbit(Stat, CPU_I_bp)) sei();
    return n;
}

// Print small font text from memory
void tiny_print(uint8_t x, uint8_t y, const char *ptr) {
    uint8_t c1,c2;
    uint16_t addr;
    uint8_t lo;
    uint8_t hi;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    while (*ptr) {
        addr = (y<<4) + x++;
        lo = (uint8_t)  (addr & 0x00ff);
        hi = (uint8_t) ((addr & 0xff00) >> 8);
        // If GRAPHICS mode, print on second layer
        if(LCDmode==GRAPHICS) hi += hibyte(SAD1);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(lo);
        lcd_write_data(hi);

        c1 = *ptr++;
        c2 = *ptr++;
        put_2char(c2, c1);
        if(c2==0) break;
    }
    if(testbit(Stat, CPU_I_bp)) sei();
}

// Print small font text from program memory
void tiny_printp(uint8_t x, uint8_t y, const char *ptr) {
    uint8_t c1,c2;
    uint16_t addr;
    uint8_t lo;
    uint8_t hi;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    while (pgm_read_byte(ptr)) {
        addr = (y<<4) + x++;
        lo = (uint8_t)  (addr & 0x00ff);
        hi = (uint8_t) ((addr & 0xff00) >> 8);
        // If GRAPHICS mode, print on second layer
        if(LCDmode==GRAPHICS) hi += hibyte(SAD1);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(lo);
        lcd_write_data(hi);

        c1 = pgm_read_byte(ptr++);
        c2 = pgm_read_byte(ptr++);
        put_2char(c2, c1);
        if(c2==0) break;
    }
    if(testbit(Stat, CPU_I_bp)) sei();
}

/*// Print timer
void print_timer(uint8_t x, uint8_t y) {
    char S[9]={'0','0',':','0','0',':','0','0','\0'};
    uint8_t n;
    // Stop interrupt while accessing timer variables
    RTC.INTCTRL = 0;
    n=hours;
	while (n>=10)	{ S[0]++; n-=10; }
	while (n>=1)	{ S[1]++; n-=1; }
    n=minutes;
	while (n>=10)	{ S[3]++; n-=10; }
	while (n>=1)	{ S[4]++; n-=1; }
    n=RTC.CNTL;
	while (n>=10)	{ S[6]++; n-=10; }
	while (n>=1)	{ S[7]++; n-=1; }
    // Re-enable interrupt
    RTC.INTCTRL = 1;
    if(LCDmode==GRAPHICS) {
        lcd_goto(x++,y); put_2char(S[1], S[0]);
        lcd_goto(x++,y); put_2char(S[3], S[2]);
        lcd_goto(x++,y); put_2char(S[5], S[4]);
        lcd_goto(x++,y); put_2char(S[7], S[6]);
    }
    else {
        lcd_goto(x,y);
        lcd_puts(S);
    }
}*/

// Print Fixed point Number
// 5 decimal digits
void printF(uint8_t x, uint8_t y, int32_t Data) {
	uint8_t D[4]={0,0,0,0},point=0,i,j=1;
    char S[7];
	uint16_t n;
    uint8_t Stat;
    Stat = SREG;        // Save global interrupt bit
    cli();              // Disable interrupts
    S[6]=0; // End of string
	if(Data<0) { S[0]='-'; Data = -Data; }
    else S[0]=' ';
	if(Data>=999900000L) n = 9999;
	else if(Data>=100000000L)  n = (uint16_t)(Data/100000);
	else if(Data>=10000000L) {
		n = (uint16_t)(Data/10000);
		point = 1;
	}
	else if(Data>=1000000L) {
		n = (uint16_t)(Data/1000);
		point = 2;
	}
	else {
		n = (uint16_t)(Data/100);
		point = 3;
	}

	while (n>=1000) { D[3]++; n-=1000; }
	while (n>=100)	{ D[2]++; n-=100; }
	while (n>=10)	{ D[1]++; n-=10; }
	while (n>=1)	{ D[0]++; n-=1; }
	for(i=3; i!=255; i--) {
		S[j++]=0x30+D[i];
		if(point==i) S[j++]='.';
	}	
    if(LCDmode==GRAPHICS) {
        lcd_goto(x++,y); put_2char(S[1], S[0]);
        lcd_goto(x++,y); put_2char(S[3], S[2]);
        lcd_goto(x++,y); put_2char(S[5], S[4]);
    }
    else {
        lcd_goto(x,y);
        lcd_puts(S);
    }
    if(testbit(Stat, CPU_I_bp)) sei();
}

// Print Fixed point Number - BIG FONT
// 5 decimal digits
void printB(uint8_t y, int32_t Data) {
	uint8_t D[4]={0,0,0,0},point=0,i,j=2;
	uint16_t n;
	if(Data<0) {
        bitmap(0,y,nm);
        Data = -Data;
    }
    else bitmap(0,y,nc);
	if(Data>=999900000L) n = 9999;
	else if(Data>=100000000L)  n = (uint16_t)(Data/100000);
	else if(Data>=10000000L) {
		n = (uint16_t)(Data/10000);
		point = 1;
	}
	else if(Data>=1000000L) {
		n = (uint16_t)(Data/1000);
		point = 2;
	}
	else {
		n = (uint16_t)(Data/100);
		point = 3;
	}

	while (n>=1000) { D[3]++; n-=1000; }
	while (n>=100)	{ D[2]++; n-=100; }
	while (n>=10)	{ D[1]++; n-=10; }
	while (n>=1)	{ D[0]++; n-=1; }
	for(i=3; i!=255; i--) {
        bitmap(j,y,bnumbers[D[i]]);
        j+=3;
		if(point==i) {
            bitmap(j,y,np);
            j++;
        }      
	}
}

void lcd_or_layers(void) {
    uint16_t Address;
    uint8_t low, high,data;
    // OR second layer on to first layer
    // Only works in LCD GRAPHICS Mode!
    for(Address=0; Address<2048; Address++) {
        // Read byte from second layer
        low = (uint8_t) (  (Address+2048) & 0x00ff);
        high = (uint8_t) (( (Address+2048) & 0xff00) >> 8);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(low);
        lcd_write_data(high);
        lcd_write_command(MREAD);       // Read data
        data=lcd_read_data();

        // Read byte from first layer and OR
        low = (uint8_t) (  (Address) & 0x00ff);
        high = (uint8_t) (( (Address) & 0xff00) >> 8);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(low);
        lcd_write_data(high);
        lcd_write_command(MREAD);       // Read data
        data|=lcd_read_data();
    
        // Write back to fisrt layer
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(low);
        lcd_write_data(high);
        lcd_write_command(MWRITE);       // Read data
        lcd_write_data(data);
    }
}

/*----------------------------------------------------------------------------
Send a run length encoded image from program memory to the LCD
	Decode algorithm:
	Get one byte, put it to the output file, and now it's the 'last' byte. 
	Loop 
	Get one byte 
	Is the current byte equal to last? 
	Yes 
		Now get another byte, this is 'counter' 
		Put current byte in the output file 
		Copy 'counter' times the 'last' byte to the output file 
		Put last copied byte in 'last' (or leave it alone) 
	No 
		Put current byte to the output file 
		Now 'last' is the current byte 
	Repeat. 

    BMP[0] contains width/8
    BMP[1] contains height
----------------------------------------------------------------------------*/
void bitmap(uint8_t x, uint8_t y, const uint8_t *BMP) {
	uint8_t u8Page=0;
  	uint8_t u8Column=0;
	uint8_t data=0,count=0,low,high,width,height;
	uint16_t i=0,Address;
    width=pgm_read_byte(&BMP[i++]);
    height=pgm_read_byte(&BMP[i++]);
  	for (u8Page = y; u8Page < (y+height); u8Page++) { /* loop on the 8 pages */
//        lcd_goto(x,u8Page);
        Address = (u8Page<<4)+x;  // Optimized for 128x128 display
        low = (uint8_t) (Address & 0x00ff);
        high = (uint8_t) ((Address & 0xff00) >> 8);
        lcd_write_command(CSRW);        // Set cursor address
        lcd_write_data(low);
        lcd_write_data(high);
        lcd_write_command(MWRITE);
		for (u8Column=0; u8Column<width; u8Column++) {
			if(count==0) {
				data = pgm_read_byte(&BMP[i++]);
				if(data==pgm_read_byte(&BMP[i++])) {
					count = pgm_read_byte(&BMP[i++]);
				}
				else {
					count = 1;
					i--;
				}
			}
			count--;
			lcd_write_data(data);
		}
	}
}
/*
void bitmap2(byte x, byte y, const byte *BMP) {
	unsigned short i=0;
    lcd_goto(0,0);
    lcd_write_command(MWRITE);
  	for (i = 0; i < 2048; i++) {
		lcd_write_data(BMP[i]);
	}
}*/


/***********************************************************************
Happy end :-)
***********************************************************************/
