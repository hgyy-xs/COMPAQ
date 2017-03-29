
//#include "include.h"
#include "ANO_CTRL.h"
#include "Imu.h"
#include "Rc.h"
#include "Para.h"

//extern MPU6050_Q_AngleData_TypeDef Q_ANGLE_DATA;
//extern T_RC_DATA Rc_Data;//1000~2000


ctrl_t ctrl_1;
ctrl_t ctrl_2;

xyz_f_t except_AS;
float g_old[ITEMS];
xyz_f_t Gyro_deg;
MPU6050_STRUCT mpu6050;
float Thr_Weight;

float thr_value;

u8 Thr_Low;

xyz_f_t except_A = {0,0,0};

xyz_f_t ctrl_angle_offset = {0,0,0};

xyz_f_t compensation;

u8 CH_out_Mapping[MAXMOTORS] = {0,1,2,3};

float motor[MAXMOTORS];
float posture_value[MAXMOTORS];
float curve[MAXMOTORS];


void Ctrl_Para_Init()		//����Ĭ�ϲ���
{
//====================================
	ctrl_1.PID[PIDROLL].kdamp  = 1;
	ctrl_1.PID[PIDPITCH].kdamp = 1;
	ctrl_1.PID[PIDYAW].kdamp 	 = 1;
	
	ctrl_1.FB = 0.20;   //��  0<fb<1
}

void SetPwm(int16_t pwm[MAXMOTORS],s16 min,s16 max)
{
	u8 i;
	s16 pwm_tem[MAXMOTORS];

	for(i=0;i<MAXMOTORS;i++)
	{
			pwm_tem[i] = pwm[i] ;
			pwm_tem[i] = LIMIT(pwm_tem[i],min,max);
	}
	
	
	TIM1->CCR1 = PWM_RADIO *( pwm_tem[CH_out_Mapping[0]] ) + INIT_DUTY;				//1	
	TIM1->CCR2 = PWM_RADIO *( pwm_tem[CH_out_Mapping[1]] ) + INIT_DUTY;				//2
	TIM5->CCR3 = PWM_RADIO *( pwm_tem[CH_out_Mapping[2]] ) + INIT_DUTY;				//3	
	TIM1->CCR4 = PWM_RADIO *( pwm_tem[CH_out_Mapping[3]] ) + INIT_DUTY;				//4
	
// 	TIM5->CCR4 = PWM_RADIO *( pwm_tem[CH_out_Mapping[4]] ) + INIT_DUTY;				//5	
// 	TIM5->CCR3 = PWM_RADIO *( pwm_tem[CH_out_Mapping[5]] ) + INIT_DUTY;				//6	
// 	TIM8->CCR4 = PWM_RADIO *( pwm_tem[CH_out_Mapping[6]] ) + INIT_DUTY;				//7	
// 	TIM8->CCR3 = PWM_RADIO *( pwm_tem[CH_out_Mapping[7]] ) + INIT_DUTY;				//8	
	
}


