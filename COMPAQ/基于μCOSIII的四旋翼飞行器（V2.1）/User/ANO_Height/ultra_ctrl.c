#include "include.h"
#include "ultra_ctrl.h" 
#include "math.h" 
#include "stdlib.h"
// #include "lcd.h"
#include "usart.h"

float H_Test;

typedef struct
{
	float err;
	float err_old;
	float err_d;
	float err_i;
	float pid_out;

}_st_height_pid_v;

typedef struct
{
	float kp;
	float kd;
	float ki;

}_st_height_pid;


#define HIGH_CTRL 1      //定义是否可以使用定高控制


_st_height_pid_v ultra_ctrl;
_st_height_pid ultra_pid;

_st_height_pid_v wz_speed_pid_v;//声明与pid调节有关数据的结构体
_st_height_pid wz_speed_pid;//声明pid参数结构体


void Ultra_Init(u16 br_num)
{
	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //开启USART2时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	//串口中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	
	//配置PC12作为UART5　Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//配置PD2作为UART5　Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	//配置UART5
	//中断被屏蔽了
	USART_InitStructure.USART_BaudRate = br_num;       //波特率可以通过地面站配置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;    //禁用奇偶校验
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
	USART_Init(UART5, &USART_InitStructure);
	


	//使能UART5接收中断
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	//使能USART5
	USART_Cmd(UART5, ENABLE); 
	

	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
//	f1初始化

// 	USART_InitTypeDef USART_InitStructure;
// 	//USART_ClockInitTypeDef USART_ClockInitStruct;
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //开启USART2时钟
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
// 	USART_DeInit(UART5); 
// 	//串口中断优先级
//  	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);	

// 	
// // 	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
// //   GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
// 	
// 	//配置PC12作为UART5　Tx
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// //   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// //   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//   GPIO_Init(GPIOC, &GPIO_InitStructure); 
// 	//配置PD2作为UART5　Rx
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// //   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
// //   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//   GPIO_Init(GPIOD, &GPIO_InitStructure); 
// 	
// 	//配置UART5
// 	//中断被屏蔽了
// 	USART_InitStructure.USART_BaudRate = br_num;       //波特率可以通过地面站配置
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8位数据
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //在帧结尾传输1个停止位
// 	USART_InitStructure.USART_Parity = USART_Parity_No;    //禁用奇偶校验
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制失能
// 	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //发送、接收使能
// 	USART_Init(UART5, &USART_InitStructure);
// 	
//  USART_Init(UART5, &USART_InitStructure); //初始化串口

// 	//使能UART5接收中断
// 	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
// 	//使能USART5
// 	USART_Cmd(UART5, ENABLE); 


////////////////////////////////////////////////////////////////////////////////////////
	

	Ultra_PID_Init();     //值的初始化
	
	
}


void Ultra_PID_Init()
{
	ultra_pid.kp = 1.5;
	ultra_pid.kd = 2.5;
	ultra_pid.ki = 0;
	
	
	
			wz_speed_pid.kp = 1;
		wz_speed_pid.kd = 0;
	wz_speed_pid.ki=1;
}


u8 Tx5Buffer[256];
u8 Tx5Counter=0;
u8 count5=0; 


void UART5_IRQHandler(void)
{
	u8 com_data;
#if SYSTEM_SUPPORT_OS 		
	OSIntEnter();    
#endif	
	

  //接收中断
	if( USART_GetITStatus(UART5,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//清除中断标志

		com_data = UART5->DR;
		
		Ultra_Get(com_data);
	}

	//发送（进入移位）中断
	if( USART_GetITStatus(UART5,USART_IT_TXE ) )
	{
				
		UART5->DR = Tx5Buffer[Tx5Counter++]; //写DR清除中断标志
          
		if(Tx5Counter == count5)
		{
			UART5->CR1 &= ~USART_CR1_TXEIE;		//关闭TXE（发送中断）中断
		}


		//USART_ClearITPendingBit(USART2,USART_IT_TXE);
	}
#if SYSTEM_SUPPORT_OS 
	OSIntExit();  											 
#endif		
}

//void Uart5_Send(unsigned char *DataToSend ,u8 data_num)
//{
//	u8 i;
//	for(i=0;i<data_num;i++)
//	{
//		Tx5Buffer[count5++] = *(DataToSend+i);
//	}

//	if(!(UART5->CR1 & USART_CR1_TXEIE))
//	{
//		USART_ITConfig(UART5, USART_IT_TXE, ENABLE); //打开发送中断
//	}

//}
 void Uart5_Send(unsigned char *DataToSend ,u8 data_num)
{
	u8 i;
	for(i=0;i<data_num;i++)
	{

		USART_SendData(UART5,DataToSend[i]);
		while(USART_GetFlagStatus(UART5,USART_FLAG_TXE) == RESET);
	}


}



s8 ultra_start_f;
u8 ultra_time;
u8 ultra_ok = 0;

void Ultra_Duty()   //超声波发送数据请求
{
	u8 temp[3];

	ultra_time++;
	ultra_time = ultra_time%2;
	
	
	if( ultra_time == 0 )  //100ms//改用发送中断，节省时间。
	{

// 	#if defined(USE_KS103)
		temp[0] = 0xd8;
		temp[1] = 0x02;
		temp[2] = 0xbD;
		Uart5_Send(temp ,3);
// 	#elif defined(USE_US100)
// 		temp[0] = 0x55;
// 		Uart5_Send(temp ,1);
// 	#endif
///////////////////////////////////////////////
		ultra_start_f = 1;   //请求超声波数据完成，
	}
}


u16 ultra_distance,ultra_distance_old;
s16 ultra_delta;
u8 temp[16];
float swap;


void Ultra_Get(u8 com_data)
{
	static u8 ultra_tmp;
	
	if( ultra_start_f == 1 )    //第一次接收到
	{
		ultra_tmp = com_data;
		ultra_start_f = 2;
	}
	else if( ultra_start_f == 2 )   //第 2次接收到，2个8位为一个完整的数据
	{
		ultra_distance = (ultra_tmp<<8) + com_data;
		ultra_start_f = 0;                   //数据接收完成
		ultra_ok = 1;
	}
	
	ultra_delta = ultra_distance - ultra_distance_old;

	
	

	
	ultra_distance_old = ultra_distance;

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float my_deathzoom(float x,float zoom)         //       if( -zoom < x <zoom )  x=0,  else  x=x-zoom 
{
	float t;
	if(x>0)
	{
		t = x - zoom;
		if(t<0)
		{
			t = 0;
		}
	}
	else
	{
		t = x + zoom;
		if(t>0)
		{
			t = 0;
		}
	}
  return (t);
}


float my_deathzoom_2(float x,float zoom)   //    if( -zoom < x <zoom )  x=0,  else  x=x
{
	float t;
	
	if( x> -zoom && x < zoom )
	{
		t = 0;
	}
	else
	{
		t = x;
	}

  return (t);
}



#define MED_WIDTH_NUM 11
#define MED_FIL_ITEM  2

float med_filter_tmp[MED_FIL_ITEM][MED_WIDTH_NUM ];//存储多种需要滤波的数据  0:snoic[0~10]
																									//												 1:speed[0~10]
float med_filter_out[MED_FIL_ITEM];

u8 med_fil_cnt[MED_FIL_ITEM];//记录每种数据存储的数量

float Moving_Median(u8 item,u8 width_num,float in)//中值滤波
{
	u8 i,j;
	float t;
	float tmp[MED_WIDTH_NUM];
	
	if(item >= MED_FIL_ITEM || width_num >= MED_WIDTH_NUM )
	{
		return 0;
	}
	else
	{
		if( ++med_fil_cnt[item] >= width_num )	
		{
			med_fil_cnt[item] = 0;
		}
		
		med_filter_tmp[item][ med_fil_cnt[item] ] = in;
		
		for(i=0;i<width_num;i++)
		{
			tmp[i] = med_filter_tmp[item][i];
		}
		
		for(i=0;i<width_num-1;i++)//从小到大排序
		{
			for(j=0;j<(width_num-1-i);j++)
			{
				if(tmp[j] > tmp[j+1])
				{
					t = tmp[j];
					tmp[j] = tmp[j+1];
					tmp[j+1] = t;
				}
			}
		}

		
		return ( tmp[(u16)width_num/2] );
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




#define ULTRA_SPEED 300         //声速？
#define ULTRA_MAX_HEIGHT 1500   // mm

#define ULTRA_INT        300    // 积分幅度

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )




/////////////////////////////////////////////////////////////////////////////
#define MAX_PWM				100			///%	最大PWM输出为100%油门
#define MAX_THR       80 			///%	油门通道最大占比80%，留20%给控制量
#define READY_SPEED   20			///%	解锁后电机转速20%油门

#define ABS(x) ( (x)>0?(x):-(x) )
u8 temp[16];
float thr_value;
u16 Set_Height;
u8 Thr_Low;
float Thr_Weight;
float height_ctrl_out;//高度控制输出
float temp1;


void Thr_Ctrl(float T)  //  T :内环(陀螺仪)准确的执行周期
{
	static float thr=500;
	static float Thr_tmp=0;
//           遥控的油门值？？？？？
//	thr = 500 + CH_filter[THR]; //油门值 0 ~ 1000
	
	Thr_tmp += 10 *3.14f *T *(thr/400.0f - Thr_tmp); //低通滤波
	Thr_tmp += T *(thr/400.0f - Thr_tmp); //低通滤波
	temp1=Thr_tmp ;
// 		sprintf((char *)temp,"temp1:%04f",temp1);
// 	LCD_ShowString(10,70,200,16,16,temp );
	
	Thr_Weight = LIMIT(Thr_tmp,0,1);    							//后边多处分离数据会用到这个值，0到1之间的值
	
// 		sprintf((char *)temp,"Thr_Weight:%04f",Thr_Weight);
// 	LCD_ShowString(10,90,200,16,16,temp );
	if( thr < 100 )
	{
		Thr_Low = 1;
	}
	else
	{
		Thr_Low = 0;
	}
	
	if(HIGH_CTRL)
	{
		High_Ctrl(thr);//CTRL_HEIGHT设置这个，开关定高功能
	
		thr_value = Thr_Weight *height_ctrl_out;   //实际使用值


	}
	else 
	thr_value = thr;   //实际使用值
	
	
	thr_value = LIMIT(thr_value,0,10 *MAX_THR *MAX_PWM/100);
}







float exp_height_speed,exp_height=285;
float ultra_speed;
float ultra_dis_lpf;
float ultra_ctrl_out;
float wz_speed,wz_speed_old;//当前位置速度，上一次位置速度

float wz_acc;//各方向加速度之和   //单独计算z轴加速度


void High_Ctrl(u16 thr)    
{
	static u8 hs_ctrl_cnt=0;

			hs_ctrl_cnt++;
			hs_ctrl_cnt = hs_ctrl_cnt%2;
			
// 			 wz_acc += ( 1 / ( 1 + 1 / ( 20 *3.14f *T ) ) ) *( (reference_v.z *mpu6050.Acc.z + reference_v.x *mpu6050.Acc.x + reference_v.y *mpu6050.Acc.y - 4096 ) - wz_acc );//x,y,z方向上的加速度和（应该是向量和）（减4096不知道有什么意义）低通滤波（前面的乘数）
	
// 			 wz_acc += ( 1 / ( 1 + 1 / ( 20 *3.14f *0.01f ) ) ) *( (mpu6050.Acc.z  ) - wz_acc );//x,y,z方向上的加速度和（应该是向量和）（减4096不知道有什么意义）低通滤波（前面的乘数）

			if(hs_ctrl_cnt == 0)//hs_ctrl_cnt每计数10次，执行一次
			{
				height_speed_ctrl(0.02f,thr,0.4f*ultra_ctrl_out,ultra_speed);  //每10次进行一次速度控制？？？？？？
			}
			
			if( ultra_start_f == 0 )     //数据接收完成，进行超声波控制
			{	
				
 				Ultra_Ctrl(0.1f,thr);//超声波周期100ms
				ultra_start_f = -1;         
			}
	
	

			height_ctrl_out = wz_speed_pid_v.pid_out;


}


void Ultra_Ctrl(float T,float thr)
{
	float ultra_sp_tmp,ultra_dis_tmp;	
	//不明白油门*声速是什么玩意
	//exp_height_speed，输出的高度速度，我理解为当前飞机晃动速度
	exp_height_speed = ULTRA_SPEED *my_deathzoom_2(thr - 500,50)/200.0f; //如果（thr-500）大于-50或小于50，则my_deathzoom_2(thr - 500,50)=0，不然=thr - 500

	exp_height_speed = LIMIT(exp_height_speed ,-ULTRA_SPEED,ULTRA_SPEED);//进行上下幅度限制

	//  -300 -- 300之间
	if( exp_height > ULTRA_MAX_HEIGHT )//期望高度>超声波最大量程，关闭
	{
		if( exp_height_speed > 0 )
		{
			exp_height_speed = 0;
		}
	}
	else if( exp_height < 20 )//预测高度小于2cm，关闭
	{
		if( exp_height_speed < 0 )
		{
			exp_height_speed = 0;
		}
	}

//	exp_height += exp_height_speed *T;//预测高度误差？（不确定）

	exp_height=Set_Height;	//给定高度1m hgyy
	
	if( thr < 100 )//理论thr不会小于100，因为thr在被传递给本函数前已经加过500
	{
		exp_height += ( 1 / ( 1 + 1 / ( 0.2f *3.14f *T ) ) ) *( -exp_height);//低通滤波
	}
	//Moving_Median：中值滤波

	ultra_sp_tmp = Moving_Median(0,5,ultra_delta/T); //ultra_delta：两次超声波测量的差值;除T代表短时间内Z方向速度
									//取3次连续数据中最小的值？？？？
	ultra_dis_tmp = Moving_Median(1,5,ultra_distance);//当前高度
	
	if( ultra_dis_tmp < 2000 )//当前高度<2000
	{
		if( ABS(ultra_sp_tmp) < 100 )//Z方向速度<100，//速度更小时，虑出来的数据更大，加快控制？？？？
		{
			ultra_speed += ( 1 / ( 1 + 1 / ( 4 *3.14f *T ) ) ) * ( (float)(ultra_sp_tmp) - ultra_speed );//Z方向速度滤波
		}
		else
		{
			ultra_speed += ( 1 / ( 1 + 1 / ( 1.0f *3.14f *T ) ) ) * ( (float)(ultra_sp_tmp) - ultra_speed );//和上面一的滤波，只不过修改了系数
		}
	}


	
	if( ultra_dis_tmp < 2000 )//当前高度<2000
	{
		//ultra_dis_lpf，超声波高度滤波值（又一次滤波）
		//针对当前中值滤波高度值ultra_dis_tmp与上一次经过中值和低通滤波值作比较，根据不同差值选择不同的滤波
		//滤波好的值作为下面运算
		if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 100 )//ultra_dis_lpf初始为0，由本行下面的程序修改其值
		{        //ultra_dis_tmp：备份值                //ultra_dis_lpf：超声波距离的滤波值
			
			// 4.0f *3.14f *T 越大，得到的值也就越大，加快应对各种状况的反应速度？？？？？
			
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 4.0f *3.14f *T ) ) ) *(ultra_dis_tmp - ultra_dis_lpf) ;//滤波
		}
		else if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 200 )
		{
			
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 2.2f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;//滤波
		}
		else if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 400 )
		{
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 1.2f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;//滤波
		}
		else
		{
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 0.6f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;
		}
	}
	else
	{
		
	}


	
	ultra_ctrl.err = ( ultra_pid.kp*(exp_height - ultra_dis_lpf) );//预测高度值与实际滤波后的高度值差
	
	ultra_ctrl.err_i += ultra_pid.ki *ultra_ctrl.err *T;//无效，ki=0
	
	ultra_ctrl.err_i = LIMIT(ultra_ctrl.err_i,-Thr_Weight *ULTRA_INT,Thr_Weight *ULTRA_INT);//积分限制
	
			ultra_ctrl.err_i = LIMIT(ultra_ctrl.err_i,-Thr_Weight *ULTRA_INT , Thr_Weight *ULTRA_INT);//积分限制
	
	ultra_ctrl.err_d = ultra_pid.kd *( 0.6f *(-wz_speed*T) + 0.4f *(ultra_ctrl.err - ultra_ctrl.err_old) );//速度一阶滤波
	
	ultra_ctrl.pid_out = ultra_ctrl.err + ultra_ctrl.err_i + ultra_ctrl.err_d;//超声波高度控制输出量
	
	H_Test=ultra_ctrl.pid_out;
	ultra_ctrl.pid_out = LIMIT(ultra_ctrl.pid_out,-500,500);//控制量限幅
		
	ultra_ctrl_out = ultra_ctrl.pid_out;//变量传递
	
	ultra_ctrl.err_old = ultra_ctrl.err;//保存上次误差

}





