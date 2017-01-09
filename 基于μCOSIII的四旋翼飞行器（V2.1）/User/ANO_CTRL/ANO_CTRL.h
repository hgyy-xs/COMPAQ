#include "stm32f4xx.h"
#include "Para.h"

#define MAX_CTRL_ASPEED 	 	300.0f									//ROL,PIT允许的最大控制角速度
#define ANGLE_TO_MAX_AS 		30.0f										//角度误差N时，期望角速度达到最大（可以通过调整CTRL_2的P值调整）
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define ABS(x) ( (x)>0?(x):-(x) )
#define MAX_CTRL_ASPEED 	 	300.0f									//ROL,PIT允许的最大控制角速度
#define MAX_CTRL_YAW_SPEED 	150.0f									//YAW允许的最大控制角速度
#define CTRL_1_INT_LIMIT 		0.5f *MAX_CTRL_ASPEED		//内环积分幅度
#define MAXMOTORS 		(4)		//电机数量
#define MAX_THR       80 			///%	油门通道最大占比80%，留20%给控制量
#define MAX_CTRL_ANGLE			25.0f										//遥控能达到的最大角度
#define CTRL_2_INT_LIMIT 		0.5f *MAX_CTRL_ANGLE		//外环积分幅度
#define MAX_PWM				100			///%	最大PWM输出为100%油门

#define TO_ANGLE 				0.06103f 		//0.061036 //   4000/65536  +-2000   ???


#define INIT_DUTY 4000 //u16(1000/0.25)
#define ACCURACY 10000 //u16(2500/0.25) //accuracy
#define PWM_RADIO 4//(8000 - 4000)/1000.0


extern xyz_f_t Gyro_deg;
extern MPU6050_STRUCT mpu6050;
extern ctrl_t ctrl_1;
extern ctrl_t ctrl_2;

void CTRL_2(float T);
void CTRL_1(float T);  //x roll,y pitch,z yaw

void Thr_Ctrl(float T);
void All_Out(float out_roll,float out_pitch,float out_yaw);

void Ctrl_Para_Init(void);		//设置默认参数

