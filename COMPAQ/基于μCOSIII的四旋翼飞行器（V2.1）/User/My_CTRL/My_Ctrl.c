#include "My_ctrl.h"
#include "include.h"

extern MPU6050_Data_TypeDef	MPU6050_GYRO_DATA,MPU6050_ACC_DATA ;
extern MPU6050_Q_AngleData_TypeDef Q_ANGLE_DATA;


struct _ctrl ctrl;
struct _sensor sensor;

int16_t Fall_Pwm=0;
int16_t Moto_duty[4];

void Prepare_IMU(void)
{
//	ctrl.roll.Shell.
	
		
	sensor.gyro.origin.x = MPU6050_GYRO_DATA.X;
	sensor.gyro.origin.y = MPU6050_GYRO_DATA.Y;
	sensor.gyro.origin.z = MPU6050_GYRO_DATA.Z ;
		
	sensor.gyro.radian.x = sensor.gyro.origin.x * Gyro_Gr - sensor.gyro.quiet.x * Gyro_Gr;
	sensor.gyro.radian.y = sensor.gyro.origin.y * Gyro_Gr - sensor.gyro.quiet.y * Gyro_Gr;
	sensor.gyro.radian.z = sensor.gyro.origin.z * Gyro_Gr - sensor.gyro.quiet.z * Gyro_Gr;

}

	

void My_Control(float rol, float pit, float yaw)   
{
	static float roll_old,pitch_old;
	if(ctrl.ctrlRate >= 2)
	{

//	  pit = pit + Rc_Data.PITCH;
pit=pit+(Rc_Data.PITCH-1491)/20;	//rc1519 若向前微调则减小1480

		ctrl.pitch.Shell.increment += pit;
		
		//set increment deathroom
		if(ctrl.pitch.Shell.increment > ctrl.pitch.Shell.increment_max)  	ctrl.pitch.Shell.increment = ctrl.pitch.Shell.increment_max;
		else if(ctrl.pitch.Shell.increment < -ctrl.pitch.Shell.increment_max)		ctrl.pitch.Shell.increment = -ctrl.pitch.Shell.increment_max;
		
		ctrl.pitch.Shell.Allout = ctrl.pitch.Shell.kp * pit + ctrl.pitch.Shell.ki * ctrl.pitch.Shell.increment + ctrl.pitch.Shell.kd * (pit - pitch_old);
		pitch_old = pit;
		

//		rol = rol + Rc_Data.ROLL;
rol=rol+(1483-Rc_Data.ROLL)/20;		//rc1525  若向右有微调则减小1477
		ctrl.roll.Shell.increment += rol;
		
		
		if(ctrl.roll.Shell.increment > ctrl.roll.Shell.increment_max)  	ctrl.roll.Shell.increment = ctrl.roll.Shell.increment_max;
		else if(ctrl.roll.Shell.increment < -ctrl.roll.Shell.increment_max)		ctrl.roll.Shell.increment = -ctrl.roll.Shell.increment_max;

		ctrl.roll.Shell.Allout  = ctrl.roll.Shell.kp * rol + ctrl.roll.Shell.ki * ctrl.roll.Shell.increment + ctrl.roll.Shell.kd * (rol - roll_old);
		roll_old = rol;
		
		
//		if(Rc_Data.YAW<-3||Rc_Data.YAW>3)
if(Rc_Data.YAW<1480||Rc_Data.YAW>1520)
				 ctrl.yaw.Shell.Allout = ctrl.yaw.Shell.kp * ((1501-Rc_Data.YAW)/32)   + ctrl.yaw.Shell.kd *sensor.gyro.radian.z/ Gyro_Gr;	
		else
//         ctrl.yaw.Shell.Allout = 10;//ctrl.yaw.Shell.kp * yaw_p + ctrl.yaw.Shell.kd * MPU6050_GYRO_Gr.Z/ Gyro_Gr;	
			ctrl.yaw.Shell.Allout+=MPU6050_GYRO_DATA.Z*0.0609756f*0.002f;
		ctrl.ctrlRate = 0; 
	}
	
//	ctrl.pitch.Shell.Allout=(Rc_Data.PITCH-1501)/10;	//手动模式调参用
	ctrl.ctrlRate ++;

	ctrl.roll.Core.Pout = ctrl.roll.Core.kp * (ctrl.roll.Shell.Allout + sensor.gyro.radian.x * RtA);
	ctrl.roll.Core.Dout = ctrl.roll.Core.kd * (sensor.gyro.origin.x - sensor.gyro.histor.x);
	
	ctrl.pitch.Core.Pout = ctrl.pitch.Core.kp * (ctrl.pitch.Shell.Allout + sensor.gyro.radian.y * RtA);
	ctrl.pitch.Core.Dout = ctrl.pitch.Core.kd * (sensor.gyro.origin.y - sensor.gyro.histor.y);
	
	ctrl.yaw.Core.Pout = ctrl.yaw.Core.kp * (ctrl.yaw.Shell.Allout + sensor.gyro.radian.z * RtA);
	ctrl.yaw.Core.Dout = ctrl.yaw.Core.kd * (sensor.gyro.origin.z - sensor.gyro.histor.z);
	
	ctrl.roll.Core.Allout = ctrl.roll.Core.Pout + ctrl.roll.Core.Dout;
	ctrl.pitch.Core.Allout = ctrl.pitch.Core.Pout + ctrl.pitch.Core.Dout;
	ctrl.yaw.Core.Allout = ctrl.yaw.Core.Pout + ctrl.yaw.Core.Dout;

	sensor.gyro.histor.x = sensor.gyro.origin.x;   
	sensor.gyro.histor.y = sensor.gyro.origin.y;
	sensor.gyro.histor.z = sensor.gyro.origin.z;
	
	
//	Thr_Ctrl(0.002);	//定高
//		if(AUX2==1)
//		{  
//		if((US100_Alt*100)<=50)  
//		{
//			     
//			date_THROTTLE_US_100=650; //如果油门过小则启动遥控器控制
//    }
//		else 
//		{
//			date_THROTTLE_US_100=650+THR_Lock;//加了超声波限高的油门	
//		}
//  	}
//	  
//		if(AUX2==0)
//		{  
//					date_throttle= RC_Data.THROTTLE/cos(angle.roll/RtA)/cos(angle.pitch/RtA);
//					if(RC_Data.THROTTLE>300)  
//				{
//							 
//					date_THROTTLE_US_100=date_throttle; //如果油门过小则启动遥控器控制
//				}
//				
//    }
		
		
		/*
		Moto_duty[0] = date_THROTTLE_US_100 - ctrl.roll.Core.pid_out - ctrl.pitch.Core.pid_out + ctrl.yaw.Core.pid_out;
		Moto_duty[1] = date_THROTTLE_US_100 - ctrl.roll.Core.pid_out + ctrl.pitch.Core.pid_out - ctrl.yaw.Core.pid_out;
		Moto_duty[2] = date_THROTTLE_US_100 + ctrl.roll.Core.pid_out - ctrl.pitch.Core.pid_out - ctrl.yaw.Core.pid_out;
		Moto_duty[3] = date_THROTTLE_US_100 + ctrl.roll.Core.pid_out + ctrl.pitch.Core.pid_out + ctrl.yaw.Core.pid_out;
		*/
		
//		Moto_duty[0] = date_THROTTLE_US_100 - ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout-PID_ADNS3080.Yout-PID_ADNS3080.Xout;
//		Moto_duty[1] = date_THROTTLE_US_100 - ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout-PID_ADNS3080.Yout+PID_ADNS3080.Xout;
//		Moto_duty[2] = date_THROTTLE_US_100 + ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout+PID_ADNS3080.Yout-PID_ADNS3080.Xout;
//		Moto_duty[3] = date_THROTTLE_US_100 + ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout+PID_ADNS3080.Yout+PID_ADNS3080.Xout;
   
//		if(Rc_Data.THROTTLE<1300)
//		{		
//		pit_i=0;
//		rol_i=0;
//		yaw_p=0;
//		}

//	PID_ROL.OUT = (PID_ROL.pout) +PID_ROL.iout +PID_ROL.dout;
//	PID_PIT.OUT = PID_PIT.pout + PID_PIT.iout + PID_PIT.dout;
//	PID_YAW.OUT = PID_YAW.pout + PID_YAW.iout + PID_YAW.dout;

	if(Rc_Data.THROTTLE<1300)
	{
		ctrl.roll.Core.Allout=0;
		ctrl.pitch.Core.Allout=0;
		ctrl.yaw.Core.Allout=0;
	}
	if(Height_Ctl)	//定高模式
	{
		if(LandFall==0&&ARMED)
		{
			if(Rc_Data.THROTTLE>1020&&ARMED)
			{
//				Moto_PWM_1 = thr_value - ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout;
//				Moto_PWM_2 = thr_value + ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout;			
//				Moto_PWM_3 = thr_value + ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout;
//				Moto_PWM_4 = thr_value - ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout;
				Moto_PWM_1 = thr_tmep - ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout;
				Moto_PWM_2 = thr_tmep + ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout;			
				Moto_PWM_3 = thr_tmep + ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout;
				Moto_PWM_4 = thr_tmep - ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout;
			}
			else
			{
				Moto_PWM_1 = 0;
				Moto_PWM_2 = 0;
				Moto_PWM_3 = 0;
				Moto_PWM_4 = 0;

			}
				Moto_PwmRflash(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4);

		}else //if(LandFall==1)
				{
					//				Touch_Down();
					ARMED=0;
					Moto_PWM_1 = 0;
					Moto_PWM_2 = 0;
					Moto_PWM_3 = 0;
					Moto_PWM_4 = 0;
					Moto_PwmRflash(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4);
				}
	}
	else	//手动模式
		if(LandFall==0&&ARMED)
		{
			if(Rc_Data.THROTTLE>1050&&ARMED)
			{
				Moto_PWM_1 = Rc_Data.THROTTLE - 1000 - ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout;
				Moto_PWM_2 = Rc_Data.THROTTLE - 1000 + ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout;
				Moto_PWM_3 = Rc_Data.THROTTLE - 1000 + ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout;
				Moto_PWM_4 = Rc_Data.THROTTLE - 1000 - ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout;
			}
			else
			{
				Moto_PWM_1 = 0;
				Moto_PWM_2 = 0;
				Moto_PWM_3 = 0;
				Moto_PWM_4 = 0;

			}
				Moto_PwmRflash(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4);

		}else //if(LandFall==1)
				{
					//				Touch_Down();
					ARMED=0;
					Moto_PWM_1 = 0;
					Moto_PWM_2 = 0;
					Moto_PWM_3 = 0;
					Moto_PWM_4 = 0;
					Moto_PwmRflash(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4);
				}

//		if(ARMED == 1)
//	{ Moto_PwmRflash(Moto_duty[0],Moto_duty[1],Moto_duty[2],Moto_duty[3]);}
//	
//	
//	else      Moto_PwmRflash(0,0,0,0);		
}


