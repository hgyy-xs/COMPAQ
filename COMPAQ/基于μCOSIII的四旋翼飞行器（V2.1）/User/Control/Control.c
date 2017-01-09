#include "control.h"
//#include "MPU6050.h"
#include "Rc.h"
#include "imu.h"
#include "tim_pwm_out.h"
//#include "ANO_CTRL.h"

PID PID_ROL,PID_PIT,PID_YAW,PID_ALT,PID_POS,PID_PID_1,PID_PID_2,PID_PID_3;


extern MPU6050_Data_TypeDef	MPU6050_GYRO_DATA,MPU6050_ACC_DATA ;
extern MPU6050_Q_AngleData_TypeDef Q_ANGLE_DATA;


int16_t getlast_roll=0,geilast_pitch=0;
float rol_i=0,pit_i=0,yaw_p=0;

vs16 Moto_PWM_1=0,Moto_PWM_2=0,Moto_PWM_3=0,Moto_PWM_4=0,Moto_PWM_5=0,Moto_PWM_6=0,Moto_PWM_7=0,Moto_PWM_8=0;
void CONTROL(float rol, float pit, float yaw)
{

	rol=rol+(1501-Rc_Data.ROLL)/20;		//rc1525  若向右有微调则减小1477


	
	rol_i+=rol;
	if(rol_i>2000)
	rol_i=2000;
	if(rol_i<-2000)
	rol_i=-2000;

	PID_ROL.pout = PID_ROL.P * rol;
	PID_ROL.dout = PID_ROL.D * MPU6050_GYRO_DATA.X;
	PID_ROL.iout = PID_ROL.I * rol_i;

	pit=pit+(Rc_Data.PITCH-1501)/20;	//rc1519 若向前微调则减小1480
	
	pit_i+=pit;
	if(pit_i>2000)
	pit_i=2000;
	if(pit_i<-2000)
	pit_i=-2000;

	PID_PIT.pout = PID_PIT.P * pit;
	PID_PIT.dout = PID_PIT.D * MPU6050_GYRO_DATA.Y;
	PID_PIT.iout = PID_PIT.I * pit_i;
	if(Rc_Data.YAW<1480||Rc_Data.YAW>1520)
	{MPU6050_GYRO_DATA.Z=MPU6050_GYRO_DATA.Z-(Rc_Data.YAW-1500)*10;}
	yaw_p+=MPU6050_GYRO_DATA.Z*0.0609756f*0.002f;// +(Rc_Get.YAW-1500)*30


	PID_YAW.pout=PID_YAW.P*yaw_p;
	PID_YAW.dout = PID_YAW.D * MPU6050_GYRO_DATA.Z;				   
	
if(Rc_Data.THROTTLE<1300)
		{
		pit_i=0;
		rol_i=0;
		yaw_p=0;
		}

	PID_ROL.OUT = (PID_ROL.pout) +PID_ROL.iout +PID_ROL.dout;
	PID_PIT.OUT = PID_PIT.pout + PID_PIT.iout + PID_PIT.dout;
	PID_YAW.OUT = PID_YAW.pout + PID_YAW.iout + PID_YAW.dout;
 
	if(Rc_Data.THROTTLE>1050&&ARMED)
	{
		Moto_PWM_1 = Rc_Data.THROTTLE - 1000 - PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT;
		Moto_PWM_2 = Rc_Data.THROTTLE - 1000 + PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT;
		Moto_PWM_3 = Rc_Data.THROTTLE - 1000 + PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT;
		Moto_PWM_4 = Rc_Data.THROTTLE - 1000 - PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT;
	}
	else
	{
		Moto_PWM_1 = 0;
		Moto_PWM_2 = 0;
		Moto_PWM_3 = 0;
		Moto_PWM_4 = 0;

	}						
/////////////以下是在没有遥控器情况下临时用于测试电机油门数据！！！！
//		Moto_PWM_1 = 1200 - 1000 - PID_ROL.OUT - PID_PIT.OUT + PID_YAW.OUT;
//		Moto_PWM_2 = 1200 - 1000 + PID_ROL.OUT - PID_PIT.OUT - PID_YAW.OUT;
//		Moto_PWM_3 = 1200 - 1000 + PID_ROL.OUT + PID_PIT.OUT + PID_YAW.OUT;
//		Moto_PWM_4 = 1200 - 1000 - PID_ROL.OUT + PID_PIT.OUT - PID_YAW.OUT;

	Moto_PwmRflash(Moto_PWM_1,Moto_PWM_2,Moto_PWM_3,Moto_PWM_4);
				
}
void Para_ResetToFactorySetup(void)
{
	PID_ROL.P=8.5;							
	PID_ROL.I=0.001;
	PID_ROL.D=0.21;
	PID_PIT.P=8.5;
	PID_PIT.I=0.001;
	PID_PIT.D=0.21;
	PID_YAW.P=8.5;
	PID_YAW.I=0.000;
	PID_YAW.D=0.30;
	PID_ALT.P=0;
	PID_ALT.I=0;
	PID_ALT.D=0;
	PID_POS.P=0;
	PID_POS.I=0;
	PID_POS.D=0;
	PID_PID_1.P=0;
	PID_PID_1.I=0;
	PID_PID_1.D=0;
	PID_PID_2.P=0;
	PID_PID_2.I=0;
	PID_PID_2.D=0;
	PID_PID_3.P=0;
	PID_PID_3.I=0;
	PID_PID_3.D=0;
	ARMED=0;

}

