#ifndef _TIM_PWM_IN_H_
#define _TIM_PWM_IN_H_
#include "stm32f4xx.h"
#include "rc.h"

extern u16 Rc_Pwm_In[10];
extern u8 Rc_Pwm_In_Ready;

void Tim_Pwm_In_Init(void);
//void Tim2_Pwm_In_Irq(void);
//void Tim3_Pwm_In_Irq(void);

//void Tim9_Pwm_In_Irq(void);
//void Tim10_Pwm_In_Irq(void);
//void Tim11_Pwm_In_Irq(void);
void _TIM3_IRQHandler(void);
void _TIM4_IRQHandler(void);

#endif








