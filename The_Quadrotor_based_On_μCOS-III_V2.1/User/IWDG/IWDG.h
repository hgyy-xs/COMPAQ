#ifndef _IWDG_H_
#define _IWDG_H_
#include "stm32f4xx.h"

void IWDG_Feed_Dog(void);
void IWDG_Init(u8 prer,u16 rlr);

#endif

