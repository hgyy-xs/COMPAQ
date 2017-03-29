#include "imu.h"
#include "JY-901.h"
#include "USART2.h"
#include "data_transfer.h"
#include "Flash_Eeprom.h"
//#include "ANO_CTRL.h"
#include "My_ctrl.h"


#define q30  1073741824.0f

MPU6050_Data_TypeDef	MPU6050_GYRO_DATA,MPU6050_ACC_DATA;
MPU6050_Q_AngleData_TypeDef Q_ANGLE_DATA;
MPU6050_OFFSET_TypeDef		GYRO_OFFSET,ACC_OFFSET,Q_ANGLE_OFFSET;			
u8	GYRO_OFFSET_OK = 1;
u8	ACC_OFFSET_OK = 1;



void IMU_Update()		//四元数转换欧拉角
{
//	static unsigned long sensor_timestamp;
	static int16_t gyro[3], accel[3], sensors;
//	static unsigned char more;
//	static long quat[4];//四元数存放数组
	static float Yaw,Roll,Pitch;//欧拉角
//	static float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//计算姿态过程用到的变量

//	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	

//	if(sensors&INV_WXYZ_QUAT)
//	{
//	 q0=quat[0] / q30;
//	 q1=quat[1] / q30;
//	 q2=quat[2] / q30;
//	 q3=quat[3] / q30;
//	 Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
//	 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
//	 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;		
//	//	printf("pitch: %.2f    roll:%.2f		yaw:%.2f\r\n",Pitch,Roll,Yaw);		//普通串口输出
//	//	 printf("X_A:  %d   Y_A:  %d  Z_A:  %d\r\n",accel[0],accel[1],accel[2]);
//	//	 printf("                                      \r\n");
//	//	 printf("X_G:  %d   Y_G:  %d  Z_G:  %d\r\n",gyro[0],gyro[1],gyro[2]);
//	//	 printf("                                      \r\n");
//	//	Data_Send_Status(Pitch,Roll,Yaw,gyro,accel);
//	//	Send_Data(gyro,accel);
//	//	delay_init(168);

	Roll=(float)stcAngle.Angle[0]/32768*180;
	Pitch=(float)stcAngle.Angle[1]/32768*180;
	Yaw=(float)stcAngle.Angle[2]/32768*180;
	
//	gyro[0]=(float)stcGyro.w[0]/32768*2000;
//	gyro[1]=(float)stcGyro.w[1]/32768*2000;
//	gyro[2]=(float)stcGyro.w[2]/32768*2000;
//	
//	accel[0]=(float)stcAcc.a[0]/32768*160;
//	accel[1]=(float)stcAcc.a[1]/32768*160;
//	accel[2]=(float)stcAcc.a[2]/32768*160;
	gyro[0]=stcGyro.w[0];
	gyro[1]=stcGyro.w[1];
	gyro[2]=stcGyro.w[2];
	
	accel[0]=stcAcc.a[0];
	accel[1]=stcAcc.a[1];
	accel[2]=stcAcc.a[2];
	
	MPU6050_GYRO_DATA.X=gyro[0]-GYRO_OFFSET.X;
	MPU6050_GYRO_DATA.Y=gyro[1]-GYRO_OFFSET.Y;
	MPU6050_GYRO_DATA.Z=gyro[2]-GYRO_OFFSET.Z;
	MPU6050_ACC_DATA.X=accel[0]-ACC_OFFSET.X;
	MPU6050_ACC_DATA.Y=accel[1]-ACC_OFFSET.Y;
	MPU6050_ACC_DATA.Z=accel[2]-ACC_OFFSET.Z;
	Q_ANGLE_DATA.YAW=Yaw-Q_ANGLE_OFFSET.Z;
	Q_ANGLE_DATA.ROLL=Roll-Q_ANGLE_OFFSET.X;
	Q_ANGLE_DATA.PITCH=Pitch-Q_ANGLE_OFFSET.Y;
//	Q_ANGLE_DATA.YAW=Yaw;
//	Q_ANGLE_DATA.ROLL=Roll;
//	Q_ANGLE_DATA.PITCH=Pitch;
//					Data_Send_Status();


//	}	
	MPU6050_OFFSET();
	
/*========================================

更新ANO_CTRL要用到的数据

=========================================*/
 Prepare_IMU();

//	mpu6050.Gyro_deg.x = mpu6050.Gyro.x *TO_ANGLE;
//	mpu6050.Gyro_deg.y = mpu6050.Gyro.y *TO_ANGLE;
//	mpu6050.Gyro_deg.z = mpu6050.Gyro.z *TO_ANGLE;
	
} 
void MPU6050_OFFSET(void)
{
	if(!GYRO_OFFSET_OK)
	{
		static int32_t	tempgx=0,tempgy=0,tempgz=0,tempqroll=0,tempqpitch=0,tempqyaw=0;
		static uint8_t cnt_g=0;
// 		LED1_ON;
		if(cnt_g==0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			Q_ANGLE_OFFSET.X=0;
			Q_ANGLE_OFFSET.Y=0;
			Q_ANGLE_OFFSET.Z=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			tempqroll=0;
			tempqpitch=0;
			tempqyaw=0;
			cnt_g = 1;
			return;
		}
		tempgx+= MPU6050_GYRO_DATA.X;
		tempgy+= MPU6050_GYRO_DATA.Y;
		tempgz+= MPU6050_GYRO_DATA.Z;
		tempqroll+=Q_ANGLE_DATA.ROLL;
		tempqpitch+=Q_ANGLE_DATA.PITCH;
		tempqyaw+=Q_ANGLE_DATA.YAW;
		if(cnt_g==20)
		{
			GYRO_OFFSET.X=tempgx/cnt_g;
			GYRO_OFFSET.Y=tempgy/cnt_g;
			GYRO_OFFSET.Z=tempgz/cnt_g;
			Q_ANGLE_OFFSET.X=tempqroll/cnt_g;
			Q_ANGLE_OFFSET.Y=tempqpitch/cnt_g;
			Q_ANGLE_OFFSET.Z=tempqyaw/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
			Data_Save();
			return;
		}
		cnt_g++;
	}
	if(!ACC_OFFSET_OK)
	{
		static int32_t	tempax=0,tempay=0,tempaz=0;
		static uint8_t cnt_a=0;
// 		LED1_ON;
		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= MPU6050_ACC_DATA.X;
		tempay+= MPU6050_ACC_DATA.Y;
		//tempaz+= MPU6050_ACC_DATA.Z;
		if(cnt_a==20)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			Data_Save();
			return;
		}
		cnt_a++;		
	}
}
