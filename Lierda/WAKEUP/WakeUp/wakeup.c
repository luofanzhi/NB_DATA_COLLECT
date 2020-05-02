#include "wakeup.h"
Work_Typedef Work_Struct;
void WKUP_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3 ;//RX
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init( GPIOA, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//TX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11 ;//RX
	GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init( GPIOB, &GPIO_InitStructure );
	
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_SetBits(GPIOB,GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);        //  led4
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//蓝牙的电源控制引脚
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1|GPIO_Pin_3;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
	GPIO_ResetBits(GPIOC,GPIO_Pin_1|GPIO_Pin_3);	//PC1为蓝牙的使能引脚***PC3为蓝牙的复位引脚
	
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
	//蓝牙接收数据引脚/****接收到数据时为高电平，无数据时为低电平*****/
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组
	USART1_Init(9600);    //NB的串口
	USART2_Init(115200); //蓝牙串口
	USART3_Init(115200); //DEBUG串口
	TIM3_Init(99,4799);//10K的计数频率，重装值为100
	TIM4_Init(99,479);//1ms
	delay_init();
	LED_GPIO_Init();
	KEY_GPIO_Init();
	NB_GPIO_Init();
	BLUE_GPIO_Init();
	LED_Init(); 
	NB_ALL_Flag_Init();
}

void weak_BLUE_init(void)
{
	//SystemInit();
	BLUE_GPIO_Init();
	USART2_Init(115200); //蓝牙串口
	USART3_Init(115200); //DEBUG串口
	LED_GPIO_Init();
	KEY_GPIO_Init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //中断优先级分组
	TIM3_Init(99,4799);//10K的计数频率，重装值为100
	TIM4_Init(99,479);//1ms
	
}
void weak_NB_init(void)
{
	BLUE_GPIO_Init();
	BLUE_POWER_OPEN();
	BLUE_EN_ENABLE();	
	NB_GPIO_Init();
	USART2_Init(115200); //蓝牙串口
	USART1_Init(9600);    //NB的串口
	USART3_Init(115200); //DEBUG串口
//	TIM3_Init(99,4799);//10K的计数频率，重装值为100
//	TIM4_Init(99,479);//1ms
}

void Wake_Deal(void)
{
	if(Sleep_Mode_Struct.WeakUP)//如果被唤醒
	{
		LED3=0;
		if(Work_Struct.Mode_type.Sleep.Sleep_Time==800)
		{
			weak_NB_init();
			Work_Struct.Mode_type.Sleep.Sleep_Time=0;
			CommunicationStruct.APP_To_JQZ_CB_Flag=0x01;
			Work_Struct.Mode_type.Weakup.WeakUP_Time=0;
			Work_Struct.Mode_type.JQZ_Mode.Update=0XFF;
		}
		if(Work_Struct.Mode_type.Sleep.Sleep_Time<800&&CommunicationStruct.APP_To_JQZ_CB_Flag==0)
		{
			Work_Struct.Mode_type.Weakup.WeakUP_Time=1;
		}
		if(Work_Struct.Mode_type.Weakup.Set_ARM_Time)
		{
			Work_Struct.Mode_type.Sleep.Sleep_Time++;
			Work_Struct.Mode_type.Weakup.Set_ARM_Time=0;
		}
		if(Work_Struct.Mode_type.Weakup.WeakUP_Over_Time==1)
		{
			Work_Struct.Mode_type.Weakup.WeakUP_Over_Time=0;
			Work_Struct.Mode_type.Sleep_Mode=1;
		}
		KEY_Set_Blue_S_Mode();
	}
	if((Work_Struct.Mode_type.Sleep_Mode==1||Work_Struct.Mode_type.Blue_S.Con_over_Time==1\
		||NB_Struct.NB_Init==1)&&CommunicationStruct.APP_To_JQZ_CB_Flag!=1)
	{
		LED3=1;
		RTC_Alarm_Set(calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec+=1);//设置闹钟时间	
		PWR_BackupAccessCmd(ENABLE);
	//		RTC_ITConfig(RTC_IT_SEC, DISABLE);
		NB_Power_Control=0;
		BLUE_POWER_CLOSE();
		KEYStruct.KEY_Time=0;
		NB_Struct.NB_Init=0;
		Work_Struct.Mode_type.Weakup.WeakUP_Time=0;
		Sleep_Mode_Struct.WeakUP=0;
		Work_Struct.Mode_type.JQZ_Mode.Update=0;
		Work_Struct.Mode_type.Sleep_Mode=0;
		Work_Struct.Mode_type.Blue_S.Con_over_Time=0;
		Sys_Enter_SleepMode();
	}
}
