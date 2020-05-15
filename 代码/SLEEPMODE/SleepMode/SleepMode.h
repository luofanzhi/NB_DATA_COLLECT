#ifndef _SleepMode_h__
#define _SleepMode_h__
#include "sys.h"
void Sys_Sleep_Mode(void);
void StopMode_GPIO_Config(void);
void Sys_Enter_SleepMode(void);

typedef struct
{
	u16 Enter_SleepMode_Time;
	uint8_t Enter_SleepMode_Flag;
	uint8_t WeakUP;
	
}Sleep_Mode_Typedef;
extern Sleep_Mode_Typedef Sleep_Mode_Struct;
#endif