float wz_acc_mms2;//当前位置加速度
float height_thr,weight_ultra=0.8,weight_acc=0.2;
float real_acc_z=0,real_speed=0,speed_ronghe=0;
float swap_acc;
float height_thr;
void height_speed_ctrl(float T,float thr,float exp_z_speed,float h_speed)
{                                              //预测高度与实际高度的pid值（速度）乘以0.4=期望的z轴速度？
// 															//thr范围 : 0 - 1000

	static float thr_lpf;
	
	static float hc_acc_i,wz_speed_0,wz_speed_1;
	
	wz_acc =2070;   //稳定态的z轴加速度
//	thr=thr-1000;		//hgyy修改

	//  wz_acc_mms2 :陀螺仪z轴实时加速度

	height_thr = LIMIT( 2 * thr , 0, 600 );//限制高度油门
// 	
	thr_lpf += ( 1 / ( 1 + 1 / ( 0.5f *3.14f *T ) ) ) *( height_thr - thr_lpf );//油门低通滤波（参考文档7：其他/5单片机的数字滤波）
	
	thr_lpf=500;	//hgyy修改

// 		
// 		swap_acc +=( 1 / ( 1 + 1 / ( 0.2 * 3.14f * 1 ) ) ) *(  fabs(a[2])-swap_acc  ) ;//Z方向速度滤波   0.76
	
	
		swap_acc=MPU6050_ACC_DATA.Z;	//hgyy
		
		real_acc_z = my_deathzoom(swap_acc - wz_acc ,50);   //去除实际加速度的抖动


		real_speed += real_acc_z * 0.5;              //计算实际的速度；

		real_speed +=( 1 / ( 1 + 1 / ( 0.5 *3.14f *T ) ) ) *( h_speed - real_speed  ) ;

// 		speed_ronghe = weight_ultra * h_speed +weight_acc *real_speed ;

		

		wz_speed += ( 1 / ( 1 + 1 / ( 1 * 3.14f * 1 ) ) ) *( real_speed - wz_speed ) ;//Z方向速度滤波
	
			
		wz_speed=LIMIT(wz_speed,-300,300);
	
	
	
	wz_speed_pid_v.err = wz_speed_pid.kp *( exp_z_speed - wz_speed );//期望速度减去当前位置速度，误差放大
	//0.002f/T *10=1，所以没有意义
	wz_speed_pid_v.err_d = 0.002f/T *10*wz_speed_pid.kd * (-real_acc_z) *T;//速度上进行D调节
	
	//wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid_v.err *T;
	//exp_z_speed在超声波下，为超声波控制输出量；在气压计下为简单处理后的油门，说白了就是期望z轴速度
	//h_speed，z轴实际速度，( exp_z_speed - h_speed ) *T=实际位置上的误差	
	wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid.kp *( exp_z_speed - h_speed ) *T;
	//积分限幅
	wz_speed_pid_v.err_i = LIMIT(wz_speed_pid_v.err_i,-Thr_Weight *300,Thr_Weight *300);
	//LIMIT，pid输出量限幅；Thr_Weight值的范围为0~1，为了分离数据？（我觉的是为了油门控制稳定）
	//thr低通滤波后为thr_lpf
	wz_speed_pid_v.pid_out = thr_lpf + Thr_Weight *LIMIT((wz_speed_pid.kp *exp_z_speed + wz_speed_pid_v.err + wz_speed_pid_v.err_d + wz_speed_pid_v.err_i),-300,300);

	wz_speed_pid_v.err_old = wz_speed_pid_v.err; 








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






// 	
// 	wz_speed_pid_v.err = wz_speed_pid.kp *( exp_z_speed - wz_speed );//期望速度减去当前位置速度，误差放大
// 	//0.002f/T *10=1，所以没有意义
// 	wz_speed_pid_v.err_d = 0.002f/T *10*wz_speed_pid.kd * (-wz_acc_mms2) *T;//速度上进行D调节
// 	
// 	//wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid_v.err *T;
// 	//exp_z_speed在超声波下，为超声波控制输出量；在气压计下为简单处理后的油门，说白了就是期望z轴速度
// 	//h_speed，z轴实际速度，( exp_z_speed - h_speed ) *T=实际位置上的误差	
// 	wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid.kp *( exp_z_speed - h_speed ) *T;
// 	//积分限幅
// 	wz_speed_pid_v.err_i = LIMIT(wz_speed_pid_v.err_i,-Thr_Weight *300,Thr_Weight *300);
// 	//LIMIT，pid输出量限幅；Thr_Weight值的范围为0~1，为了分离数据？（我觉的是为了油门控制稳定）
// 	//thr低通滤波后为thr_lpf
// 	wz_speed_pid_v.pid_out = thr_lpf + Thr_Weight *LIMIT((wz_speed_pid.kp *exp_z_speed + wz_speed_pid_v.err + wz_speed_pid_v.err_d + wz_speed_pid_v.err_i),-300,300);

// 	wz_speed_pid_v.err_old = wz_speed_pid_v.err; 
}


/*
	PID 参数：
		内环：
				wz_speed_pid.kp
				wz_speed_pid.ki
				wz_speed_pid.kd
		外环：
			
				ultra_pid.kp
				ultra_pid.ki
				ultra_pid.kd
*/



