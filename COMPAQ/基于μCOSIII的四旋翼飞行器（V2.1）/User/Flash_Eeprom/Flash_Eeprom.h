#ifndef _FLASH_EEPROM_H_
#define _FLASH_EEPROM_H_
#include "stm32f4xx.h"
#include "control.h"
//#include "MPU6050.h"

#define FLASH_USER_START_ADDR   (uint32_t)0x080E0000

#define FLASH_USER_END_ADDR     (uint32_t)0x080FFFFF

u8 Data_Save(void);
u8 Data_Read(void);

#endif
