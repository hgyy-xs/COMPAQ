#include "stm32f4xx.h" 
#include "include.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
//#include "os_app_hooks.h"
// μCOSIII μCOSIII移植

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define LED0_TASK_PRIO		4
//任务堆栈大小	
#define LED0_STK_SIZE 		128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//任务优先级
#define LED1_TASK_PRIO		5
//任务堆栈大小	
#define LED1_STK_SIZE 		128
//任务控制块
OS_TCB Led1TaskTCB;
//任务堆栈	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
//任务函数
void led1_task(void *p_arg);

//任务优先级
#define FLOAT_TASK_PRIO		6
//任务堆栈大小
#define FLOAT_STK_SIZE		128
//任务控制块
OS_TCB	FloatTaskTCB;
//任务堆栈
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//任务函数
void float_task(void *p_arg);

//任务优先级
#define DataTransfer_Task_PRIO		7
//任务堆栈大小	
#define DataTransfer_Task_SIZE 		512
//任务控制块
OS_TCB DataTransfer_Task_TCB;
//任务堆栈	
CPU_STK DataTransfer_Task_STK[DataTransfer_Task_SIZE];
//任务函数
void DataTransfer_task(void *p_arg);

OS_MUTEX	DateCheck_MUTEX;		//定义一个互斥信号量


////////////////////////////////////////////////////////
OS_TMR 	tmr1;		//定时器1
OS_TMR	tmr2;		//定时器2
void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数
void tmr2_callback(void *p_tmr, void *p_arg);	//定时器2回调函数
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);		  
	uart_init(115200);	
	USART2_Init(115200);	
	Ultra_Init(9600);	//超声波uart5
	KS103_PID_Para();
	LED_Init();		
	Nvic_Init();
	Timer_Sys_Init(1000);	//定时器频率1000Hz
	Tim_Pwm_Out_Init();	
	Tim_Pwm_In_Init();
	Para_ResetToFactorySetup();	
	paramLoad();		//测试串级PID的参数
	Rc_Data.THROTTLE=1051;		//无遥控器时测试值
	Rc_Data.ROLL=1500;
	Rc_Data.PITCH=1500;
	Rc_Data.YAW=1500;
	ARMED=0;					
	LED_OFFSET(); //等待LED指示
	IWDG_Init(4,320);	//看门狗初始化，喂狗时间 <500ms
	
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,		//定时器1
                (CPU_CHAR	*)"tmr1",		//定时器名字
                (OS_TICK	 )5,			//5*10=50ms
                (OS_TICK	 )10,          //10*10=100ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码					
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )5,			//5*10=50ms	
                (OS_TICK	 )5,   			//5*10ms		
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);
				
	OS_CRITICAL_ENTER();	//进入临界区
		//创建一个互斥信号量
	OSMutexCreate((OS_MUTEX*	)&DateCheck_MUTEX,
				  (CPU_CHAR*	)"DateCheck_MUTEX",
                  (OS_ERR*		)&err);

	//创建LED0任务
//	OSTaskCreate((OS_TCB 	* )&Led0TaskTCB,		
//				 (CPU_CHAR	* )"led0 task", 		
//                 (OS_TASK_PTR )led0_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )LED0_TASK_PRIO,     
//                 (CPU_STK   * )&LED0_TASK_STK[0],	
//                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
//                 (CPU_STK_SIZE)LED0_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,					
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
//                 (OS_ERR 	* )&err);				
				 
	//创建LED1任务
//	OSTaskCreate((OS_TCB 	* )&Led1TaskTCB,		
//				 (CPU_CHAR	* )"led1 task", 		
//                 (OS_TASK_PTR )led1_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )LED1_TASK_PRIO,     	
//                 (CPU_STK   * )&LED1_TASK_STK[0],	
//                 (CPU_STK_SIZE)LED1_STK_SIZE/10,	
//                 (CPU_STK_SIZE)LED1_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
//                 (OS_ERR 	* )&err);
				 
	//创建浮点测试任务
