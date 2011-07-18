/*
 * LEDTask.h
 *
 * Created: 14/07/2011 15:13:57
 *  Author: sf306
 */ 

#ifndef APPTASK_H_
#define APPTASK_H_

void vTaskOne( void *pvParameters );
void vTaskTwo(void *pvParameters);
void vTaskThree(void *pvParameters);
void CreateTaskOne(void);
void CreateTaskTwo(void);
void CreateTaskThree(void);
void positionLCDPrint(void);



#endif /* APPTASK_H_ */