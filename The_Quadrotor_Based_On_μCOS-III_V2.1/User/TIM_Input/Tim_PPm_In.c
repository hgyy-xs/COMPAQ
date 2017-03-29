#include "Tim_PPm_In.h"

u16 Rc_Pwm_In[10];
u8 Rc_Pwm_In_Ready=0;

//u16 Rc_Pwm_In[8];

void Tim_Pwm_In_Init(void)	
{
	GPIO_InitTypeDef GPIO_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM4, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOD, ENABLE);
	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	 
	
//////////////////////////////////////////////////////////////////////////////////////////////
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);


//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM3);
//	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);
  
	TIM3->PSC = (168/2)-1;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM3, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM3, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM3, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC2, ENABLE);
//	TIM_ITConfig(TIM3, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);
/////////////////////////////////////////////////////////////////////////////////////////////
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM3);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM3);
  
	TIM4->PSC = (168/2)-1;
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_BothEdge;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
  TIM_ICInit(TIM4, &TIM_ICInitStructure);
  
  /* TIM enable counter */
  TIM_Cmd(TIM4, ENABLE);

  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC1, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM4, TIM_IT_CC4, ENABLE);
}

void _TIM3_IRQHandler(void)					//PC.6 PC.7  PB.0  PB.1
{
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif		
	static u16 temp_cnt1,temp_cnt1_2,temp_cnt2,temp_cnt2_2,temp_cnt3,temp_cnt3_2,temp_cnt4,temp_cnt4_2;
	
//	Feed_Rc_Dog(1);//RC
	
	if(TIM3->SR & TIM_IT_CC1) 
	{
		TIM3->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM3->SR = ~TIM_FLAG_CC1OF;
		if(GPIOC->IDR & GPIO_Pin_6)
		{
			temp_cnt1 = TIM_GetCapture1(TIM3);
		}
		else
		{
			temp_cnt1_2 = TIM_GetCapture1(TIM3);
			if(temp_cnt1_2>=temp_cnt1)
				Rc_Pwm_In[0] = temp_cnt1_2-temp_cnt1;
			else
				Rc_Pwm_In[0] = 0xffff-temp_cnt1+temp_cnt1_2+1;
		}
	}
	if(TIM3->SR & TIM_IT_CC2) 
	{
		TIM3->SR = ~TIM_IT_CC2;
		TIM3->SR = ~TIM_FLAG_CC2OF;
		if(GPIOC->IDR & GPIO_Pin_7)
		{
			temp_cnt2 = TIM_GetCapture2(TIM3);
		}
		else
		{
			temp_cnt2_2 = TIM_GetCapture2(TIM3);
			if(temp_cnt2_2>=temp_cnt2)
				Rc_Pwm_In[1] = temp_cnt2_2-temp_cnt2;
			else
				Rc_Pwm_In[1] = 0xffff-temp_cnt2+temp_cnt2_2+1;
		}
	}
	if(TIM3->SR & TIM_IT_CC3) 
	{
		TIM3->SR = ~TIM_IT_CC3;
		TIM3->SR = ~TIM_FLAG_CC3OF;
		if(GPIOB->IDR & GPIO_Pin_0)
		{
			temp_cnt3 = TIM_GetCapture3(TIM3);
		}
		else
		{
			temp_cnt3_2 = TIM_GetCapture3(TIM3);
			if(temp_cnt3_2>=temp_cnt3)
				Rc_Pwm_In[2] = temp_cnt3_2-temp_cnt3;
			else
				Rc_Pwm_In[2] = 0xffff-temp_cnt3+temp_cnt3_2+1;
		}
	}
	if(TIM3->SR & TIM_IT_CC4) 
	{
		TIM3->SR = ~TIM_IT_CC4;
		TIM3->SR = ~TIM_FLAG_CC4OF;
		if(GPIOB->IDR & GPIO_Pin_1)
		{
			temp_cnt4 = TIM_GetCapture4(TIM3);
		}
		else
		{
			temp_cnt4_2 = TIM_GetCapture4(TIM3);
			if(temp_cnt4_2>=temp_cnt4)
				Rc_Pwm_In[3] = temp_cnt4_2-temp_cnt4;
			else
				Rc_Pwm_In[3] = 0xffff-temp_cnt4+temp_cnt4_2+1;
		}
	}
#if SYSTEM_SUPPORT_OS 
	OSIntExit();  											 
#endif	
}