void Touch_Down(void)//稳定性有待测试
{

	if(Moto_PWM_1<10||Moto_PWM_2<10||Moto_PWM_3<10||Moto_PWM_4<10)
	{
		Moto_PWM_1 = 0;
		Moto_PWM_2 = 0;
		Moto_PWM_3 = 0;
		Moto_PWM_4 = 0;
		ARMED=0;
	}
	else
	{
		Fall_Pwm+=1;
		Moto_PWM_1 = Rc_Data.THROTTLE - 1000 - ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout-Fall_Pwm;
		Moto_PWM_2 = Rc_Data.THROTTLE - 1000 + ctrl.roll.Core.Allout - ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout-Fall_Pwm;
		Moto_PWM_3 = Rc_Data.THROTTLE - 1000 + ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout + ctrl.yaw.Core.Allout-Fall_Pwm;
		Moto_PWM_4 = Rc_Data.THROTTLE - 1000 - ctrl.roll.Core.Allout + ctrl.pitch.Core.Allout - ctrl.yaw.Core.Allout-Fall_Pwm;
	}
		Moto_PwmRflash(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4);

}
void paramLoad(void)
{
//	EE_READ_ACC_OFFSET(); 
//	Gyro_OFFSET();       

	// The data of pitch
	ctrl.pitch.Shell.kp = 1.0;
	ctrl.pitch.Shell.ki = 0.00;
	ctrl.pitch.Shell.kd = 8.5;
	
	ctrl.pitch.Core.kp = 5.0;
	ctrl.pitch.Core.kd = 0.1;
	
	//The data of roll
	ctrl.roll.Shell.kp = 1.0;
	ctrl.roll.Shell.ki = 0.00;
	ctrl.roll.Shell.kd = 8.5;

	ctrl.roll.Core.kp = 5.0;
	ctrl.roll.Core.kd = 0.1;
	
	//The data of yaw
	ctrl.yaw.Shell.kp = 3.5;
	ctrl.yaw.Shell.kd = 0.0;
	
	ctrl.yaw.Core.kp = 8.5;
	ctrl.yaw.Core.kd = 0.2;
	

	PID_ROL.P=ctrl.roll.Shell.kp;							
	PID_ROL.I=ctrl.roll.Shell.ki;
	PID_ROL.D=ctrl.roll.Shell.kd;
	PID_PIT.P=ctrl.pitch.Shell.kp;
	PID_PIT.I=ctrl.pitch.Shell.ki;
	PID_PIT.D=ctrl.pitch.Shell.kd;
	PID_YAW.P=ctrl.yaw.Shell.kp;
	PID_YAW.I=0.0;
	PID_YAW.D=ctrl.yaw.Shell.kd;
	PID_ALT.P=ctrl.roll.Core.kp;
	PID_ALT.I=0.0;
	PID_ALT.D=ctrl.roll.Core.kd;
	PID_POS.P=ctrl.pitch.Core.kp;
	PID_POS.I=0.0;
	PID_POS.D=ctrl.pitch.Core.kd;
	PID_PID_1.P=ctrl.yaw.Core.kp;
	PID_PID_1.I=0.0;
	PID_PID_1.D=ctrl.yaw.Core.kd ;
	PID_PID_2.P=0;
	PID_PID_2.I=0;
	PID_PID_2.D=0;
	PID_PID_3.P=0;
	PID_PID_3.I=0;
	PID_PID_3.D=0;
	ARMED=0;


//	//超声波的油门限高PID参数
//	PID_US100.P=150;
//	PID_US100.D=80;
//	PID_US100.I=0;
//	
//  PID_ADNS3080.P=20;
//	PID_ADNS3080.D=60;
//	
	//limit for the max increment
	ctrl.pitch.Shell.increment_max = 200;
	ctrl.roll.Shell.increment_max = 200;
	
	ctrl.ctrlRate = 0;
}