void CTRL_1(float T)  //x roll,y pitch,z yaw
{
	xyz_f_t EXP_LPF_TMP;
	/* ��������Ŀ�꣩���ٶ� */
	EXP_LPF_TMP.x = MAX_CTRL_ASPEED *(ctrl_2.out.x/ANGLE_TO_MAX_AS);//*( (CH_filter[0])/500.0f );//
	EXP_LPF_TMP.y = MAX_CTRL_ASPEED *(ctrl_2.out.y/ANGLE_TO_MAX_AS);//*( (CH_filter[1])/500.0f );//
	EXP_LPF_TMP.z = MAX_CTRL_ASPEED *(ctrl_2.out.z/ANGLE_TO_MAX_AS);
	
	except_AS.x = EXP_LPF_TMP.x;//20 *3.14 *T *( EXP_LPF_TMP.x - except_AS.x );//
	except_AS.y = EXP_LPF_TMP.y;//20 *3.14 *T *( EXP_LPF_TMP.y - except_AS.y );//
	except_AS.z = EXP_LPF_TMP.z;//20 *3.14 *T *( EXP_LPF_TMP.z - except_AS.z );//
	/* �������ٶ��޷� */
	except_AS.x = LIMIT(except_AS.x, -MAX_CTRL_ASPEED,MAX_CTRL_ASPEED );
	except_AS.y = LIMIT(except_AS.y, -MAX_CTRL_ASPEED,MAX_CTRL_ASPEED );
	except_AS.z = LIMIT(except_AS.z, -MAX_CTRL_ASPEED,MAX_CTRL_ASPEED );

	/* ���ٶ�ֱ��΢�֣��Ǽ��ٶȣ������������γɽ��ٶȵ����ᣨ�谭���ٶȵı仯��*/
	ctrl_1.damp.x = ( mpu6050.Gyro_deg.x - g_old[A_X]) *( 0.002f/T );//ctrl_1.PID[PIDROLL].kdamp
	ctrl_1.damp.y = (-mpu6050.Gyro_deg.y - g_old[A_Y]) *( 0.002f/T );//ctrl_1.PID[PIDPITCH].kdamp *
	ctrl_1.damp.z = (-mpu6050.Gyro_deg.z - g_old[A_Z]) *( 0.002f/T );//ctrl_1.PID[PIDYAW].kdamp	 *
	/* ���ٶ���� */
	ctrl_1.err.x =  ( except_AS.x - mpu6050.Gyro_deg.x ) *(300.0f/MAX_CTRL_ASPEED);
	ctrl_1.err.y =  ( except_AS.y + mpu6050.Gyro_deg.y ) *(300.0f/MAX_CTRL_ASPEED);  //-y
	ctrl_1.err.z =  ( except_AS.z + mpu6050.Gyro_deg.z ) *(300.0f/MAX_CTRL_ASPEED);	 //-z
	/* ���ٶ����Ȩ�� */
	ctrl_1.err_weight.x = ABS(ctrl_1.err.x)/MAX_CTRL_ASPEED;
	ctrl_1.err_weight.y = ABS(ctrl_1.err.y)/MAX_CTRL_ASPEED;
	ctrl_1.err_weight.z = ABS(ctrl_1.err.z)/MAX_CTRL_YAW_SPEED;
	/* ���ٶ�΢�� */
	ctrl_1.err_d.x = ( ctrl_1.PID[PIDROLL].kd  *( -10 *ctrl_1.damp.x) *( 0.002f/T ) );
	ctrl_1.err_d.y = ( ctrl_1.PID[PIDPITCH].kd *( -10 *ctrl_1.damp.y) *( 0.002f/T ) );
	ctrl_1.err_d.z = ( ctrl_1.PID[PIDYAW].kd   *( -10 *ctrl_1.damp.z) *( 0.002f/T ) );

//	ctrl_1.err_d.x += 40 *3.14 *0.002 *( 10 *ctrl_1.PID[PIDROLL].kd  *(ctrl_1.err.x - ctrl_1.err_old.x) *( 0.002f/T ) - ctrl_1.err_d.x);
//	ctrl_1.err_d.y += 40 *3.14 *0.002 *( 10 *ctrl_1.PID[PIDPITCH].kd *(ctrl_1.err.y - ctrl_1.err_old.y) *( 0.002f/T ) - ctrl_1.err_d.y);
//	ctrl_1.err_d.z += 40 *3.14 *0.002 *( 10 *ctrl_1.PID[PIDYAW].kd   *(ctrl_1.err.z - ctrl_1.err_old.z) *( 0.002f/T ) - ctrl_1.err_d.z);

	/* ���ٶ������� */
	ctrl_1.err_i.x += ctrl_1.PID[PIDROLL].ki  *(ctrl_1.err.x - ctrl_1.damp.x) *T;
	ctrl_1.err_i.y += ctrl_1.PID[PIDPITCH].ki *(ctrl_1.err.y - ctrl_1.damp.y) *T;
	ctrl_1.err_i.z += ctrl_1.PID[PIDYAW].ki 	*(ctrl_1.err.z - ctrl_1.damp.z) *T;
	/* ���ٶ������ַ��� */
	ctrl_1.eliminate_I.x = Thr_Weight *CTRL_1_INT_LIMIT ;
	ctrl_1.eliminate_I.y = Thr_Weight *CTRL_1_INT_LIMIT ;
	ctrl_1.eliminate_I.z = Thr_Weight *CTRL_1_INT_LIMIT ;
	/* ���ٶ��������޷� */
	ctrl_1.err_i.x = LIMIT( ctrl_1.err_i.x, -ctrl_1.eliminate_I.x,ctrl_1.eliminate_I.x );
	ctrl_1.err_i.y = LIMIT( ctrl_1.err_i.y, -ctrl_1.eliminate_I.y,ctrl_1.eliminate_I.y );
	ctrl_1.err_i.z = LIMIT( ctrl_1.err_i.z, -ctrl_1.eliminate_I.z,ctrl_1.eliminate_I.z );
	/* ���ٶ�PID��� */
	ctrl_1.out.x = 3 *( ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.x),0,1)*except_AS.x + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDROLL].kp  *( ctrl_1.err.x + ctrl_1.err_d.x + ctrl_1.err_i.x ) );
										//*(MAX_CTRL_ASPEED/300.0f);
	ctrl_1.out.y = 3 *( ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.y),0,1)*except_AS.y + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDPITCH].kp *( ctrl_1.err.y + ctrl_1.err_d.y + ctrl_1.err_i.y ) );
										//*(MAX_CTRL_ASPEED/300.0f);
	ctrl_1.out.z = 3 *( ctrl_1.FB *LIMIT((0.45f + 0.55f*ctrl_2.err_weight.z),0,1)*except_AS.z + ( 1 - ctrl_1.FB ) *ctrl_1.PID[PIDYAW].kp   *( ctrl_1.err.z + ctrl_1.err_d.z + ctrl_1.err_i.z ) );
										//*(MAX_CTRL_ASPEED/300.0f);
	Thr_Ctrl(T);// ���ſ���
	
	All_Out(ctrl_1.out.x,ctrl_1.out.y,ctrl_1.out.z);


	ctrl_1.err_old.x = ctrl_1.err.x;
	ctrl_1.err_old.y = ctrl_1.err.y;
	ctrl_1.err_old.z = ctrl_1.err.z;

	g_old[A_X] =  mpu6050.Gyro_deg.x ;
	g_old[A_Y] = -mpu6050.Gyro_deg.y ;
	g_old[A_Z] = -mpu6050.Gyro_deg.z ;
}


