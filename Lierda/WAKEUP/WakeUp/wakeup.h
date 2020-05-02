#ifndef _wakeup_h__
#define _wakeup_h__
#include "sys.h"
#include "Systerm_Init.h"


typedef struct
{
	uint8_t EnWork_Type;
	struct mode
	{
		uint8_t Work_Mode;
		struct JQZ
		{
			uint8_t Update;
			
		}JQZ_Mode;
		uint8_t WeakUP_Mode;
		uint8_t Sleep_Mode;
		struct WeakUP
		{
			uint8_t Set_ARM_Time;
			uint16_t WeakUP_Time;
			uint8_t WeakUP_Over_Time;
		}Weakup;
		struct SLEEP
		{
			uint32_t Sleep_Time;
		}Sleep;
		struct BLUE
		{
			uint32_t Con_Time;
			uint8_t Con_over_Time;
		}Blue_S;
	}Mode_type;
}Work_Typedef;
extern Work_Typedef Work_Struct;
void WKUP_GPIO_Init(void);
void weak_BLUE_init(void);
void weak_NB_init(void);
void Wake_Deal(void);

#endif



