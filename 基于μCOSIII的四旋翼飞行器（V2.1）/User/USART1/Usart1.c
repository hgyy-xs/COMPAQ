//#include "usart1.h"
////#include "config.h"
//#include "include.h"
//#include "ANO_Tech_STM32_I2C.h"

//#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
//#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
//#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
//#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))
//typedef union {unsigned char byte[4];float num;}t_floattobyte;
//t_floattobyte floattobyte;
///*******************************************************************************
//* Function Name  : USART_Configuration
//* Description    : Configure Open_USARTx 
//* Input          : None
//* Output         : None
//* Return         : None
//* Attention		 : None
//*******************************************************************************/
//u8 TxBuffer[256];
//u8 TxCounter=0;
//u8 count=0; 
//u8 RxBuffer[50];
//u8 RxState = 0;
//u8 RxBufferNum = 0;
//u8 RxBufferCnt = 0;
//u8 RxLen = 0;

//void Usart1_Init(u32 baudrate)
//{ 												
//  GPIO_InitTypeDef GPIO_InitStructure;
//  USART_InitTypeDef USART_InitStructure; 

//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
//  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

//  /*
//  *  Open_USARTx_TX -> PA9 , Open_USARTx_RX -PA10
//  */
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);


//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//  GPIO_Init(GPIOA, &GPIO_InitStructure);

//  USART_InitStructure.USART_BaudRate = baudrate;
//  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//  USART_InitStructure.USART_StopBits = USART_StopBits_1;
//  USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//  USART_Init(USART1, &USART_InitStructure);
//  /* Enable the Open_USART Transmit interrupt: this interrupt is generated when the 
//     Open_USARTx transmit data register is empty */
//  USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

//  USART_Cmd(USART1, ENABLE);
//}

//void Uart1_IRQ(void)
//{
//	if(USART1->SR & USART_SR_ORE)//ORE中断
//	{
//		u8 com_data = USART1->DR;//USART_ClearFlag(USART1,USART_IT_ORE);
//	}
//	//发送中断
//	if((USART1->SR & (1<<7))&&(USART1->CR1 & USART_CR1_TXEIE))//if(USART_GetITStatus(USART1,USART_IT_TXE)!=RESET)
//	{
//		USART1->DR = TxBuffer[TxCounter++]; //写DR清除中断标志          
//		if(TxCounter == count)
//		{
//			USART1->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE中断//USART_ITConfig(USART1,USART_IT_TXE,DISABLE);
//		}
//	}
//	//接收中断 (接收寄存器非空) /////////////////////////////////////////////////////////////////////////////////////////
//	if(USART1->SR & (1<<5))//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)    
//	{
//		u8 com_data = USART1->DR;
//		static u8 _data_len = 0,_data_cnt = 0;
//		if(RxState==0&&com_data==0xAA)
//		{
//			RxState=1;
//			RxBuffer[0]=com_data;
//		}
//		else if(RxState==1&&com_data==0xAF)
//		{
//			RxState=2;
//			RxBuffer[1]=com_data;
//		}
//		else if(RxState==2&&com_data>0&&com_data<0XF1)
//		{
//			RxState=3;
//			RxBuffer[2]=com_data;
//		}
//		else if(RxState==3&&com_data<50)
//		{
//			RxState = 4;
//			RxBuffer[3]=com_data;
//			_data_len = com_data;
//			_data_cnt = 0;
//		}
//		else if(RxState==4&&_data_len>0)
//		{
//			_data_len--;
//			RxBuffer[4+_data_cnt++]=com_data;
//			if(_data_len==0)
//				RxState = 5;
//		}
//		else if(RxState==5)
//		{
//			RxState = 0;
//			RxBuffer[4+_data_cnt]=com_data;
//			Data_Receive_Anl(RxBuffer,_data_cnt+5);
//		}
//		else
//			RxState = 0;
//	}
//}
///**************************实现函数********************************************
//*******************************************************************************/
//uint8_t Uart1_Put_Char(unsigned char DataToSend)
//{
//	TxBuffer[count++] = DataToSend;
//  if(!(USART1->CR1 & USART_CR1_TXEIE))
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
//	return DataToSend;
//}
//void Uart1_Put_Buf(unsigned char *DataToSend , u8 data_num)
//{
//	u8 i=0;
//	for( i=0;i<data_num;i++)
//		TxBuffer[count++] = *(DataToSend+i);
//	if(!(USART1->CR1 & USART_CR1_TXEIE))
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE); 
//}
//uint8_t Uart1_Put_Int16(uint16_t DataToSend)
//{
//	uint8_t sum = 0;
//	TxBuffer[count++] = BYTE1(DataToSend);
//	TxBuffer[count++] = BYTE0(DataToSend);
//	if(!(USART1->CR1 & USART_CR1_TXEIE))
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//	sum += BYTE1(DataToSend);
//	sum += BYTE0(DataToSend);
//	return sum;
//}
//uint8_t Uart1_Put_Float(float DataToSend)
//{
//	uint8_t sum = 0;
//	floattobyte.num=DataToSend;
//	TxBuffer[count++] = floattobyte.byte[3];  
//	TxBuffer[count++] = floattobyte.byte[2];  
//	TxBuffer[count++] = floattobyte.byte[1];  
//	TxBuffer[count++] = floattobyte.byte[0];  
//	if(!(USART1->CR1 & USART_CR1_TXEIE))
//		USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
//	sum += BYTE3(DataToSend);
//	sum += BYTE2(DataToSend);
//	sum += BYTE1(DataToSend);
//	sum += BYTE0(DataToSend);
//	return sum;	
//}
//void Uart1_Put_String(unsigned char *Str)
//{
//	//判断Str指向的数据是否有效.
//	while(*Str)
//	{
//	//是否是回车字符 如果是,则发送相应的回车 0x0d 0x0a
//	if(*Str=='\r')Uart1_Put_Char(0x0d);
//		else if(*Str=='\n')Uart1_Put_Char(0x0a);
//			else Uart1_Put_Char(*Str);
//	//指针++ 指向下一个字节.
//	Str++;
//	}
//}

