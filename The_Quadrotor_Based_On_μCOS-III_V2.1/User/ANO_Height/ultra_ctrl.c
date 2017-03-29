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


#define HIGH_CTRL 1      //�����Ƿ����ʹ�ö��߿���


_st_height_pid_v ultra_ctrl;
_st_height_pid ultra_pid;

_st_height_pid_v wz_speed_pid_v;//������pid�����й����ݵĽṹ��
_st_height_pid wz_speed_pid;//����pid�����ṹ��


void Ultra_Init(u16 br_num)
{
	
	USART_InitTypeDef USART_InitStructure;
	//USART_ClockInitTypeDef USART_ClockInitStruct;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //����USART2ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	
	//�����ж����ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);	

	
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
	
	//����PC12��ΪUART5��Tx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	//����PD2��ΪUART5��Rx
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	//����UART5
	//�жϱ�������
	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
	USART_Init(UART5, &USART_InitStructure);
	


	//ʹ��UART5�����ж�
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
	//ʹ��USART5
	USART_Cmd(UART5, ENABLE); 
	

	
	
	///////////////////////////////////////////////////////////////////////////////////////////////
//	f1��ʼ��

// 	USART_InitTypeDef USART_InitStructure;
// 	//USART_ClockInitTypeDef USART_ClockInitStruct;
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	
// 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE); //����USART2ʱ��
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_AFIO,ENABLE);	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
// 	USART_DeInit(UART5); 
// 	//�����ж����ȼ�
//  	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);	

// 	
// // 	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5);
// //   GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);
// 	
// 	//����PC12��ΪUART5��Tx
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; 
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// //   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
// //   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
//   GPIO_Init(GPIOC, &GPIO_InitStructure); 
// 	//����PD2��ΪUART5��Rx
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ; 
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
// //   GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
// //   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
//   GPIO_Init(GPIOD, &GPIO_InitStructure); 
// 	
// 	//����UART5
// 	//�жϱ�������
// 	USART_InitStructure.USART_BaudRate = br_num;       //�����ʿ���ͨ������վ����
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b;  //8λ����
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1;   //��֡��β����1��ֹͣλ
// 	USART_InitStructure.USART_Parity = USART_Parity_No;    //������żУ��
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //Ӳ��������ʧ��
// 	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  //���͡�����ʹ��
// 	USART_Init(UART5, &USART_InitStructure);
// 	
//  USART_Init(UART5, &USART_InitStructure); //��ʼ������

// 	//ʹ��UART5�����ж�
// 	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);
// 	//ʹ��USART5
// 	USART_Cmd(UART5, ENABLE); 


