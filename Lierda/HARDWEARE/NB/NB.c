#include "NB.h"
#include "usart.h"
#include "string.h"
#include "led.h"
#include "BlueData_Deal.h"
#include "SimplePrintf.h"
#include "delay.h"
#include "communication_to_app.h" 
#include "flash.h"
#include "wakeup.h"
#include "oled.h"
#include "beep.h"
u16 NB_time;
u8 NB_SendData_flag;
u8 NB_ReceveData_flag;
u8 NB_AT_Num;
u8 NB_ReceveData_Failed_Flag;
u16 NB_Close_Power_time;

NB_Typedef NB_Struct;

char Device_information_char_Buff[50];
uint8_t Device_information_u8_Buff[50];

/********************************NB模组IO口初始化*********************************/
void NB_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_8|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed =	GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_12);
	//GPIO_SetBits
	//GPIO_ResetBits
}
void NB_NO_Rest()
{
	NB_Switch=0;
}		

void NB_Rest()
{
	NB_Switch=1;
}

#include "time.h"
u8 CurrentTim[6];//年，月，日，时，分。各一个字节
struct tm GTM_Time;

void GTM_ConversionUTC(uint16_t Count)
{
      static u32 TimSec;
	    GTM_Time.tm_year    = 2000+CurrentTim[0]-1900;//年
			GTM_Time.tm_mon     =  CurrentTim[1]-1;//月
			GTM_Time.tm_mday		=  CurrentTim[2];//日
			GTM_Time.tm_hour		=  CurrentTim[3];//时
			GTM_Time.tm_min			=  CurrentTim[4];//分
			GTM_Time.tm_sec			=  CurrentTim[5];//秒
			 TimSec =  mktime(&GTM_Time); 
			 TimSec += Count;
			 localtime_r(&TimSec, &GTM_Time);
			 
		CurrentTim[0]	= GTM_Time.tm_year+1900-2000 ;//年
		CurrentTim[1]=	GTM_Time.tm_mon+1  ;//月
		CurrentTim[2]=	GTM_Time.tm_mday		 ;//日
		CurrentTim[3]	= GTM_Time.tm_hour		  ;//时
		CurrentTim[4]=	GTM_Time.tm_min			  ;//分
		CurrentTim[5]	= GTM_Time.tm_sec			  ;//秒
}









/************************NB上传数据函数*********************************/
void NB_Update_Data(void)
{
	char cmd[129]; 
	strcpy(cmd, "AT+NMGS=59,"); 
	strcat(cmd,BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff);
	memset(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff,0,sizeof(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff));
	memset(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff,0,sizeof(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff));
	Uart1_SendAtCommand(cmd);	
	
}


