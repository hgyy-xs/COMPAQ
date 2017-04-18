#include "data_transfer.h"
//#include "MPU6050.h"
#include "rc.h"
#include "Flash_Eeprom.h"
#include "include.h"
//采用上位机鼠标遥控控制宏定义
#define CONTROL_USE_RC


#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

u8 Data_Check,Send_Status,Send_Senser,Send_Senser_2,Send_RCData,Send_GpsData,Send_Offset,Send_PID1,Send_PID2,Send_PID3,Send_MotoPwm;	//数据标志位
u8 data_to_send[120];

static void ANO_DT_Send_Check(u8 head, u8 check_sum)
{	
	u8 sum = 0;
	u8 i=0;
	data_to_send[0]=0xAA;
	data_to_send[1]=0xAA;
	data_to_send[2]=0xEF;
	data_to_send[3]=2;
	data_to_send[4]=head;
	data_to_send[5]=check_sum;
	
	
	for(i=0;i<6;i++)
		sum += data_to_send[i];
	data_to_send[6]=sum;

	for(i=0;i<7;i++)
	AnBT_Uart1_Send_Char(data_to_send[i]);
}
void Data_Receive_Anl(u8 *data_buf,u8 num)	////////////////////////////暂时不用
{			

	vs16 rc_value_temp;
	u8 sum = 0;
	u8 i=0;
	for( i=0;i<(num-1);i++)
		sum += *(data_buf+i);
	if(!(sum==*(data_buf+num-1)))		return;		
	if(!(*(data_buf)==0xAA && *(data_buf+1)==0xAF))		return;		
/////////////////////////////////////////////////////////////////////////////////////
//	if(*(data_buf+2)==0X01)
//	{
//		if(*(data_buf+4)==0XA1&&*(data_buf+5)==0X03)
//		{
//			GYRO_OFFSET_OK = 0;
//			ACC_OFFSET_OK = 0;
//		}
//		if(*(data_buf+4)==0XA1&&*(data_buf+5)==0X01)
//			ACC_OFFSET_OK = 0;
//		if(*(data_buf+4)==0XA1&&*(data_buf+5)==0X02)
//			GYRO_OFFSET_OK = 0;
//		if(*(data_buf+4)==0XA2&&*(data_buf+5)==0X01)
//			Send_PID1 = 1;
//		if(*(data_buf+4)==0XA2&&*(data_buf+5)==0X02)
//			Send_PID2 = 1;
//		if(*(data_buf+4)==0XA2&&*(data_buf+5)==0X03)
//			Send_PID3 = 1;
//	}
#ifdef CONTROL_USE_RC
	if(*(data_buf+2)==0X03)
	{
		Rc_Data.THROTTLE = (vs16)(*(data_buf+4)<<8)|*(data_buf+5);
		Rc_Data.YAW = (vs16)(*(data_buf+6)<<8)|*(data_buf+7);
		Rc_Data.ROLL = (vs16)(*(data_buf+8)<<8)|*(data_buf+9);
		Rc_Data.PITCH = (vs16)(*(data_buf+10)<<8)|*(data_buf+11);
		Rc_Data.AUX1 = (vs16)(*(data_buf+12)<<8)|*(data_buf+13);
		Rc_Data.AUX2 = (vs16)(*(data_buf+14)<<8)|*(data_buf+15);
		Rc_Data.AUX3 = (vs16)(*(data_buf+16)<<8)|*(data_buf+17);
		Rc_Data.AUX4 = (vs16)(*(data_buf+18)<<8)|*(data_buf+19);
		Rc_Data.AUX5 = (vs16)(*(data_buf+20)<<8)|*(data_buf+21);
		Rc_Data.AUX6 = (vs16)(*(data_buf+21)<<8)|*(data_buf+22);
//		Rc_Data(&Rc_D,&Rc_C);
	}
#endif

	if(*(data_buf+2)==0X01)
	{
		if(*(data_buf+4)==0X01)
			ACC_OFFSET_OK = 0;
			Send_Offset=1;
		if(*(data_buf+4)==0X02)
			GYRO_OFFSET_OK = 0;
			Send_Offset=1;
		if(*(data_buf+4)==0X03)
		{
			ACC_OFFSET_OK = 0;
			GYRO_OFFSET_OK = 0;
		}
	}
	
	if(*(data_buf+2)==0X02)
	{
		if(*(data_buf+4)==0X01)
		{
			Send_PID1 = 1;
			Send_PID2 = 1;
			Send_PID3 = 1;
//			f.send_pid4 = 1;
//			f.send_pid5 = 1;
//			f.send_pid6 = 1;
		}
		if(*(data_buf+4)==0X02)
		{
			
		}
		if(*(data_buf+4)==0XA0)		
		{
//			f.send_version = 1;
		}
		if(*(data_buf+4)==0XA1)		
		{
			Para_ResetToFactorySetup();
		}
	}
	if(*(data_buf+2)==0X10)								//PID1
	{
			PID_ROL.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_ROL.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/1000;
			PID_ROL.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PIT.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_PIT.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/1000;
			PID_PIT.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_YAW.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_YAW.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_YAW.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
		    ANO_DT_Send_Check(*(data_buf+2),sum);
			Send_PID1 = 1;
	}
	if(*(data_buf+2)==0X11)								//PID2
	{
			PID_ALT.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_ALT.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_ALT.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_POS.P = (float)((vs16)(*(data_buf+10)<<8)|*(data_buf+11))/100;
			PID_POS.I = (float)((vs16)(*(data_buf+12)<<8)|*(data_buf+13))/100;
			PID_POS.D = (float)((vs16)(*(data_buf+14)<<8)|*(data_buf+15))/100;
			PID_PID_1.P = (float)((vs16)(*(data_buf+16)<<8)|*(data_buf+17))/100;
			PID_PID_1.I = (float)((vs16)(*(data_buf+18)<<8)|*(data_buf+19))/100;
			PID_PID_1.D = (float)((vs16)(*(data_buf+20)<<8)|*(data_buf+21))/100;
			ANO_DT_Send_Check(*(data_buf+2),sum);
			Send_PID2 = 1;
	}
	if(*(data_buf+2)==0X12)								//PID3
	{
			PID_PID_2.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_PID_2.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_PID_2.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			PID_PID_3.P = (float)((vs16)(*(data_buf+4)<<8)|*(data_buf+5))/100;
			PID_PID_3.I = (float)((vs16)(*(data_buf+6)<<8)|*(data_buf+7))/100;
			PID_PID_3.D = (float)((vs16)(*(data_buf+8)<<8)|*(data_buf+9))/100;
			ANO_DT_Send_Check(*(data_buf+2),sum);		
			Send_PID3 = 1;
			Data_Save();		//缺省中。。。。
			PID_Reflash();		//Myctrl 中PID参数更新
	}
	if(*(data_buf+2)==0X13)								//PID4
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X14)								//PID5
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
	if(*(data_buf+2)==0X15)								//PID6
	{
		ANO_DT_Send_Check(*(data_buf+2),sum);
	}
