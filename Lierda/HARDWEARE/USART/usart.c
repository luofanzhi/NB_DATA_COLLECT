#include "usart.h"
#include "sys.h"
#include "delay.h"
#include "string.h"
#include "NB.h"
#include "led.h"
#include "BlueTooth.h"
#include "Blue_time.h"
#include "SimplePrintf.h"
#include "BlueData_Deal.h"
#include "communication_to_app.h" 
#include "flash.h"
#include "wakeup.h"
#include "oled.h"
u16 StringTime=0;
u8 StringTimeTempt=0;
u8 PrintfFlag;
u8 USART1_RX_Count,USART2_RX_Count,USART3_RX_Count;
char USART1_RX_Buff[255],USART2_RX_Buff[255], USART3_RX_Buff[255];
CommunicationTypedef CommunicationStruct;
BlueDataRceiveMode BlueDataRceiveModeStruct;
void USART1_Init(uint32_t baud)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_Initstcture;
	NVIC_InitTypeDef NVIC_Initstcture;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA| RCC_APB2Periph_AFIO,ENABLE);//ʹ��GPIOA
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//ʹ�ܴ���1
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	
	USART_Initstcture.USART_BaudRate=baud;
	USART_Initstcture.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//��Ӳ��������
	USART_Initstcture.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;//���ͺͽ��ն�ʹ��
	USART_Initstcture.USART_Parity=USART_Parity_No;//����żУ��
	USART_Initstcture.USART_StopBits=USART_StopBits_1;//ֹͣλΪ1λ
	USART_Initstcture.USART_WordLength=USART_WordLength_8b;//����λΪ8λ
	USART_Init(USART1,&USART_Initstcture);
	
	USART_Cmd(USART1,ENABLE);//ʹ���ж�1
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//ʹ�ܴ���1�Ľ����ж�
	
	NVIC_Initstcture.NVIC_IRQChannel=USART1_IRQn;
	NVIC_Initstcture.NVIC_IRQChannelCmd=ENABLE;//ʹ���ж�ͨ��
	NVIC_Initstcture.NVIC_IRQChannelPreemptionPriority=0;//������ռ���ȼ�
	NVIC_Initstcture.NVIC_IRQChannelSubPriority=1;//���ô����ȼ�
	NVIC_Init(&NVIC_Initstcture);
}



void USART2_Init(uint32_t baud)
{	
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
				
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
	
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3 ;//RX
			GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
			GPIO_Init( GPIOA, &GPIO_InitStructure );
													
			USART_InitStructure.USART_BaudRate = baud;  
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init( USART2, &USART_InitStructure );

			USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );//���յ�һ���ֽڵ��ж�
			//USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//���յ�һ֡���ݵ��ж�
			USART_Cmd(USART2, ENABLE);
			
			NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
			NVIC_Init(&NVIC_InitStructure);				
}



void USART3_Init(uint32_t baud)
{	
			GPIO_InitTypeDef GPIO_InitStructure;
			USART_InitTypeDef USART_InitStructure;
			NVIC_InitTypeDef NVIC_InitStructure;
				
		  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO , ENABLE);
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
  
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//TX
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);

			GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11 ;//RX
			GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
			GPIO_Init( GPIOB, &GPIO_InitStructure );
													
			USART_InitStructure.USART_BaudRate = baud;  
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			USART_InitStructure.USART_StopBits = USART_StopBits_1;
			USART_InitStructure.USART_Parity = USART_Parity_No;
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
			USART_Init( USART3, &USART_InitStructure );

			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE );//���յ�һ���ֽڵ��ж�
			USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//���յ�һ֡���ݵ��ж�
			USART_Cmd(USART3, ENABLE);
			
			NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
			NVIC_Init(&NVIC_InitStructure);	
}







void USART1_char_send(char c)
{
		USART_SendData(USART1, c);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		{
			   
		}
}


void USART1_Send_String(char *dat)
{	
	while(*dat)
	{
		USART1_char_send(*dat);
		dat++;
	}
}

void USART2_char_send(char c)
{
		USART_SendData(USART2, c);
		while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
		{
			
		}
}


void USART2_Send_String(char *dat)
{	
		while(*dat)
		{
			USART2_char_send(*dat);
			dat++;
		}
}


void USART3_char_send(char c)
{
		USART_SendData(USART3, c);
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
		{
			
		}
}


void USART3_Send_String(char *dat)
{	
		while(*dat)
		{
			USART3_char_send(*dat);
			dat++;
		}
}

