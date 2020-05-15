#include "NB_time.h"
#include "sys.h"
#include "NB.h"
#include "led.h"
#include "BlueTooth.h"
#include "BlueData_Deal.h"
#include "Blue_time.h"
#include "BlueTooth.h"

//ͨ�ö�ʱ��3�жϳ�ʼ��
//����ʱ��ѡ��ΪAPB1��2������APB1Ϊ36M
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//����ʹ�õ��Ƕ�ʱ��3!
void TIM3_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;  //TIM���¼���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx					 
}
//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
			
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



