
#include "include.h"
#include "Para.h"

#define RtA 		57.295779 
#define AtR    	0.0174624				 	//  1/RtA             RtA����     //�ȵ��Ƕ�  //1����=180/3.14(��) 1��=3.14/180(����)��	
#define Acc_G 	0.0011963				  //  1/(32768/4/9.8)   ���ٶ�����Ϊ4G	
#define Gyro_G 	0.03051756				//  1/32.768          ����������Ϊ +��1000     	//   �˲�����Ӧ����1000��ÿ��  //1/32.8LSB/S.
#define Gyro_Gr	0.0005326         //  1/32.768/57.3                               // �˲�����Ӧ����2000��ÿ��  //Gyro_G/RtA.




void Prepare_IMU(void);

void My_Control(float rol, float pit, float yaw);

void paramLoad(void);//��ʼĬ�ϲ���

void PID_Reflash(void);	//�������θ���

void Touch_Down(void);