void Uart1_SendLR(void)   //�س���������
{
       USART1_char_send(0x0D);      //�س�  
       USART1_char_send(0x0A);      //����
}

void Uart2_SendLR(void)   //�س���������
{
       USART2_char_send(0x0D);      //�س�
			 //USART2_char_send(0x0A);      //����	
	
}

void Uart3_SendLR(void)   //�س���������
{
       USART3_char_send(0x0D);      //�س�  
       USART3_char_send(0x0A);      //����
}


void Uart1_SendAtCommand(char *cmd)//����ATָ��
{
		if(PrintfFlag)
				{ USART3_Send_String(cmd);//��ӡ����1��������
		    Uart3_SendLR();}//��ӡ����1���ͻس�����	
			delay_ms(200);
			USART1_Send_String(cmd);
			Uart1_SendLR();

}

void Uart2_SendAtCommand(char *cmd)//����ATָ��
{
		
	  if(PrintfFlag)
				{ USART3_Send_String(cmd);//��ӡ����1��������
		    Uart3_SendLR();}//��ӡ����1���ͻس�����
				delay_ms(200);
	  USART2_Send_String(cmd);
		Uart2_SendLR();

}


void Uart3_SendAtCommand(char *cmd)//����ATָ��
{
		USART3_Send_String(cmd);
		Uart3_SendLR();
		//delay_ms(200);
}


// ����һ������
void USART3_Send_Arr(char *dat,uint8_t len)
{
    uint8_t i=0;
    for(i=0;i<len;i++)
    {
        USART3_char_send(*dat);
        dat++;
    }
}

void USART3_Send_xbit(char *dat,uint8_t len,uint8_t q)
{
	uint8_t i=0;
    for(i=0;i<len;i++)
    {
		if(i%q==0)Uart3_SendLR();
        USART3_char_send(*dat);
        dat++;
		
    }
}

	
void USART2_Send_Arr(uint8_t *dat,uint8_t len)
{
    uint8_t i=0;
    for(i=0;i<len;i++)
    {  
			      if(PrintfFlag)USART3_char_send(*dat);
		  	USART2_char_send(*dat);
        dat++;
    }
		delay_ms(300);
}

void USART1_Send_Arr(char *dat,uint8_t len)
{
    uint8_t i=0;
    for(i=0;i<len;i++)
    {
             if(PrintfFlag)USART3_char_send(*dat);
  			USART1_char_send(*dat);
        dat++;
    }
}




/*************������ڻ���*************/
void USart_CLR_RecvBuf(u8 i)
{
//     uint8_t j;
					switch(i)
					{
//						case 1: for(j=0;j<255;j++){USART1_RX_Buff[j] =0;} USART1_RX_Count=0;break;
//						case 2: for(j=0;j<255;j++){USART2_RX_Buff[j] =0;} USART2_RX_Count=0; break;
//						case 3: for(j=0;j<33;j++){USART3_RX_Buff[j] =0;} USART3_RX_Count=0; break;
//						default:break;
						
						case 1:memset(USART1_RX_Buff,0,sizeof(USART1_RX_Buff)); USART1_RX_Count=0;break;//LFZ
						case 2:memset(USART2_RX_Buff,0,sizeof(USART2_RX_Buff)); USART2_RX_Count=0; break;
						case 3:memset(USART3_RX_Buff,0,sizeof(USART3_RX_Buff)); USART3_RX_Count=0; break;
						default:break;	
					}		
}


/***************************�رմ���***************************/
void Close_USART(uint8_t data)//�رմ�������GPIOΪ������������
{
			GPIO_InitTypeDef  GPIO_InitStructure;
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);   
	    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		      //�������ٶ�Ϊ50MHz
	    GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_AIN  ;//����̬ʱ��

	    switch(data)
		  {
						case 1:		USART_Cmd(USART1, DISABLE);
											GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
											GPIO_Init(GPIOA, &GPIO_InitStructure);	
											break;
				
						case 2:		USART_Cmd(USART2, DISABLE);
											GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
											GPIO_Init(GPIOA, &GPIO_InitStructure);	
											break;
						case 3:		USART_Cmd(USART3, DISABLE);
											GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
											GPIO_Init(GPIOB, &GPIO_InitStructure);	
											break;	
						case 4:		USART_Cmd(USART3, DISABLE);USART_Cmd(USART2, DISABLE);USART_Cmd(USART1, DISABLE);
											GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
											GPIO_Init(GPIOB, &GPIO_InitStructure);	
											GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_9|GPIO_Pin_10;
											GPIO_Init(GPIOA, &GPIO_InitStructure);	
											break;	
					default:break;
			}
}


