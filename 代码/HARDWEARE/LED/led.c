#include "led.h"
#include "sys.h"

void LED_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14);
}

void LEDn_ON(u8 num)
{
	switch(num)
	{
		case 1:LED1=0;
			break;
		case 2:LED2=0;
			break;
		case 3:LED3=0;
			break;
		case 4:LED1=0;LED2=0;LED3=0;
			break;
		default:
			break;
	}
}

void LEDn_OFF(u8 num)
{
	switch(num)
	{
		case 1:LED1=1;
			break;
		case 2:LED2=1;
			break;
		case 3:LED3=1;
			break;
		case 4:LED1=1;LED2=1;LED3=1;
			break;
		default:
			break;
	}	
}




