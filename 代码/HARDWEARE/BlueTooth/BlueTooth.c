#include "BlueTooth.h"
#include <string.h>
#include "usart.h"
#include "SimplePrintf.h"
#include "stdio.h"
#include "stdlib.h"
#include "Blue_time.h"
#include "BlueData_Deal.h"
#include "delay.h"
#include "communication_to_app.h" 
#include "key.h" 
#include "wakeup.h"
#include "oled.h"
u16 Blue_time;
u8 Blue_SendData_Flag;
u8 Blue_ReceveData_Flag;
u8 Blue_AT_num;
u8 Blue_ReceveData_fail_Flag;
char CONNECT_MAC[12];
uint8_t CONNECT_NAME[14];
uint8_t	CONNECT_NAME_uint[7];
char Tuff[100];
uint8_t BLE_TO_NB_Data[100];
BlueConnect BlueConnectStruct;
void BLUE_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);        //  led4
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_1);//�����ĵ�Դ��������
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_5|GPIO_Pin_7;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
	GPIO_ResetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_3);	//PC1Ϊ������ʹ������***PC3Ϊ�����ĸ�λ����
	
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_4;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
	//����������������/****���յ�����ʱΪ�ߵ�ƽ��������ʱΪ�͵�ƽ*****/
	
		GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6;                    //
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;               //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;          //
    GPIO_Init(GPIOA,&GPIO_InitStructure); 
	//����ָʾ��*****/	
}


void BLUE_POWER_OPEN(void)//����������Դ
{
	BLUE_POWER=1;
}
void BLUE_POWER_CLOSE(void)//�ر�������Դ
{
	BLUE_POWER=0;
}

void BLUE_EN_ENABLE(void)//����ʹ��
{
	BLUE_EN=1;
}

void BLUE_EN_DISABLE(void)//����ʧ��
{
	BLUE_EN=0;
}

void BLUE_RST_ENABLE(void)//������λ
{
	BLUE_RST=0;
}

void BLUE_RST_DISABLE(void)//��������λ
{
	BLUE_RST=1;
}

void Blue_Flag_Init(void)
{
	Blue_AT_num =0;
	Blue_ReceveData_Flag =1;
	Blue_SendData_Flag =1;
	Blue_time =0;
}


void Blue_S_AT_Config(void)//��ģʽ��AT����
{
	if(Blue_SendData_Flag ==1)
	{
		Blue_SendData_Flag=0;
		switch (Blue_AT_num)
		{
			case 0:		OLED_Clear();
						OLED_Display_GB2312_string(1,0,"������������..."); 
						BLUE_EN_ENABLE();
						raw_printf("��ǰ����Ϊ��ģʽ:");
						Blue_time =50;
				break;
			case 1:		
						USart_CLR_RecvBuf(2);
						Uart2_SendAtCommand("AT+DEFAULT");//�ָ���������
						Blue_time =1;
				break;
			case 2:		if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand("AT+ROLE=S");//����Ϊ��ģʽ
							Blue_time =10;
						}
						else 
						{
							USart_CLR_RecvBuf(2);
						Uart2_SendAtCommand("AT+DEFAULT");//�ָ���������
							Blue_AT_num=1;
							Blue_time =10;
						}
				break;
			case 3:		if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand("AT+BAUD=115200");//���ò�����
							Blue_time =10;
						}
				break;
			case 4:		if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand("AT+NAME=NB_BLUETOOTH��1");//������������
							Blue_time =10;
						}
				break;
			case 5:		if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand("AT+RFPM=5");//�����������͹���
							Blue_time =10;
						}
				break;
			case 6:		if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand("AT+MODE=0");//����ģʽ
							Blue_time =10;
						}
				break;	

			case 7:		if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand("AT+RX");//��ѯ����ģ�����
							Blue_time =10;
							BlueDataRceiveModeStruct.Blue_AT_Mode=0;
							ConnectBlueStruct.ConnectBlueReceive_AT=0;
							BlueDataRceiveModeStruct.Blue_To_APP=1;
							Work_Struct.Mode_type.Blue_S.Con_Time=240000L;
						}
				break;	
			case 8:		
						Blue_Flag_Init();
						KEYStruct.KEY_Time=0;
						CommunicationStruct.S_Mode_Start =0;
						KEYStruct.KEY_S_Mode_Time=0;
						LEDn_ON(1);
						OLED_Clear();
						OLED_Display_GB2312_string(1,0,"������������"); 
						OLED_Display_GB2312_string(1,2,"�������ƣ�"); 
						OLED_Display_GB2312_string(1,4,"NB_BLUETOOTH��1"); 
						BlueConnectStruct.Connect=1;
				break;
			default:	Blue_time=0;
						Blue_AT_num=0;
			break;
			
			
		}

	}
}








//����ת�ַ���
void itoi(char *str, unsigned long num)
{
    char index[]="0123456789";
    char i = 0, j = 0,temp;
    do
    {
        str[i++] = index[num%10];
        num /= 10;
    }while(num);

    str[i]='\0';
    for(j=0; j<=(i-1)/2; j++)
    {
        temp=str[j];
        str[j]=str[i-j-1];
        str[i-j-1]=temp;
    }
}











void BlueTooth_S_AT_Send(void)
{
	if(Blue_SendData_Flag&&Blue_ReceveData_Flag)
	{
			Blue_ReceveData_Flag=0;
			Blue_S_AT_Config();
	}	
}

void BLE_StopScan_Set(void)//ֹͣɨ��
{
//	USart_CLR_RecvBuf(2);
	Uart2_SendAtCommand("AT+STOPSCAN");//
}

void BLE_CLRBOND(void)
{
	USart_CLR_RecvBuf(2);
	Uart2_SendAtCommand("AT+CLRBOND");//ȡ����
}


u8 CheckDataSmae(char input[12],char compare[12])
{
     u8 i=0,j=0,temp;
	 temp =12;
	 if(temp>1)
	 {
			for(i=0;i<12;i++)
			{
			    if(input[i] !=compare[i])
					{
								j=1;break;
					}
			}
	  }
		 return j;
}












