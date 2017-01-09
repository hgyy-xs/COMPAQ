#include "stm32f4xx.h"
#include "delay.h"
#include "key.h"


void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
//	GPIO_SetBits(GPIOE,GPIO_Pin_1);	//À­¸ß
//	GPIOE->BSRRH=GPIO_Pin_1;	//À­µÍ
}

u8 KEY_Scan(void)
{	 

		 u8 keyvalue;
;
		keyvalue=GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1);

		return keyvalue;
	

}