//	
//	
//	
//	
//	
///////////////////////////////////////////////////////////////////////////////////////////////////
	if(*(data_buf+2)==0x03)							
	{
////		u8 _cnt = 4;
////		#ifdef	RC_USE_DEADBAND
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.THROTTLE = RC_MIDDLE;
////			else
////				Rc_Data.THROTTLE = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.YAW = RC_MIDDLE;
////			else
////				Rc_Data.YAW = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.ROLL = RC_MIDDLE;
////			else
////				Rc_Data.ROLL = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.PITCH = RC_MIDDLE;
////			else
////				Rc_Data.PITCH = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.AUX1 = RC_MIDDLE;
////			else
////				Rc_Data.AUX1 = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.AUX2 = RC_MIDDLE;
////			else
////				Rc_Data.AUX2 = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.AUX3 = RC_MIDDLE;
////			else
////				Rc_Data.AUX3 = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.AUX4 = RC_MIDDLE;
////			else
////				Rc_Data.AUX4 = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.AUX5 = RC_MIDDLE;
////			else
////				Rc_Data.AUX5 = rc_value_temp;
////			rc_value_temp = (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			if((RC_MIDDLE-RC_DEADBAND) < rc_value_temp && rc_value_temp < (RC_MIDDLE+RC_DEADBAND))
////				Rc_Data.AUX6 = RC_MIDDLE;
////			else
////				Rc_Data.AUX6 = rc_value_temp;
////		#else
////			Rc_Data.THROTTLE 	= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.YAW				= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.ROLL			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.PITCH			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.AUX1			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.AUX2			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.AUX3			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.AUX4			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.AUX5			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////			Rc_Data.AUX6			= (vs16)(*(data_buf+_cnt)<<8)|*(data_buf+_cnt+1);
////			_cnt += 2;
////		#endif
////		Rc_DataCal();
	}
}
//void Send_Data(void)
//{
//	if(Send_Status)
//	{
//		Send_Status = 0;	//清除上传姿态数据标志位
//		Data_Send_Status();
//	}
//	else if(Send_GpsData)
//	{
//		Send_GpsData = 0;
////		Data_Send_GpsData();		//缺省中。。。。
//	}
//	else if(Send_Senser)
//	{
//		Send_Senser = 0;
//		Data_Send_Senser();
//	}
//	else if(Send_PID1)
//	{
//		Send_PID1 = 0;
//		Data_Send_PID1();
//	}
//	else if(Send_PID2)
//	{
//		Send_PID2 = 0;
//		Data_Send_PID2();
//	}
//	else if(Send_PID3)
//	{
//		Send_PID3 = 0;
//		Data_Send_PID3();
//	}
//	else if(Send_RCData)
//	{
//		Send_RCData = 0;
//		Data_Send_RCData();		//缺省中。。。。
//	}
//	else if(Send_Offset)
//	{
//		Send_Offset = 0;
//		Data_Send_OFFSET();
//	}
//	else if(Send_MotoPwm)
//	{
//		Send_MotoPwm = 0;
//		Data_Send_MotoPWM();
//	}
//}
void Send_Data(void)//针对ucos做出修改
{
	if(Send_Status)
	{
		Send_Status = 0;	//清除上传姿态数据标志位
		Data_Send_Status();
	}
	 if(Send_GpsData)
	{
		Send_GpsData = 0;
//		Data_Send_GpsData();		//缺省中。。。。
	}
	if(Send_Senser)
	{
		Send_Senser = 0;
		Data_Send_Senser();
	}
	if(Send_Senser)
	{
		Send_Senser_2 = 0;
		Data_Send_Senser_2();
	}
	 if(Send_PID1)
	{
		Send_PID1 = 0;
		Data_Send_PID1();
	}
	 if(Send_PID2)
	{
		Send_PID2 = 0;
		Data_Send_PID2();
	}
	 if(Send_PID3)
	{
		Send_PID3 = 0;
		Data_Send_PID3();
	}
	 if(Send_RCData)
	{
		Send_RCData = 0;
		Data_Send_RCData();		//缺省中。。。。
	}
	 if(Send_Offset)
	{
		Send_Offset = 0;
		Data_Send_OFFSET();
	}
	 if(Send_MotoPwm)
	{
		Send_MotoPwm = 0;
		Data_Send_MotoPWM();
	}
}
void Data_Send_Status(void)
{	
	vs16 _temp;
	vs32 _temp2;
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x01;
	data_to_send[_cnt++]=0;
	
	_temp = (int)(Q_ANGLE_DATA.ROLL*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Q_ANGLE_DATA.PITCH*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = (int)(Q_ANGLE_DATA.YAW*100);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp2=(int)(ultra_distance*10);
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	
	if(Height_Ctl)
	data_to_send[_cnt++] = 0x02;	//飞行模式  姿态（0x01）定高（0x02）定点（0x03）
	else if(!Height_Ctl)
			data_to_send[_cnt++] = 0x01;	//飞行模式  姿态（0x01）定高（0x02）定点（0x03）

	data_to_send[_cnt++] = ARMED;
	data_to_send[3] = _cnt-4;	
	

	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}
void Data_Send_Senser(void)		
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	vs16 _temp;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x02;
	data_to_send[_cnt++]=0;
	_temp=MPU6050_ACC_DATA.X;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp=MPU6050_ACC_DATA.Y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp=MPU6050_ACC_DATA.Z;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp=MPU6050_GYRO_DATA.X;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp=MPU6050_GYRO_DATA.Y;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp=MPU6050_GYRO_DATA.Z;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
//	_temp=;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);

//	data_to_send[_cnt++]=BYTE1(AK8975_Data[0]);		//缺省中。。。。
//	data_to_send[_cnt++]=BYTE0(AK8975_Data[0]);
//	data_to_send[_cnt++]=BYTE1(AK8975_Data[1]);
//	data_to_send[_cnt++]=BYTE0(AK8975_Data[1]);
//	data_to_send[_cnt++]=BYTE1(AK8975_Data[2]);
//	data_to_send[_cnt++]=BYTE0(AK8975_Data[2]);
	
	data_to_send[3] = _cnt-4;	
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}

