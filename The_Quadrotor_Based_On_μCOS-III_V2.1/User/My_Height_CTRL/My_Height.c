#include "My_Height.h"
#include "include.h"


struct _PID PID_KS103;        //��������pid����


float alt_KS103_1;           //���ֵ
float KS103_ALT_V;			//��������������ٶ�
float KS103_Alt;			//��������
		

float tr_tmep1,thr_last,thr_tmep1,thr_tmep,  /*��ǰ���������ŵ�������*/
THR_Lock=0;


/*	
	Q:����������Q���󣬶�̬��Ӧ��죬�����ȶ��Ա仵
	R:����������R���󣬶�̬��Ӧ�����������ȶ��Ա��	
*/
#define KALMAN_Q        0.02
#define KALMAN_R        6.0000


/*====================================================================================================*
**���� : KalmanFilter
**���� : �������˲�
**���� :  ResrcData:���ٶ�ת������    ProcessNiose_Q:��������    MeasureNoise_R: ��������
**        x_last:X(k-1):�ϴ�ϵͳ״̬          p_last
**ݔ�� : None
**��ע : None
**====================================================================================================*/
static double KalmanFilter_x(const double ResrcData,double ProcessNiose_Q,double MeasureNoise_R)
{
   double R = MeasureNoise_R;
   double Q = ProcessNiose_Q;
   static double x_last;
   double x_mid = x_last;
   double x_now;
   static double p_last;
   double p_mid ;
   double p_now;
   double kg;        

   x_mid=x_last;                        //x_last=x(k-1|k-1),x_mid=x(k|k-1)            //x_last����һ�����ŽǶ�ֵ
   p_mid=p_last+Q;                      //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=����     //p_mid����һ�����ŽǶ�Э����   Q��Ԥ��ֵЭ���
   kg=p_mid/(p_mid+R);                  //kgΪkalman filter��RΪ����            //kg������������ֵ��  R����������Э����
   x_now=x_mid+kg*(ResrcData-x_mid);    //���Ƴ�������ֵ     //����ϵͳ״̬����=�ϴ�ϵͳ״̬+����������*���ٶȼ����ݸ���ֵ
                
   p_now=(1-kg)*p_mid;                  //����ֵ��Ӧ��covariance     //�˴����ŽǶ�Э���           
   p_last = p_now;                      //����covarianceֵ
   x_last = x_now;                      //����ϵͳ״ֵ̬
   return x_now;                
 }

/*
   ������	PID������ʼ������main.c�ļ��г�ʼ�����ã�
*/

void KS103_PID_Para(void)	//�����õ�ֵ������������
{
	PID_KS103.kp=150;
	PID_KS103.ki=0.1;
	PID_KS103.kd=80;
}

/*
   ����������  PD�㷨
*/


void KS103_CONTROL(float KS103_Alt_Target_1)
{
	
	KS103_Alt=KalmanFilter_x(ultra_distance,KALMAN_Q,KALMAN_R);
	KS103_ALT_V=ultra_delta/0.05f;
	KS103_ALT_V=KalmanFilter_x(KS103_ALT_V,KALMAN_Q,KALMAN_R);
	
//	KS103_Alt=KS103_Alt*((float)cos(Q_ANGLE_DATA.PITCH/57.295779f))*((float)cos(Q_ANGLE_DATA.ROLL/57.295779f)); //��̬����
//	KS103_Alt_Target_1=Set_Height;
	alt_KS103_1 =(KS103_Alt - KS103_Alt_Target_1);   	
  
//		if(KS103_Alt>500)	//�߶�����޷������������ƣ�
//		{
//			LandFall=1;
//		}
  
	PID_KS103.Pout=-(PID_KS103.kp)*alt_KS103_1;
	PID_KS103.Dout=-(PID_KS103.kd)*KS103_ALT_V;     
	tr_tmep1=thr_last+PID_KS103.ki*(PID_KS103.Pout+PID_KS103.Dout);
	thr_last=thr_tmep1;


	 PID_KS103.Allout=tr_tmep1+PID_KS103.Pout+PID_KS103.Dout;
	thr_tmep=PID_KS103.Allout;
	  if(thr_tmep>500)                             
    thr_tmep=500; 
    if(thr_tmep<-500) 
    thr_tmep=-500; 
//		
//	  if(thr_tmep<0)
//		  thr_tmep=0;
    THR_Lock=thr_tmep;                           
	
}