//	OSTaskCreate((OS_TCB 	* )&FloatTaskTCB,		
//				 (CPU_CHAR	* )"float test task", 		
//                 (OS_TASK_PTR )float_task, 			
//                 (void		* )0,					
//                 (OS_PRIO	  )FLOAT_TASK_PRIO,     	
//                 (CPU_STK   * )&FLOAT_TASK_STK[0],	
//                 (CPU_STK_SIZE)FLOAT_STK_SIZE/10,	
//                 (CPU_STK_SIZE)FLOAT_STK_SIZE,		
//                 (OS_MSG_QTY  )0,					
//                 (OS_TICK	  )0,					
//                 (void   	* )0,				
//                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
//                 (OS_ERR 	* )&err);		
					 
	//创建数传任务
	OSTaskCreate((OS_TCB 	* )&DataTransfer_Task_TCB,		
				 (CPU_CHAR	* )"Datatransfer task", 		
                 (OS_TASK_PTR )DataTransfer_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )DataTransfer_Task_PRIO,     
                 (CPU_STK   * )&DataTransfer_Task_STK[0],	
                 (CPU_STK_SIZE)DataTransfer_Task_SIZE/10,	
                 (CPU_STK_SIZE)DataTransfer_Task_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);			



	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务	
	OSTmrStart(&tmr1,&err);	//开启定时器1	
	OSTmrStart(&tmr2,&err);	//开启定时器2	

	OS_CRITICAL_EXIT();	//进入临界区
}

//定时器1的回调函数	100ms
void tmr1_callback(void *p_tmr, void *p_arg)
{
//	static u8 tmr1_num=0;
//	tmr1_num++;		//定时器1执行次数加1
	IWDG_Feed_Dog();
	


//	CONTROL(Q_ANGLE_DATA.ROLL,Q_ANGLE_DATA.PITCH,Q_ANGLE_DATA.YAW);	

}

//定时器2的回调函数	50ms
void tmr2_callback(void *p_tmr,void *p_arg)
{
//	static u8 tmr2_num = 0;
//	tmr2_num++;		//定时器2执行次数加1
//	printf("定时器2运行结束\r\n");
	Ultra_Duty();
	Thr_Ctrl(0.05);	//定高
//	KS103_CONTROL(Set_Height);
}
//led0任务函数
//void led0_task(void *p_arg)
//{
//	OS_ERR err;
//	p_arg = p_arg;
//	while(1)
//	{
//		LED0=0;
//		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
//		LED0=1;
//		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
//	}
//}

////led1任务函数
//void led1_task(void *p_arg)
//{
//	OS_ERR err;
//	p_arg = p_arg;
//	while(1)
//	{
//		LED1=~LED1;
//		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
//	}
//}

//测试任务
//void float_task(void *p_arg)
//{
//	CPU_SR_ALLOC();
//	static float float_num=0.01;
//	while(1)
//	{
//		float_num+=0.01f;
//		OS_CRITICAL_ENTER();	//进入临界区
//		printf("float_num的值为: %.4f\r\n",float_num);
//		OS_CRITICAL_EXIT();		//退出临界区
//		delay_ms(500);			//延时500ms
//	}
//}

//数传任务
void DataTransfer_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
//		IWDG_Feed_Dog();	
//		IMU_Update();	
//	    OSMutexPend (&DateCheck_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//请求互斥信号量
		Send_Data();//针对系统任务调度做出修改
//		if(Data_Check)	
//		{
//			Data_Check = 0;
//			Send_Data();
//		}			
//		Data_Send_Status();
//		Data_Send_Senser();
//		Data_Send_RCData();	
//		Data_Send_MotoPWM();
		

//		OSMutexPost(&DateCheck_MUTEX,OS_OPT_POST_NONE,&err);				//释放互斥信号量
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //延时20ms
	}
}
//定时器未开启
//				OSTmrStart(&tmr1,&err);	//开启定时器1
//				OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//关闭定时器1