void Data_Send_Senser_2(void)		
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	u16 _temp;
	vs32 _temp2;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x07;
	data_to_send[_cnt++]=0;

	_temp2=(vs32)(ultra_distance*10);
	data_to_send[_cnt++]=BYTE3(_temp2);
	data_to_send[_cnt++]=BYTE2(_temp2);
	data_to_send[_cnt++]=BYTE1(_temp2);
	data_to_send[_cnt++]=BYTE0(_temp2);
	_temp=(u16)(Set_Height*10);
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);

//	_temp=ultra_distance;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp=MPU6050_GYRO_DATA.Y;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
//	_temp=MPU6050_GYRO_DATA.Z;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);
	
//	_temp=;
//	data_to_send[_cnt++]=BYTE1(_temp);
//	data_to_send[_cnt++]=BYTE0(_temp);

//	data_to_send[_cnt++]=BYTE1(AK8975_Data[0]);		//缺省中。。。。
//	data_to_send[_cnt++]=BYTE0(AK8975_Data[0]);
//	data_to_send[_cnt++]=BYTE1(AK8975_Data[1]);
//	data_to_send[_cnt++]=BYTE0(AK8975_Data[1]);
//	data_to_send[_cnt++]=BYTE1(AK8975_Data[2]);
//	data_to_send[_cnt++]=BYTE0(AK8975_Data[2]);
	
	data_to_send[3] = _cnt-4;	
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++] = sum;
	
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}

