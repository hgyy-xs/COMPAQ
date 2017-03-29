#include "stm32f4xx.h" 
#include "include.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "includes.h"
//#include "os_app_hooks.h"
// ��COSIII ��COSIII��ֲ

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define LED0_TASK_PRIO		4
//�����ջ��С	
#define LED0_STK_SIZE 		128
//������ƿ�
OS_TCB Led0TaskTCB;
//�����ջ	
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
void led0_task(void *p_arg);

//�������ȼ�
#define LED1_TASK_PRIO		5
//�����ջ��С	
#define LED1_STK_SIZE 		128
//������ƿ�
OS_TCB Led1TaskTCB;
//�����ջ	
CPU_STK LED1_TASK_STK[LED1_STK_SIZE];
//������
void led1_task(void *p_arg);

//�������ȼ�
#define FLOAT_TASK_PRIO		6
//�����ջ��С
#define FLOAT_STK_SIZE		128
//������ƿ�
OS_TCB	FloatTaskTCB;
//�����ջ
__align(8) CPU_STK	FLOAT_TASK_STK[FLOAT_STK_SIZE];
//������
void float_task(void *p_arg);

//�������ȼ�
#define DataTransfer_Task_PRIO		7
//�����ջ��С	
#define DataTransfer_Task_SIZE 		512
//������ƿ�
OS_TCB DataTransfer_Task_TCB;
//�����ջ	
CPU_STK DataTransfer_Task_STK[DataTransfer_Task_SIZE];
//������
void DataTransfer_task(void *p_arg);

OS_MUTEX	DateCheck_MUTEX;		//����һ�������ź���


////////////////////////////////////////////////////////
OS_TMR 	tmr1;		//��ʱ��1
OS_TMR	tmr2;		//��ʱ��2
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����
int main(void)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	delay_init(168);		  
	uart_init(115200);	
	USART2_Init(115200);	
	Ultra_Init(9600);	//������uart5
	KS103_PID_Para();
	LED_Init();		
	Nvic_Init();
	Timer_Sys_Init(1000);	//��ʱ��Ƶ��1000Hz
	Tim_Pwm_Out_Init();	
	Tim_Pwm_In_Init();
	Para_ResetToFactorySetup();	
	paramLoad();		//���Դ���PID�Ĳ���
	Rc_Data.THROTTLE=1051;		//��ң����ʱ����ֵ
	Rc_Data.ROLL=1500;
	Rc_Data.PITCH=1500;
	Rc_Data.YAW=1500;
	ARMED=0;					
	LED_OFFSET(); //�ȴ�LEDָʾ
	IWDG_Init(4,320);	//���Ź���ʼ����ι��ʱ�� <500ms
	
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	while(1);
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )5,			//5*10=50ms
                (OS_TICK	 )10,          //10*10=100ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����					
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )5,			//5*10=50ms	
                (OS_TICK	 )5,   			//5*10ms		
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);
				
	OS_CRITICAL_ENTER();	//�����ٽ���
		//����һ�������ź���
	OSMutexCreate((OS_MUTEX*	)&DateCheck_MUTEX,
				  (CPU_CHAR*	)"DateCheck_MUTEX",
                  (OS_ERR*		)&err);

	//����LED0����
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
				 
	//����LED1����
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
				 
	//���������������
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
					 
	//������������
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



	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ����	
	OSTmrStart(&tmr1,&err);	//������ʱ��1	
	OSTmrStart(&tmr2,&err);	//������ʱ��2	

	OS_CRITICAL_EXIT();	//�����ٽ���
}

//��ʱ��1�Ļص�����	100ms
void tmr1_callback(void *p_tmr, void *p_arg)
{
//	static u8 tmr1_num=0;
//	tmr1_num++;		//��ʱ��1ִ�д�����1
	IWDG_Feed_Dog();
	


//	CONTROL(Q_ANGLE_DATA.ROLL,Q_ANGLE_DATA.PITCH,Q_ANGLE_DATA.YAW);	

}

//��ʱ��2�Ļص�����	50ms
void tmr2_callback(void *p_tmr,void *p_arg)
{
//	static u8 tmr2_num = 0;
//	tmr2_num++;		//��ʱ��2ִ�д�����1
//	printf("��ʱ��2���н���\r\n");
	Ultra_Duty();
	Thr_Ctrl(0.05);	//����
//	KS103_CONTROL(Set_Height);
}
//led0������
//void led0_task(void *p_arg)
//{
//	OS_ERR err;
//	p_arg = p_arg;
//	while(1)
//	{
//		LED0=0;
//		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
//		LED0=1;
//		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
//	}
//}

////led1������
//void led1_task(void *p_arg)
//{
//	OS_ERR err;
//	p_arg = p_arg;
//	while(1)
//	{
//		LED1=~LED1;
//		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
//	}
//}

//��������
//void float_task(void *p_arg)
//{
//	CPU_SR_ALLOC();
//	static float float_num=0.01;
//	while(1)
//	{
//		float_num+=0.01f;
//		OS_CRITICAL_ENTER();	//�����ٽ���
//		printf("float_num��ֵΪ: %.4f\r\n",float_num);
//		OS_CRITICAL_EXIT();		//�˳��ٽ���
//		delay_ms(500);			//��ʱ500ms
//	}
//}

//��������
void DataTransfer_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{
//		IWDG_Feed_Dog();	
//		IMU_Update();	
//	    OSMutexPend (&DateCheck_MUTEX,0,OS_OPT_PEND_BLOCKING,0,&err);	//���󻥳��ź���
		Send_Data();//���ϵͳ������������޸�
//		if(Data_Check)	
//		{
//			Data_Check = 0;
//			Send_Data();
//		}			
//		Data_Send_Status();
//		Data_Send_Senser();
//		Data_Send_RCData();	
//		Data_Send_MotoPWM();
		

//		OSMutexPost(&DateCheck_MUTEX,OS_OPT_POST_NONE,&err);				//�ͷŻ����ź���
		OSTimeDlyHMSM(0,0,0,20,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ20ms
	}
}
//��ʱ��δ����
//				OSTmrStart(&tmr1,&err);	//������ʱ��1
//				OSTmrStop(&tmr1,OS_OPT_TMR_NONE,0,&err);	//�رն�ʱ��1




