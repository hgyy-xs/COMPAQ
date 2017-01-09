#ifndef __USART2_H
#define __USART2_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "string.h"
#include "JY-901.h"

extern struct STime		stcTime;
extern struct SAcc 		stcAcc;
extern struct SGyro 		stcGyro;
extern struct SAngle 	stcAngle;
extern struct SMag 		stcMag;
extern struct SDStatus stcDStatus;
extern struct SPress 	stcPress;
extern struct SLonLat 	stcLonLat;
extern struct SGPSV 		stcGPSV;

void CopeSerialData(unsigned char ucData);//CopeSerialData为串口中断调用函数，串口每收到一个数据，调用一次这个函数。


void USART2_Init(u32 baudrate);
//void UART2_Put_Char(char DataToSend);
//u8 UART2_Get_Char(void);
//void UART2_Put_String(char *Str);
#endif

//------------------End of File----------------------------

