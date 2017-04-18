// #ifndef _ULTRA_CTRL_H
// #define _ULTRA_CTRL_H

#include "sys.h"
#define USE_KS103			1		//使用ks103型号超声波

extern float ultra_ctrl_out;

extern float height_ctrl_out;

extern u8 baro_ctrl_start;

extern float ultra_speed,wz_speed,baro_height;
extern u8 Rx_Buf[];

extern s8 ultra_start_f;
extern u16 ultra_distance;
extern s16 ultra_delta; 


extern float H_Test;

extern float thr_value;
extern u16 Set_Height;





void Ultrasonic_Init(void);
void Ultra_Duty(void);
void Ultra_Get(u8);



void Ultra_Init(u16 br_num);



/////////////////////////////////////////////////////////////////


void Usart2_Init(u32 br_num);
void Usart2_IRQ(void);
void Usart2_Send(unsigned char *DataToSend ,u8 data_num);

void Uart5_Init(u32 br_num);
void Uart5_IRQ(void);
void Uart5_Send(unsigned char *DataToSend ,u8 data_num);

/////////////////////////////////////////////////////////////////







//////////////////////////////////////////////////////////////////



void Height_Ctrl(float T,float thr);

void High_Ctrl(u16 thr);

void Ultra_PID_Init(void);

void WZ_Speed_PID_Init(void);

void height_speed_ctrl(float T,float thr,float exp_z_speed,float );


void Ultra_Ctrl(float T,float thr);

void Thr_Ctrl(float T);  //  T :内环(陀螺仪)准确的执行周期

//////////////////////////////////////////////////////////////////





// #endif