void Data_Send_RCData(void)				
{
	u8 _cnt=0;
	u8 i=0;
	u8 sum = 0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x03;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Rc_Data.THROTTLE);
	data_to_send[_cnt++]=BYTE0(Rc_Data.THROTTLE);
	data_to_send[_cnt++]=BYTE1(Rc_Data.YAW);
	data_to_send[_cnt++]=BYTE0(Rc_Data.YAW);
	data_to_send[_cnt++]=BYTE1(Rc_Data.ROLL);
	data_to_send[_cnt++]=BYTE0(Rc_Data.ROLL);
	data_to_send[_cnt++]=BYTE1(Rc_Data.PITCH);
	data_to_send[_cnt++]=BYTE0(Rc_Data.PITCH);
	data_to_send[_cnt++]=BYTE1(Rc_Data.AUX1);
	data_to_send[_cnt++]=BYTE0(Rc_Data.AUX1);
	data_to_send[_cnt++]=BYTE1(Rc_Data.AUX2);
	data_to_send[_cnt++]=BYTE0(Rc_Data.AUX2);
	data_to_send[_cnt++]=BYTE1(Rc_Data.AUX3);
	data_to_send[_cnt++]=BYTE0(Rc_Data.AUX3);
	data_to_send[_cnt++]=BYTE1(Rc_Data.AUX4);
	data_to_send[_cnt++]=BYTE0(Rc_Data.AUX4);
	data_to_send[_cnt++]=BYTE1(Rc_Data.AUX5);
	data_to_send[_cnt++]=BYTE0(Rc_Data.AUX5);
	data_to_send[_cnt++]=BYTE1(Rc_Data.AUX6);
	data_to_send[_cnt++]=BYTE0(Rc_Data.AUX6);	
	data_to_send[3] = _cnt-4;	
	for(i=0;i<_cnt;i++)
		sum += data_to_send[i];
	
	data_to_send[_cnt++]=sum;
	
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}

