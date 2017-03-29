#ifndef _TIMER_H_
#define _TIMER_H_
#include "stm32f4xx.h"

void Timer_Sys_Init(u32 period_num);
void Nvic_Init(void);

#endif
