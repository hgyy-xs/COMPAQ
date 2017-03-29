//#include "anbt_dmp_mpu6050.h"
#include <stm32f4xx.h>
//#include "MPU6050.h"
#include "data_transfer.h"
#include "Flash_Eeprom.h"

#define q30  1073741824.0f
//#define BYTE0(dwTemp)       (*(char *)(&dwTemp))
//#define BYTE1(dwTemp)       (*((char *)(&dwTemp) + 1))
//#define BYTE2(dwTemp)       (*((char *)(&dwTemp) + 2))
//#define BYTE3(dwTemp)       (*((char *)(&dwTemp) + 3))

u8 IMU_Update_State;	//DMP姿态结算完成标志位

MPU6050_Data_TypeDef	MPU6050_GYRO_DATA,MPU6050_ACC_DATA;
MPU6050_Q_AngleData_TypeDef Q_ANGLE_DATA;
MPU6050_OFFSET_TypeDef		GYRO_OFFSET,ACC_OFFSET,Q_ANGLE_OFFSET;			//定义零飘数据要用到的数据类型
u8	GYRO_OFFSET_OK = 1;
u8	ACC_OFFSET_OK = 1;

void IMU_Update()		//四元数转换欧拉角
{	
	static unsigned long sensor_timestamp;
	static short gyro[3], accel[3], sensors;//陀螺仪存放数组，加速度存放数组，返回状态量
	static unsigned char more;
	static long quat[4];//四元数存放数组
	static float Yaw=0.00,Roll,Pitch;//欧拉角
	static float q0=1.0f,q1=0.0f,q2=0.0f,q3=0.0f;//计算姿态过程用到的变量

	dmp_read_fifo(gyro, accel, quat, &sensor_timestamp, &sensors,&more);	

	if(sensors&INV_WXYZ_QUAT)
	{
	 q0=quat[0] / q30;
	 q1=quat[1] / q30;
	 q2=quat[2] / q30;
	 q3=quat[3] / q30;
	 Pitch = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
	 Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	 Yaw = 	atan2(2*(q1*q2 + q0*q3),q0*q0+q1*q1-q2*q2-q3*q3) * 57.3;		
	//	printf("pitch: %.2f    roll:%.2f		yaw:%.2f\r\n",Pitch,Roll,Yaw);		//普通串口输出
	//	 printf("X_A:  %d   Y_A:  %d  Z_A:  %d\r\n",accel[0],accel[1],accel[2]);
	//	 printf("                                      \r\n");
	//	 printf("X_G:  %d   Y_G:  %d  Z_G:  %d\r\n",gyro[0],gyro[1],gyro[2]);
	//	 printf("                                      \r\n");
	//	Data_Send_Status(Pitch,Roll,Yaw,gyro,accel);
	//	Send_Data(gyro,accel);
	//	delay_init(168);
	MPU6050_GYRO_DATA.X=gyro[0]-GYRO_OFFSET.X;
	MPU6050_GYRO_DATA.Y=gyro[1]-GYRO_OFFSET.Y;
	MPU6050_GYRO_DATA.Z=gyro[2]-GYRO_OFFSET.Z;
	MPU6050_ACC_DATA.X=accel[0]-ACC_OFFSET.X;
	MPU6050_ACC_DATA.Y=accel[1]-ACC_OFFSET.Y;
	MPU6050_ACC_DATA.Z=accel[2]-ACC_OFFSET.Z;
	Q_ANGLE_DATA.YAW=Yaw-Q_ANGLE_OFFSET.Z;
	Q_ANGLE_DATA.ROLL=Roll-Q_ANGLE_OFFSET.X;
	Q_ANGLE_DATA.PITCH=Pitch-Q_ANGLE_OFFSET.Y;
//					Data_Send_Status();

	IMU_Update_State=1;

	}	
	MPU6050_OFFSET();
	
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
		if(cnt_g==200)
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
		//tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
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





//#include "MPU6050.h"

//#define MPU6050_ADDRESS_AD0_LOW     0x68 // address pin low (GND), default for InvenSense evaluation board
//#define MPU6050_ADDRESS_AD0_HIGH    0x69 // address pin high (VCC)
//#define MPU6050_DEFAULT_ADDRESS     MPU6050_ADDRESS_AD0_LOW

//#define MPU6050_RA_XG_OFFS_TC       0x00 //[7] PWR_MODE, [6:1] XG_OFFS_TC, [0] OTP_BNK_VLD
//#define MPU6050_RA_YG_OFFS_TC       0x01 //[7] PWR_MODE, [6:1] YG_OFFS_TC, [0] OTP_BNK_VLD
//#define MPU6050_RA_ZG_OFFS_TC       0x02 //[7] PWR_MODE, [6:1] ZG_OFFS_TC, [0] OTP_BNK_VLD
//#define MPU6050_RA_X_FINE_GAIN      0x03 //[7:0] X_FINE_GAIN
//#define MPU6050_RA_Y_FINE_GAIN      0x04 //[7:0] Y_FINE_GAIN
//#define MPU6050_RA_Z_FINE_GAIN      0x05 //[7:0] Z_FINE_GAIN
//#define MPU6050_RA_XA_OFFS_H        0x06 //[15:0] XA_OFFS
//#define MPU6050_RA_XA_OFFS_L_TC     0x07
//#define MPU6050_RA_YA_OFFS_H        0x08 //[15:0] YA_OFFS
//#define MPU6050_RA_YA_OFFS_L_TC     0x09
//#define MPU6050_RA_ZA_OFFS_H        0x0A //[15:0] ZA_OFFS
//#define MPU6050_RA_ZA_OFFS_L_TC     0x0B
//#define MPU6050_RA_XG_OFFS_USRH     0x13 //[15:0] XG_OFFS_USR
//#define MPU6050_RA_XG_OFFS_USRL     0x14
//#define MPU6050_RA_YG_OFFS_USRH     0x15 //[15:0] YG_OFFS_USR
//#define MPU6050_RA_YG_OFFS_USRL     0x16
//#define MPU6050_RA_ZG_OFFS_USRH     0x17 //[15:0] ZG_OFFS_USR
//#define MPU6050_RA_ZG_OFFS_USRL     0x18
//#define MPU6050_RA_SMPLRT_DIV       0x19
//#define MPU6050_RA_CONFIG           0x1A
//#define MPU6050_RA_GYRO_CONFIG      0x1B
//#define MPU6050_RA_ACCEL_CONFIG     0x1C
//#define MPU6050_RA_FF_THR           0x1D
//#define MPU6050_RA_FF_DUR           0x1E
//#define MPU6050_RA_MOT_THR          0x1F
//#define MPU6050_RA_MOT_DUR          0x20
//#define MPU6050_RA_ZRMOT_THR        0x21
//#define MPU6050_RA_ZRMOT_DUR        0x22
//#define MPU6050_RA_FIFO_EN          0x23
//#define MPU6050_RA_I2C_MST_CTRL     0x24
//#define MPU6050_RA_I2C_SLV0_ADDR    0x25
//#define MPU6050_RA_I2C_SLV0_REG     0x26
//#define MPU6050_RA_I2C_SLV0_CTRL    0x27
//#define MPU6050_RA_I2C_SLV1_ADDR    0x28
//#define MPU6050_RA_I2C_SLV1_REG     0x29
//#define MPU6050_RA_I2C_SLV1_CTRL    0x2A
//#define MPU6050_RA_I2C_SLV2_ADDR    0x2B
//#define MPU6050_RA_I2C_SLV2_REG     0x2C
//#define MPU6050_RA_I2C_SLV2_CTRL    0x2D
//#define MPU6050_RA_I2C_SLV3_ADDR    0x2E
//#define MPU6050_RA_I2C_SLV3_REG     0x2F
//#define MPU6050_RA_I2C_SLV3_CTRL    0x30
//#define MPU6050_RA_I2C_SLV4_ADDR    0x31
//#define MPU6050_RA_I2C_SLV4_REG     0x32
//#define MPU6050_RA_I2C_SLV4_DO      0x33
//#define MPU6050_RA_I2C_SLV4_CTRL    0x34
//#define MPU6050_RA_I2C_SLV4_DI      0x35
//#define MPU6050_RA_I2C_MST_STATUS   0x36
//#define MPU6050_RA_INT_PIN_CFG      0x37
//#define MPU6050_RA_INT_ENABLE       0x38
//#define MPU6050_RA_DMP_INT_STATUS   0x39
//#define MPU6050_RA_INT_STATUS       0x3A
//#define MPU6050_RA_ACCEL_XOUT_H     0x3B
//#define MPU6050_RA_ACCEL_XOUT_L     0x3C
//#define MPU6050_RA_ACCEL_YOUT_H     0x3D
//#define MPU6050_RA_ACCEL_YOUT_L     0x3E
//#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
//#define MPU6050_RA_ACCEL_ZOUT_L     0x40
//#define MPU6050_RA_TEMP_OUT_H       0x41
//#define MPU6050_RA_TEMP_OUT_L       0x42
//#define MPU6050_RA_GYRO_XOUT_H      0x43
//#define MPU6050_RA_GYRO_XOUT_L      0x44
//#define MPU6050_RA_GYRO_YOUT_H      0x45
//#define MPU6050_RA_GYRO_YOUT_L      0x46
//#define MPU6050_RA_GYRO_ZOUT_H      0x47
//#define MPU6050_RA_GYRO_ZOUT_L      0x48
//#define MPU6050_RA_EXT_SENS_DATA_00 0x49
//#define MPU6050_RA_EXT_SENS_DATA_01 0x4A
//#define MPU6050_RA_EXT_SENS_DATA_02 0x4B
//#define MPU6050_RA_EXT_SENS_DATA_03 0x4C
//#define MPU6050_RA_EXT_SENS_DATA_04 0x4D
//#define MPU6050_RA_EXT_SENS_DATA_05 0x4E
//#define MPU6050_RA_EXT_SENS_DATA_06 0x4F
//#define MPU6050_RA_EXT_SENS_DATA_07 0x50
//#define MPU6050_RA_EXT_SENS_DATA_08 0x51
//#define MPU6050_RA_EXT_SENS_DATA_09 0x52
//#define MPU6050_RA_EXT_SENS_DATA_10 0x53
//#define MPU6050_RA_EXT_SENS_DATA_11 0x54
//#define MPU6050_RA_EXT_SENS_DATA_12 0x55
//#define MPU6050_RA_EXT_SENS_DATA_13 0x56
//#define MPU6050_RA_EXT_SENS_DATA_14 0x57
//#define MPU6050_RA_EXT_SENS_DATA_15 0x58
//#define MPU6050_RA_EXT_SENS_DATA_16 0x59
//#define MPU6050_RA_EXT_SENS_DATA_17 0x5A
//#define MPU6050_RA_EXT_SENS_DATA_18 0x5B
//#define MPU6050_RA_EXT_SENS_DATA_19 0x5C
//#define MPU6050_RA_EXT_SENS_DATA_20 0x5D
//#define MPU6050_RA_EXT_SENS_DATA_21 0x5E
//#define MPU6050_RA_EXT_SENS_DATA_22 0x5F
//#define MPU6050_RA_EXT_SENS_DATA_23 0x60
//#define MPU6050_RA_MOT_DETECT_STATUS    0x61
//#define MPU6050_RA_I2C_SLV0_DO      0x63
//#define MPU6050_RA_I2C_SLV1_DO      0x64
//#define MPU6050_RA_I2C_SLV2_DO      0x65
//#define MPU6050_RA_I2C_SLV3_DO      0x66
//#define MPU6050_RA_I2C_MST_DELAY_CTRL   0x67
//#define MPU6050_RA_SIGNAL_PATH_RESET    0x68
//#define MPU6050_RA_MOT_DETECT_CTRL      0x69
//#define MPU6050_RA_USER_CTRL        0x6A
//#define MPU6050_RA_PWR_MGMT_1       0x6B
//#define MPU6050_RA_PWR_MGMT_2       0x6C
//#define MPU6050_RA_BANK_SEL         0x6D
//#define MPU6050_RA_MEM_START_ADDR   0x6E
//#define MPU6050_RA_MEM_R_W          0x6F
//#define MPU6050_RA_DMP_CFG_1        0x70
//#define MPU6050_RA_DMP_CFG_2        0x71
//#define MPU6050_RA_FIFO_COUNTH      0x72
//#define MPU6050_RA_FIFO_COUNTL      0x73
//#define MPU6050_RA_FIFO_R_W         0x74
//#define MPU6050_RA_WHO_AM_I         0x75

//#define MPU6050_TC_PWR_MODE_BIT     7
//#define MPU6050_TC_OFFSET_BIT       6
//#define MPU6050_TC_OFFSET_LENGTH    6
//#define MPU6050_TC_OTP_BNK_VLD_BIT  0

//#define MPU6050_VDDIO_LEVEL_VLOGIC  0
//#define MPU6050_VDDIO_LEVEL_VDD     1

//#define MPU6050_CFG_EXT_SYNC_SET_BIT    5
//#define MPU6050_CFG_EXT_SYNC_SET_LENGTH 3
//#define MPU6050_CFG_DLPF_CFG_BIT    2
//#define MPU6050_CFG_DLPF_CFG_LENGTH 3

//#define MPU6050_EXT_SYNC_DISABLED       0x0
//#define MPU6050_EXT_SYNC_TEMP_OUT_L     0x1
//#define MPU6050_EXT_SYNC_GYRO_XOUT_L    0x2
//#define MPU6050_EXT_SYNC_GYRO_YOUT_L    0x3
//#define MPU6050_EXT_SYNC_GYRO_ZOUT_L    0x4
//#define MPU6050_EXT_SYNC_ACCEL_XOUT_L   0x5
//#define MPU6050_EXT_SYNC_ACCEL_YOUT_L   0x6
//#define MPU6050_EXT_SYNC_ACCEL_ZOUT_L   0x7

//#define MPU6050_DLPF_BW_256         0x00
//#define MPU6050_DLPF_BW_188         0x01
//#define MPU6050_DLPF_BW_98          0x02
//#define MPU6050_DLPF_BW_42          0x03
//#define MPU6050_DLPF_BW_20          0x04
//#define MPU6050_DLPF_BW_10          0x05
//#define MPU6050_DLPF_BW_5           0x06

//#define MPU6050_GCONFIG_FS_SEL_BIT      4
//#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

//#define MPU6050_GYRO_FS_250         0x00
//#define MPU6050_GYRO_FS_500         0x01
//#define MPU6050_GYRO_FS_1000        0x02
//#define MPU6050_GYRO_FS_2000        0x03

//#define MPU6050_ACONFIG_XA_ST_BIT           7
//#define MPU6050_ACONFIG_YA_ST_BIT           6
//#define MPU6050_ACONFIG_ZA_ST_BIT           5
//#define MPU6050_ACONFIG_AFS_SEL_BIT         4
//#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2
//#define MPU6050_ACONFIG_ACCEL_HPF_BIT       2
//#define MPU6050_ACONFIG_ACCEL_HPF_LENGTH    3

//#define MPU6050_ACCEL_FS_2          0x00
//#define MPU6050_ACCEL_FS_4          0x01
//#define MPU6050_ACCEL_FS_8          0x02
//#define MPU6050_ACCEL_FS_16         0x03

//#define MPU6050_DHPF_RESET          0x00
//#define MPU6050_DHPF_5              0x01
//#define MPU6050_DHPF_2P5            0x02
//#define MPU6050_DHPF_1P25           0x03
//#define MPU6050_DHPF_0P63           0x04
//#define MPU6050_DHPF_HOLD           0x07

//#define MPU6050_TEMP_FIFO_EN_BIT    7
//#define MPU6050_XG_FIFO_EN_BIT      6
//#define MPU6050_YG_FIFO_EN_BIT      5
//#define MPU6050_ZG_FIFO_EN_BIT      4
//#define MPU6050_ACCEL_FIFO_EN_BIT   3
//#define MPU6050_SLV2_FIFO_EN_BIT    2
//#define MPU6050_SLV1_FIFO_EN_BIT    1
//#define MPU6050_SLV0_FIFO_EN_BIT    0

//#define MPU6050_MULT_MST_EN_BIT     7
//#define MPU6050_WAIT_FOR_ES_BIT     6
//#define MPU6050_SLV_3_FIFO_EN_BIT   5
//#define MPU6050_I2C_MST_P_NSR_BIT   4
//#define MPU6050_I2C_MST_CLK_BIT     3
//#define MPU6050_I2C_MST_CLK_LENGTH  4

//#define MPU6050_CLOCK_DIV_348       0x0
//#define MPU6050_CLOCK_DIV_333       0x1
//#define MPU6050_CLOCK_DIV_320       0x2
//#define MPU6050_CLOCK_DIV_308       0x3
//#define MPU6050_CLOCK_DIV_296       0x4
//#define MPU6050_CLOCK_DIV_286       0x5
//#define MPU6050_CLOCK_DIV_276       0x6
//#define MPU6050_CLOCK_DIV_267       0x7
//#define MPU6050_CLOCK_DIV_258       0x8
//#define MPU6050_CLOCK_DIV_500       0x9
//#define MPU6050_CLOCK_DIV_471       0xA
//#define MPU6050_CLOCK_DIV_444       0xB
//#define MPU6050_CLOCK_DIV_421       0xC
//#define MPU6050_CLOCK_DIV_400       0xD
//#define MPU6050_CLOCK_DIV_381       0xE
//#define MPU6050_CLOCK_DIV_364       0xF

//#define MPU6050_I2C_SLV_RW_BIT      7
//#define MPU6050_I2C_SLV_ADDR_BIT    6
//#define MPU6050_I2C_SLV_ADDR_LENGTH 7
//#define MPU6050_I2C_SLV_EN_BIT      7
//#define MPU6050_I2C_SLV_BYTE_SW_BIT 6
//#define MPU6050_I2C_SLV_REG_DIS_BIT 5
//#define MPU6050_I2C_SLV_GRP_BIT     4
//#define MPU6050_I2C_SLV_LEN_BIT     3
//#define MPU6050_I2C_SLV_LEN_LENGTH  4

//#define MPU6050_I2C_SLV4_RW_BIT         7
//#define MPU6050_I2C_SLV4_ADDR_BIT       6
//#define MPU6050_I2C_SLV4_ADDR_LENGTH    7
//#define MPU6050_I2C_SLV4_EN_BIT         7
//#define MPU6050_I2C_SLV4_INT_EN_BIT     6
//#define MPU6050_I2C_SLV4_REG_DIS_BIT    5
//#define MPU6050_I2C_SLV4_MST_DLY_BIT    4
//#define MPU6050_I2C_SLV4_MST_DLY_LENGTH 5

//#define MPU6050_MST_PASS_THROUGH_BIT    7
//#define MPU6050_MST_I2C_SLV4_DONE_BIT   6
//#define MPU6050_MST_I2C_LOST_ARB_BIT    5
//#define MPU6050_MST_I2C_SLV4_NACK_BIT   4
//#define MPU6050_MST_I2C_SLV3_NACK_BIT   3
//#define MPU6050_MST_I2C_SLV2_NACK_BIT   2
//#define MPU6050_MST_I2C_SLV1_NACK_BIT   1
//#define MPU6050_MST_I2C_SLV0_NACK_BIT   0

//#define MPU6050_INTCFG_INT_LEVEL_BIT        7
//#define MPU6050_INTCFG_INT_OPEN_BIT         6
//#define MPU6050_INTCFG_LATCH_INT_EN_BIT     5
//#define MPU6050_INTCFG_INT_RD_CLEAR_BIT     4
//#define MPU6050_INTCFG_FSYNC_INT_LEVEL_BIT  3
//#define MPU6050_INTCFG_FSYNC_INT_EN_BIT     2
//#define MPU6050_INTCFG_I2C_BYPASS_EN_BIT    1
//#define MPU6050_INTCFG_CLKOUT_EN_BIT        0

//#define MPU6050_INTMODE_ACTIVEHIGH  0x00
//#define MPU6050_INTMODE_ACTIVELOW   0x01

//#define MPU6050_INTDRV_PUSHPULL     0x00
//#define MPU6050_INTDRV_OPENDRAIN    0x01

//#define MPU6050_INTLATCH_50USPULSE  0x00
//#define MPU6050_INTLATCH_WAITCLEAR  0x01

//#define MPU6050_INTCLEAR_STATUSREAD 0x00
//#define MPU6050_INTCLEAR_ANYREAD    0x01

//#define MPU6050_INTERRUPT_FF_BIT            7
//#define MPU6050_INTERRUPT_MOT_BIT           6
//#define MPU6050_INTERRUPT_ZMOT_BIT          5
//#define MPU6050_INTERRUPT_FIFO_OFLOW_BIT    4
//#define MPU6050_INTERRUPT_I2C_MST_INT_BIT   3
//#define MPU6050_INTERRUPT_PLL_RDY_INT_BIT   2
//#define MPU6050_INTERRUPT_DMP_INT_BIT       1
//#define MPU6050_INTERRUPT_DATA_RDY_BIT      0

//// TODO: figure out what these actually do
//// UMPL source code is not very obivous
//#define MPU6050_DMPINT_5_BIT            5
//#define MPU6050_DMPINT_4_BIT            4
//#define MPU6050_DMPINT_3_BIT            3
//#define MPU6050_DMPINT_2_BIT            2
//#define MPU6050_DMPINT_1_BIT            1
//#define MPU6050_DMPINT_0_BIT            0

//#define MPU6050_MOTION_MOT_XNEG_BIT     7
//#define MPU6050_MOTION_MOT_XPOS_BIT     6
//#define MPU6050_MOTION_MOT_YNEG_BIT     5
//#define MPU6050_MOTION_MOT_YPOS_BIT     4
//#define MPU6050_MOTION_MOT_ZNEG_BIT     3
//#define MPU6050_MOTION_MOT_ZPOS_BIT     2
//#define MPU6050_MOTION_MOT_ZRMOT_BIT    0

//#define MPU6050_DELAYCTRL_DELAY_ES_SHADOW_BIT   7
//#define MPU6050_DELAYCTRL_I2C_SLV4_DLY_EN_BIT   4
//#define MPU6050_DELAYCTRL_I2C_SLV3_DLY_EN_BIT   3
//#define MPU6050_DELAYCTRL_I2C_SLV2_DLY_EN_BIT   2
//#define MPU6050_DELAYCTRL_I2C_SLV1_DLY_EN_BIT   1
//#define MPU6050_DELAYCTRL_I2C_SLV0_DLY_EN_BIT   0

//#define MPU6050_PATHRESET_GYRO_RESET_BIT    2
//#define MPU6050_PATHRESET_ACCEL_RESET_BIT   1
//#define MPU6050_PATHRESET_TEMP_RESET_BIT    0

//#define MPU6050_DETECT_ACCEL_ON_DELAY_BIT       5
//#define MPU6050_DETECT_ACCEL_ON_DELAY_LENGTH    2
//#define MPU6050_DETECT_FF_COUNT_BIT             3
//#define MPU6050_DETECT_FF_COUNT_LENGTH          2
//#define MPU6050_DETECT_MOT_COUNT_BIT            1
//#define MPU6050_DETECT_MOT_COUNT_LENGTH         2

//#define MPU6050_DETECT_DECREMENT_RESET  0x0
//#define MPU6050_DETECT_DECREMENT_1      0x1
//#define MPU6050_DETECT_DECREMENT_2      0x2
//#define MPU6050_DETECT_DECREMENT_4      0x3

//#define MPU6050_USERCTRL_DMP_EN_BIT             7
//#define MPU6050_USERCTRL_FIFO_EN_BIT            6
//#define MPU6050_USERCTRL_I2C_MST_EN_BIT         5
//#define MPU6050_USERCTRL_I2C_IF_DIS_BIT         4
//#define MPU6050_USERCTRL_DMP_RESET_BIT          3
//#define MPU6050_USERCTRL_FIFO_RESET_BIT         2
//#define MPU6050_USERCTRL_I2C_MST_RESET_BIT      1
//#define MPU6050_USERCTRL_SIG_COND_RESET_BIT     0

//#define MPU6050_PWR1_DEVICE_RESET_BIT   7
//#define MPU6050_PWR1_SLEEP_BIT          6
//#define MPU6050_PWR1_CYCLE_BIT          5
//#define MPU6050_PWR1_TEMP_DIS_BIT       3
//#define MPU6050_PWR1_CLKSEL_BIT         2
//#define MPU6050_PWR1_CLKSEL_LENGTH      3

//#define MPU6050_CLOCK_INTERNAL          0x00
//#define MPU6050_CLOCK_PLL_XGYRO         0x01
//#define MPU6050_CLOCK_PLL_YGYRO         0x02
//#define MPU6050_CLOCK_PLL_ZGYRO         0x03
//#define MPU6050_CLOCK_PLL_EXT32K        0x04
//#define MPU6050_CLOCK_PLL_EXT19M        0x05
//#define MPU6050_CLOCK_KEEP_RESET        0x07

//#define MPU6050_PWR2_LP_WAKE_CTRL_BIT       7
//#define MPU6050_PWR2_LP_WAKE_CTRL_LENGTH    2
//#define MPU6050_PWR2_STBY_XA_BIT            5
//#define MPU6050_PWR2_STBY_YA_BIT            4
//#define MPU6050_PWR2_STBY_ZA_BIT            3
//#define MPU6050_PWR2_STBY_XG_BIT            2
//#define MPU6050_PWR2_STBY_YG_BIT            1
//#define MPU6050_PWR2_STBY_ZG_BIT            0

//#define MPU6050_WAKE_FREQ_1P25      0x0
//#define MPU6050_WAKE_FREQ_2P5       0x1
//#define MPU6050_WAKE_FREQ_5         0x2
//#define MPU6050_WAKE_FREQ_10        0x3

//#define MPU6050_BANKSEL_PRFTCH_EN_BIT       6
//#define MPU6050_BANKSEL_CFG_USER_BANK_BIT   5
//#define MPU6050_BANKSEL_MEM_SEL_BIT         4
//#define MPU6050_BANKSEL_MEM_SEL_LENGTH      5

//#define MPU6050_WHO_AM_I_BIT        6
//#define MPU6050_WHO_AM_I_LENGTH     6


//u8						mpu6050_buffer[14];					//iic读取后存放数据
//T_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET;			//??
//u8						GYRO_OFFSET_OK = 1;
//u8						ACC_OFFSET_OK = 1;
//T_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST;		//???????

//void Delay_ms_mpu(u16 nms)
//{
//	u8 i=0;
//	for( i=0;i<nms;i++)
//	{
//		u16 delay_cnt = 50000;
//		if(delay_cnt)
//			delay_cnt--;
//	}
//} 
///**************************实现函数********************************************
////将iic读取到得数据分拆,放入相应寄存器
//*******************************************************************************/
//void MPU6050_Dataanl(void)
//{
//	MPU6050_ACC_LAST.X=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - ACC_OFFSET.X;
//	MPU6050_ACC_LAST.Y=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - ACC_OFFSET.Y;
//	MPU6050_ACC_LAST.Z=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) - ACC_OFFSET.Z;
//	//跳过温度ADC
//	MPU6050_GYRO_LAST.X=((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]) - GYRO_OFFSET.X;
//	MPU6050_GYRO_LAST.Y=((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) - GYRO_OFFSET.Y;
//	MPU6050_GYRO_LAST.Z=((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) - GYRO_OFFSET.Z;
//	
////	if(!GYRO_OFFSET_OK)
////	{
////		static int32_t	tempgx=0,tempgy=0,tempgz=0;
////		static uint8_t cnt_g=0;
////// 		LED1_ON;
////		if(cnt_g==0)
////		{
////			GYRO_OFFSET.X=0;
////			GYRO_OFFSET.Y=0;		/*************************************备注此处被注释部分是校准陀螺仪用的函数**********************************/
////			GYRO_OFFSET.Z=0;
////			tempgx = 0;
////			tempgy = 0;
////			tempgz = 0;
////			cnt_g = 1;
////			return;
////		}
////		tempgx+= MPU6050_GYRO_LAST.X;
////		tempgy+= MPU6050_GYRO_LAST.Y;
////		tempgz+= MPU6050_GYRO_LAST.Z;
////		if(cnt_g==200)
////		{
////			GYRO_OFFSET.X=tempgx/cnt_g;
////			GYRO_OFFSET.Y=tempgy/cnt_g;
////			GYRO_OFFSET.Z=tempgz/cnt_g;
////			cnt_g = 0;
////			GYRO_OFFSET_OK = 1;
////			Data_Save();	
////			return;
////		}
////		cnt_g++;
////	}
////	if(!ACC_OFFSET_OK)
////	{
////		static int32_t	tempax=0,tempay=0,tempaz=0;
////		static uint8_t cnt_a=0;
////// 		LED1_ON;
////		if(cnt_a==0)
////		{
////			ACC_OFFSET.X = 0;
////			ACC_OFFSET.Y = 0;
////			ACC_OFFSET.Z = 0;
////			tempax = 0;
////			tempay = 0;
////			tempaz = 0;
////			cnt_a = 1;
////			return;
////		}
////		tempax+= MPU6050_ACC_LAST.X;
////		tempay+= MPU6050_ACC_LAST.Y;
////		//tempaz+= MPU6050_ACC_LAST.Z;
////		if(cnt_a==200)
////		{
////			ACC_OFFSET.X=tempax/cnt_a;
////			ACC_OFFSET.Y=tempay/cnt_a;
////			ACC_OFFSET.Z=tempaz/cnt_a;
////			cnt_a = 0;
////			ACC_OFFSET_OK = 1;
////			Data_Save();
////			return;
////		}
////		cnt_a++;		
////	}
//}
///**************************实现函数********************************************
////将iic读取到得数据分拆,放入相应寄存器,更新MPU6050_Last
//*******************************************************************************/
//u8 MPU6050_Read(void)
//{
//	return ANO_Tech_I2C1_Read_Int(devAddr,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050_buffer);
//}
///**************************实现函数********************************************
//*函数原型:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
//*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的1个位
//输入	dev  目标设备地址
//reg	   寄存器地址
//bitNum  要修改目标字节的bitNum位
//data  为0 时，目标位将被清0 否则将被置位
//返回   成功 为1 
//失败为0
//*******************************************************************************/ 
//void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
//	u8 b;
//	Delay_ms_mpu(5);
//	ANO_Tech_I2C1_Read_Buf(dev, reg, 1, &b);
//	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
//	Delay_ms_mpu(5);
//	ANO_Tech_I2C1_Write_1Byte_Buf(dev, reg, b);
//}
///**************************实现函数********************************************
//*函数原型:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
//*功　　能:	    读 修改 写 指定设备 指定寄存器一个字节 中的多个位
//输入	dev  目标设备地址
//reg	   寄存器地址
//bitStart  目标字节的起始位
//length   位长度
//data    存放改变目标字节位的值
//返回   成功 为1 
//失败为0
//*******************************************************************************/ 
//void IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
//{
//	
//	u8 b,mask;
//	Delay_ms_mpu(10);
//	ANO_Tech_I2C1_Read_Buf(dev, reg, 1, &b);
//	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
//	data <<= (8 - length);
//	data >>= (7 - bitStart);
//	b &= mask;
//	b |= data;
//	Delay_ms_mpu(10);
//	ANO_Tech_I2C1_Write_1Byte_Buf(dev, reg, b);
//}
///**************************实现函数********************************************
//*函数原型:		void MPU6050_setClockSource(uint8_t source)
//*功　　能:	    设置  MPU6050 的时钟源
//* CLK_SEL | Clock Source
//* --------+--------------------------------------
//* 0       | Internal oscillator
//* 1       | PLL with X Gyro reference
//* 2       | PLL with Y Gyro reference
//* 3       | PLL with Z Gyro reference
//* 4       | PLL with external 32.768kHz reference
//* 5       | PLL with external 19.2MHz reference
//* 6       | Reserved
//* 7       | Stops the clock and keeps the timing generator in reset
//*******************************************************************************/
//void MPU6050_setClockSource(uint8_t source){
//	IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
//	
//}
///** Set full-scale gyroscope range.
//* @param range New full-scale gyroscope range value
//* @see getFullScaleRange()
//* @see MPU6050_GYRO_FS_250
//* @see MPU6050_RA_GYRO_CONFIG
//* @see MPU6050_GCONFIG_FS_SEL_BIT
//* @see MPU6050_GCONFIG_FS_SEL_LENGTH
//*/
//void MPU6050_setFullScaleGyroRange(uint8_t range) {
//	IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
//}

///**************************实现函数********************************************
//*函数原型:		void MPU6050_setFullScaleAccelRange(uint8_t range)
//*功　　能:	    设置  MPU6050 加速度计的最大量程
//*******************************************************************************/
//void MPU6050_setFullScaleAccelRange(uint8_t range) {
//	IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
//}
///**************************实现函数********************************************
//*函数原型:		void MPU6050_setSleepEnabled(uint8_t enabled)
//*功　　能:	    设置  MPU6050 是否进入睡眠模式
//enabled =1   睡觉
//enabled =0   工作
//*******************************************************************************/
//void MPU6050_setSleepEnabled(uint8_t enabled) {
//	IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
//}

///**************************实现函数********************************************
//*函数原型:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
//*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
//*******************************************************************************/
//void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
//	IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
//}

///**************************实现函数********************************************
//*函数原型:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
//*功　　能:	    设置 MPU6050 是否为AUX I2C线的主机
//*******************************************************************************/
//void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
//}

//void MPU6050_setDLPF(uint8_t mode)
//{
//	IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
//}
///**************************实现函数********************************************
//*函数原型:		void MPU6050_initialize(void)
//*功　　能:	    初始化 	MPU6050 以进入可用状态。
//*******************************************************************************/
//void MPU6050_Init(void)
//{

//// 		ANO_Tech_I2C1_Write_1Byte(0xd0,0x1b,0x00);//gyro 250du/s
//// 		ANO_Tech_I2C1_Write_1Byte(0xd0,0x1c,0x08);//acc +-4g
//// 		ANO_Tech_I2C1_Write_1Byte(0xd0,0x24,13);//400khz
//// 		ANO_Tech_I2C1_Write_1Byte(0xd0,0x6a,0x00);//mst_en=0
//// 		ANO_Tech_I2C1_Write_1Byte(0xd0,0x6b,0x00);//sleep=0
//	MPU6050_setSleepEnabled(0); //进入工作状态
//	Delay_ms_mpu(200);
//	MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //设置时钟  0x6b   0x01
//	Delay_ms_mpu(200);
//	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_2000);//陀螺仪最大量程 +-2000度每秒
//	Delay_ms_mpu(50);
//	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_4);	//加速度度最大量程 +-4G
//	Delay_ms_mpu(50);
//	MPU6050_setDLPF(MPU6050_DLPF_BW_42);
//	Delay_ms_mpu(50);
//	MPU6050_setI2CMasterModeEnabled(0);	 //不让MPU6050 控制AUXI2C
//	Delay_ms_mpu(50);
//	MPU6050_setI2CBypassEnabled(1);	 //主控制器的I2C与	MPU6050的AUXI2C	直通。控制器可以直接访问HMC5883L
//	Delay_ms_mpu(50);
//}