void _TIM4_IRQHandler(void)					//PD.12 PD.13 PD.14 PD.15
{
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif	
	static u16 temp_cnt1,temp_cnt1_2,temp_cnt2,temp_cnt2_2,temp_cnt3,temp_cnt3_2,temp_cnt4,temp_cnt4_2;
	
//	Feed_Rc_Dog(1);//RC
	
	if(TIM4->SR & TIM_IT_CC1) 
	{
		TIM4->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
		TIM4->SR = ~TIM_FLAG_CC1OF;
		if(GPIOD->IDR & GPIO_Pin_12)
		{
			temp_cnt1 = TIM_GetCapture1(TIM4);
		}
		else
		{
			temp_cnt1_2 = TIM_GetCapture1(TIM4);
			if(temp_cnt1_2>=temp_cnt1)
				Rc_Pwm_In[4] = temp_cnt1_2-temp_cnt1;
			else
				Rc_Pwm_In[4] = 0xffff-temp_cnt1+temp_cnt1_2+1;
		}
	}
	if(TIM4->SR & TIM_IT_CC2) 
	{
		TIM4->SR = ~TIM_IT_CC2;
		TIM4->SR = ~TIM_FLAG_CC2OF;
		if(GPIOD->IDR & GPIO_Pin_13)
		{
			temp_cnt2 = TIM_GetCapture2(TIM4);
		}
		else
		{
			temp_cnt2_2 = TIM_GetCapture2(TIM4);
			if(temp_cnt2_2>=temp_cnt2)
				Rc_Pwm_In[5] = temp_cnt2_2-temp_cnt2;
			else
				Rc_Pwm_In[5] = 0xffff-temp_cnt2+temp_cnt2_2+1;
		}
	}
	if(TIM4->SR & TIM_IT_CC3) 
	{
		TIM4->SR = ~TIM_IT_CC3;
		TIM4->SR = ~TIM_FLAG_CC3OF;
		if(GPIOD->IDR & GPIO_Pin_14)
		{
			temp_cnt3 = TIM_GetCapture3(TIM4);
		}
		else
		{
			temp_cnt3_2 = TIM_GetCapture3(TIM4);
			if(temp_cnt3_2>=temp_cnt3)
				Rc_Pwm_In[6] = temp_cnt3_2-temp_cnt3;
			else
				Rc_Pwm_In[6] = 0xffff-temp_cnt3+temp_cnt3_2+1;
		}
	}
	if(TIM4->SR & TIM_IT_CC4) 
	{
		TIM4->SR = ~TIM_IT_CC4;
		TIM4->SR = ~TIM_FLAG_CC4OF;
		if(GPIOD->IDR & GPIO_Pin_15)
		{
			temp_cnt4 = TIM_GetCapture4(TIM4);
		}
		else
		{
			temp_cnt4_2 = TIM_GetCapture4(TIM4);
			if(temp_cnt4_2>=temp_cnt4)
				Rc_Pwm_In[7] = temp_cnt4_2-temp_cnt4;
			else
				Rc_Pwm_In[7] = 0xffff-temp_cnt4+temp_cnt4_2+1;
		}
	}
	Rc_Pwm_In_Ready = 1;
	Rc_DataAnl();///////////////////////////////////////////////////////
#if SYSTEM_SUPPORT_OS 
	OSIntExit();  											 
#endif
}



