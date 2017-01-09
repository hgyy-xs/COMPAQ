#include "led.h" 
#include "delay.h"
#include "include.h"
#include "imu.h"
void LED_Init(void)
{    	 
	u8 i;
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOFʱ��
  //GPIOF9,F10��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//LED0��LED1��ӦIO��
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO	
  GPIO_SetBits(GPIOB,GPIO_Pin_0);//GPIOF9,F10���øߣ�����	
	for(i=0;i<10;i++)
		LED_State(6);
	GPIOB->BSRRH=GPIO_Pin_0 ;
	delay_ms(1000);
	delay_ms(1000);

}

void LED_OFFSET(void)
{
	u8 n;
	delay_ms(1000);
	delay_ms(1000);
	for(n=0;n<10;n++)
	LED_State(1);
//	Q_ANGLE_OFFSET.X=-0.42;		
//	Q_ANGLE_OFFSET.Y=-1.31;	
	Q_ANGLE_OFFSET.X=-0.58;		//�������������x   ˮƽ����ʾ+���پ͡�����
	Q_ANGLE_OFFSET.Y=-1.13;		//�����������y		ˮƽ����ʾ-���پͼӶ���
	Q_ANGLE_OFFSET.Z=0;	
}
void LED_State(u8 state)
{
	switch(state)
	{
		case 1 :
			LED(1);
			delay_ms(100);
			LED(0);
			delay_ms(100);
			break;
		case 2 :
			LED(1);
			delay_ms(200);
			LED(0);
			delay_ms(200);
		case 3 :
			LED(1);
			delay_ms(300);
			LED(0);
			delay_ms(300);
		case 4 :
			LED(1);
			delay_ms(400);
			LED(0);
			delay_ms(400);
		case 5 :
			LED(1);
			delay_ms(500);
			LED(0);
			delay_ms(500);
		case 6 :
			LED(1);
			delay_ms(80);
			LED(0);
			delay_ms(80);
		case 7 :
			LED(1);
			delay_ms(50);
			LED(0);
			delay_ms(50);		
	}
		
}