////////////////////////////////////////////////////////////////////////////////////////
	

	Ultra_PID_Init();     //ֵ�ĳ�ʼ��
	
	
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
	

  //�����ж�
	if( USART_GetITStatus(UART5,USART_IT_RXNE) )
	{
		USART_ClearITPendingBit(UART5,USART_IT_RXNE);//����жϱ�־

		com_data = UART5->DR;
		
		Ultra_Get(com_data);
	}

	//���ͣ�������λ���ж�
	if( USART_GetITStatus(UART5,USART_IT_TXE ) )
	{
				
		UART5->DR = Tx5Buffer[Tx5Counter++]; //дDR����жϱ�־
          
		if(Tx5Counter == count5)
		{
			UART5->CR1 &= ~USART_CR1_TXEIE;		//�ر�TXE�������жϣ��ж�
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
//		USART_ITConfig(UART5, USART_IT_TXE, ENABLE); //�򿪷����ж�
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

void Ultra_Duty()   //������������������
{
	u8 temp[3];

	ultra_time++;
	ultra_time = ultra_time%2;
	
	
	if( ultra_time == 0 )  //100ms//���÷����жϣ���ʡʱ�䡣
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
		ultra_start_f = 1;   //��������������ɣ�
	}
}


u16 ultra_distance,ultra_distance_old;
s16 ultra_delta;
u8 temp[16];
float swap;


void Ultra_Get(u8 com_data)
{
	static u8 ultra_tmp;
	
	if( ultra_start_f == 1 )    //��һ�ν��յ�
	{
		ultra_tmp = com_data;
		ultra_start_f = 2;
	}
	else if( ultra_start_f == 2 )   //�� 2�ν��յ���2��8λΪһ������������
	{
		ultra_distance = (ultra_tmp<<8) + com_data;
		ultra_start_f = 0;                   //���ݽ������
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

float med_filter_tmp[MED_FIL_ITEM][MED_WIDTH_NUM ];//�洢������Ҫ�˲�������  0:snoic[0~10]
																									//												 1:speed[0~10]
float med_filter_out[MED_FIL_ITEM];

u8 med_fil_cnt[MED_FIL_ITEM];//��¼ÿ�����ݴ洢������

float Moving_Median(u8 item,u8 width_num,float in)//��ֵ�˲�
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
		
		for(i=0;i<width_num-1;i++)//��С��������
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




#define ULTRA_SPEED 300         //���٣�
#define ULTRA_MAX_HEIGHT 1500   // mm

#define ULTRA_INT        300    // ���ַ���

#define LIMIT( x,min,max ) ( (x) < (min)  ? (min) : ( (x) > (max) ? (max) : (x) ) )




/////////////////////////////////////////////////////////////////////////////
#define MAX_PWM				100			///%	���PWM���Ϊ100%����
#define MAX_THR       80 			///%	����ͨ�����ռ��80%����20%��������
#define READY_SPEED   20			///%	��������ת��20%����

#define ABS(x) ( (x)>0?(x):-(x) )
u8 temp[16];
float thr_value;
u16 Set_Height;
u8 Thr_Low;
float Thr_Weight;
float height_ctrl_out;//�߶ȿ������
float temp1;


void Thr_Ctrl(float T)  //  T :�ڻ�(������)׼ȷ��ִ������
{
	static float thr=500;
	static float Thr_tmp=0;
//           ң�ص�����ֵ����������
//	thr = 500 + CH_filter[THR]; //����ֵ 0 ~ 1000
	
	Thr_tmp += 10 *3.14f *T *(thr/400.0f - Thr_tmp); //��ͨ�˲�
	Thr_tmp += T *(thr/400.0f - Thr_tmp); //��ͨ�˲�
	temp1=Thr_tmp ;
// 		sprintf((char *)temp,"temp1:%04f",temp1);
// 	LCD_ShowString(10,70,200,16,16,temp );
	
	Thr_Weight = LIMIT(Thr_tmp,0,1);    							//��߶ദ�������ݻ��õ����ֵ��0��1֮���ֵ
	
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
		High_Ctrl(thr);//CTRL_HEIGHT������������ض��߹���
	
		thr_value = Thr_Weight *height_ctrl_out;   //ʵ��ʹ��ֵ


	}
	else 
	thr_value = thr;   //ʵ��ʹ��ֵ
	
	
	thr_value = LIMIT(thr_value,0,10 *MAX_THR *MAX_PWM/100);
}







float exp_height_speed,exp_height=285;
float ultra_speed;
float ultra_dis_lpf;
float ultra_ctrl_out;
float wz_speed,wz_speed_old;//��ǰλ���ٶȣ���һ��λ���ٶ�

float wz_acc;//��������ٶ�֮��   //��������z����ٶ�


void High_Ctrl(u16 thr)    
{
	static u8 hs_ctrl_cnt=0;

			hs_ctrl_cnt++;
			hs_ctrl_cnt = hs_ctrl_cnt%2;
			
// 			 wz_acc += ( 1 / ( 1 + 1 / ( 20 *3.14f *T ) ) ) *( (reference_v.z *mpu6050.Acc.z + reference_v.x *mpu6050.Acc.x + reference_v.y *mpu6050.Acc.y - 4096 ) - wz_acc );//x,y,z�����ϵļ��ٶȺͣ�Ӧ���������ͣ�����4096��֪����ʲô���壩��ͨ�˲���ǰ��ĳ�����
	
// 			 wz_acc += ( 1 / ( 1 + 1 / ( 20 *3.14f *0.01f ) ) ) *( (mpu6050.Acc.z  ) - wz_acc );//x,y,z�����ϵļ��ٶȺͣ�Ӧ���������ͣ�����4096��֪����ʲô���壩��ͨ�˲���ǰ��ĳ�����

			if(hs_ctrl_cnt == 0)//hs_ctrl_cntÿ����10�Σ�ִ��һ��
			{
				height_speed_ctrl(0.02f,thr,0.4f*ultra_ctrl_out,ultra_speed);  //ÿ10�ν���һ���ٶȿ��ƣ�����������
			}
			
			if( ultra_start_f == 0 )     //���ݽ�����ɣ����г���������
			{	
				
 				Ultra_Ctrl(0.1f,thr);//����������100ms
				ultra_start_f = -1;         
			}
	
	

			height_ctrl_out = wz_speed_pid_v.pid_out;


}


void Ultra_Ctrl(float T,float thr)
{
	float ultra_sp_tmp,ultra_dis_tmp;	
	//����������*������ʲô����
	//exp_height_speed������ĸ߶��ٶȣ������Ϊ��ǰ�ɻ��ζ��ٶ�
	exp_height_speed = ULTRA_SPEED *my_deathzoom_2(thr - 500,50)/200.0f; //�����thr-500������-50��С��50����my_deathzoom_2(thr - 500,50)=0����Ȼ=thr - 500

	exp_height_speed = LIMIT(exp_height_speed ,-ULTRA_SPEED,ULTRA_SPEED);//�������·�������

	//  -300 -- 300֮��
	if( exp_height > ULTRA_MAX_HEIGHT )//�����߶�>������������̣��ر�
	{
		if( exp_height_speed > 0 )
		{
			exp_height_speed = 0;
		}
	}
	else if( exp_height < 20 )//Ԥ��߶�С��2cm���ر�
	{
		if( exp_height_speed < 0 )
		{
			exp_height_speed = 0;
		}
	}

//	exp_height += exp_height_speed *T;//Ԥ��߶�������ȷ����

	exp_height=Set_Height;	//�����߶�1m hgyy
	
	if( thr < 100 )//����thr����С��100����Ϊthr�ڱ����ݸ�������ǰ�Ѿ��ӹ�500
	{
		exp_height += ( 1 / ( 1 + 1 / ( 0.2f *3.14f *T ) ) ) *( -exp_height);//��ͨ�˲�
	}
	//Moving_Median����ֵ�˲�

	ultra_sp_tmp = Moving_Median(0,5,ultra_delta/T); //ultra_delta�����γ����������Ĳ�ֵ;��T�����ʱ����Z�����ٶ�
									//ȡ3��������������С��ֵ��������
	ultra_dis_tmp = Moving_Median(1,5,ultra_distance);//��ǰ�߶�
	
	if( ultra_dis_tmp < 2000 )//��ǰ�߶�<2000
	{
		if( ABS(ultra_sp_tmp) < 100 )//Z�����ٶ�<100��//�ٶȸ�Сʱ���ǳ��������ݸ��󣬼ӿ���ƣ�������
		{
			ultra_speed += ( 1 / ( 1 + 1 / ( 4 *3.14f *T ) ) ) * ( (float)(ultra_sp_tmp) - ultra_speed );//Z�����ٶ��˲�
		}
		else
		{
			ultra_speed += ( 1 / ( 1 + 1 / ( 1.0f *3.14f *T ) ) ) * ( (float)(ultra_sp_tmp) - ultra_speed );//������һ���˲���ֻ�����޸���ϵ��
		}
	}


	
	if( ultra_dis_tmp < 2000 )//��ǰ�߶�<2000
	{
		//ultra_dis_lpf���������߶��˲�ֵ����һ���˲���
		//��Ե�ǰ��ֵ�˲��߶�ֵultra_dis_tmp����һ�ξ�����ֵ�͵�ͨ�˲�ֵ���Ƚϣ����ݲ�ͬ��ֵѡ��ͬ���˲�
		//�˲��õ�ֵ��Ϊ��������
		if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 100 )//ultra_dis_lpf��ʼΪ0���ɱ�������ĳ����޸���ֵ
		{        //ultra_dis_tmp������ֵ                //ultra_dis_lpf��������������˲�ֵ
			
			// 4.0f *3.14f *T Խ�󣬵õ���ֵҲ��Խ�󣬼ӿ�Ӧ�Ը���״���ķ�Ӧ�ٶȣ���������
			
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 4.0f *3.14f *T ) ) ) *(ultra_dis_tmp - ultra_dis_lpf) ;//�˲�
		}
		else if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 200 )
		{
			
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 2.2f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;//�˲�
		}
		else if( ABS(ultra_dis_tmp - ultra_dis_lpf) < 400 )
		{
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 1.2f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;//�˲�
		}
		else
		{
			ultra_dis_lpf += ( 1 / ( 1 + 1 / ( 0.6f *3.14f *T ) ) ) *(ultra_dis_tmp- ultra_dis_lpf) ;
		}
	}
	else
	{
		
	}


	
	ultra_ctrl.err = ( ultra_pid.kp*(exp_height - ultra_dis_lpf) );//Ԥ��߶�ֵ��ʵ���˲���ĸ߶�ֵ��
	
	ultra_ctrl.err_i += ultra_pid.ki *ultra_ctrl.err *T;//��Ч��ki=0
	
	ultra_ctrl.err_i = LIMIT(ultra_ctrl.err_i,-Thr_Weight *ULTRA_INT,Thr_Weight *ULTRA_INT);//��������
	
			ultra_ctrl.err_i = LIMIT(ultra_ctrl.err_i,-Thr_Weight *ULTRA_INT , Thr_Weight *ULTRA_INT);//��������
	
	ultra_ctrl.err_d = ultra_pid.kd *( 0.6f *(-wz_speed*T) + 0.4f *(ultra_ctrl.err - ultra_ctrl.err_old) );//�ٶ�һ���˲�
	
	ultra_ctrl.pid_out = ultra_ctrl.err + ultra_ctrl.err_i + ultra_ctrl.err_d;//�������߶ȿ��������
	
	H_Test=ultra_ctrl.pid_out;
	ultra_ctrl.pid_out = LIMIT(ultra_ctrl.pid_out,-500,500);//�������޷�
		
	ultra_ctrl_out = ultra_ctrl.pid_out;//��������
	
	ultra_ctrl.err_old = ultra_ctrl.err;//�����ϴ����

}





