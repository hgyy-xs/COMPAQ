#include "Tim_Pwm_Out.h"

uint32_t CCR1_Val = 21000;
uint32_t CCR2_Val = 21000;
uint32_t CCR3_Val = 21000;
uint32_t CCR4_Val = 21000;
#define Moto_PwmMax 999
void Tim_Pwm_Out_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	uint16_t PrescalerValue = 0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
////	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
////	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
////		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	

	
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
////  GPIO_Init(GPIOD, &GPIO_InitStructure); 

//////  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM3);
//////  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM3); 
//////  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM3);
//////  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM3);
////	GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_TIM4);
////	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4); 
////	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
////	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
////	
////	/* Compute the prescaler value */
////  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
////  /* Time base configuration */
////  TIM_TimeBaseStructure.TIM_Period = 52499;									//重装值
////  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		//分频系数
////  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
////  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
////  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

////  /* PWM1 Mode configuration: Channel1 */
////  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = 0;//CCR1_Val;
////  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
////  TIM_OC1Init(TIM4, &TIM_OCInitStructure);
////  TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
////  /* PWM1 Mode configuration: Channel2 */
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = 0;//CCR2_Val;
////  TIM_OC2Init(TIM4, &TIM_OCInitStructure);
////  TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
////  /* PWM1 Mode configuration: Channel3 */
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = 0;//CCR3_Val;
////  TIM_OC3Init(TIM4, &TIM_OCInitStructure);
////  TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
////  /* PWM1 Mode configuration: Channel4 */
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = 0;//CCR4_Val;
////  TIM_OC4Init(TIM4, &TIM_OCInitStructure);
////  TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
////	
////  TIM_ARRPreloadConfig(TIM4, ENABLE);
////  TIM_Cmd(TIM4, ENABLE);
/////////////////////////////////////////////////////////////////////////////定时器5  PA.0--PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure); 

  GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5);
	
	/* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 52499;									//重装值
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		//分频系数
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//CCR1_Val;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OC1Init(TIM5, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(TIM5, TIM_OCPreload_Enable);
  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//CCR2_Val;
  TIM_OC2Init(TIM5, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);
  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//CCR3_Val;
  TIM_OC3Init(TIM5, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
  /* PWM1 Mode configuration: Channel4 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse = 0;//CCR4_Val;
  TIM_OC4Init(TIM5, &TIM_OCInitStructure);
  TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
	
  TIM_ARRPreloadConfig(TIM5, ENABLE);
  TIM_CtrlPWMOutputs(TIM5, ENABLE);
  TIM_Cmd(TIM5, ENABLE);
/////////////////////////////////////////////////////////////////////////////  定时器1 
////	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
////  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
////  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
////  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
////  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
////  GPIO_Init(GPIOE, &GPIO_InitStructure); 

//////  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1);
//////  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); 
//////  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_TIM1);
//////  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);
////	
////	/* Compute the prescaler value */
////  PrescalerValue = (uint16_t) ((SystemCoreClock) / 21000000) - 1;
////  /* Time base configuration */
////  TIM_TimeBaseStructure.TIM_Period = 52499;									//重装值
////  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;		//分频系数
////  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
////  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
////	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
////  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

////  /* PWM1 Mode configuration: Channel1 */
////  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
////  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
////  TIM_OCInitStructure.TIM_Pulse = CCR1_Val;
////  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
////	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
////  TIM_OC1Init(TIM1, &TIM_OCInitStructure);
////  //TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);
////  /* PWM1 Mode configuration: Channel2 */
////  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = CCR2_Val;
////  TIM_OC2Init(TIM1, &TIM_OCInitStructure);
////  //TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);
////  /* PWM1 Mode configuration: Channel3 */
////  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;
////  TIM_OC3Init(TIM1, &TIM_OCInitStructure);
////  //TIM_OC3PreloadConfig(TIM1, TIM_OCPreload_Enable);
////  /* PWM1 Mode configuration: Channel4 */
////  //TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
////  TIM_OCInitStructure.TIM_Pulse = CCR4_Val;
////  TIM_OC4Init(TIM1, &TIM_OCInitStructure);
////  //TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);
////	
////	TIM_CtrlPWMOutputs(TIM1, ENABLE);
////  TIM_ARRPreloadConfig(TIM1, ENABLE);
////  TIM_Cmd(TIM1, ENABLE);
}

void Moto_PwmRflash(int32_t MOTO1_PWM,int32_t MOTO2_PWM,int32_t MOTO3_PWM,int32_t MOTO4_PWM)
{		
	if(MOTO1_PWM>Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	if(MOTO2_PWM>Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	if(MOTO3_PWM>Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	if(MOTO4_PWM>Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
	if(MOTO1_PWM<0)	MOTO1_PWM = 0;
	if(MOTO2_PWM<0)	MOTO2_PWM = 0;
	if(MOTO3_PWM<0)	MOTO3_PWM = 0;
	if(MOTO4_PWM<0)	MOTO4_PWM = 0;
	
	 TIM5->CCR1= 21*MOTO1_PWM+21000;
   	 TIM5->CCR2= 21*MOTO2_PWM+21000;
	 TIM5->CCR3= 21*MOTO3_PWM+21000;
	 TIM5->CCR4= 21*MOTO4_PWM+21000;
}
