#ifndef _DATA_TRANSFER_H_
#define _DATA_TRANSFER_H_
//#include "config.h"	//ȱʡ�С�������
#include "data_transfer.h"
#include "include.h"
extern u8 Data_Check,Send_Status,Send_Senser,Send_Senser_2,Send_RCData,Send_GpsData,Send_Offset,Send_PID1,Send_PID2,Send_PID3,Send_MotoPwm;		//ȱʡ�С�������


void Data_Receive_Anl(u8 *data_buf,u8 num);
void Send_Data(void);
void Data_Send_Status(void);	
void Data_Send_Senser(void);	
void Data_Send_Senser_2(void);	

void Data_Send_RCData(void);			//����ȱʡ�С�������
//void Data_Send_GpsData(void);
void Data_Send_OFFSET(void);	
void Data_Send_PID1(void);
void Data_Send_PID2(void);
void Data_Send_PID3(void);
void Data_Send_MotoPWM(void);

void NRF_Send_Test(void);

#endif