float wz_acc_mms2;//��ǰλ�ü��ٶ�
float height_thr,weight_ultra=0.8,weight_acc=0.2;
float real_acc_z=0,real_speed=0,speed_ronghe=0;
float swap_acc;
float height_thr;
void height_speed_ctrl(float T,float thr,float exp_z_speed,float h_speed)
{                                              //Ԥ��߶���ʵ�ʸ߶ȵ�pidֵ���ٶȣ�����0.4=������z���ٶȣ�
// 															//thr��Χ : 0 - 1000

	static float thr_lpf;
	
	static float hc_acc_i,wz_speed_0,wz_speed_1;
	
	wz_acc =2070;   //�ȶ�̬��z����ٶ�
//	thr=thr-1000;		//hgyy�޸�

	//  wz_acc_mms2 :������z��ʵʱ���ٶ�

	height_thr = LIMIT( 2 * thr , 0, 600 );//���Ƹ߶�����
// 	
	thr_lpf += ( 1 / ( 1 + 1 / ( 0.5f *3.14f *T ) ) ) *( height_thr - thr_lpf );//���ŵ�ͨ�˲����ο��ĵ�7������/5��Ƭ���������˲���
	
	thr_lpf=500;	//hgyy�޸�

// 		
// 		swap_acc +=( 1 / ( 1 + 1 / ( 0.2 * 3.14f * 1 ) ) ) *(  fabs(a[2])-swap_acc  ) ;//Z�����ٶ��˲�   0.76
	
	
		swap_acc=MPU6050_ACC_DATA.Z;	//hgyy
		
		real_acc_z = my_deathzoom(swap_acc - wz_acc ,50);   //ȥ��ʵ�ʼ��ٶȵĶ���


		real_speed += real_acc_z * 0.5;              //����ʵ�ʵ��ٶȣ�

		real_speed +=( 1 / ( 1 + 1 / ( 0.5 *3.14f *T ) ) ) *( h_speed - real_speed  ) ;

// 		speed_ronghe = weight_ultra * h_speed +weight_acc *real_speed ;

		

		wz_speed += ( 1 / ( 1 + 1 / ( 1 * 3.14f * 1 ) ) ) *( real_speed - wz_speed ) ;//Z�����ٶ��˲�
	
			
		wz_speed=LIMIT(wz_speed,-300,300);
	
	
	
	wz_speed_pid_v.err = wz_speed_pid.kp *( exp_z_speed - wz_speed );//�����ٶȼ�ȥ��ǰλ���ٶȣ����Ŵ�
	//0.002f/T *10=1������û������
	wz_speed_pid_v.err_d = 0.002f/T *10*wz_speed_pid.kd * (-real_acc_z) *T;//�ٶ��Ͻ���D����
	
	//wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid_v.err *T;
	//exp_z_speed�ڳ������£�Ϊ���������������������ѹ����Ϊ�򵥴��������ţ�˵���˾�������z���ٶ�
	//h_speed��z��ʵ���ٶȣ�( exp_z_speed - h_speed ) *T=ʵ��λ���ϵ����	
	wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid.kp *( exp_z_speed - h_speed ) *T;
	//�����޷�
	wz_speed_pid_v.err_i = LIMIT(wz_speed_pid_v.err_i,-Thr_Weight *300,Thr_Weight *300);
	//LIMIT��pid������޷���Thr_Weightֵ�ķ�ΧΪ0~1��Ϊ�˷������ݣ����Ҿ�����Ϊ�����ſ����ȶ���
	//thr��ͨ�˲���Ϊthr_lpf
	wz_speed_pid_v.pid_out = thr_lpf + Thr_Weight *LIMIT((wz_speed_pid.kp *exp_z_speed + wz_speed_pid_v.err + wz_speed_pid_v.err_d + wz_speed_pid_v.err_i),-300,300);

	wz_speed_pid_v.err_old = wz_speed_pid_v.err; 








///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////






// 	
// 	wz_speed_pid_v.err = wz_speed_pid.kp *( exp_z_speed - wz_speed );//�����ٶȼ�ȥ��ǰλ���ٶȣ����Ŵ�
// 	//0.002f/T *10=1������û������
// 	wz_speed_pid_v.err_d = 0.002f/T *10*wz_speed_pid.kd * (-wz_acc_mms2) *T;//�ٶ��Ͻ���D����
// 	
// 	//wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid_v.err *T;
// 	//exp_z_speed�ڳ������£�Ϊ���������������������ѹ����Ϊ�򵥴��������ţ�˵���˾�������z���ٶ�
// 	//h_speed��z��ʵ���ٶȣ�( exp_z_speed - h_speed ) *T=ʵ��λ���ϵ����	
// 	wz_speed_pid_v.err_i += wz_speed_pid.ki *wz_speed_pid.kp *( exp_z_speed - h_speed ) *T;
// 	//�����޷�
// 	wz_speed_pid_v.err_i = LIMIT(wz_speed_pid_v.err_i,-Thr_Weight *300,Thr_Weight *300);
// 	//LIMIT��pid������޷���Thr_Weightֵ�ķ�ΧΪ0~1��Ϊ�˷������ݣ����Ҿ�����Ϊ�����ſ����ȶ���
// 	//thr��ͨ�˲���Ϊthr_lpf
// 	wz_speed_pid_v.pid_out = thr_lpf + Thr_Weight *LIMIT((wz_speed_pid.kp *exp_z_speed + wz_speed_pid_v.err + wz_speed_pid_v.err_d + wz_speed_pid_v.err_i),-300,300);

// 	wz_speed_pid_v.err_old = wz_speed_pid_v.err; 
}


/*
	PID ������
		�ڻ���
				wz_speed_pid.kp
				wz_speed_pid.ki
				wz_speed_pid.kd
		�⻷��
			
				ultra_pid.kp
				ultra_pid.ki
				ultra_pid.kd
*/