//void Tim9_Pwm_In_Irq(void)				//PE.5  PE.6
//{
//#if SYSTEM_SUPPORT_OS 		
//	OSIntEnter();    
//#endif
//	static u16 temp_cnt1,temp_cnt1_2,temp_cnt2,temp_cnt2_2;
//	
//	if(TIM9->SR & TIM_IT_CC1) 
//	{
//		TIM9->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//		TIM9->SR = ~TIM_FLAG_CC1OF;
//		if(GPIOE->IDR & GPIO_Pin_5)
//		{
//			temp_cnt1 = TIM_GetCapture1(TIM9);
//		}
//		else
//		{
//			temp_cnt1_2 = TIM_GetCapture1(TIM9);
//			if(temp_cnt1_2>=temp_cnt1)
//				Rc_Pwm_In[6] = temp_cnt1_2-temp_cnt1;
//			else
//				Rc_Pwm_In[6] = 0xffff-temp_cnt1+temp_cnt1_2;
//		}
//	}
//	if(TIM9->SR & TIM_IT_CC2) 
//	{
//		TIM9->SR = ~TIM_IT_CC2;
//		TIM9->SR = ~TIM_FLAG_CC2OF;
//		if(GPIOE->IDR & GPIO_Pin_6)
//		{
//			temp_cnt2 = TIM_GetCapture2(TIM9);
//		}
//		else
//		{
//			temp_cnt2_2 = TIM_GetCapture2(TIM9);
//			if(temp_cnt2_2>=temp_cnt2)
//				Rc_Pwm_In[7] = temp_cnt2_2-temp_cnt2;
//			else
//				Rc_Pwm_In[7] = 0xffff-temp_cnt2+temp_cnt2_2;
//		}
//	}
//#if SYSTEM_SUPPORT_OS 
//	OSIntExit();  											 
//#endif		
//}
//void Tim10_Pwm_In_Irq(void)			//PB.8
//{
//#if SYSTEM_SUPPORT_OS 		
//	OSIntEnter();    
//#endif	
//	static u16 temp_cnt1,temp_cnt1_2;
//	
//	if(TIM10->SR & TIM_IT_CC1) 
//	{
//		TIM10->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//		TIM10->SR = ~TIM_FLAG_CC1OF;
//		if(GPIOB->IDR & GPIO_Pin_8)
//		{
//			temp_cnt1 = TIM_GetCapture1(TIM10);
//		}
//		else
//		{
//			temp_cnt1_2 = TIM_GetCapture1(TIM10);
//			if(temp_cnt1_2>=temp_cnt1)
//				Rc_Pwm_In[8] = temp_cnt1_2-temp_cnt1;
//			else
//				Rc_Pwm_In[8] = 0xffff-temp_cnt1+temp_cnt1_2;
//			Rc_Pwm_In_Ready = 1;
//			Rc_DataAnl();///////////////////////////////////////////////////////
//		}
//	}
//#if SYSTEM_SUPPORT_OS 	
//	OSIntExit();  											 
//#endif
//}
//void Tim11_Pwm_In_Irq(void)				//PB.9
//{
//#if SYSTEM_SUPPORT_OS 		
//	OSIntEnter();    
//#endif
//	static u16 temp_cnt1,temp_cnt1_2;
//	
//	if(TIM11->SR & TIM_IT_CC1) 
//	{
//		TIM11->SR = ~TIM_IT_CC1;//TIM_ClearITPendingBit(TIM3, TIM_IT_CC1);
//		TIM11->SR = ~TIM_FLAG_CC1OF;
//		if(GPIOB->IDR & GPIO_Pin_9)
//		{
//			temp_cnt1 = TIM_GetCapture1(TIM11);
//		}
//		else
//		{
//			temp_cnt1_2 = TIM_GetCapture1(TIM11);
//			if(temp_cnt1_2>=temp_cnt1)
//				Rc_Pwm_In[9] = temp_cnt1_2-temp_cnt1;
//			else
//				Rc_Pwm_In[9] = 0xffff-temp_cnt1+temp_cnt1_2;
//		}
//	}
//#if SYSTEM_SUPPORT_OS 
//	OSIntExit();  											 
//#endif	
//}