/*========================================

更新ANO_CTRL要用到的数据

=========================================*/
//void Para_ResetToFactorySetup(void)
//{
//	PID_ROL.P=1.0;							
//	PID_ROL.I=1.0;
//	PID_ROL.D=1.0;
//	PID_PIT.P=1.0;
//	PID_PIT.I=1.0;
//	PID_PIT.D=1.0;
//	PID_YAW.P=1.0;
//	PID_YAW.I=1.0;
//	PID_YAW.D=1.0;
//	PID_ALT.P=1.0;
//	PID_ALT.I=0.5;
//	PID_ALT.D=1.0;
//	PID_POS.P=1.0;
//	PID_POS.I=0.5;
//	PID_POS.D=1.0;
//	PID_PID_1.P=2.0;
//	PID_PID_1.I=0.5;
//	PID_PID_1.D=1.0;
//	PID_PID_2.P=0;
//	PID_PID_2.I=0;
//	PID_PID_2.D=0;
//	PID_PID_3.P=0;
//	PID_PID_3.I=0;
//	PID_PID_3.D=0;
//	ARMED=0;
//	
//ctrl_2.PID[PIDROLL].kp=PID_ROL.P;//外环默认参数
//ctrl_2.PID[PIDROLL].ki=PID_ROL.I;
//ctrl_2.PID[PIDROLL].kd=PID_ROL.D;

//ctrl_2.PID[PIDPITCH].kp=PID_PIT.P;
//ctrl_2.PID[PIDPITCH].ki=PID_PIT.I;
//ctrl_2.PID[PIDPITCH].kd=PID_PIT.D;

//ctrl_2.PID[PIDYAW].kp=PID_YAW.P;
//ctrl_2.PID[PIDYAW].ki=PID_YAW.I;
//ctrl_2.PID[PIDYAW].kd=PID_YAW.D;

//ctrl_1.PID[PIDROLL].kp=PID_ALT.P;//内环默认参数
//ctrl_1.PID[PIDROLL].ki=PID_ALT.I;
//ctrl_1.PID[PIDROLL].kd=PID_ALT.D;

//ctrl_1.PID[PIDPITCH].kp=PID_POS.P;
//ctrl_1.PID[PIDPITCH].ki=PID_POS.I;
//ctrl_1.PID[PIDPITCH].kd=PID_POS.D;

//ctrl_1.PID[PIDYAW].kp=PID_PID_1.P;
//ctrl_1.PID[PIDYAW].ki=PID_PID_1.I;
//ctrl_1.PID[PIDYAW].kd=PID_PID_1.D;

// Ctrl_Para_Init();
//}

