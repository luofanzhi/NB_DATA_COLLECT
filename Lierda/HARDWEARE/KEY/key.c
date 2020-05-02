#include "key.h"
#include "sys.h"
#include "delay.h"
#include "BlueData_Deal.h"
#include "usart.h"
#include "led.h"
#include "BlueTooth.h"
#define StartScan 1	

KEYTypedef KEYStruct;
void KEY_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);        //  led4
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
}

u8 KEY_Scan(u8 mode)//mode-1-支持连按
{
	static u8 key_up=1;//按键松开标志
	if(mode)key_up=1;//支持连按
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0))
	{
		delay_ms(5);
		key_up=0;
		if(KEY1==0)return 1;
		else if(KEY2==0)return 2;
		else if(KEY3==0)return 3;
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1)key_up=1;
	return 0;//无按键按下
}

void KEY_SCAN(uint8_t Jishi_Time)//计时单位为：分钟
{
	if(KEY_Scan(1)==2)
	{
		KEYStruct.KEY_MODE.short_key=1;
		KEYStruct.KEY_Time++;
		if(KEYStruct.KEY_Time>50)
		{
			BLUE_POWER_OPEN();
			BLUE_EN_ENABLE();
			BlueDataRceiveModeStruct.Blue_AT_Mode=2;
			ConnectBlueStruct.ConnectBlueReceive_AT=1;;//串口接收切换成扫描模式
			ConnectBlueStruct.ScanTime=Jishi_Time*60000;
			KEYStruct.KEY_MODE.long_key=1;
			Uart2_SendAtCommand("AT+ROLE=M");
			delay_ms(200);
			Uart2_SendAtCommand("AT+SCAN:RSSI=-80");
			KEYStruct.KEY_Time=600;
		}
		else 
		{
			KEYStruct.KEY_MODE.short_key=1;

		}
		
	} 
	 if(StartScan==1&&KEYStruct.KEY_MODE.long_key==1)
		{
			LED1=0;
			Take_Out_NAME_MAC(80);
			if(ConnectBlueStruct.StopScan)
			{
				KEYStruct.KEY2_Flag=0;
				ConnectBlueStruct.StopScan=0;
				USart_CLR_RecvBuf(2);
				#undef StartScan
			}
		}	
}
