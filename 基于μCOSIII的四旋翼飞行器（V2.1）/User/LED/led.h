#ifndef __LED_H
#define __LED_H
#include "sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2014/5/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	


//LED�˿ڶ���
#define LED0 PBout(0)	// DS0
#define LED1 PFout(10)	// DS1	 

#define LED(x)	x ? GPIO_ResetBits(GPIOB,GPIO_Pin_0): GPIO_SetBits(GPIOB,GPIO_Pin_0)


void LED_Init(void);//��ʼ��		
void LED_State(u8 state);
void LED_OFFSET(void);
#endif
