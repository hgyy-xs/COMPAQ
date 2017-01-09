#ifndef _MPU6050_H_
#define _MPU6050_H_
#include "stm32f4xx.h"
#include "anbt_dmp_mpu6050.h"
#include "anbt_dmp_driver.h"
#include "anbt_dmp_fun.h"
#define DATA_TRANSFER_USE_USART
extern u8 IMU_Update_State;	//ÿ�λ�ȡ����̬���־λ��1

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

extern MPU6050_OFFSET_TypeDef		GYRO_OFFSET,ACC_OFFSET;			//������Ʈ����Ҫ�õ�����������
extern u8	GYRO_OFFSET_OK;
extern u8	ACC_OFFSET_OK;


void IMU_Update(void);		//���µ�ǰ��̬
void MPU6050_OFFSET(void);	//��ƮУ׼

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
//extern T_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//����һ�ζ�ȡֵ
//extern T_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//��Ư
//extern u8							GYRO_OFFSET_OK;
//extern u8							ACC_OFFSET_OK;

//void MPU6050_Init(void);
//u8 MPU6050_Read(void);	//��ʼ����6050ԭʼ���ݣ���iicæ���� 0 ��iic����д�ɹ� ���� 1 
//void MPU6050_Dataanl(void);	//��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���

//#endif
