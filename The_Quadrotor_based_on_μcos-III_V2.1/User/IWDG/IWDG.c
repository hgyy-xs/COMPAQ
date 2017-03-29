#include "stm32f4xx.h"
#include "IWDG.h"


/*
	IWDG的溢出时间计算公式：
					Tout=((4*2^prer)*rlr)/40  （单位：ms）
*/

void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG->KR=0x5555;//使能对RLR和PR寄存器的访问
	IWDG->PR=prer;//设置预分频值
	IWDG->RLR=rlr;//设置重装载值
	IWDG->KR=0xCCCC;//使能看门狗
	IWDG->KR=0xAAAA;//喂狗
	
}

void IWDG_Feed_Dog(void)
{
	IWDG->KR=0xAAAA;
}
