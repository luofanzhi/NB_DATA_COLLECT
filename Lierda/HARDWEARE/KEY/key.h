#ifndef _key_h__
#define _key_h__
#include "sys.h"

#define KEY2 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY1 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_11)
#define KEY3 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
typedef struct
{
	uint8_t KEY2_Flag;
	uint8_t KEY2_Scan;
	uint16_t KEY_Time;
	struct KEY
	{
		uint8_t long_key;
		uint8_t short_key;
	}KEY_MODE;
	uint16_t KEY_S_Mode_Time;
}KEYTypedef;
extern KEYTypedef KEYStruct;



void KEY_GPIO_Init(void);
u8 KEY_Scan(u8 mode);
void KEY_SCAN(uint8_t Jishi_Time);


#endif