void CTRL_2(float T)
{
// 	static xyz_f_t acc_no_g;
// 	static xyz_f_t acc_no_g_lpf;
//=========================== �����Ƕ� ========================================
//	except_A.x  = MAX_CTRL_ANGLE  *( my_deathzoom( ( CH_filter[ROL]) ,30 )/500.0f );   //30
//	except_A.y  = MAX_CTRL_ANGLE  *( my_deathzoom( (-CH_filter[PIT]) ,30 )/500.0f );  //30
	except_A.x  = MAX_CTRL_ANGLE  *( my_deathzoom( ( Rc_Data.ROLL	) ,30 )/500.0f );   //30
	except_A.y  = MAX_CTRL_ANGLE  *( my_deathzoom( (-Rc_Data.PITCH) ,30 )/500.0f );  //30	
//	if( Thr_Low == 0 )
//	{
//		except_A.z += (s16)( MAX_CTRL_YAW_SPEED *( my_deathzoom_2( (CH_filter[YAW]) ,40 )/500.0f ) ) *T ;  //50
//	}
		if( Thr_Low == 0 )
	{
		except_A.z += (s16)( MAX_CTRL_YAW_SPEED *( my_deathzoom_2( (Rc_Data.YAW	) ,40 )/500.0f ) ) *T ;  //50
	}
	else
	{
		except_A.z += 1 *3.14 *T *( Q_ANGLE_DATA.YAW - except_A.z );
	}
	except_A.z = To_180_degrees(except_A.z);
//==============================================================================
// 	acc_no_g.x =  mpu6050.Acc.x - reference_v.x *4096;
// 	acc_no_g.y =  mpu6050.Acc.y - reference_v.y *4096;
// 	acc_no_g.z =  mpu6050.Acc.z - reference_v.z *4096;
// 	
// 	acc_no_g_lpf.x += 0.5f *T *3.14f * ( acc_no_g.x - acc_no_g_lpf.x );
// 	acc_no_g_lpf.y += 0.5f *T *3.14f * ( acc_no_g.y - acc_no_g_lpf.y );
// 	acc_no_g_lpf.z += 0.5f *T *3.14f * ( acc_no_g.z - acc_no_g_lpf.z );
// 	
// 	compensation.x = LIMIT( 0.003f *acc_no_g_lpf.x, -10,10 );
// 	compensation.y = LIMIT( 0.003f *acc_no_g_lpf.y, -10,10 );
// 	compensation.z = LIMIT( 0.003f *acc_no_g_lpf.z, -10,10 );
//==============================================================================	

  /* �õ��Ƕ���� */
	ctrl_2.err.x =  To_180_degrees( ctrl_angle_offset.x + except_A.x - Q_ANGLE_DATA.ROLL  );
	ctrl_2.err.y =  To_180_degrees( ctrl_angle_offset.y + except_A.y - Q_ANGLE_DATA.PITCH );
	ctrl_2.err.z =  To_180_degrees( ctrl_angle_offset.z + except_A.z - Q_ANGLE_DATA.YAW	 );
	/* ����Ƕ����Ȩ�� */
	ctrl_2.err_weight.x = ABS(ctrl_2.err.x)/ANGLE_TO_MAX_AS;
	ctrl_2.err_weight.y = ABS(ctrl_2.err.y)/ANGLE_TO_MAX_AS;
	ctrl_2.err_weight.z = ABS(ctrl_2.err.z)/ANGLE_TO_MAX_AS;
	/* �Ƕ����΢�֣�����������߱仯��*/
	ctrl_2.err_d.x = 10 *ctrl_2.PID[PIDROLL].kd  *(ctrl_2.err.x - ctrl_2.err_old.x) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.x );
	ctrl_2.err_d.y = 10 *ctrl_2.PID[PIDPITCH].kd *(ctrl_2.err.y - ctrl_2.err_old.y) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.y );
	ctrl_2.err_d.z = 10 *ctrl_2.PID[PIDYAW].kd 	 *(ctrl_2.err.z - ctrl_2.err_old.z) *( 0.005f/T ) *( 0.65f + 0.35f *ctrl_2.err_weight.z );
	/* �Ƕ������� */
	ctrl_2.err_i.x += ctrl_2.PID[PIDROLL].ki  *ctrl_2.err.x *T;
	ctrl_2.err_i.y += ctrl_2.PID[PIDPITCH].ki *ctrl_2.err.y *T;
	ctrl_2.err_i.z += ctrl_2.PID[PIDYAW].ki 	*ctrl_2.err.z *T;
	/* �Ƕ������ַ��� */
	ctrl_2.eliminate_I.x = Thr_Weight *CTRL_2_INT_LIMIT;
	ctrl_2.eliminate_I.y = Thr_Weight *CTRL_2_INT_LIMIT;
	ctrl_2.eliminate_I.z = Thr_Weight *CTRL_2_INT_LIMIT;
	/* �Ƕ��������޷� */
	ctrl_2.err_i.x = LIMIT( ctrl_2.err_i.x, -ctrl_2.eliminate_I.x,ctrl_2.eliminate_I.x );
	ctrl_2.err_i.y = LIMIT( ctrl_2.err_i.y, -ctrl_2.eliminate_I.y,ctrl_2.eliminate_I.y );
	ctrl_2.err_i.z = LIMIT( ctrl_2.err_i.z, -ctrl_2.eliminate_I.z,ctrl_2.eliminate_I.z );
	/* �����ڼ������������ĽǶ�����޷� */
	ctrl_2.err.x = LIMIT( ctrl_2.err.x, -90, 90 );
	ctrl_2.err.y = LIMIT( ctrl_2.err.y, -90, 90 );
	ctrl_2.err.z = LIMIT( ctrl_2.err.z, -90, 90 );
	/* �Ƕ�PID��� */
	ctrl_2.out.x = ctrl_2.PID[PIDROLL].kp  *( ctrl_2.err.x + ctrl_2.err_d.x + ctrl_2.err_i.x );	//rol
	ctrl_2.out.y = ctrl_2.PID[PIDPITCH].kp *( ctrl_2.err.y + ctrl_2.err_d.y + ctrl_2.err_i.y );  //pit
	ctrl_2.out.z = ctrl_2.PID[PIDYAW].kp   *( ctrl_2.err.z + ctrl_2.err_d.z + ctrl_2.err_i.z );
	/* ��¼��ʷ���� */	
	ctrl_2.err_old.x = ctrl_2.err.x;
	ctrl_2.err_old.y = ctrl_2.err.y;
	ctrl_2.err_old.z = ctrl_2.err.z;

}

