#ifndef _TIM_PWM_OUT_H_
#define _TIM_PWM_OUT_H_
#include "stm32f4xx.h"

void Tim_Pwm_Out_Init(void);
void Moto_PwmRflash(int32_t MOTO1_PWM,int32_t MOTO2_PWM,int32_t MOTO3_PWM,int32_t MOTO4_PWM);
#endif