void NB_Flag_Init(void)
{
		NB_AT_Num =0;
		NB_SendData_flag =1;
		NB_ReceveData_flag=1;
		NB_Close_Power_time=0;
}
uint8_t NB_Bard_AT_Send(u8 a)
{
	uint8_t m,n;
	static char temp[12];
	static char i;
	static char IP[40];
	STMFLASH_Read(FLASH_SAVE_ADDR+2048,(u16*)CommunicationStruct.IP_COM_u8,20);
	HEXArrayToStringArray((u8*)CommunicationStruct.IP_COM_u8,CommunicationStruct.IP_COM_char,20);
	StringArrayToHEXArray(CommunicationStruct.IP_COM_char,CommunicationStruct.IP_COM_string,40);
	strcpy(IP,"AT+NCDP=");
	for(i=0;i<15;i++){IP[i+8]=CommunicationStruct.IP_COM_string[i+5];}
	IP[23]=CommunicationStruct.IP_COM_string[4];
	IP[24]=CommunicationStruct.IP_COM_string[0];
	IP[25]=CommunicationStruct.IP_COM_string[1];
	IP[26]=CommunicationStruct.IP_COM_string[2];
	IP[27]=CommunicationStruct.IP_COM_string[3];
	if(NB_SendData_flag)
	{
		NB_SendData_flag=0;
		switch(NB_AT_Num)
		{
			
			case 0:		NB_Power_Control=1;// 1-开启NB电源，0-关闭NB电源
						//NB_Rest();
						NB_time=50;
				break;
			case 1:		USart_CLR_RecvBuf(1);
						Uart1_SendAtCommand("AT");//测试AT命令
						NB_time=50;
				break;
						
			case 2:		
						if(strstr(USART1_RX_Buff,"")!=NULL)
						{
							USart_CLR_RecvBuf(1);
							NB_ReceveData_Failed_Flag=0;
							Uart1_SendAtCommand("AT+NCDP=180.101.147.115,5683");//设置IoT平台IP地址
							//Uart1_SendAtCommand(IP);//设置IoT平台IP地址
							NB_time=50;
						}
				break;
			
			case 3:		
						if(strstr(USART1_RX_Buff,"")!=NULL)
						{
							USart_CLR_RecvBuf(1);
							Uart1_SendAtCommand("AT+CGMR");//查询固件版本
							NB_time=50;
						}
				break;
			
			case 4:		if(strstr(USART1_RX_Buff,"")!=NULL)//SECURITY
						{
							USart_CLR_RecvBuf(1);
							Uart1_SendAtCommand("AT+CGSN=1");//查询IMEI号
							NB_time=100;
						}
				break;
			
			case 5:		if(strstr(USART1_RX_Buff,"+CGSN:")!=NULL)
						{
							for(m=0;m<50;m++)
							{
								if(USART1_RX_Buff[m-2]=='N'&&USART1_RX_Buff[m-1]==':')
								{
									for(n=0;n<15;n++){Device_information_char_Buff[n]=USART1_RX_Buff[n+m];}
									Device_information_char_Buff[15]='0';
								}
							}
							USart_CLR_RecvBuf(1);
							Uart1_SendAtCommand("AT+NCONFIG?");//配置自动联网，扰码开启
							NB_time=50;
						}
						else 
						{
							USart_CLR_RecvBuf(1);
							Uart1_SendAtCommand("AT+CGSN=1");//查询IMEI号
							NB_AT_Num=4;
						}
				break;
			
			case 6:			if(strstr(USART1_RX_Buff,"+NCONFIG:AUTOCONNECT,TRUE")!=NULL)
							{
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+NBAND?");
								NB_time=50;
								
							}
			case 7:			if(strstr(USART1_RX_Buff, "+NBAND:5") != NULL)
							{
								NB_ReceveData_Failed_Flag=0;
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+CFUN=1");
								NB_time=50;
							}
					break;				
							
						
			case 8:			if(strstr(USART1_RX_Buff,"OK")!=NULL)
							{
								NB_ReceveData_Failed_Flag=0;
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+CGATT?");
								NB_time=50;
								
							}
			break;			
//			
			case 9:			if(strstr(USART1_RX_Buff,"+CGATT:1")!=NULL)
							{
								NB_ReceveData_Failed_Flag=0;
								NB_ReceveData_Failed_Flag=0;
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+CIMI");
								NB_time=50;
							}
							else
							{
								
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+CGATT?");
								NB_AT_Num=8;
								NB_ReceveData_Failed_Flag++;
								if(NB_ReceveData_Failed_Flag>30)
								{
									if(NB_Struct.Net_time==1)
									{
										OLED_Display_GB2312_string(1,0,"SIM卡驻网失败！");	
										OLED_Display_GB2312_string(1,2,"请检查SIM卡！");
										OLED_Display_GB2312_string(1,4,"确认无误后请重启");
										NB_AT_Num=20;
										break;
									}
									if(NB_Struct.Net_time<1)
									{
										NB_Struct.Net_time+=1;
										NB_Flag_Init();
										NB_Power_Control=0;
										delay_ms(500);
										NB_AT_Num=0;
									}
									
//									Work_Struct.Mode_type.Sleep_Mode=1;
//									USart_CLR_RecvBuf(1);
//									Uart1_SendAtCommand("AT+CIMI");
//									NB_AT_Num=9;
									NB_ReceveData_Failed_Flag=0;
								}
							}
				break;
//			
			case 10:			
							if(strstr(USART1_RX_Buff,"460")!=NULL)
							{
								NB_ReceveData_Failed_Flag=0;
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+NCCID");
								NB_time=20;
							}
				break;
//			
			case 11:			if(strstr(USART1_RX_Buff,"+NCCID")!=NULL)
							{
								USart_CLR_RecvBuf(1);
								Uart1_SendAtCommand("AT+CSQ");
								NB_time=50;
							}
				break;
//			
			case 12:		if(strstr(USART1_RX_Buff,"+CSQ:")!=NULL)
							{
								for(m=0;m<20;m++)
								{
									if(USART1_RX_Buff[m-2]=='Q'&&USART1_RX_Buff[m-1]==':')
									{	
										if(USART1_RX_Buff[m+1]!=',')
										{
											Device_information_char_Buff[16]=USART1_RX_Buff[m];
											Device_information_char_Buff[17]=USART1_RX_Buff[m+1];
										}
										else if(USART1_RX_Buff[m+1]==',')
										{
											Device_information_char_Buff[16]='0';
											Device_information_char_Buff[17]=USART1_RX_Buff[m];
										}
										
									}
								}
								USart_CLR_RecvBuf(1);                    
								Uart1_SendAtCommand("AT+CEREG?");
								NB_time=5;
							}
				break;
//			
			case 13:		if(strstr(USART1_RX_Buff, "+CEREG:0,1") != NULL)
							{
								USart_CLR_RecvBuf(1);                    
								Uart1_SendAtCommand("AT+CSCON?");
								NB_ReceveData_Failed_Flag=0;
								NB_time=50;
								
							}
							else
							{
								NB_AT_Num=12;
								NB_ReceveData_Failed_Flag++;
								Uart1_SendAtCommand("AT+CEREG?");
								NB_time=50;
							}
				break;
//			
			case 14:	if(strstr(USART1_RX_Buff, "+CSCON:0,1") != NULL)
						{
							NB_ReceveData_Failed_Flag=0;
							USart_CLR_RecvBuf(1);
							Uart1_SendAtCommand("AT+CCLK?");
							NB_time=50;
						}
						else
						{
							NB_AT_Num=13;
							USart_CLR_RecvBuf(1);
							Uart1_SendAtCommand("AT+CSCON?");
							NB_time=50;
							NB_ReceveData_Failed_Flag++;
							if(NB_ReceveData_Failed_Flag>100)
							{
								Work_Struct.Mode_type.Sleep_Mode=1;
								NB_ReceveData_Failed_Flag=0;
							}
						}
				break;
		
			case 15:	if(strstr(USART1_RX_Buff, "+CCLK:") != NULL)
						{
							temp[0]=USART1_RX_Buff[8];//年
							temp[1]=USART1_RX_Buff[9];
							temp[2]=USART1_RX_Buff[11];//月
							temp[3]=USART1_RX_Buff[12];
							temp[4]=USART1_RX_Buff[14];//日
							temp[5]=USART1_RX_Buff[15];
							temp[6]=USART1_RX_Buff[17];//时
							temp[7]=USART1_RX_Buff[18];
							temp[8]=USART1_RX_Buff[20];//分
							temp[9]=USART1_RX_Buff[21];
							temp[10]=USART1_RX_Buff[23];//秒
							temp[11]=USART1_RX_Buff[24];
							AscillToString(temp, temp,12);//Ascii转16进制
							for(i=0;i<6;i++)
							CurrentTim[i] = temp[i*2]*10+temp[i*2+1];//数组高低位合并
							GTM_ConversionUTC(28800);//3600*8小时                 						
							USart_CLR_RecvBuf(1); 
							Uart1_SendAtCommand("AT+NNMI=0");
							NB_time=100;
						}	
				break;
			case 16:		if(strstr(USART1_RX_Buff, "OK") != NULL)
							{
								if(a==1)
								{
									STMFLASH_Write(FLASH_SAVE_ADDR+4096,(u16*)Device_information_char_Buff,18);
									NB_Struct.NB_Init=1;
									ConnectBlueStruct.Read_Flag=0;
									USart_CLR_RecvBuf(1); 
									NB_time=100;
									USART3_Send_Arr(Device_information_char_Buff,18);
									OLED_Clear();
									OLED_Display_GB2312_string(1,0,"系统初始化成功!!");
									OLED_Display_GB2312_string(1,4,"长按按键启动蓝牙");
									BEEP_ON(3);
								}
								else if(a==2)
								{
									STMFLASH_Write(FLASH_SAVE_ADDR+4096,(u16*)Device_information_char_Buff,18);
									ConnectBlueStruct.Read_Flag=0;
									ConnectBlueStruct.BlueScanSuccess=1;
									ConnectBlueStruct.connect_flag=1;
									CommunicationStruct.APP_To_JQZ_CB_Flag=0;
									USart_CLR_RecvBuf(1); 
									NB_time=100;
								}
								
								
								//NB_Update_Data();
								
							}
				
				break;
							
			default :		NB_time=0;
							NB_AT_Num=0;
							NB_SendData_flag=0;
							NB_ReceveData_flag=0;	
				break;
			
		}
	}
	return NB_ReceveData_Failed_Flag;
}

