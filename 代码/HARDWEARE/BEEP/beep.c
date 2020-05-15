#include "beep.h"

void BEEP_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOB,GPIO_Pin_3);
}

void BEEP_ON(uint8_t times)
{
	u8 i;
	if(times>0)
	{
		for(i=0;i<times;i++)
		{
			BEEP=1;
			delay_ms(40);
			BEEP=0;
			delay_ms(40);
		}
	}
	else 
	{
		BEEP=0;
	}
	
}


