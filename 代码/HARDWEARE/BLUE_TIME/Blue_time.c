#include "Blue_time.h"
#include "BlueTooth.h"
#include "usart.h"
#include "BlueData_Deal.h"
#include "communication_to_app.h" 
#include "SleepMode.h"
#include "wakeup.h"
//ͨ�ö�ʱ��4�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ48M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM4��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM���¼���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	TIM_Cmd(TIM4, ENABLE);  //
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
					 
}


void OPEN_TIME4(void)
{
	TIM_Cmd(TIM4, ENABLE);  //ʹ��TIM4
}

void CLOSE_TIME4(void)
{
	TIM_Cmd(TIM4, DISABLE);  //ʧ��TIM4	
}



//��ʱ��4�жϷ������
void TIM4_IRQHandler(void)   //TIM4�ж�
{
		if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)  //���TIM4�����жϷ������
		{
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־
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







