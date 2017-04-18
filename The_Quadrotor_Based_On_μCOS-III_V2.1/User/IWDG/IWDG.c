#include "stm32f4xx.h"
#include "IWDG.h"


/*
	IWDG�����ʱ����㹫ʽ��
					Tout=((4*2^prer)*rlr)/40  ����λ��ms��
*/

void IWDG_Init(u8 prer,u16 rlr)
{
	IWDG->KR=0x5555;//ʹ�ܶ�RLR��PR�Ĵ����ķ���
	IWDG->PR=prer;//����Ԥ��Ƶֵ
	IWDG->RLR=rlr;//������װ��ֵ
	IWDG->KR=0xCCCC;//ʹ�ܿ��Ź�
	IWDG->KR=0xAAAA;//ι��
	
}

void IWDG_Feed_Dog(void)
{
	IWDG->KR=0xAAAA;
}
