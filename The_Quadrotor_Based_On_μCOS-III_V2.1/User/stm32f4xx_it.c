/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include"include.h" 
#include "includes.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
//void PendSV_Handler(void)
//{
//}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
//void SysTick_Handler(void)
//{
// 
//}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
void TIM2_IRQHandler(void)
{
//	Tim2_Pwm_In_Irq();
}
void TIM3_IRQHandler(void)
{
//	Tim3_Pwm_In_Irq();
	_TIM3_IRQHandler();
}
void TIM4_IRQHandler(void)
{
	_TIM4_IRQHandler();
}
 void TIM1_BRK_TIM9_IRQHandler(void)
{
//	Tim9_Pwm_In_Irq();
}
void TIM1_UP_TIM10_IRQHandler(void)
{
//	Tim10_Pwm_In_Irq();
}
void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
//	Tim11_Pwm_In_Irq();
}
/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  hgyy
  * @version V1.0
  * @date    from 2015 10/07
  * @brief   
  *          中断服务函数
  *          时基在main函数中修改
  ******************************************************************************
  * @attention
  *
  *	
  *
  ******************************************************************************
  */
void TIM6_DAC_IRQHandler(void)
{
	static u8 tick_1ms=0,tick_2ms=0,tick_4ms=0,tick_5ms=0,tick_10ms=0,tick_15ms=0,tick_20ms=0,tick_50ms=0;
	static u16 tick_500ms=0,tick_1000ms=0;
//	static u8 MPU6050_Read_OK = 0;
	static u16 led_flash_cnt0=200,led_flash_cnt1=0;
			
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif	
//	void Timer_Sys_Irq(void)
//	{
		if(TIM6->SR & TIM_IT_Update)		//if ( TIM_GetITStatus(TIM3 , TIM_IT_Update) != RESET ) 
		{
			
//			if(IMU_Update_State!=0)	//DMP姿态结算完成标志检测
//			{		
//			IMU_Update_State=0;
			tick_1ms++;
			tick_2ms++;
			tick_4ms++;
			tick_5ms++;
			tick_10ms++;
			tick_15ms++;
			tick_20ms++;
			tick_50ms++;
			tick_500ms++;
			tick_1000ms++;
			led_flash_cnt1++;

			Data_Check = 1;	//检查标志位

//			Send_Data();
//			if(! MPU6050_Read_OK)	MPU6050_Read_OK = MPU6050_Read();	//等待iic可以读取6050数据，可以读取数据时跳过死循环
			
			if(tick_1ms>=1)	//待修改。。
			{
				tick_1ms = 0;
				IMU_Update();	//从DMP读取数据				
//				MPU6050_OFFSET();		
//				MPU6050_Dataanl();	
//				MPU6050_Read_OK = MPU6050_Read();
			}
//			if(MPU6050_Read_OK) DEBUG_CNT++;
			
			if(tick_2ms>=2)	
			{
				tick_2ms = 0;
//				Prepare_Data();	//对6050原始数据滤波
//				Get_Attitude();	//四元数姿态解算	
//				IMU_Update();	//从DMP读取数据
				
//				CONTROL(Q_ANGLE_DATA.ROLL,Q_ANGLE_DATA.PITCH,Q_ANGLE_DATA.YAW);	
				My_Control(Q_ANGLE_DATA.ROLL,Q_ANGLE_DATA.PITCH,Q_ANGLE_DATA.YAW);
//				CTRL_1(0.002f);	//内环
//				Data_Send_Status();
//				Data_Send_Senser();
				
//					printf("进入姿态控制中断\n");

			}
			if(tick_4ms>=4)
			{
				tick_4ms=0;
//				CTRL_2(0.004f);	//外环
			}
			
			if(tick_5ms>=15)	//缺省中。。。
			{
				tick_5ms = 0;
				Send_RCData = 1;
				Send_MotoPwm = 1;
			}
			
			if(tick_10ms>=10)	//缺省中。。。
			{
				tick_10ms = 0;

				Send_Status = 1;
				Send_Senser = 1;
			
//					printf("进入上位机发送数据中断！\n");

			}
			
			if(tick_20ms==40)	//缺省中。。。
			{
				tick_20ms = 0;
//				
				if(!ARMED) 		led_flash_cnt1=led_flash_cnt0 = 0;
				else			led_flash_cnt0 = 100;
//				
//				Send_RCData = 1;
//				Send_MotoPwm = 1;
			}
			if(tick_50ms>=50)
			{
//				Ultra_Duty();
//				Thr_Ctrl(0.002);	//定高
				tick_50ms=0;
				Send_Senser_2=1;
				KS103_CONTROL(Set_Height);
			}
			
			if(tick_1000ms==2000)	//缺省中。。。
			{
//				temp2 = DEBUG_CNT;
//				DEBUG_CNT=0;
//				ms1000 = 0;
			}
			
			if(led_flash_cnt1==led_flash_cnt0)
			{
				static u8 led_sta=0;
				
				led_flash_cnt1 = 0;
				
				if(led_sta)
				{
					led_sta = 0;
//					Led_Control(9,0);
					LED(0);
				}
				else
				{
					led_sta = 1;
//					Led_Control(9,1);
					LED(1);
				}
			}
			else if(led_flash_cnt1>led_flash_cnt0)
				led_flash_cnt1 = led_flash_cnt0 - 1;
			
			}
			
			TIM6->SR = ~TIM_FLAG_Update;//TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);   //清除中断标志

#if SYSTEM_SUPPORT_OS 
	OSIntExit();  											 
#endif			
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