/***************************�򿪴���***************************/
void Open_USART(uint8_t data)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);   

	    switch(data)
		  {
						case 1:		USART_Cmd(USART1, ENABLE);
									GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;//�����������
									GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
									GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
									GPIO_Init(GPIOA,&GPIO_InitStructure);
									
									GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;//��������
									GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
									GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
									GPIO_Init(GPIOA,&GPIO_InitStructure);
											break;
				
						case 2:		USART_Cmd(USART2, ENABLE);
									GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;//TX
									GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
									GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
									GPIO_Init(GPIOA, &GPIO_InitStructure);

									GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_3 ;//RX
									GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
									GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
									GPIO_Init( GPIOA, &GPIO_InitStructure );	
											break;
						case 3:		USART_Cmd(USART3, ENABLE);
									GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//TX
									GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
									GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
									GPIO_Init(GPIOB, &GPIO_InitStructure);

									GPIO_InitStructure.GPIO_Pin =   GPIO_Pin_11 ;//RX
									GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_IN_FLOATING ;
									GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
									GPIO_Init( GPIOB, &GPIO_InitStructure );	
											break;	
					default:break;
			}
}




//��ӳ��fputc�������˺���Ϊ�����������Ļ�������
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
	USART_SendData(USART3, (uint8_t) ch);
	return ch;
}



/*****************�����жϷ�����***************/
void USART1_IRQHandler(void)//����1���жϷ�����
{

		 char TempData;
		char Receive_Data;
	   if(USART_GetFlagStatus(USART1, USART_FLAG_ORE)!= RESET)//���ش������
			{
						USART_ClearFlag(USART1, USART_FLAG_ORE);
						USART_ReceiveData(USART1);
			}
			if(USART_GetITStatus(USART1, USART_IT_RXNE)!= RESET)
			{
				if(NB_Struct.Mode==1)//ATģʽ
				{
						NB_time=30;
						NB_ReceveData_flag=1;
						USART_ClearITPendingBit(USART1, USART_IT_RXNE);
						USART1_RX_Buff[USART1_RX_Count] = USART1->DR;
						   if(PrintfFlag)
							   { TempData = USART_ReceiveData(USART1);  
                   USART_SendData(USART3,TempData); } //����1�յ������ݶ��ɴ���1��ӡ����
						USART1_RX_Count++;
						if(USART1_RX_Count>255)USART1_RX_Count=0;
				}
				if(NB_Struct.Mode==Data_Mode)//����ģʽ
				{
					Receive_Data=USART_ReceiveData(USART1);
					USART_ClearITPendingBit(USART1, USART_IT_RXNE);
					if(NB_Struct.Recevie_Buff_Cont==0)
					{
						NB_Struct.Recevie_Data_Buff[0]=Receive_Data;
						if(NB_Struct.Recevie_Data_Buff[0]=='6')
						{
							NB_Struct.Recevie_Buff_Cont=1;
						}
						else NB_Struct.Recevie_Buff_Cont=0;
					}
					else if(NB_Struct.Recevie_Buff_Cont==1)
					{
						NB_Struct.Recevie_Data_Buff[1]=Receive_Data;
						if(NB_Struct.Recevie_Data_Buff[1]=='8')
						{
							NB_Struct.Recevie_Buff_Cont=2;
						}
					}
					else NB_Struct.Recevie_Data_Buff[NB_Struct.Recevie_Buff_Cont++]=Receive_Data;
					if(NB_Struct.Recevie_Buff_Cont>255)NB_Struct.Recevie_Buff_Cont=0;
					if(PrintfFlag){TempData = USART_ReceiveData(USART1);USART_SendData(USART3,TempData); } //����1�յ������ݶ��ɴ���1��ӡ���� 		
				}					

			}
	
}