void NB_Iot_AT_Send(u8 b)
{
		if(NB_SendData_flag)
		{
			if(NB_ReceveData_flag)//
			{
				NB_ReceveData_flag=0;
				NB_Bard_AT_Send(b);			
			}
		}
}

void NB_INIT(void)
{
	if(ConnectBlueStruct.Read_Flag)//启动NB
	{
		NB_Struct.Mode =1;
		NB_Iot_AT_Send(1);
	}
}


void NB_UPDATE_DATA(void)//NB上传数据
{
	u8 i;
	char name[142];
	char mac[122];
	if(CommunicationStruct.APP_To_JQZ_CB_Flag==0x01)
	{
		NB_Struct.Mode=1;
		Uart2_SendAtCommand("AT+DISCONNECT");
		delay_ms(200);
		Uart2_SendAtCommand("AT+ROLE=M");
		STMFLASH_Read(SAVE_ADDR_NAME_1,(u16*)name,70);
		STMFLASH_Read(SAVE_ADDR_MAC_1,(u16*)mac,60);
		for(i=0;i<140;i++){BlueNAME[i]=name[i];}
		for(i=0;i<120;i++){BlueMAC[i]=mac[i];}	
		ConnectBlueStruct.Connect_lenth =strlen(BlueNAME)/14*2;
		StringToBytes(BlueNAME,StringConvert16_BlueNAME,140);//蓝牙名称字符串转16进制
		NB_Flag_Init();
		NB_Power_Control=0;
		delay_ms(1000);
		NB_Power_Control=1;
		CommunicationStruct.APP_To_JQZ_CB_Flag++;
	}
	if(CommunicationStruct.JQZ_CB_UPDATE==0x01&&CommunicationStruct.APP_To_JQZ_CB_Flag==0x02)
	{
		//LED3=!LED3;
		NB_Struct.Mode =1;
		NB_Iot_AT_Send(2);	
	}
	if(Work_Struct.Mode_type.JQZ_Mode.Update==1)//第二次上传10个
	{
		STMFLASH_Read(SAVE_ADDR_NAME_2,(u16*)name,70);
		STMFLASH_Read(SAVE_ADDR_MAC_2,(u16*)mac,60);
		for(i=0;i<140;i++){BlueNAME[i]=name[i];}
		for(i=0;i<120;i++){BlueMAC[i]=mac[i];}	
		ConnectBlueStruct.Connect_lenth =strlen(BlueNAME)/14*2;
		if(ConnectBlueStruct.Connect_lenth<10||(BlueMAC[60]=='0'&&BlueMAC[61]=='0'&&BlueMAC[62]=='0'&&BlueMAC[63]=='0'))
		{
			ConnectBlueStruct.BlueScanSuccess=0;
			ConnectBlueStruct.connect_flag=0;
			delay_ms(100);
			raw_printf("抄表结束");
			Work_Struct.Mode_type.JQZ_Mode.Update=0;
		}
		else 
		{
			StringToBytes(BlueNAME,StringConvert16_BlueNAME,140);//蓝牙名称字符串转16进制
			ConnectBlueStruct.BlueScanSuccess=1;
			ConnectBlueStruct.connect_flag=1;
			Work_Struct.Mode_type.JQZ_Mode.Update++;
		}
	}
	if(Work_Struct.Mode_type.JQZ_Mode.Update==3)//第三次上传10个
	{
		STMFLASH_Read(SAVE_ADDR_NAME_3,(u16*)name,70);
		STMFLASH_Read(SAVE_ADDR_MAC_3,(u16*)mac,60);
		for(i=0;i<140;i++){BlueNAME[i]=name[i];}
		for(i=0;i<120;i++){BlueMAC[i]=mac[i];}	
		ConnectBlueStruct.Connect_lenth =strlen(BlueNAME)/14*2;
		if(ConnectBlueStruct.Connect_lenth<10||(BlueMAC[60]=='0'&&BlueMAC[61]=='0'&&BlueMAC[62]=='0'&&BlueMAC[63]=='0'))
		{
			ConnectBlueStruct.BlueScanSuccess=0;
			ConnectBlueStruct.connect_flag=0;
			delay_ms(100);
			raw_printf("抄表结束");
			Work_Struct.Mode_type.JQZ_Mode.Update=0;
		}
		else 
		{
			StringToBytes(BlueNAME,StringConvert16_BlueNAME,140);//蓝牙名称字符串转16进制
			ConnectBlueStruct.BlueScanSuccess=1;
			ConnectBlueStruct.connect_flag=1;
			Work_Struct.Mode_type.JQZ_Mode.Update++;
		}
	}
	if(Work_Struct.Mode_type.JQZ_Mode.Update==5)//第四次上传10个
	{
		
		STMFLASH_Read(SAVE_ADDR_NAME_4,(u16*)name,70);
		STMFLASH_Read(SAVE_ADDR_MAC_4,(u16*)mac,60);
		for(i=0;i<140;i++){BlueNAME[i]=name[i];}
		for(i=0;i<120;i++){BlueMAC[i]=mac[i];}	
		ConnectBlueStruct.Connect_lenth =strlen(BlueNAME)/14*2;//实际表节点数
		if(ConnectBlueStruct.Connect_lenth<10||(BlueMAC[60]=='0'&&BlueMAC[61]=='0'&&BlueMAC[62]=='0'&&BlueMAC[63]=='0'))
		{
			ConnectBlueStruct.BlueScanSuccess=0;
			ConnectBlueStruct.connect_flag=0;
			delay_ms(100);
			raw_printf("抄表结束");
			Work_Struct.Mode_type.JQZ_Mode.Update=0;
		}
		else 
		{
			StringToBytes(BlueNAME,StringConvert16_BlueNAME,140);//蓝牙名称字符串转16进制
			ConnectBlueStruct.BlueScanSuccess=1;
			ConnectBlueStruct.connect_flag=1;
			Work_Struct.Mode_type.JQZ_Mode.Update++;
		}
	}
	if(Work_Struct.Mode_type.JQZ_Mode.Update==7)
	{
		ConnectBlueStruct.BlueScanSuccess=0;
		ConnectBlueStruct.connect_flag=0;
		delay_ms(100);
		raw_printf("抄表结束");
		Work_Struct.Mode_type.JQZ_Mode.Update=0;
	}
	if(ConnectBlueStruct.connect_flag)//读表数据并上传
	{
		BLUE_POWER_OPEN();
		BLUE_EN_ENABLE();
		Connect_Blue_Read_Data(ConnectBlueStruct.Connect_quality);
		Connect_Fail_Deal();//第一次读表数据失败的进行第二次读表数据
	}
}



