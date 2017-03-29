#ifndef _RC_RC_H_
#define _RC_RC_H_
#include "Tim_PPm_In.h"
//#include "config.h"
#define RC_CHANL_NUM	7
#define RC_MIDDLE 		1500
#define RC_MIN				1100
#define	RC_MAX				1900
#define RC_DEADBAND		5
#define RC_USE_DEADBAND
typedef struct int16_rcget{
				int16_t ROLL;
				int16_t PITCH;
				int16_t THROTTLE;
				int16_t YAW;
				int16_t AUX1;
				int16_t AUX2;
				int16_t AUX3;
				int16_t AUX4;
				int16_t AUX5;
				int16_t AUX6;}T_RC_DATA;

extern T_RC_DATA Rc_Data;//1000~2000
extern u8 ARMED;
extern u8 LandFall;
extern u8 Height_Ctl;

void Rc_DataAnl(void);
void Rc_DataCal(void);

#endif
