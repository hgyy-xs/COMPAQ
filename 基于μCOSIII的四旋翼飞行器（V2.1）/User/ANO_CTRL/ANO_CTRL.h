#include "stm32f4xx.h"
#include "Para.h"

#define MAX_CTRL_ASPEED 	 	300.0f									//ROL,PIT����������ƽ��ٶ�
#define ANGLE_TO_MAX_AS 		30.0f										//�Ƕ����Nʱ���������ٶȴﵽ��󣨿���ͨ������CTRL_2��Pֵ������
#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )
#define ABS(x) ( (x)>0?(x):-(x) )
#define MAX_CTRL_ASPEED 	 	300.0f									//ROL,PIT����������ƽ��ٶ�
#define MAX_CTRL_YAW_SPEED 	150.0f									//YAW����������ƽ��ٶ�
#define CTRL_1_INT_LIMIT 		0.5f *MAX_CTRL_ASPEED		//�ڻ����ַ���
#define MAXMOTORS 		(4)		//�������
#define MAX_THR       80 			///%	����ͨ�����ռ��80%����20%��������
#define MAX_CTRL_ANGLE			25.0f										//ң���ܴﵽ�����Ƕ�
#define CTRL_2_INT_LIMIT 		0.5f *MAX_CTRL_ANGLE		//�⻷���ַ���
#define MAX_PWM				100			///%	���PWM���Ϊ100%����

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

void Ctrl_Para_Init(void);		//����Ĭ�ϲ���

