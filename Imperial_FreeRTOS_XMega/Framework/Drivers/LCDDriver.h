/*
 * IncFile1.h
 *
 * Created: 22/07/2011 15:36:17
 *  Author: sf306
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

void vStartLCD(void);
static void vLCDTask( void *pvParameters );
void vPrintChar(int x, int y, char input);
void vPrintString(int x, int y, char *input);
void vPrintNumber(int x, int y, int number);
void vClearScreen(void);
void vClearPosition(int x, int y);


#endif /* INCFILE1_H_ */