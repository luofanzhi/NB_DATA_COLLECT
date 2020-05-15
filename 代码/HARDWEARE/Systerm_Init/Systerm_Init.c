#include "Systerm_Init.h"
#include "beep.h"
#include "oled.h"
 void SYSTERM_INIT(void)
{
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //�ж����ȼ�����
		USART1_Init(9600);    //NB�Ĵ���
		USART2_Init(115200); //��������
		USART3_Init(115200); //DEBUG����
		TIM3_Init(99,4799);//10K�ļ���Ƶ�ʣ���װֵΪ100
		TIM4_Init(99,479);//1ms
		delay_init();
		LED_GPIO_Init();
		KEY_GPIO_Init();
		NB_GPIO_Init();
		BLUE_GPIO_Init();
		LED_Init(); 
		NB_ALL_Flag_Init();
		//IWDG_Init(6,28125);//�������Ź���ʼ��-Ԥ��Ƶϵ��Ϊ64������ֵΪ625�����ʱ��Ϊ1S	//(4*2^5)*31250/40 ms	
		RTC_Init();
		RTC_NVIC_Config();
		ConnectBlueStruct.Connect_quality=20;//���ӱ�˵Ľ����
		Read_NAME_AND_NAME();//��ȡ����ı�ź�MAC��ַ
		BEEP_GPIO_Init();
		PrintfFlag=1;
		ReceivMAC_Flag=0;
}

void LED_Init(void)
{
		LEDn_ON(4);
		delay_ms(1000);
		LEDn_OFF(4);
}

void NB_ALL_Flag_Init(void)
{
		
		NB_Flag_Init();
		Blue_Flag_Init();
		BlueConnectStruct.BlueConnect_time=0;
		BlueDataRceiveModeStruct.DataMode.Start_NB=0;
		NB_AT_Num=0;
		BlueDataRceiveModeStruct.Blue_AT_Mode=0;
		BlueDataRceiveModeStruct.Blue_Data_Mode=0;
		ConnectBlueStruct.Connect_Number=0;
		ConnectBlueStruct.ReadBlueData_Success=0;
		BlueDataRceiveModeStruct.DataMode.BlueDataCount=0;
		ConnectBlueStruct.StopScan=0;
		ConnectBlueStruct.Connect_OverTime_Flag=0;
		ConnectBlueStruct.ReadBlueData_fail=0;
		FLASH_Struct.Write_Success=0;
		k=0;
		Sleep_Mode_Struct.Enter_SleepMode_Flag=0;
		Sleep_Mode_Struct.WeakUP=0;
		BlueDataRceiveModeStruct.Blue_To_APP=0;
		CommunicationStruct.Communication_RX_Cont=0;
		CommunicationStruct.APP_To_JQZ_CB_Flag=0;
		NB_Struct.NB_Init=0;
		Work_Struct.Mode_type.JQZ_Mode.Update=0;
		Work_Struct.Mode_type.Weakup.Set_ARM_Time=0;
		Work_Struct.Mode_type.Weakup.WeakUP_Over_Time=0;
		Work_Struct.Mode_type.Sleep_Mode=0;
		Work_Struct.Mode_type.Blue_S.Con_over_Time=0;	
		CommunicationStruct.APP_To_JQZ_CB_Flag=0;
		CommunicationStruct.JQZ_CB_UPDATE=0;
		NB_Struct.NB_1=1;
		NB_Struct.Net_time=0;
}

void Deveice_Display(void)
{
	OLED_Display_GB2312_string(1,0,"������ҵ��ѧ");	
	OLED_Display_GB2312_string(1,2,"���̼���ѧԺ"); 
	OLED_Display_GB2312_string(1,4,"�������޷�֧");	
	OLED_Display_GB2312_string(1,6,"ѧ��201610213116"); 
	delay_s(2);	
	OLED_Clear();
	OLED_Display_GB2312_string(1,0,"��ҵ�����Ŀ��");	
	OLED_Display_GB2312_string(1,2,"����������NBģ��"); 
	OLED_Display_GB2312_string(1,4,"�������ռ���"); 
	OLED_Display_GB2312_string(1,6,"ָ����ʦ����Ⱥ"); 
	delay_s(2);	
	OLED_Clear();
	OLED_Display_GB2312_string(1,0,"ϵͳ��ʼ��..."); 
	BEEP_ON(1);
}


