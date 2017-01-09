#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "stm32f4xx.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"
#include "anbt_dmp_fun.h"
#define DATA_TRANSFER_USE_USART
extern u8 IMU_Update_State;	//每次获取完姿态后标志位置1

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
}MPU6050_Data_TypeDef;	

typedef struct
{
	int16_t X;
	int16_t Y;
	int16_t Z;
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

extern MPU6050_OFFSET_TypeDef		GYRO_OFFSET,ACC_OFFSET;			//定义零飘数据要用到的数据类型
extern u8	GYRO_OFFSET_OK;
extern u8	ACC_OFFSET_OK;


void IMU_Update(void);		//更新当前姿态
void MPU6050_OFFSET(void);	//零飘校准

#endif
//#ifndef __MPU6050_H_
//#define __MPU6050_H_

//#include "stm32f4xx.h"
//#include "ANO_Tech_STM32_I2C.h"
////#include "flash_eeprom.h"

//#define devAddr  0xD0

//typedef struct{
//				int16_t X;
//				int16_t Y;
//				int16_t Z;}T_INT16_XYZ;
//extern T_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//最新一次读取值
//extern T_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//零漂
//extern u8							GYRO_OFFSET_OK;
//extern u8							ACC_OFFSET_OK;

//void MPU6050_Init(void);
//u8 MPU6050_Read(void);	//初始化读6050原始数据，若iic忙返回 0 若iic触发写成功 返回 1 
//void MPU6050_Dataanl(void);	//将iic读取到得数据分拆,放入相应寄存器

//#endif
