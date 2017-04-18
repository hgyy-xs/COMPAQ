#include "My_Height.h"
#include "include.h"


struct _PID PID_KS103;        //超声波的pid参数


float alt_KS103_1;           //误差值
float KS103_ALT_V;			//超声波测算出的速度
float KS103_Alt;			//声波数据
		

float tr_tmep1,thr_last,thr_tmep1,thr_tmep,  /*当前超声波油门调节量。*/
THR_Lock=0;


/*	
	Q:过程噪声，Q增大，动态响应变快，收敛稳定性变坏
	R:测量噪声，R增大，动态响应变慢，收敛稳定性变好	
*/
#define KALMAN_Q        0.02
#define KALMAN_R        6.0000


/*====================================================================================================*
**函数 : KalmanFilter
**功能 : 卡尔曼滤波
**输入 :  ResrcData:角速度转角数据    ProcessNiose_Q:过程噪音    MeasureNoise_R: 测量噪音
**        x_last:X(k-1):上次系统状态          p_last
**輸出 : None
**备注 : None
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

   x_mid=x_last;                        //x_last=x(k-1|k-1),x_mid=x(k|k-1)            //x_last：上一次最优角度值
   p_mid=p_last+Q;                      //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声     //p_mid：上一次最优角度协方差   Q：预测值协方差。
   kg=p_mid/(p_mid+R);                  //kg为kalman filter，R为噪声            //kg：卡尔曼收敛值。  R：测量误差的协方差
   x_now=x_mid+kg*(ResrcData-x_mid);    //估计出的最优值     //更新系统状态估计=上次系统状态+卡尔曼增益*角速度计数据更新值
                
   p_now=(1-kg)*p_mid;                  //最优值对应的covariance     //此次最优角度协方差。           
   p_last = p_now;                      //更新covariance值
   x_last = x_now;                      //更新系统状态值
   return x_now;                
 }

/*
   超声波	PID参数初始化（在main.c文件中初始化调用）
*/

void KS103_PID_Para(void)	//测试用的值，参数待测试
{
	PID_KS103.kp=150;
	PID_KS103.ki=0.1;
	PID_KS103.kd=80;
}

/*
   超声波定高  PD算法
*/


void KS103_CONTROL(float KS103_Alt_Target_1)
{
	
	KS103_Alt=KalmanFilter_x(ultra_distance,KALMAN_Q,KALMAN_R);
	KS103_ALT_V=ultra_delta/0.05f;
	KS103_ALT_V=KalmanFilter_x(KS103_ALT_V,KALMAN_Q,KALMAN_R);
	
//	KS103_Alt=KS103_Alt*((float)cos(Q_ANGLE_DATA.PITCH/57.295779f))*((float)cos(Q_ANGLE_DATA.ROLL/57.295779f)); //姿态补偿
//	KS103_Alt_Target_1=Set_Height;
	alt_KS103_1 =(KS103_Alt - KS103_Alt_Target_1);   	
  
//		if(KS103_Alt>500)	//高度误差限幅保护（待完善）
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






