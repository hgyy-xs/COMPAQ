#include "Flash_Eeprom.h"
#include "include.h"

/* Base address of the Flash sectors */ 
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

#define PAGE_BYTES	2048
#define PAGE_NUM		64

static u8 clear_flash(u16 sector);
static u8 get_act_page(void);
static u8 write_float(u32 start_addr, u16 cnt, float data);
static u8 write_int(u32 start_addr, u16 cnt, u32 data);
static float read_float(u32 start_addr, u16 cnt);
static u32 read_int(u32 start_addr, u16 cnt);

u8 Data_Save(void)
{	
	u8 act_page_num = 255;	//ø…“‘ π”√µƒ“≥¬Î
	u32 act_page_start = 0;	//∏√“≥∆ ºµÿ÷∑
	u16 data_cnt = 1;
	act_page_num = get_act_page();
	if(act_page_num>(PAGE_NUM-2))
	{
		if(clear_flash(FLASH_Sector_11))
			act_page_num = 0;
		else
			return 0;	//≤¡≥˝ ß∞‹
	}
	
	act_page_start = FLASH_USER_START_ADDR + (act_page_num * PAGE_BYTES);
	FLASH_Unlock();
	
	if(FLASH_ProgramWord(act_page_start, 0x12345678) == FLASH_COMPLETE);
  else
		return 0;	//–¥»Î ß∞‹
	
	if(!write_float(act_page_start, data_cnt++, PID_ROL.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_ROL.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_ROL.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PIT.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PIT.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PIT.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_YAW.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_YAW.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_YAW.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_ALT.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_ALT.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_ALT.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_POS.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_POS.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_POS.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PID_1.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PID_1.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PID_1.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PID_2.P) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PID_2.I) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_float(act_page_start, data_cnt++, PID_PID_2.D) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_int(act_page_start, data_cnt++, GYRO_OFFSET.X) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_int(act_page_start, data_cnt++, GYRO_OFFSET.Y) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_int(act_page_start, data_cnt++, GYRO_OFFSET.Z) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_int(act_page_start, data_cnt++, ACC_OFFSET.X) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_int(act_page_start, data_cnt++, ACC_OFFSET.Y) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	if(!write_int(act_page_start, data_cnt++, ACC_OFFSET.Z) == FLASH_COMPLETE) return 0;	//–¥»Î ß∞‹
	
	FLASH_Lock(); 

	return 1;
}
u8 Data_Read(void)
{
	u8 act_page_num = 255;	//ø…“‘ π”√µƒ“≥¬Î
	u32 act_page_start = 0;	//∏√“≥∆ ºµÿ÷∑
	u16 data_cnt = 1;	
	act_page_num = get_act_page();
	if(act_page_num==0)
		return 0;
	else
		act_page_num -= 1;
	act_page_start = FLASH_USER_START_ADDR + (act_page_num * PAGE_BYTES);
	

	PID_ROL.P = read_float(act_page_start, data_cnt++);
	PID_ROL.I = read_float(act_page_start, data_cnt++);
	PID_ROL.D = read_float(act_page_start, data_cnt++);
	PID_PIT.P = read_float(act_page_start, data_cnt++);
	PID_PIT.I = read_float(act_page_start, data_cnt++);
	PID_PIT.D = read_float(act_page_start, data_cnt++);
	PID_YAW.P = read_float(act_page_start, data_cnt++);
	PID_YAW.I = read_float(act_page_start, data_cnt++);
	PID_YAW.D = read_float(act_page_start, data_cnt++);
	PID_ALT.P = read_float(act_page_start, data_cnt++);
	PID_ALT.I = read_float(act_page_start, data_cnt++);
	PID_ALT.D = read_float(act_page_start, data_cnt++);
	PID_POS.P = read_float(act_page_start, data_cnt++);
	PID_POS.I = read_float(act_page_start, data_cnt++);
	PID_POS.D = read_float(act_page_start, data_cnt++);
	PID_PID_1.P = read_float(act_page_start, data_cnt++);
	PID_PID_1.I = read_float(act_page_start, data_cnt++);
	PID_PID_1.D = read_float(act_page_start, data_cnt++);
	PID_PID_2.P = read_float(act_page_start, data_cnt++);
	PID_PID_2.I = read_float(act_page_start, data_cnt++);
	PID_PID_2.D = read_float(act_page_start, data_cnt++);
	GYRO_OFFSET.X = read_int(act_page_start, data_cnt++);
	GYRO_OFFSET.Y = read_int(act_page_start, data_cnt++);
	GYRO_OFFSET.Z = read_int(act_page_start, data_cnt++);
	ACC_OFFSET.X = read_int(act_page_start, data_cnt++);
	ACC_OFFSET.Y = read_int(act_page_start, data_cnt++);
	ACC_OFFSET.Z = read_int(act_page_start, data_cnt++);
	
	return 1;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
static u8 clear_flash(u16 sector)
{
	u8 _return = 0;
	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | 
                  FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
	if (FLASH_EraseSector(sector, VoltageRange_3) != FLASH_COMPLETE)
    { 
      _return = 0;// ß∞‹
    }
	FLASH_Lock(); 
	_return = 1;	//≥…π¶
	return _return;
}
static u8 get_act_page(void)
{
	u8 i;
	for(i=0;i<PAGE_NUM;i++)
	{
		if((*(__IO uint32_t*)(FLASH_USER_START_ADDR+i*PAGE_BYTES))==0xffffffff)
		{
			return i;
		}
	}
	return 0xff;
}
static u8 write_float(u32 start_addr, u16 cnt, float data)
{	
	u32 temp;
	if(cnt>510)
		return 0;


	temp = *(uint32_t *)(&data);
	if(FLASH_ProgramWord(start_addr+(cnt*4), temp) == FLASH_COMPLETE)
		return 1;
	else 
		return 0;	//–¥»Î ß∞‹
}
static u8 write_int(u32 start_addr, u16 cnt, u32 data)
{
	if(cnt>510)
		return 0;

	if(FLASH_ProgramWord(start_addr+(cnt*4), data) == FLASH_COMPLETE)
		return 1;
	else 
		return 0;	//–¥»Î ß∞‹
}
static float read_float(u32 start_addr, u16 cnt)
{
	u32 temp = *(__IO uint32_t*)(start_addr+(cnt*4));
	return *(float *)(&temp);
}
static u32 read_int(u32 start_addr, u16 cnt)
{
	return *(__IO uint32_t*)(start_addr+(cnt*4));
}
