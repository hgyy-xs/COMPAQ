
#include "include.h"
#include "Para.h"

#define RtA 		57.295779 
#define AtR    	0.0174624				 	//  1/RtA             RtA倒数     //度到角度  //1弧度=180/3.14(度) 1度=3.14/180(弧度)。	
#define Acc_G 	0.0011963				  //  1/(32768/4/9.8)   加速度量程为4G	
#define Gyro_G 	0.03051756				//  1/32.768          陀螺仪量程为 +—1000     	//   此参数对应陀螺1000度每秒  //1/32.8LSB/S.
#define Gyro_Gr	0.0005326         //  1/32.768/57.3                               // 此参数对应陀螺2000度每秒  //Gyro_G/RtA.




void Prepare_IMU(void);

void My_Control(float rol, float pit, float yaw);

void paramLoad(void);//初始默认参数

void PID_Reflash(void);	//数传调参更新

void Touch_Down(void);
