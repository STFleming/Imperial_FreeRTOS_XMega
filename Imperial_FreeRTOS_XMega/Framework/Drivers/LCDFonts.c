/*
 * LCDFonts.c
 *
 * Created: 18/07/2011 09:53:17
 *  Author: sf306
 */ 
#include "LCDFonts.h"
#include <avr/pgmspace.h>

// Small fonts, each character takes 5 nibbles (5 bytes contain 2 characters)
const uint8_t PROGMEM font[190] = {
    0x00, 0x00, 0x04, 0x44, 0x04,   // 0:   Space !
    0xAA, 0x00, 0x00, 0xAE, 0xEA,   // 1:   " #
    0x4E, 0xC6, 0xEA, 0x24, 0x8A,   // 2:   $ %
    0x4A, 0x4C, 0xE4, 0x40, 0x00,   // 3:   & '
    0x24, 0x44, 0x28, 0x44, 0x48,   // 4:   ( )
    0xA4, 0xA0, 0x00, 0x4E, 0x40,   // 5:   * +
    0x00, 0x04, 0x80, 0x0E, 0x00,   // 6:   , -
    0x00, 0x00, 0x42, 0x24, 0x88,   // 7:   . /
    0x4A, 0xAA, 0x44, 0xC4, 0x4E,   // 8:   0 1
    0xC2, 0x68, 0xEE, 0x26, 0x2E,   // 9:   2 3
    0xAA, 0xE2, 0x2E, 0x8C, 0x2C,   // 10:  4 5
    0xE8, 0xEA, 0xEE, 0x24, 0x44,   // 11:  6 7
    0xEA, 0xEA, 0xEE, 0xAE, 0x2E,   // 12:  8 9
    0x04, 0x04, 0x00, 0x40, 0x48,   // 13:  : ;
    0x24, 0x84, 0x20, 0xE0, 0xE0,   // 14:  < =
    0x84, 0x24, 0x8C, 0x24, 0x04,   // 15:  > ?
    0x0E, 0xEE, 0x2E, 0xAE, 0xAA,   // 16:  @ A
    0xEA, 0xCA, 0xEE, 0x88, 0x8E,   // 17:  B C
    0xCA, 0xAA, 0xCE, 0x8E, 0x8E,   // 18:  D E
    0xE8, 0xE8, 0x8E, 0x8A, 0xAE,   // 19:  F G
    0xAA, 0xEA, 0xAE, 0x44, 0x4E,   // 20:  H I
    0x22, 0x2A, 0xEA, 0xAC, 0xAA,   // 21:  J K
    0x88, 0x88, 0xEA, 0xEA, 0xAA,   // 22:  L M
    0xEA, 0xAA, 0xAE, 0xAA, 0xAE,   // 23:  N O
    0xEA, 0xE8, 0x84, 0xAA, 0xA6,   // 24:  P Q
    0xCA, 0xCA, 0xAE, 0x8E, 0x2E,   // 25:  R S
    0xE4, 0x44, 0x4A, 0xAA, 0xAE,   // 26:  T U
    0xAA, 0xAA, 0x4A, 0xAA, 0xEA,   // 27:  V W
    0xAA, 0x4A, 0xAA, 0xA4, 0x44,   // 28:  X Y
    0xE2, 0x48, 0xE6, 0x44, 0x46,   // 29:  Z [
    0x88, 0x42, 0x2C, 0x44, 0x4C,   // 30:  \ ]
    0x4A, 0x00, 0x00, 0x00, 0x0E,   // 31:  ^ _
    0x42, 0x00, 0x00, 0x0A, 0xAE,   // 32:  ` u     (a)
    0x00, 0xEA, 0xA2, 0x45, 0x99,   // 33:  n /     (b)  (c)
    0x44, 0xD5, 0xD0, 0x05, 0x52,   // 34:  div     (de)
    0x00, 0x32, 0x20, 0x0E, 0xAA,   // 35:  m       (fg)
    0x4E, 0x44, 0xCC, 0x44, 0xE4,   // 36:  up down (h)  (i)
    0x44, 0xAA, 0xE0, 0x00, 0x00    // 37:  delta   (j)
};
// Point
const uint8_t PROGMEM np[]= {
1,32, // width/8 , height
'\x00','\x00','\x1B','\x3C','\x7E','\x7E','\x03','\x3C',
};
// Blank
// Minus
const uint8_t PROGMEM nm[]= {
2,32, // width/8 , height
'\x00','\x00','\x1C','\xFF','\xF8','\xFF','\xF8','\xFF',
'\xF8','\xFF','\xF8','\xFF','\xF8','\x00','\x00','\x1A',
};
const uint8_t PROGMEM nc[]= {
2,32, // width/8 , height
'\x00','\x00','\x40',
};

// BIG number fonts
const uint8_t PROGMEM n0[]= {
3,32, // width/8 , height
'\x00','\xFF','\x00','\x03','\xFF','\xC0','\x0F','\xFF',
'\xE0','\x0F','\xFF','\xF0','\x1F','\xFF','\xF8','\x3F',
'\xC3','\xF8','\x3F','\x81','\xFC','\x3F','\x00','\xFC',
'\x3F','\x00','\xFC','\x7E','\x00','\x7C','\x7E','\x00',
'\x7E','\x7E','\x02','\x00','\x7E','\x7E','\x02','\x00',
'\x7E','\x7E','\x02','\x00','\x7E','\x7E','\x02','\x00',
'\x7E','\x7E','\x02','\x00','\x7E','\x7E','\x02','\x00',
'\x7E','\x7E','\x02','\x00','\x7E','\x7E','\x02','\x00',
'\x7E','\x7E','\x02','\x00','\x7E','\x7E','\x02','\x00',
'\x7E','\x7E','\x02','\x00','\x7E','\x3E','\x00','\x7E',
'\x3F','\x00','\xFC','\x3F','\x00','\xFC','\x3F','\x81',
'\xFC','\x1F','\xC3','\xFC','\x1F','\xFF','\xF8','\x0F',
'\xFF','\xF0','\x07','\xFF','\xF0','\x03','\xFF','\xC0',
'\x00','\xFF','\x00', 
};
const uint8_t PROGMEM n1[]= {
3,32, // width/8 , height
'\x00','\xFE','\x00','\x01','\xFE','\x00','\x07','\xFE',
'\x00','\x0F','\xFE','\x00','\x1F','\xFE','\x00','\x3F',
'\xFE','\x00','\x3F','\x7E','\x00','\x3E','\x7E','\x00',
'\x38','\x7E','\x00','\x30','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x00','\x02','\x7E','\x00','\x00','\x02',
'\x7E','\x00','\x3F','\xFF','\xFC','\x3F','\xFF','\xFC',
'\x3F','\xFF','\xFC','\x3F','\xFF','\xFC','\x3F','\xFF',
'\xFC',
};
const uint8_t PROGMEM n2[]= {
3,32, // width/8 , height
'\x00','\xFF','\x00','\x07','\xFF','\xC0','\x1F','\xFF',
'\xE0','\x1F','\xFF','\xF0','\x1F','\xFF','\xF0','\x1E',
'\x0F','\xF8','\x18','\x07','\xF8','\x00','\x03','\xF8',
'\x00','\x03','\xF8','\x00','\x03','\xF8','\x00','\x03',
'\xF8','\x00','\x03','\xF8','\x00','\x07','\xF0','\x00',
'\x07','\xF0','\x00','\x0F','\xE0','\x00','\x0F','\xC0',
'\x00','\x1F','\xC0','\x00','\x3F','\x80','\x00','\x7F',
'\x00','\x00','\x02','\xFE','\x00','\x01','\xFC','\x00',
'\x03','\xF8','\x00','\x07','\xF0','\x00','\x07','\xE0',
'\x00','\x0F','\xC0','\x00','\x1F','\xC0','\x00','\x1F',
'\x80','\x00','\x3F','\xFF','\xFC','\x3F','\xFF','\xFC',
'\x3F','\xFF','\xFC','\x3F','\xFF','\xFC','\x3F','\xFF',
'\xFC',
};
const uint8_t PROGMEM n3[]= {
3,32, // width/8 , height
'\x00','\xFF','\x00','\x07','\xFF','\xC0','\x1F','\xFF',
'\xE0','\x1F','\xFF','\xF0','\x1F','\xFF','\xF0','\x1E',
'\x0F','\xF8','\x18','\x07','\xF8','\x00','\x03','\xF8',
'\x00','\x03','\xF8','\x00','\x03','\xF8','\x00','\x03',
'\xF8','\x00','\x07','\xF0','\x00','\x1F','\xF0','\x07',
'\xFF','\xE0','\x07','\xFF','\xC0','\x07','\xFF','\x80',
'\x07','\xFF','\xE0','\x07','\xFF','\xF0','\x00','\x0F',
'\xF8','\x00','\x03','\xFC','\x00','\x03','\xFC','\x00',
'\x01','\xFC','\x00','\x01','\xFC','\x00','\x01','\xFC',
'\x00','\x01','\xFC','\x30','\x03','\xFC','\x3C','\x0F',
'\xF8','\x3F','\xFF','\xF8','\x3F','\xFF','\xF0','\x3F',
'\xFF','\xE0','\x0F','\xFF','\x80','\x03','\xFE','\x00',
};
const uint8_t PROGMEM n4[]= {
3,32, // width/8 , height
'\x00','\x1F','\xE0','\x00','\x1F','\xE0','\x00','\x3F',
'\xE0','\x00','\x3F','\xE0','\x00','\x7F','\xE0','\x00',
'\x7F','\xE0','\x00','\xFF','\xE0','\x00','\xFF','\xE0',
'\x01','\xFF','\xE0','\x01','\xF7','\xE0','\x03','\xF7',
'\xE0','\x03','\xE7','\xE0','\x07','\xE7','\xE0','\x07',
'\xC7','\xE0','\x0F','\xC7','\xE0','\x0F','\x87','\xE0',
'\x1F','\x87','\xE0','\x1F','\x07','\xE0','\x3F','\x07',
'\xE0','\x3F','\x07','\xE0','\x3F','\xFF','\xFC','\x3F',
'\xFF','\xFC','\x3F','\xFF','\xFC','\x3F','\xFF','\xFC',
'\x3F','\xFF','\xFC','\x00','\x07','\xE0','\x00','\x07',
'\xE0','\x00','\x07','\xE0','\x00','\x07','\xE0','\x00',
'\x07','\xE0','\x00','\x07','\xE0','\x00','\x07','\xE0',
};
const uint8_t PROGMEM n5[]= {
3,32, // width/8 , height
'\x07','\xFF','\xF0','\x07','\xFF','\xF0','\x07','\xFF',
'\xF0','\x07','\xFF','\xF0','\x07','\xFF','\xF0','\x07',
'\xC0','\x00','\x0F','\xC0','\x00','\x0F','\xC0','\x00',
'\x0F','\x80','\x00','\x0F','\x80','\x00','\x0F','\x80',
'\x00','\x0F','\x80','\x00','\x0F','\xFF','\x00','\x0F',
'\xFF','\xC0','\x0F','\xFF','\xF0','\x0F','\xFF','\xF0',
'\x0F','\xFF','\xF8','\x00','\x07','\xF8','\x00','\x01',
'\xFC','\x00','\x00','\x02','\xFC','\x00','\x00','\x02',
'\xFC','\x00','\x00','\x02','\xFC','\x00','\x00','\x02',
'\xFC','\x00','\x00','\x02','\xFC','\x38','\x01','\xFC',
'\x3E','\x01','\xF8','\x3F','\x87','\xF8','\x3F','\xFF',
'\xF0','\x3F','\xFF','\xF0','\x1F','\xFF','\xC0','\x07',
'\xFF','\x80','\x01','\xFE','\x00',
};
const uint8_t PROGMEM n6[]= {
3,32, // width/8 , height
'\x00','\x3F','\xC0','\x00','\xFF','\xF0','\x01','\xFF',
'\xF0','\x03','\xFF','\xF0','\x07','\xFF','\xF0','\x0F',
'\xF0','\x30','\x0F','\xC0','\x00','\x1F','\x80','\x00',
'\x1F','\x80','\x00','\x1F','\x00','\x00','\x02','\x1F',
'\x00','\x00','\x02','\x3F','\x00','\x00','\x02','\x3F',
'\x3F','\x02','\x80','\x3F','\xFF','\xE0','\x3F','\xFF',
'\xF0','\x3F','\xFF','\xF8','\x3F','\xFF','\xF8','\x3F',
'\xC3','\xFC','\x3F','\x01','\xFC','\x3F','\x00','\xFC',
'\x3F','\x00','\xFC','\x3F','\x00','\xFC','\x3F','\x00',
'\xFC','\x3F','\x00','\xFC','\x1F','\x80','\xFC','\x1F',
'\x81','\xF8','\x1F','\xC3','\xF8','\x0F','\xFF','\xF0',
'\x0F','\xFF','\xF0','\x07','\xFF','\xE0','\x03','\xFF',
'\x80','\x00','\xFE','\x00',
};
const uint8_t PROGMEM n7[]= {
3,32, // width/8 , height
'\x3F','\xFF','\xFC','\x3F','\xFF','\xFC','\x3F','\xFF',
'\xFC','\x3F','\xFF','\xFC','\x3F','\xFF','\xFC','\x3F',
'\xFF','\xFC','\x00','\x03','\xF8','\x00','\x03','\xF8',
'\x00','\x03','\xF0','\x00','\x07','\xF0','\x00','\x07',
'\xF0','\x00','\x07','\xE0','\x00','\x0F','\xE0','\x00',
'\x0F','\xC0','\x00','\x0F','\xC0','\x00','\x1F','\xC0',
'\x00','\x1F','\x80','\x00','\x3F','\x80','\x00','\x3F',
'\x80','\x00','\x3F','\x00','\x00','\x02','\x7F','\x00',
'\x00','\x02','\x7F','\x00','\x00','\x02','\x7E','\x00',
'\x00','\x02','\xFE','\x00','\x00','\x02','\xFC','\x00',
'\x01','\xFC','\x00','\x01','\xFC','\x00','\x03','\xF8',
'\x00','\x03','\xF8','\x00','\x07','\xF8','\x00','\x07',
'\xF0','\x00','\x07','\xF0','\x00',
};
const uint8_t PROGMEM n8[]= {
3,32, // width/8 , height
'\x00','\x7F','\x00','\x01','\xFF','\xC0','\x07','\xFF',
'\xE0','\x0F','\xFF','\xF0','\x0F','\xFF','\xF0','\x1F',
'\xC3','\xF8','\x1F','\x81','\xF8','\x1F','\x81','\xF8',
'\x1F','\x81','\xF8','\x1F','\x81','\xF8','\x1F','\x81',
'\xF8','\x0F','\xC3','\xF0','\x0F','\xFF','\xF0','\x07',
'\xFF','\xE0','\x03','\xFF','\xC0','\x01','\xFF','\x80',
'\x01','\xFF','\x80','\x07','\xFF','\xE0','\x0F','\xFF',
'\xF0','\x1F','\xC3','\xF8','\x1F','\x81','\xFC','\x3F',
'\x00','\xFC','\x3F','\x00','\xFC','\x3F','\x00','\xFC',
'\x3F','\x00','\xFC','\x3F','\x81','\xFC','\x3F','\xC3',
'\xF8','\x1F','\xFF','\xF8','\x1F','\xFF','\xF0','\x0F',
'\xFF','\xE0','\x07','\xFF','\xC0','\x00','\xFF','\x00',
};
const uint8_t PROGMEM n9[]= {
3,32, // width/8 , height
'\x00','\x7F','\x00','\x01','\xFF','\xC0','\x07','\xFF',
'\xE0','\x0F','\xFF','\xF0','\x0F','\xFF','\xF0','\x1F',
'\xC3','\xF8','\x1F','\x81','\xF8','\x3F','\x01','\xF8',
'\x3F','\x00','\xFC','\x3F','\x00','\xFC','\x3F','\x00',
'\xFC','\x3F','\x00','\xFC','\x3F','\x00','\xFC','\x3F',
'\x80','\xFC','\x3F','\xC3','\xFC','\x1F','\xFF','\xFC',
'\x1F','\xFF','\xFC','\x0F','\xFF','\xFC','\x07','\xFF',
'\xFC','\x01','\xFC','\xFC','\x02','\x00','\x00','\x02',
'\xFC','\x00','\x00','\x02','\xF8','\x00','\x00','\x02',
'\xF8','\x00','\x01','\xF8','\x00','\x01','\xF8','\x00',
'\x03','\xF0','\x0C','\x0F','\xF0','\x0F','\xFF','\xE0',
'\x0F','\xFF','\xC0','\x0F','\xFF','\x80','\x0F','\xFF',
'\x00','\x03','\xFC','\x00',
};

const uint8_t *bnumbers[] = { n0,n1,n2,n3,n4,n5,n6,n7,n8,n9 };