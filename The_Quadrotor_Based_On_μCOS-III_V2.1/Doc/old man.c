//#include "stm32f4xx.h" 
//#include "include.h"

//int main(void)
//{ 	
//	delay_init(168);		  
//	uart_init(115200);	
//	USART2_Init(115200);	
//	LED_Init();		
//	Nvic_Init();
//	Timer_Sys_Init(500);	//定时器频率2000Hz
//	Tim_Pwm_Out_Init();		
//	Tim_Pwm_In_Init();
//	Para_ResetToFactorySetup();	
//	Rc_Data.THROTTLE=1051;		//无遥控器时测试值
//	Rc_Data.ROLL=1500;
//	Rc_Data.PITCH=1500;
//	Rc_Data.YAW=1500;
//	ARMED=0;					
//	LED_OFFSET(); //等待LED指示
//	IWDG_Init(4,320);	//看门狗初始化，喂狗时间 <500ms
//	while(1)
//	{
//		IWDG_Feed_Dog();	
//		IMU_Update();	
//	
//	if(Data_Check)	
//	{
//		Data_Check = 0;
//		Send_Data();	
//		
//	}

//	}
//}




