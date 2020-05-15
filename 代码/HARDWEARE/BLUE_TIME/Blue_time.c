#include "Blue_time.h"
#include "BlueTooth.h"
#include "usart.h"
#include "BlueData_Deal.h"
#include "communication_to_app.h" 
#include "SleepMode.h"
#include "wakeup.h"
//通用定时器4中断初始化
//这里时钟选择为APB1的2倍，而APB1为48M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器4!
void TIM4_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //时钟使能
	
	//定时器TIM4初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向下计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
	TIM_Cmd(TIM4, ENABLE);  //
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //使能指定的TIM4中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器
					 
}


void OPEN_TIME4(void)
{
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}

void CLOSE_TIME4(void)
{
	TIM_Cmd(TIM4, DISABLE);  //失能TIM4	
}



//定时器4中断服务程序
void TIM4_IRQHandler(void)   //TIM4中断
{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //检查TIM4更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //清除TIMx更新中断标志
			if(ConnectBlueStruct.ReadData_Time>0)
			{
				ConnectBlueStruct.ReadData_Time--;
				if(ConnectBlueStruct.ReadData_Time==0)
				{
					ConnectBlueStruct.ReadBlueData_fail=1;
				}
			}
			if(ConnectBlueStruct.ScanTime>0)
			{
				ConnectBlueStruct.ScanTime--;
				if(ConnectBlueStruct.ScanTime==0)
				{
					ConnectBlueStruct.Scan_OverTime_Flag=1;
				}
			}
			if(Sleep_Mode_Struct.Enter_SleepMode_Time>0)
			{
				Sleep_Mode_Struct.Enter_SleepMode_Time--;
				if(Sleep_Mode_Struct.Enter_SleepMode_Time==0)
				{
					Sleep_Mode_Struct.Enter_SleepMode_Flag=1;
				}
			}
			if(CommunicationStruct.Receive_Time>0)
			{
				CommunicationStruct.Receive_Time--;
				if(CommunicationStruct.Receive_Time ==0)
				{
					CommunicationStruct.Receive_Over_Time=1;
				}
			}
			if(Work_Struct.Mode_type.Blue_S.Con_Time>0)
			{
				Work_Struct.Mode_type.Blue_S.Con_Time--;
				if(Work_Struct.Mode_type.Blue_S.Con_Time==0)
				{
					Work_Struct.Mode_type.Blue_S.Con_over_Time=1;	
				}
			}
			
		}		
		
}







