#include "NB_time.h"
#include "sys.h"
#include "NB.h"
#include "led.h"
#include "BlueTooth.h"
#include "BlueData_Deal.h"
#include "Blue_time.h"
#include "BlueTooth.h"

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM向下计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
			
			if(NB_time>0)
			{
				NB_time--;
				if(NB_time==0)
				{
					NB_AT_Num++;
					//LED1=!LED1;
					NB_SendData_flag=1;
					NB_ReceveData_flag=1;
				}
			}
			 if(NB_Close_Power_time>0)
			{
				NB_Close_Power_time--;
				if(NB_Close_Power_time==0)
				{
					NB_Power_Control=0;           
					LEDn_OFF(1);
					LEDn_OFF(2);
					LEDn_OFF(3);
				}
			}
			 if(Blue_time >0)
			{
				Blue_time--;
				if(Blue_time ==0)
				{
					Blue_AT_num ++;
					Blue_ReceveData_Flag=1;
					Blue_SendData_Flag =1;
				}
			}
			if(ConnectBlueStruct.Connect_Time>0)
			{
				ConnectBlueStruct.Connect_Time--;
				if(ConnectBlueStruct.Connect_Time==0)
				{
					ConnectBlueStruct.Connect_OverTime_Flag=1;
				}
			}

		}
}



