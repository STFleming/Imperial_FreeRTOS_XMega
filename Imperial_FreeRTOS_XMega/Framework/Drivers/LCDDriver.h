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


#endif /* INCFILE1_H_ */