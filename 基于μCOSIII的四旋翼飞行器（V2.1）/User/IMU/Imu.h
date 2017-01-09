#ifndef _IMU_H_
#define _IMU_H_
#include "stm32f4xx.h"

#define DATA_TRANSFER_USE_USART

typedef struct
{
	float X;
	float Y;
	float Z;
}MPU6050_Data_TypeDef;	

typedef struct
{
	float X;
	float Y;
	float Z;
}MPU6050_OFFSET_TypeDef;

//typedef struct
//{
//	float YAW;
//	float ROLL;
//	float PITCH;
//}QANGLE_OFFSET_typedef;

typedef struct
{
	float YAW;
	float ROLL;
	float PITCH;
}MPU6050_Q_AngleData_TypeDef;


extern MPU6050_Data_TypeDef	MPU6050_GYRO_DATA,MPU6050_ACC_DATA;
extern MPU6050_Q_AngleData_TypeDef Q_ANGLE_DATA;

extern MPU6050_OFFSET_TypeDef		GYRO_OFFSET,ACC_OFFSET,Q_ANGLE_OFFSET;			
extern u8	GYRO_OFFSET_OK;
extern u8	ACC_OFFSET_OK;


void IMU_Update(void);		
void MPU6050_OFFSET(void);

#endif
