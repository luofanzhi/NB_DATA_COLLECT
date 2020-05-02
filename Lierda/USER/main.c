#include "Systerm_Init.h"
#include "stm32f10x_it.h" 
#include "oled.h"
#include "stdlib.h"
#include "delay.h"
#include "beep.h"
#define Scan_min(n) 60000*n				//��ʱɨ���¼�
 int main(void)
{	
	SystemInit();						//ϵͳʱ�ӳ�ʼ��
	SYSTERM_INIT();						//ϵͳ��ʼ��
	OLED_Init();						//OLED��ʼ��
	OLED_ColorTurn(0);					//0������ʾ��1- ��ɫ��ʾ
	OLED_DisplayTurn(0);				//0������ʾ��1-��Ļ��ת��ʾ
	OLED_Clear();						//����
	BLUE_POWER_OPEN();					//��������Դ
	Deveice_Display();					//�豸��Ϣ��ʾ
	BlueConnectStruct.Connect=0;	
	
  while(1)
	{	
		NB_INIT();						//�ϵ��ʼ��NB������EMI�ź��ź�ǿ��
		KEY_Set_Blue_S_Mode();			//�̰����������л��ɴ�ģʽ��APPͨѶ
		APP_To_BlueScan(Scan_min(5));	//APP����ɨ��, ��ʱɨ�赥λ������
		Determine_Deveice();			//APPͨѶ������Ϣ����
		JZQ_Write_NAME_MAC();			//APPд���
		NB_UPDATE_DATA();				//NB�ϴ�������--����ϴ�40������ˮ��
		//Wake_Deal();					//��ʱ���Ѵ���		
		if(BlueConnectStruct.Connect==1)
		{
			if(BLUE_CONNECT_COND==0)
			{
				OLED_Display_GB2312_string(1,6,"�����ȴ�����..."); 
			}
			if(BLUE_CONNECT_COND==1)
			{
				OLED_Display_GB2312_string(1,6,"���������ӣ�����"); 
				BlueConnectStruct.Connect=2;
			}
		}
		else if(BlueConnectStruct.Connect==2)
		{
			if(BLUE_CONNECT_COND==0)
			{
				BlueConnectStruct.Connect=1;
				
			}
		}	
		
						
	}
 }

 
 
 
