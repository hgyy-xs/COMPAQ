#ifndef __PARA_H__
#define __PARA_H__
#include "stm32f4xx.h"


typedef struct _PID
{
	float kp;
	float ki;
	float kd;
	float increment;
	float increment_max;
	float Err;
	float iErr;
	float dErr;
	float Err_old;
	float Pout;
	float Iout;
	float Dout;
	float Allout;
}PID_TypeDef;


struct PID_Cascade
{
	struct _PID Shell;
	struct _PID Core;
};

struct _ctrl
{
	u8  ctrlRate;
    struct PID_Cascade pitch;    
    struct PID_Cascade roll;  
	struct PID_Cascade yaw;  	
};  

struct _int16
{
	int16_t x;
	int16_t y;
	int16_t z;
};	

struct _float
{
	float x;
	float y;
	float z;
};
struct _trans
{
	struct _int16 origin;  //原始值
	struct _int16 averag;  //平均值
	struct _int16 histor;  //历史值
	struct _int16 quiet;   //静态值
	struct _float radian;  //弧度值 
};

struct _sensor
{   
	struct _trans acc;
	struct _trans gyro;
};

//#include "ANO_CTRL.h"

//extern xyz_f_t;
//extern MPU6050_STRUCT;

//extern xyz_f_t Gyro_deg;
//extern MPU6050_STRUCT mpu6050;

//typedef struct 
//{
//    float x;
//	float y;
//	float z;
//}xyz_f_t;


//typedef struct 
//{
//    s16 x;
//	s16 y;
//	s16 z;

//}xyz_s16_t;


//typedef struct
//{
//	float kp;
//	float kd;
//	float ki;
//	float kdamp;

//}pid_t;


//enum {
//    PIDROLL,
//    PIDPITCH,
//    PIDYAW,
//		PID4,
//		PID5,
//		PID6,

//		PIDITEMS
//};

//typedef struct
//{
//	xyz_f_t err;
//	xyz_f_t err_old;
//	xyz_f_t err_i;
//	xyz_f_t eliminate_I;
//	xyz_f_t err_d;
//	xyz_f_t damp;
//	xyz_f_t out;
//	pid_t 	PID[PIDITEMS];
//	xyz_f_t err_weight;
//	float FB;

//}ctrl_t;

//enum
//{
// A_X = 0,
// A_Y ,
// A_Z ,
// G_Y ,
// G_X ,
// G_Z ,
// TEM ,
// ITEMS ,
//}; 


//typedef struct 
//{
//	char Acc_CALIBRATE;
//	char Gyro_CALIBRATE;
//	char Cali_3d;
//  xyz_s16_t Acc_I16;
//	xyz_s16_t Gyro_I16;

//	xyz_f_t Acc;
//	xyz_f_t Gyro;

////	XYZ_STRUCT Acc_deg;
//	xyz_f_t Gyro_deg;
//	
//	xyz_f_t Acc_Offset;
//	xyz_f_t Gyro_Offset;
//	xyz_f_t Gyro_Auto_Offset;
//	xyz_f_t vec_3d_cali;
//	float Acc_Temprea_Offset;
//	float Gyro_Temprea_Offset;
//	
//	float Gyro_Temprea_Adjust;
//	float ACC_Temprea_Adjust;

//	s16 Tempreature;
//	float TEM_LPF;
//	float Ftempreature;
//}MPU6050_STRUCT;


//float my_deathzoom(float x,float zoom);

//float my_deathzoom_2(float x,float zoom);

//float To_180_degrees(float x);

#endif