void Thr_Ctrl(float T)
{
	static float thr;
	static float Thr_tmp;
//	thr = 500 + CH_filter[THR]; //����ֵ 0 ~ 1000
	thr = 500 + Rc_Data.THROTTLE; //����ֵ 0 ~ 1000

	Thr_tmp += 10 *3.14f *T *(thr/400.0f - Thr_tmp); //��ͨ�˲�
	Thr_Weight = LIMIT(Thr_tmp,0,1);    							//��߶ദ�������ݻ��õ����ֵ
	
	if( thr < 100 )
	{
		Thr_Low = 1;
	}
	else
	{
		Thr_Low = 0;
	}
	
	#if(CTRL_HEIGHT)
	Height_Ctrl(T,thr);
	
	thr_value = Thr_Weight *height_ctrl_out;   //ʵ��ʹ��ֵ
	#else
	thr_value = thr;   //ʵ��ʹ��ֵ
	#endif
	
	thr_value = LIMIT(thr_value,0,10 *MAX_THR *MAX_PWM/100);
}


void All_Out(float out_roll,float out_pitch,float out_yaw)
{
	s16 motor_out[MAXMOTORS];
	u8 i;
	float posture_value[MAXMOTORS];
  float curve[MAXMOTORS];
	

	out_yaw = LIMIT( out_yaw , -5*MAX_THR ,5*MAX_THR ); //50%
	
	posture_value[0] = - out_roll + out_pitch + out_yaw ;
	posture_value[1] = + out_roll + out_pitch - out_yaw ;
	posture_value[2] = + out_roll - out_pitch + out_yaw ;
	posture_value[3] = - out_roll - out_pitch - out_yaw ;
	
	for(i=0;i<4;i++)
	{
		posture_value[i] = LIMIT(posture_value[i], -1000,1000 );
	}
	
	curve[0] = (0.55f + 0.45f *ABS(posture_value[0])/1000.0f) *posture_value[0] ;
	curve[1] = (0.55f + 0.45f *ABS(posture_value[1])/1000.0f) *posture_value[1] ;
	curve[2] = (0.55f + 0.45f *ABS(posture_value[2])/1000.0f) *posture_value[2] ;
	curve[3] = (0.55f + 0.45f *ABS(posture_value[3])/1000.0f) *posture_value[3] ;
	
	motor[0] = thr_value + Thr_Weight *curve[0] ;
	motor[1] = thr_value + Thr_Weight *curve[1] ;
	motor[2] = thr_value + Thr_Weight *curve[2] ;
	motor[3] = thr_value + Thr_Weight *curve[3] ;
	/* �Ƿ���� */
//	if(fly_ready)
//	{
//		if( !Thr_Low )  //��������
//		{
//			for(i=0;i<4;i++)
//			{
//				motor[i] = LIMIT(motor[i], (10 *READY_SPEED),(10*MAX_PWM) );
//			}
//		}
//		else						//���ŵ�
//		{
//			for(i=0;i<4;i++)
//			{
//				motor[i] = LIMIT(motor[i], 0,(10*MAX_PWM) );
//			}
//		}
//	}
//	else
//	{
//		for(i=0;i<4;i++)
//		{
//			motor[i] = 0;
//		}
//	}
	/* xxx */
	motor_out[0] = (s16)(motor[0]);  
	motor_out[1] = (s16)(motor[1]);	 
	motor_out[2] = (s16)(motor[2]);
	motor_out[3] = (s16)(motor[3]);

	
	SetPwm(motor_out,0,1000); //1000
	
}




//	mpu6050.Gyro_deg.x = mpu6050.Gyro.x *TO_ANGLE;
//	mpu6050.Gyro_deg.y = mpu6050.Gyro.y *TO_ANGLE;
//	mpu6050.Gyro_deg.z = mpu6050.Gyro.z *TO_ANGLE;