void USART2_IRQHandler(void)//����1���жϷ�����
{
	u8 i;
	char TempData;
	uint8_t read_data; 
	uint8_t communication_data;
//	uint8_t tempt_name[130];
//	uint8_t tempt_mac[120];
	char write_00_buf[255];
	 USART_ClearFlag(USART2,USART_FLAG_TC);
	   if(USART_GetFlagStatus(USART2, USART_FLAG_ORE)!= RESET)//���ش������
			{
						USART_ClearFlag(USART2, USART_FLAG_ORE);
						USART_ReceiveData(USART2);
			}
			if(USART_GetITStatus(USART2, USART_IT_RXNE)!= RESET)
			{
						if(BlueDataRceiveModeStruct.Blue_AT_Mode==1)//ATģʽ
						{
								Blue_ReceveData_Flag=1;
								Blue_time=1;
								USART2_RX_Buff[USART2_RX_Count++] = USART2->DR;	
								if(PrintfFlag)
								{TempData = USART_ReceiveData(USART2);  
								USART_SendData(USART3,TempData);} //����1�յ������ݶ��ɴ���3��ӡ����
								if(USART2_RX_Count>=255){USART2_RX_Count=0;}
								USART_ClearITPendingBit(USART2, USART_IT_RXNE);
						}
						else if(BlueDataRceiveModeStruct.Blue_AT_Mode==2)//ɨ��ģʽ
						{
							StringTime++;
							if(StringTime>70&&ConnectBlueStruct.ConnectBlueReceive_AT==1)
							{
								StringTime=0;
								USART2_RX_Count=0;
								StringTimeTempt=1;
								Uart2_SendAtCommand("AT+STOPSCAN");
							}
								USART2_RX_Buff[USART2_RX_Count++] = USART2->DR;	
								if(PrintfFlag)
								{TempData = USART_ReceiveData(USART2);  
								USART_SendData(USART3,TempData);} //����1�յ������ݶ��ɴ���3��ӡ����
								if(USART2_RX_Count>=255){USART2_RX_Count=0;}
								USART_ClearITPendingBit(USART2, USART_IT_RXNE);
						}
						else if(BlueDataRceiveModeStruct.Blue_Data_Mode==1)//��������ģʽ
						{
									read_data=USART_ReceiveData(USART2)&0xff;	
									if(BlueDataRceiveModeStruct.DataMode.BlueDataCount==0)
									{
										BlueDataRceiveModeStruct.DataMode.BlueDataBuff[0]=read_data;
										if(BlueDataRceiveModeStruct.DataMode.BlueDataBuff[0]==0x68)
										{
											BlueDataRceiveModeStruct.DataMode.BlueDataCount=1;
										}
										else BlueDataRceiveModeStruct.DataMode.BlueDataCount=0;
									}
									else if(BlueDataRceiveModeStruct.DataMode.BlueDataCount==1)
									{
										BlueDataRceiveModeStruct.DataMode.BlueDataBuff[1]=read_data;
										if(BlueDataRceiveModeStruct.DataMode.BlueDataBuff[1]==0x10)
										{
											BlueDataRceiveModeStruct.DataMode .BlueDataCount=2;
										}
									}
									else BlueDataRceiveModeStruct.DataMode.BlueDataBuff[BlueDataRceiveModeStruct.DataMode.BlueDataCount++] = read_data;
									if(BlueDataRceiveModeStruct.DataMode.BlueDataBuff[BlueDataRceiveModeStruct.DataMode.BlueDataBuff[10]+12]==0x16)
									{
										ConnectBlueStruct.ReadBlueData_Success =1;
										if(ConnectBlueStruct.ReadBlueData_Success)
										{
											memset(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff,0,sizeof(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff));
											HEXArrayToStringArray(BlueDataRceiveModeStruct.DataMode.BlueDataBuff,BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff,59);
											BlueDataRceiveModeStruct.DataMode.BlueDataCount=0;
										}
									}
									if(BlueDataRceiveModeStruct.DataMode.BlueDataCount>=255)
									{
										BlueDataRceiveModeStruct.DataMode.BlueDataCount=0;
									}
									
									if(PrintfFlag)//����2�յ������ݶ��ɴ���3��ӡ����
									{
										read_data = USART_ReceiveData(USART2);  
										USART_SendData(USART3,read_data);
									} 
								
								USART_ClearITPendingBit(USART2, USART_IT_RXNE);
						}
						else if(BlueDataRceiveModeStruct.Blue_To_APP)//��APPͨѶģʽ
						{	
							
							communication_data=USART_ReceiveData(USART2)&0xff;
							if(CommunicationStruct.Communication_RX_Cont==0)
							{
								CommunicationStruct.CommunicationBuff[0]=communication_data;
								if(CommunicationStruct.CommunicationBuff[0]==0x68)
								{
									CommunicationStruct.Receive_Time=500;//������Ϣ500MS��ʱ���ش���
									CommunicationStruct.Communication_RX_Cont=1;
								}
								else
								{
									CommunicationStruct.Communication_RX_Cont=0;
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
								}									
							}
							else if(CommunicationStruct.Communication_RX_Cont==1)
							{
								CommunicationStruct.CommunicationBuff[1]=communication_data;
								if(CommunicationStruct.CommunicationBuff[1]==0x20)
								{
									CommunicationStruct.Communication_RX_Cont=2;
								}
							}
							else CommunicationStruct.CommunicationBuff[CommunicationStruct.Communication_RX_Cont++]=communication_data;
							if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+12]==0x16)
							{
								CommunicationStruct.Receive_Over_Time =0;
								CommunicationStruct.Receive_Time =0;
								OLEDStruct.oled_clear=1;
								if(CommunicationStruct.CommunicationBuff[9]==0x32&&CommunicationStruct.CommunicationBuff[12]==0x2f&&CommunicationStruct.CommunicationBuff[14]==0xff)
								{
									//68 20 AA AA AA AA AA AA AA 32 06 90 2F 00 FF 00 00 22 16
									CommunicationStruct.Communication_RX_Cont=0;
									Read_Device_information();//�����豸��Ϣ
									OLED_Clear();
									OLED_Display_GB2312_string(1,0,"��ȡ�豸��Ϣ�ɹ�"); 
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x33&&CommunicationStruct.CommunicationBuff[12]==0x3f&&CommunicationStruct.CommunicationBuff[14]==0xee)
								{
									CommunicationStruct.Communication_RX_Cont=0;
									Scan_Answer();//����ɨ�践��
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									CommunicationStruct.Start=1;//��ʼɨ���־
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x34&&CommunicationStruct.CommunicationBuff[12]==0x4f)
								{
									if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x01)
									{	
										CommunicationStruct.write_ser=0x01;
										OLED_Clear();
										OLED_Display_GB2312_string(1,0,"д���1~10�ɹ�"); 	
									}
									if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x02)
									{
										CommunicationStruct.write_ser=0x02;
										OLED_Clear();
										OLED_Display_GB2312_string(1,0,"д���11~20�ɹ�"); 
									}
									if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x03)
									{
										CommunicationStruct.write_ser=0x03;
										OLED_Clear();
										OLED_Display_GB2312_string(1,0,"д���21~30�ɹ�"); 
									}
									if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x04)
									{
										CommunicationStruct.write_ser=0x04;
										OLED_Clear();
										OLED_Display_GB2312_string(1,0,"д���31~40�ɹ�"); 
									}
									if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x05)
									{
										CommunicationStruct.Communication_RX_Cont=0;
										memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
										Error_Return();
									}
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x35&&CommunicationStruct.CommunicationBuff[12]==0x5f&&CommunicationStruct.CommunicationBuff[14]==0xdd)
								{
									//68 20 AA AA AA AA AA AA AA 35 06 90 5F 00 DD 00 00 CS 16
									if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x01)
									{
										Read_name_mac(0x01);//���ؼ������洢�ı�ź�MAC-----SER=0X01
										CommunicationStruct.Communication_RX_Cont=0;
										memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
										OLED_Clear();
										OLED_Display_GB2312_string(1,0,"����ųɹ���"); 
									}
									else if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x02)
									{
										Read_name_mac(0x02);//���ؼ������洢�ı�ź�MAC-----SER=0X02
										CommunicationStruct.Communication_RX_Cont=0;
										memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									}
									else if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x03)
									{
										Read_name_mac(0x03);//���ؼ������洢�ı�ź�MAC-----SER=0X03
										CommunicationStruct.Communication_RX_Cont=0;
										memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									}
									else if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x04)
									{
										Read_name_mac(0x04);//���ؼ������洢�ı�ź�MAC-----SER=0X04
										CommunicationStruct.Communication_RX_Cont=0;
										memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									}
									else if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+10]==0x05)
									{
										Read_name_mac(0x05);//���ش������40ֻ����ˮ��
										CommunicationStruct.Communication_RX_Cont=0;
										memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									}
									
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x15&&CommunicationStruct.CommunicationBuff[11]==0xb0&&CommunicationStruct.CommunicationBuff[12]==0x19)
								{
									for(i=0;i<20;i++){CommunicationStruct.IP_COM_u8[i]=CommunicationStruct.CommunicationBuff[i+15];}
									STMFLASH_Write(FLASH_SAVE_ADDR+2048,(u16*)CommunicationStruct.IP_COM_u8,20);
									Set_JZQ_IP_Return();//���ü�����IP���˿ڷ���
									CommunicationStruct.Communication_RX_Cont=0;
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									OLED_Clear();
									OLED_Display_GB2312_string(1,0,"����IP�Ͷ˿ڳɹ�"); 
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x03&&CommunicationStruct.CommunicationBuff[11]==0x91&&CommunicationStruct.CommunicationBuff[12]==0x0b)
								{
									//68 20 AA AA AA AA AA AA AA 03 03 91 0B 00 D0 16
									Read_JZQ_IP_COM();//��ȡ������IP���˿ڷ���
									CommunicationStruct.Communication_RX_Cont=0;
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									OLED_Clear();
									OLED_Display_GB2312_string(1,0,"��ȡIP�Ͷ˿ڳɹ�"); 
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x26&&CommunicationStruct.CommunicationBuff[11]==0xaa&&CommunicationStruct.CommunicationBuff[12]==0xbb)
								{
									//68 20 AA AA AA AA AA AA AA 26 03 AA BB 00 BC 16
									for(i=0;i<255;i++){write_00_buf[i]='0';}
									STMFLASH_Write(SAVE_ADDR_NAME_1,(u16*)write_00_buf,142);
									STMFLASH_Write(SAVE_ADDR_MAC_1,(u16*)write_00_buf,122);
									STMFLASH_Write(SAVE_ADDR_NAME_2,(u16*)write_00_buf,142);
									STMFLASH_Write(SAVE_ADDR_MAC_2,(u16*)write_00_buf,122);
									STMFLASH_Write(SAVE_ADDR_NAME_3,(u16*)write_00_buf,142);
									STMFLASH_Write(SAVE_ADDR_MAC_3,(u16*)write_00_buf,122);
									STMFLASH_Write(SAVE_ADDR_NAME_4,(u16*)write_00_buf,142);
									STMFLASH_Write(SAVE_ADDR_MAC_4,(u16*)write_00_buf,122);
									Clear_NAME_MAC();//���ڵ㷵��
									CommunicationStruct.Communication_RX_Cont=0;
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									OLED_Clear();
									OLED_Display_GB2312_string(1,0,"���ڵ�ɹ�"); 
								}
								else if(CommunicationStruct.CommunicationBuff[9]==0x36&&CommunicationStruct.CommunicationBuff[11]==0xcc&&CommunicationStruct.CommunicationBuff[12]==0xbb)
								{
									CommunicationStruct.APP_To_JQZ_CB_Flag=1;
//									NB_Power_Control=1;// 1-����NB��Դ��0-�ر�NB��Դ
//									NB_Struct.Mode =AT_Mode;
//									Work_Struct.Mode_type.Weakup.WeakUP_Time=0;
									CommunicationStruct.JQZ_CB_UPDATE=1;
									APP_TO_JZQ_CB_RETURN();//����������
									CommunicationStruct.Communication_RX_Cont=0;
									memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
									OLED_Clear();
									OLED_Display_GB2312_string(1,0,"���ڳ���..."); 
								}
							}
							if(CommunicationStruct.Communication_RX_Cont>=255)CommunicationStruct.Communication_RX_Cont=0;
								
						}
					if(PrintfFlag)//����2�յ������ݶ��ɴ���3��ӡ����
								{
									communication_data = USART_ReceiveData(USART2);  
									USART_SendData(USART3,communication_data);
								} 	
								USART_ClearITPendingBit(USART2, USART_IT_RXNE);
			}	
}



void USART3_IRQHandler(void)//����1���жϷ�����
{
	char aa; 
	if(USART_GetFlagStatus(USART3, USART_FLAG_ORE)!= RESET)//���ش������
	{
		USART_ClearFlag(USART3, USART_FLAG_ORE);
		USART_ReceiveData(USART3);			
	}	
	if(USART_GetITStatus(USART3, USART_IT_RXNE)!= RESET)
	{
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
		USART3_RX_Buff[USART3_RX_Count++] = USART3->DR;	
		if(USART3_RX_Count>=255){USART3_RX_Count=0;}
		
		if(PrintfFlag)
		{aa = USART_ReceiveData(USART3);  
		USART_SendData(USART3,aa);
		} //����1�յ������ݶ��ɴ���3��ӡ����
	}
						
}