void PID_Reflash(void)
{
	ctrl.roll.Shell.kp=PID_ROL.P;							
	ctrl.roll.Shell.ki=PID_ROL.I;
	ctrl.roll.Shell.kd=PID_ROL.D;
	ctrl.pitch.Shell.kp=PID_PIT.P;
	ctrl.pitch.Shell.ki=PID_PIT.I;
	ctrl.pitch.Shell.kd=PID_PIT.D;
	ctrl.yaw.Shell.kp=PID_YAW.P;
	PID_YAW.I=0.0;
	ctrl.yaw.Shell.kd=PID_YAW.D;
	ctrl.roll.Core.kp=PID_ALT.P;
	PID_ALT.I=0.0;
	ctrl.roll.Core.kd=PID_ALT.D;
	ctrl.pitch.Core.kp=PID_POS.P;
	PID_POS.I=0.0;
	ctrl.pitch.Core.kd=PID_POS.D;
	ctrl.yaw.Core.kp=PID_PID_1.P;
	PID_PID_1.I=0.0;
	ctrl.yaw.Core.kd=PID_PID_1.D;
	PID_PID_2.P=0;
	PID_PID_2.I=0;
	PID_PID_2.D=0;
	PID_PID_3.P=0;
	PID_PID_3.I=0;
	PID_PID_3.D=0;
}
