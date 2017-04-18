#include "rc.h"
#include "include.h"

#define RC_FUN_CNT	80

T_RC_DATA Rc_Data;//1000~2000
float RC_Target_ROL=0,RC_Target_PIT=0,RC_Target_YAW=0;
u8 ARMED = 0;
u8 LandFall=0;
u8 Height_Ctl=0;
//void Rc_DataAnl(void)
//{
//	Rc_Data.THROTTLE	=	Rc_Pwm_In[9];
//	Rc_Data.YAW				=	Rc_Pwm_In[8];
//	Rc_Data.ROLL			=	Rc_Pwm_In[7];
//	Rc_Data.PITCH			=	Rc_Pwm_In[6];
//	Rc_Data.AUX1			=	Rc_Pwm_In[1];
//	Rc_Data.AUX2			=	Rc_Pwm_In[0];
//	Rc_Data.AUX3			=	Rc_Pwm_In[5];
//	Rc_Data.AUX4			=	Rc_Pwm_In[2];
//	Rc_Data.AUX5			=	Rc_Pwm_In[3];
//	Rc_Data.AUX6			=	Rc_Pwm_In[4];
//	
//	Rc_DataCal();
//}
//void Rc_DataCal(void)
//{
//	static u8 fun_cnt = 0;
//	RC_Target_ROL = (Rc_Data.ROLL-1500)/30;
//	RC_Target_PIT = (Rc_Data.PITCH-1500)/30;
//	RC_Target_YAW = (Rc_Data.YAW-1500)/30;
//	
//	if(Rc_Data.THROTTLE < RC_MIN)
//	{
//		if(Rc_Data.YAW < RC_MIN)
//		{
//			fun_cnt++;
//			if(fun_cnt==RC_FUN_CNT)
//				ARMED = 1;
//		}
//		else if(Rc_Data.YAW > RC_MAX)
//		{
//			fun_cnt++;
//			if(fun_cnt==RC_FUN_CNT)
//				ARMED = 0;
//		}
//		else
//			fun_cnt = 0;
//	}
//}
void Rc_DataAnl(void)
{
	
	if(Rc_Pwm_In[1] < 1800)//阈值安全设置
		Rc_Data.THROTTLE		=	Rc_Pwm_In[1];
	else
	{
		Rc_Data.THROTTLE=1000;
		ARMED=0;
	}
	if(Rc_Pwm_In[0] < 2100)
		Rc_Data.YAW				=	Rc_Pwm_In[0];
	else
		Rc_Data.YAW=1500;
	if(Rc_Pwm_In[3] < 2100)
		Rc_Data.ROLL			=	Rc_Pwm_In[3];
	else
		Rc_Data.ROLL=1500;
	if(Rc_Pwm_In[7] < 2100)
		Rc_Data.PITCH			=	Rc_Pwm_In[7];
	else
		Rc_Data.PITCH=1500;
	
	Rc_Data.AUX1			=	Rc_Pwm_In[5];
	Rc_Data.AUX2			=	Rc_Pwm_In[6];
	Rc_Data.AUX3			=	Rc_Pwm_In[4];
	Rc_Data.AUX4			=	Rc_Pwm_In[2];
	Rc_Data.AUX5			=	Rc_Pwm_In[8];
	Rc_Data.AUX6			=	Rc_Pwm_In[9];
	
	Rc_DataCal();
}
void Rc_DataCal(void)
{
	static u8 fun_cnt = 0;
	RC_Target_ROL = (Rc_Data.ROLL-1500)/30;
	RC_Target_PIT = (Rc_Data.PITCH-1500)/30;
	RC_Target_YAW = (Rc_Data.YAW-1500)/30;
	
	if(Rc_Data.THROTTLE < RC_MIN)	
	{
		if(Rc_Data.YAW > RC_MAX)	
		{
			if(Rc_Data.PITCH > RC_MAX)	
			{
				if(Rc_Data.ROLL < RC_MIN)	
				{
					fun_cnt++;
					if(fun_cnt==RC_FUN_CNT)
						ARMED = 1;
				}else 
					  fun_cnt=0;
				
			}else
				  fun_cnt=0;
				
				
		}else if(Rc_Data.YAW < RC_MIN)	
		{
			fun_cnt++;
			if(fun_cnt==10)
				ARMED = 0;			
		}else 					
			fun_cnt=0;
	}
	
	if(1200<Rc_Data.AUX3&&Rc_Data.AUX3<1800)
	{
		LandFall=1;//触发降落标志
	}else if(Rc_Data.AUX3>1800)
			ARMED=0;
			else LandFall=0;
	if(1800<Rc_Data.AUX1)
	{
		Height_Ctl=1;
		Set_Height=(Rc_Data.THROTTLE-1000)*2;
	}
	else Height_Ctl=0;
					
}