void Data_Send_MotoPWM(void)
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x06;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(Moto_PWM_1);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_1);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_2);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_2);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_3);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_3);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_4);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_4);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_5);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_5);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_6);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_6);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_7);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_7);
	data_to_send[_cnt++]=BYTE1(Moto_PWM_8);
	data_to_send[_cnt++]=BYTE0(Moto_PWM_8);
	data_to_send[3] = _cnt-4;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}
void Data_Send_OFFSET(void)					//暂时不用！！！
{
	u8 _cnt=0;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x16;
	data_to_send[_cnt++]=0;
	data_to_send[_cnt++]=BYTE1(ACC_OFFSET.X);
	data_to_send[_cnt++]=BYTE0(ACC_OFFSET.X);
	data_to_send[_cnt++]=BYTE1(ACC_OFFSET.Y);
	data_to_send[_cnt++]=BYTE0(ACC_OFFSET.Y);
	data_to_send[_cnt++]=BYTE1(ACC_OFFSET.Z);
	data_to_send[_cnt++]=BYTE0(ACC_OFFSET.Z);
	data_to_send[_cnt++]=BYTE1(GYRO_OFFSET.X);
	data_to_send[_cnt++]=BYTE0(GYRO_OFFSET.X);
	data_to_send[_cnt++]=BYTE1(GYRO_OFFSET.Y);
	data_to_send[_cnt++]=BYTE0(GYRO_OFFSET.Y);
	data_to_send[_cnt++]=BYTE1(GYRO_OFFSET.Z);
	data_to_send[_cnt++]=BYTE0(GYRO_OFFSET.Z);	
	data_to_send[3] = _cnt-4;	
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];	
	data_to_send[_cnt++]=sum;	
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}
void Data_Send_PID1(void)		
{
	u8 _cnt=0;
	vs16 _temp;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x10;
	data_to_send[_cnt++]=0;
	
	_temp = PID_ROL.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ROL.I * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ROL.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PIT.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PIT.I * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PIT.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_YAW.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_YAW.I * 1000;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_YAW.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);	
	data_to_send[3] = _cnt-4;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];
	data_to_send[_cnt++]=sum;	
		for(i=0;i<_cnt;i++)
		AnBT_Uart1_Send_Char(data_to_send[i]);
}
void Data_Send_PID2(void)		//数据缺省中。。。
{
	u8 _cnt=0;
	vs16 _temp;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x11;
	data_to_send[_cnt++]=0;

	_temp = PID_ALT.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ALT.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_ALT.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_POS.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_POS.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_POS.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_1.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_1.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_1.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];	
	data_to_send[_cnt++]=sum;	
		for(i=0;i<_cnt;i++)		
		AnBT_Uart1_Send_Char(data_to_send[i]);
}
void Data_Send_PID3(void)		//数据缺省中。。。
{
	u8 _cnt=0;
	vs16 _temp;
	u8 sum = 0;
	u8 i=0;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0xAA;
	data_to_send[_cnt++]=0x12;
	data_to_send[_cnt++]=0;
	
	_temp = PID_PID_2.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_2.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_2.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_3.P * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_3.I * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	_temp = PID_PID_3.D * 100;
	data_to_send[_cnt++]=BYTE1(_temp);
	data_to_send[_cnt++]=BYTE0(_temp);
	
	data_to_send[3] = _cnt-4;
	for( i=0;i<_cnt;i++)
		sum += data_to_send[i];	
	data_to_send[_cnt++]=sum;
	for(i=0;i<_cnt;i++)
	AnBT_Uart1_Send_Char(data_to_send[i]);
}

