#include "USART2.h"
#include "includes.h"
#include "include.h"
struct STime		stcTime;
struct SAcc 		stcAcc;
struct SGyro 		stcGyro;
struct SAngle 	stcAngle;
struct SMag 		stcMag;
struct SDStatus stcDStatus;
struct SPress 	stcPress;
struct SLonLat 	stcLonLat;
struct SGPSV 		stcGPSV;



void CopeSerialData(unsigned char ucData)
{
	static unsigned char ucRxBuffer[250];
	static unsigned char ucRxCnt = 0;	
	
	ucRxBuffer[ucRxCnt++]=ucData;
	if (ucRxBuffer[0]!=0x55) //����ͷ���ԣ������¿�ʼѰ��0x55����ͷ
	{
		ucRxCnt=0;
		return;
	}
	if (ucRxCnt<11) {return;}//���ݲ���11�����򷵻�
	else
	{
		switch(ucRxBuffer[1])
		{
			case 0x50:	memcpy(&stcTime,&ucRxBuffer[2],8);break;//memcpyΪ�������Դ����ڴ濽��������������"string.h"�������ջ��������ַ����������ݹ�ͬ�����棬�Ӷ�ʵ�����ݵĽ�����
			case 0x51:	memcpy(&stcAcc,&ucRxBuffer[2],8);break;
			case 0x52:	memcpy(&stcGyro,&ucRxBuffer[2],8);break;
			case 0x53:	memcpy(&stcAngle,&ucRxBuffer[2],8);break;
			case 0x54:	memcpy(&stcMag,&ucRxBuffer[2],8);break;
			case 0x55:	memcpy(&stcDStatus,&ucRxBuffer[2],8);break;
			case 0x56:	memcpy(&stcPress,&ucRxBuffer[2],8);break;
			case 0x57:	memcpy(&stcLonLat,&ucRxBuffer[2],8);break;
			case 0x58:	memcpy(&stcGPSV,&ucRxBuffer[2],8);break;
		}
		ucRxCnt=0;
	}
}
/*****************************�ж����ȼ�����timer.c�ж���!************************/
//void NVIC_Configuration(void)
//{
//        NVIC_InitTypeDef NVIC_InitStructure; 
//          /* Enable the USART1 Interrupt */
//        NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
//        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 7;
//        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//        NVIC_Init(&NVIC_InitStructure);
//}

//void USART2_IRQHandler(void)	//�����ж�
//{
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
//	{
//		CopeSerialData((unsigned char)USART2->DR);//��������
////		printf("���봮�ڽ����ж�\n");
//		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
//	}
//	USART_ClearITPendingBit(USART2,USART_IT_ORE);
////		printf("�˳����ڽ����ж�\n");

//}

void USART2_IRQHandler(void)	//�����ж�
{
	static u8 TxBuffer[256];
	static u8 TxCounter=0;
	static u8 count=0; 
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif	

	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
	  {   
		USART_SendData(USART2, TxBuffer[TxCounter++]); 
		if(TxCounter == count) 
			{
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);// ȫ���������
			}
		USART_ClearITPendingBit(USART2, USART_IT_TXE); 
	  }
		else if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	  {
			CopeSerialData((unsigned char)USART2->DR);//��������
			USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	  }
		USART_ClearITPendingBit(USART2,USART_IT_ORE);
#if SYSTEM_SUPPORT_OS 
	OSIntExit();  											 
#endif	
}
void USART2_Init(u32 baudrate)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //ʹ��GPIOAʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//ʹ��USART2ʱ��
  
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	  	//USART1�˿����� 
	//Open_USARTx_TX -> PD5 , Open_USARTx_RX -PD6
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA2��PA3

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

   //USART2 ��ʼ������
		/* 
	  UART2������:
	  1.������Ϊ���ó���ָ�������� baudrate;
	  2. 8λ����			  USART_WordLength_8b;
	  3.һ��ֹͣλ			  USART_StopBits_1;
	  4. ����żЧ��			  USART_Parity_No ;
	  5.��ʹ��Ӳ��������	  USART_HardwareFlowControl_None;
	  6.ʹ�ܷ��ͺͽ��չ���	  USART_Mode_Rx | USART_Mode_Tx;
	 */
	USART_InitStructure.USART_BaudRate = baudrate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������2
	
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//��������ж�	
	USART_Cmd(USART2, ENABLE);  //ʹ�ܴ���2
	
	USART_ClearFlag(USART2, USART_FLAG_TC);
	USART_ITConfig(USART2, USART_IT_TXE, DISABLE);  
//	NVIC_Configuration();

}




