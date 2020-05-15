#include "BlueData_Deal.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "delay.h"
#include "BlueTooth.h"
#include "SimplePrintf.h"
#include "NB.h"
#include "flash.h"
#include "SleepMode.h"
#include "communication_to_app.h" 
#include "wakeup.h"
uint8_t yici=1;
u8 string_12_num=0;
u8 string_14_num=0;
u8 string_14_num_2=0;
u8 ReceivMAC_Flag;
u8 k=0;
u8 T=0;

char tempt1_Read_MAC[230];
char tempt2_Read_NAME[230];
char tempt3_Read_MAC[120];
char tempt4_Read_NAME[140];
char tempt5_Read_MAC[120];
char tempt6_Read_NAME[140];
char tempt7_Read_MAC[120];
char tempt8_Read_NAME[140];

char BlueNAME[255];//
char BlueMAC[216];//
char CurrentNAME_String[14];
char CurrentMAC_String[12];
char Connect_Fail_NAME_Buff[255];
char Connect_Fail_MAC_Buff[255];
char Fail_NAME[14];
char Fail_MAC[12];
uint8_t ReadBuff[30];
uint8_t StringConvert16_BlueNAME[216];
ConnectBlueTypedef ConnectBlueStruct;
Connect_Fail_typedef Connect_Fail_Struct;
//ASCII转字符
void AscillToString(char *InputData,char *OutputData,u8 length)
{
    u8 i;
	for(i=0;i<length;i++)
	{
		if(InputData[i]>='0'&&InputData[i]<='9')
		{
						OutputData[i]	= InputData[i] -'0';
		}
		else if(InputData[i]>='A'&&InputData[i]<='F')
		{
				OutputData[i]	= InputData[i] -'A';
				OutputData[i] += 10;
		}
		else if(InputData[i]>='a'&&InputData[i]<='f')
		{
					OutputData[i]	= InputData[i] - 'a';
				  OutputData[i]	+= 10;
		}
	}
}



//16进制数组转字符串
void HEXArrayToStringArray(uint8_t *inputdata,char *output,u16 length)
{
	uint16_t i =0;
	uint8_t TempBuff[254];
	for(i=0;i<length;i++)
	{
		TempBuff[2*i] = inputdata[i]>>4;
		TempBuff[2*i+1] = inputdata[i]&0x0f;
	}
	for(i=0;i<2*length;i++)
	{
		sprintf(&output[i],"%X",TempBuff[i]);
	}
}

//16进制字符串转数组
void StringArrayToHEXArray(char *inputdata,char *output,u16 length)
{
	int i,n = 0;
	for(i = 0; i<(length-1); i += 2)//数组长度含结束符1Byte故减去一个长度 
	{
		if(inputdata[i] >= 'A' && inputdata[i] <= 'F')
					output[n] = inputdata[i] - 'A' + 10;
		
		else if(inputdata[i] >= 'a' && inputdata[i] <= 'f')
					output[n] = inputdata[i] - 'a' + 10;
		else 
				output[n] = inputdata[i] - '0';
		if(inputdata[i + 1] >= 'A' && inputdata[i + 1] <= 'F')
					output[n] = (output[n] << 4) | (inputdata[i + 1] - 'A' + 10);
		
		else if(inputdata[i + 1] >= 'a' && inputdata[i + 1] <= 'f')
					output[n] = (output[n] << 4) | (inputdata[i + 1] - 'a' + 10);
		else 
				output[n] = (output[n] << 4) | (inputdata[i + 1] - '0');
		++n;
	}   
}

/***********************字符转16进制***********************************/
uint16_t StringToBytes( char* pSrc, uint8_t* pDst, uint16_t nSrcLength)  
{   
    uint16_t i = 0;
	
    for (i = 0; i < nSrcLength; i += 2)   
    {   
        if ((*pSrc >= '0') && (*pSrc <= '9'))   
        {   
            *pDst = (*pSrc - '0') << 4;   
        }   
        else   if((*pSrc >= 'A') && (*pSrc <= 'F'))
        {   
            *pDst = (*pSrc - 'A' + 10) << 4;   
        }  
        else
        {   
            *pDst = (*pSrc - 'a' + 10) << 4;   
        }
        pSrc++;   			 
        if ((*pSrc>='0') && (*pSrc<='9'))   
        {   
            *pDst |= (*pSrc - '0');   
        }   
        else   if((*pSrc >= 'A') && (*pSrc <= 'F'))
        {   
            *pDst |= (*pSrc - 'A' + 10);   
        }   
        else
        {   
            *pDst |= (*pSrc - 'a' + 10);   
        }
        pSrc++;   
        pDst++;   
    }   
    return (nSrcLength / 2);   
} 

//十进制转BCD,length为BCD长度
void DectoBCD(int Dec,char *Bcd, int length)  
{  
    int i;  
    int temp;  
    for (i = length - 1; i >= 0; i--)  
    {  
        temp = Dec % 100;  
        Bcd[i] = ((temp / 10) << 4) + ((temp % 10) & 0x0F);  
        Dec /= 100;  
    }   
}  




// 查找字符串中，指定字符的位置
//F_data为字符数据
//num为第几个。返回结果为0则表示无此字符。
uint8_t find_data(char *dat , char F_data , uint8_t num )
{	
	char num_data=0;
	uint8_t i=0;
	for(i=0;i<255;i++)
	{
		if(dat[i] == F_data)  
		{
			num_data++;	
			if(num_data == num)
				return i+1;
		}
	}
	return 0;
}


//查找字符串被查找字符位置，返回值ff为没有，其他值为存在地址
u8 FindStringToString(char *src, char *sub)
{
	u8 ls,la,i;
	ls = strlen(src);//被查找值
	la = strlen(sub);//查找值
	if (la > ls)
		return 0xff;
	for (i=0;i<=ls-la;i++)
	{
		if(strncmp(src+i,sub,la)==0) 
		return i;
	}
	return 0xff;
}
//查找指定位置字符串中是否包含有该字符串
char FindStringToStringPlace(char *src, char *sub,u8 StartBit)
{
	u8 i,j,k;
	char TempBuff[255];
	i=strlen(src);
	for(j=0;j<i-StartBit;j++)
	{
		TempBuff[j] = src[StartBit+j];
	}
	k= FindStringToString(TempBuff, sub)+StartBit;
	return k;
}
//查找数组中指定起始位指定长度是否含有某个元素
//u8 FindSameData(char *input,char*output,u8 startbit,u8 length)
//{
//	char temp[length];
//	u8 i,j;
//	for(i=0;i<length;i++)
//	{ temp[i] =  input[startbit+i]; }
//	j= (strstr(temp, output) != NULL);
//	return j;
//}







//主模式下蓝牙扫描的数据核对
void Check_MAC(void)
{
	if(USART2_RX_Buff[9]=='M'&&USART2_RX_Buff[10]=='A'&&USART2_RX_Buff[11]=='C')
	{
		if(USART2_RX_Buff[35]=='N'&&USART2_RX_Buff[36]=='A'&&USART2_RX_Buff[37]=='M'&&USART2_RX_Buff[38]=='E')
		{
			if(USART2_RX_Buff[26]=='R'&&USART2_RX_Buff[27]=='S')
			{
				if(USART2_RX_Buff[28]=='S'&&USART2_RX_Buff[29]=='I')
				{
					if(USART2_RX_Buff[40]=='J'&&USART2_RX_Buff[41]=='X')
					{
							ReceivMAC_Flag=1;
							
					}
					
				}
			}
			
		}		
	}
}	


void BLUE_AT_M_Init(u8 Power)
{
	char cmd[16],str[6];
	itoi(str,Power);  
	strcpy(cmd, "AT+SCAN:RSSI=-"); 
	strcat(cmd,str);
	if(Blue_SendData_Flag ==1)
	{
		Blue_SendData_Flag=0;
		switch (Blue_AT_num)
		{
			case 0:		BLUE_POWER_OPEN();
						BLUE_EN_ENABLE();
						raw_printf("启动扫描:");
						Blue_time =100;
				break;
			case 1:		
						
						USart_CLR_RecvBuf(2);
						Uart2_SendAtCommand(cmd);
			break;		
			default:
				break;
		}
	}
}

void SaveMAC_NAME(uint8_t MAC_NUM,uint8_t NAME_NUM,uint8_t NUM,u8 Power)
{
	
	u8 j=0;
	u8 i=0;
	char cmd[16],str[6];
	itoi(str,Power);  
	strcpy(cmd, "AT+SCAN:RSSI=-"); 
	strcat(cmd,str);
		for(j=0;j<50;j++)
		{
			if(USART2_RX_Buff[j-2]=='J'&&USART2_RX_Buff[j-1]=='X')
			{
				for(i=0;i<14;i++){CurrentNAME_String[i]=USART2_RX_Buff[j++];}
			}
		}
		if(strstr(BlueNAME,CurrentNAME_String) == NULL)
		{		
			for(j=0;j<60;j++)
			{
				if(USART2_RX_Buff[j-2]=='J'&&USART2_RX_Buff[j-1]=='X')
				{
					for(i=0;i<14;i++){BlueNAME[i+NAME_NUM]=USART2_RX_Buff[j++];}
				}
			}
				for(j=0;j<60;j++)
				{
					if(USART2_RX_Buff[j-4]=='M'&&USART2_RX_Buff[j-3]=='A'&&USART2_RX_Buff[j-2]=='C'&&USART2_RX_Buff[j-1]=='=')
					{
						for(i=0;i<12;i++){BlueMAC[i+MAC_NUM]=USART2_RX_Buff[j++];}	
						USart_CLR_RecvBuf(2);
						Uart2_SendAtCommand(cmd);
					}
				}
		}
		else if(strstr(BlueNAME, CurrentNAME_String) != NULL)
		{
			LED2=!LED2;
			USart_CLR_RecvBuf(2);
			memset(CurrentNAME_String,0,sizeof(CurrentNAME_String));
			Uart2_SendAtCommand(cmd);
			k=NUM;
		}
}
void Take_Out_NAME_MAC(u8 Power)
{
	u8 j=0;
	u8 i=0;
	char cmd[16],str[6];
	itoi(str,Power);  
	strcpy(cmd, "AT+SCAN:RSSI=-"); 
	strcat(cmd,str);
//	BlueDataRceiveModeStruct.Blue_AT_Mode=1;
//	ConnectBlueStruct.ConnectBlueReceive_AT=1;
//	BlueDataRceiveModeStruct.Blue_To_APP=0;
//	BlueDataRceiveModeStruct.Blue_Data_Mode=0;
	 if(StringTimeTempt)
	{
			StringTimeTempt=0;		
			Check_MAC();
				if(!ReceivMAC_Flag)
				{
					memset(BlueNAME,0,sizeof(BlueNAME));
					memset(BlueMAC,0,sizeof(BlueMAC));
					memset(CurrentNAME_String,0,sizeof(CurrentNAME_String));
					USart_CLR_RecvBuf(2);
					Uart2_SendAtCommand(cmd);
					T=0;
					k=0;
					ReceivMAC_Flag=1;
				}
				 if(ReceivMAC_Flag)
				{
					k++;
						switch (k)
						{
							case 1:		SaveMAC_NAME(0,0,0,80);		
								break;
							case 2:		SaveMAC_NAME(12,14,1,80);
								break;
							case 3:		SaveMAC_NAME(24,28,2,80);
								break;
							case 4:		SaveMAC_NAME(36,42,3,80);
								break;
							case 5:		SaveMAC_NAME(48,56,4,80);
								break;
							case 6:		SaveMAC_NAME(60,70,5,80);
								break;
							case 7:		SaveMAC_NAME(72,84,6,80);
								break;		
							case 8:		SaveMAC_NAME(84,98,7,80);
								break;
							case 9:		SaveMAC_NAME(96,112,8,80);
								break;
							case 10:	SaveMAC_NAME(108,126,9,80);
								break;
							case 11:	SaveMAC_NAME(120,140,10,80);
								break;
							case 12:	SaveMAC_NAME(132,154,11,80);
								break;
							case 13:	SaveMAC_NAME(144,168,12,80);
								break;
							case 14:	SaveMAC_NAME(156,182,13,80);
								break;		
							case 15:	SaveMAC_NAME(168,196,14,80);
								break;
							case 16:	SaveMAC_NAME(180,210,15,80);
								break;
							case 17:	SaveMAC_NAME(192,224,16,80);
								break;
							case 18:	for(j=0;j<12;j++){CurrentMAC_String[j]=USART2_RX_Buff[j+13];}
										for(j=0;j<14;j++){CurrentNAME_String[j]=USART2_RX_Buff[j+42];}
										if(strstr(BlueNAME, CurrentNAME_String) == NULL&&strstr(BlueMAC, CurrentMAC_String) == NULL)
										{
											for(j=0;j<60;j++)
											{
												if(USART2_RX_Buff[j-2]=='J'&&USART2_RX_Buff[j-1]=='X')
												{
													LED2=1;
													for(i=0;i<14;i++){BlueNAME[i+238]=USART2_RX_Buff[j++];}										
													ConnectBlueStruct.ConnectBlueReceive_AT=0;
													
													k=19;
													//break;
												}
												if(USART2_RX_Buff[j-4]=='M'&&USART2_RX_Buff[j-3]=='A'&&USART2_RX_Buff[j-2]=='C'&&USART2_RX_Buff[j-1]=='=')
												{
													for(i=0;i<12;i++){BlueMAC[i+204]=USART2_RX_Buff[j++];}
												}
											}
											delay_ms(300);
											Uart3_SendLR();
											raw_printf("扫描到的蓝牙表号:");
											Uart3_SendLR();
											USART3_Send_xbit(BlueNAME,252,14);//252
											Uart3_SendLR();
											raw_printf("扫描到的蓝牙MAC地址:");
											Uart3_SendLR();
											USART3_Send_xbit(BlueMAC,216,12);//216
											Uart3_SendLR();
											USart_CLR_RecvBuf(2);

//											/*********扫描到的表号及MAC写入FLASH中*********/
											STMFLASH_Write(SAVE_ADDR_NAME_1,(u16*)BlueNAME,140);
											STMFLASH_Write(SAVE_ADDR_MAC_1,(u16*)BlueMAC,120);
											for(i=0;i<112;i++){BlueNAME[i]=BlueNAME[i+140];}
											for(i=0;i<96;i++){BlueMAC[i]=BlueNAME[i+120];}
											STMFLASH_Write(SAVE_ADDR_NAME_2,(u16*)BlueNAME,112);
											STMFLASH_Write(SAVE_ADDR_MAC_2,(u16*)BlueMAC,96);
//											/******************/
											
											CommunicationStruct.Start_Scan1=0;
											ConnectBlueStruct.Connect_Number=0;
											ConnectBlueStruct.StopScan=1;
										}
										else if(strstr(BlueNAME, CurrentNAME_String) != NULL)
										{
											USart_CLR_RecvBuf(2);
											Uart2_SendAtCommand(cmd);
											k=17;
										}
								break;		
							default:	
								break;
							
						}
									
				}	
			}
}	


void Read_Data_Send(uint8_t a,uint8_t amc)//连接表端并读取表数据
{
	u8 i;
	char CONNECT[23];
		
	if(BlueDataRceiveModeStruct.NB_UpdateBlueData_Success)
	{
		BlueDataRceiveModeStruct.Blue_AT_Mode=1;
		BlueDataRceiveModeStruct.Blue_Data_Mode=0;
		USart_CLR_RecvBuf(2);
		Uart3_SendLR();
		Uart2_SendAtCommand("AT+DISCONNECT");
		memset(BlueDataRceiveModeStruct.DataMode.BlueDataBuff,0,sizeof(BlueDataRceiveModeStruct.DataMode.BlueDataBuff));
		BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=0;
		ConnectBlueStruct.OK_flag=1;
	}

	else if(strstr(USART2_RX_Buff,"OK")!=NULL||strstr(USART2_RX_Buff,"ERROR")!=NULL)//||strstr(USART2_RX_Buff,"ERROR")!=NULL
	{
		memset(CONNECT,0,sizeof(CONNECT));
		memset(Fail_NAME,0,sizeof(Fail_NAME));
		strcpy(CONNECT,"AT+CONNECT="); 
		for(i=0;i<12;i++){CONNECT[i+11]=BlueMAC[i+amc];}
		for(i=0;i<12;i++){Fail_MAC[i]=BlueMAC[i+amc];}
		for(i=0;i<14;i++){Fail_NAME[i]=BlueNAME[i+string_14_num_2];}
		string_14_num_2+=14;
		USart_CLR_RecvBuf(2);
		Uart3_SendLR();
		Uart2_SendAtCommand(CONNECT);
		ConnectBlueStruct.Connect_Time=3000;
		ConnectBlueStruct.Connect_Number=a;			
	}
}

void Read_Data_Update(uint8_t a,uint8_t name)//上传表数据
{
	u8 i;
	char CONNECT[23];
	if(strstr(USART2_RX_Buff, "AT+CONNECTED") != NULL)//连接成功
	{
		ConnectBlueStruct.Connect_Time=0;
		ConnectBlueStruct.Connect_OverTime_Flag=0;
		BlueDataRceiveModeStruct.Blue_AT_Mode=0;
		BlueDataRceiveModeStruct.Blue_To_APP=0;
		BlueDataRceiveModeStruct.Blue_Data_Mode=1;
		memset(ReadBuff,0,sizeof(ReadBuff));
		Read_Meter_19Protocol(name);
		USART2_Send_Arr(ReadBuff,19);
		USart_CLR_RecvBuf(2);
		ConnectBlueStruct.ReadData_Time=10000;
	}
	else if(ConnectBlueStruct.Connect_OverTime_Flag )
	{
		ConnectBlueStruct.ReadData_Time=0;
		ConnectBlueStruct.Connect_OverTime_Flag=0;
		ConnectBlueStruct.ReadBlueData_fail=0;
		 if(strstr(USART2_RX_Buff, "AT+CONNECTED") == NULL)//连接失败
		{
			ConnectBlueStruct.Connect_fail_Num++;
			raw_printf("连接表端失败,连接下一个");
			memset(CONNECT,0,sizeof(CONNECT));
			for(i=0;i<12;i++){Connect_Fail_MAC_Buff[i+string_12_num]=Fail_MAC[i];}
			for(i=0;i<14;i++){Connect_Fail_NAME_Buff[i+string_14_num]=Fail_NAME[i];}
			string_12_num+=12;
			string_14_num+=14;
			//BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=1;
			ConnectBlueStruct.Connect_Number=a;
			Uart3_SendLR();
			USart_CLR_RecvBuf(2);
			Uart2_SendAtCommand("AT");
			delay_ms(200);
			Uart2_SendAtCommand("AT");
		}
	}
	else if(ConnectBlueStruct.ReadBlueData_fail)
	{
		ConnectBlueStruct.ReadData_Time=0;
		ConnectBlueStruct.ReadBlueData_fail=0;
		ConnectBlueStruct.Connect_Time=0;
		ConnectBlueStruct.Connect_fail_Num++;
		raw_printf("读数据失败,连接下一个");
		memset(CONNECT,0,sizeof(CONNECT));
		BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=1;
		ConnectBlueStruct.Connect_Number=a;
		USart_CLR_RecvBuf(2);
//		Uart3_SendLR();
//		Uart2_SendAtCommand("DISCONNECT");
	}
	 else if(ConnectBlueStruct.ReadBlueData_Success)
	{
		ConnectBlueStruct.ReadBlueData_fail=0;
		ConnectBlueStruct.ReadData_Time=0;
		ConnectBlueStruct.ReadBlueData_Success=0;
		USart_CLR_RecvBuf(1);
		NB_Update_Data();
	}
	else if(strstr(USART1_RX_Buff,"OK")!=NULL)
	{
		
		BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=1;
		ConnectBlueStruct.Connect_Number=a;
		//Uart2_SendAtCommand("DISCONNECT");
		delay_ms(200);
		USart_CLR_RecvBuf(1);
	}
	else if(strstr(USART1_RX_Buff,"ERROR")!=NULL)
	{
		USart_CLR_RecvBuf(1);
		BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=1;
		ConnectBlueStruct.Connect_Number=a;
	}
}


void Connect_Fail_Deal(void)
{
	u8 i;
	if(Connect_Fail_Struct.Fail_Num==1&&yici==1)
	{
		memset(BlueNAME,0,sizeof(BlueNAME));
		memset(BlueMAC,0,sizeof(BlueMAC));
		for(i=0;i<strlen(Connect_Fail_NAME_Buff);i++){BlueNAME[i]=Connect_Fail_NAME_Buff[i];}
		for(i=0;i<strlen(Connect_Fail_MAC_Buff);i++){BlueMAC[i]=Connect_Fail_MAC_Buff[i];}
		StringToBytes(BlueNAME,StringConvert16_BlueNAME,strlen(Connect_Fail_NAME_Buff));//蓝牙名称字符串转16进制
		memset(Connect_Fail_NAME_Buff,0,sizeof(Connect_Fail_NAME_Buff));
		memset(Connect_Fail_MAC_Buff,0,sizeof(Connect_Fail_MAC_Buff));
		Connect_Fail_Struct.Fail_Lenth=(strlen(Connect_Fail_NAME_Buff))/14*2;
		ConnectBlueStruct.Connect_quality=Connect_Fail_Struct.Fail_Lenth;
		ConnectBlueStruct.connect_flag=1;
		ConnectBlueStruct.BlueScanSuccess=1;
		ConnectBlueStruct.Connect_Number=0;
		ConnectBlueStruct.Connect_fail_Num=0;
		string_14_num_2=0;
		string_12_num=0;
		string_14_num=0;
		Connect_Fail_Struct.Fail_Num=0;
		ConnectBlueStruct.Connect_fail_Num=0;
		yici=2;
	}
	else if(yici==2)
	{
		if(ConnectBlueStruct.Connect_quality==ConnectBlueStruct.Connect_Number)
		{
			ConnectBlueStruct.connect_flag=0;
			Work_Struct.Mode_type.JQZ_Mode.Update++;
			ConnectBlueStruct.Connect_Number=0;
			yici=0;	
			raw_printf("读取表端数据结束");
			Uart2_SendAtCommand("AT+DISCONNECT");
			delay_ms(100);
			if(BLUE_CONNECT_COND==1)
			{
				Uart2_SendAtCommand("AT+DISCONNECT");
			}
			Uart2_SendAtCommand("AT");	
		}
		
	}
}


void Connect_Blue_Read_Data(u8 jiedianshu)//读表数据
{
	u8 i;
	char CONNECT[23]; 
	if(ConnectBlueStruct.Connect_quality==ConnectBlueStruct.Connect_Number&&yici==1)
	{
		ConnectBlueStruct.Connect_Number=100;
		memset(Fail_NAME,0,sizeof(Fail_NAME));
//		Uart3_SendLR();
//		raw_printf("读取表数据失败的表号和MAC地址:");
//		USART3_Send_xbit(Connect_Fail_NAME_Buff,strlen(Connect_Fail_NAME_Buff),14);
//		Uart3_SendLR();
//		USART3_Send_xbit(Connect_Fail_MAC_Buff,strlen(Connect_Fail_MAC_Buff),12);
		Connect_Fail_Struct.Fail_Num=1;
		Sleep_Mode_Struct.Enter_SleepMode_Time=10000;
	}
	switch(ConnectBlueStruct.Connect_Number)
	{
			case 0:		
						if(ConnectBlueStruct.BlueScanSuccess)//ConnectBlueStruct.BlueScanSuccess
						{
							memset(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff,0,sizeof(BlueDataRceiveModeStruct.DataMode.BlueDataStringBuff));
							BlueDataRceiveModeStruct.Blue_AT_Mode=1;
							BlueDataRceiveModeStruct.Blue_Data_Mode=0;
							USart_CLR_RecvBuf(2);
							Uart3_SendLR();
							raw_printf("开始连接表端");
							Uart2_SendAtCommand("AT+ROLE=M");//CLRBOND
							ConnectBlueStruct.BlueScanSuccess=0;
						}
						else if(strstr(USART2_RX_Buff,"OK")!=NULL)
						{
							memset(CONNECT,0,sizeof(CONNECT));
							memset(Fail_NAME,0,sizeof(Fail_NAME));
							strcpy(CONNECT,"AT+CONNECT="); 
							for(i=0;i<12;i++){CONNECT[i+11]=BlueMAC[i];}
							for(i=0;i<12;i++){Fail_MAC[i]=BlueMAC[i];}
							for(i=0;i<14;i++){Fail_NAME[i]=BlueNAME[i+string_14_num_2];}
							string_14_num_2+=14;
							Uart3_SendLR();
							USart_CLR_RecvBuf(2);
							Uart2_SendAtCommand(CONNECT);
							Uart3_SendLR();
							memset(CONNECT,0,sizeof(CONNECT));
							ConnectBlueStruct.Connect_Time=4000;
							ConnectBlueStruct.Connect_Number=1;
							memset(BlueDataRceiveModeStruct.DataMode.BlueDataBuff,0,sizeof(BlueDataRceiveModeStruct.DataMode.BlueDataBuff));
						}						
			break;
	 
		case 1:			Read_Data_Update(2,6);								
			break;	
		case 2:			Read_Data_Send(3,12);
			break;
		case 3:			Read_Data_Update(4,13);	
			break;
		case 4:			Read_Data_Send(5,24);
			break;
		case 5:			Read_Data_Update(6,20);			
			break;		
		case 6:			Read_Data_Send(7,36);	
			break;
		case 7:			Read_Data_Update(8,27);	
			break;
		case 8:			Read_Data_Send(9,48);
			break;
		case 9:			Read_Data_Update(10,34);			
			break;		
		case 10:		Read_Data_Send(11,60);	
			break;
		case 11:		Read_Data_Update(12,41);	
			break;	
		case 12:		Read_Data_Send(13,72);					
			break;
		case 13:		Read_Data_Update(14,48);
			break;
		case 14:		Read_Data_Send(15,84);
			break;
		case 15:		Read_Data_Update(16,55);			
			break;		
		case 16:		Read_Data_Send(17,96);	
			break;
		case 17:		Read_Data_Update(18,62);	
			break;
		case 18:		Read_Data_Send(19,108);
			break;
//		case 19:		Read_Data_Update(20,69);			
//			break;	
//		case 20:		Read_Data_Send(21,120);	
//			break;
////		case 21:		Read_Data_Update(22,76);	
//			break;	
			
			
//		case 22:		Read_Data_Send(23,132);					
//			break;
//		case 23:		Read_Data_Update(24,83);
//			break;
//		case 24:		Read_Data_Send(25,144);
//			break;
//		case 25:		Read_Data_Update(26,90);			
//			break;		
//		case 26:		Read_Data_Send(27,156);	
//			break;
//		case 27:		Read_Data_Update(28,97);	
//			break;
//		case 28:		Read_Data_Send(29,168);
//			break;
//		case 29:		Read_Data_Update(30,104);			
//			break;		
//		case 30:		Read_Data_Send(31,180);	
//			break;
//		case 31:		Read_Data_Update(32,111);				
//			break;
//		case 32:		Read_Data_Send(33,192);
//			break;
//		case 33:		Read_Data_Update(34,118);	
//			break;
//		case 34:		Read_Data_Send(35,204);	
//			break;		
		case 19:		//Read_Data_Update(36,125);
		
		
		
						if(strstr(USART2_RX_Buff, "AT+CONNECTED") != NULL)//连接成功
						{
							ConnectBlueStruct.Connect_Time=0;
							BlueDataRceiveModeStruct.Blue_AT_Mode=0;
							BlueDataRceiveModeStruct.Blue_Data_Mode=1;
							//memset(ReadBuff,0,sizeof(ReadBuff));
							Read_Meter_19Protocol(69);
							USART2_Send_Arr(ReadBuff,19);
							USart_CLR_RecvBuf(2);
							ConnectBlueStruct.ReadData_Time=10000;
						}
						else if(ConnectBlueStruct.Connect_OverTime_Flag )
						{
							ConnectBlueStruct.Connect_OverTime_Flag=0;
							 if(strstr(USART2_RX_Buff, "AT+CONNECTED") == NULL)//连接失败
							{
								//Work_Struct.Mode_type.JQZ_Mode.Update++;
								//ConnectBlueStruct.Connect_fail_Num++;
								Uart3_SendLR();
								raw_printf("连接蓝牙失败");
								printf("%d",ConnectBlueStruct.Connect_fail_Num);
								printf("个");
								memset(Fail_NAME,0,sizeof(Fail_NAME));
								for(i=0;i<12;i++){Connect_Fail_MAC_Buff[i+string_12_num]=Fail_MAC[i];}
								string_14_num+=14;
								for(i=0;i<14;i++){Connect_Fail_NAME_Buff[i+string_14_num]=Fail_NAME[i];}
								raw_printf("读取表数据失败的表号和MAC地址:\r\n");
								USART3_Send_xbit(Connect_Fail_NAME_Buff,strlen(Connect_Fail_NAME_Buff),14);
								Uart3_SendLR();
								USART3_Send_xbit(Connect_Fail_MAC_Buff,strlen(Connect_Fail_MAC_Buff),12);
								memset(CONNECT,0,sizeof(CONNECT));
								BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=1;
								ConnectBlueStruct.Connect_Number=20;	
								Connect_Fail_Struct.Fail_Num=1;
								yici=1;
							}
						}
						else if(ConnectBlueStruct.ReadBlueData_fail)
						{
							USART3_Send_xbit(Connect_Fail_MAC_Buff,strlen(Connect_Fail_MAC_Buff),12);
							ConnectBlueStruct.ReadBlueData_fail=0;
							ConnectBlueStruct.Connect_fail_Num++;
							memset(CONNECT,0,sizeof(CONNECT));
							
							BlueDataRceiveModeStruct.NB_UpdateBlueData_Success=1;
							ConnectBlueStruct.Connect_Number=20;
							USart_CLR_RecvBuf(2);
						}
						 else if(ConnectBlueStruct.ReadBlueData_Success)
						{
							ConnectBlueStruct.ReadData_Time=0;
							USart_CLR_RecvBuf(1);
							NB_Update_Data();
							ConnectBlueStruct.ReadBlueData_Success=0;
							
						}
						else if(strstr(USART1_RX_Buff,"OK")!=NULL)
						{
							Uart3_SendLR();
							raw_printf("读取表数据失败的表号和MAC地址:\r\n");
							USART3_Send_xbit(Connect_Fail_NAME_Buff,strlen(Connect_Fail_NAME_Buff),14);
							USART3_Send_xbit(Connect_Fail_MAC_Buff,strlen(Connect_Fail_MAC_Buff),12);
							Uart2_SendAtCommand("AT+DISCONNECT");
							Connect_Fail_Struct.Fail_Num=1;
							yici=1;
							delay_ms(1000);
							if(BLUE_CONNECT_COND==1)
							{
								Uart2_SendAtCommand("AT+DISCONNECT");
							}
							//Work_Struct.Mode_type.JQZ_Mode.Update++;
							ConnectBlueStruct.Connect_Number=20;
						}
			break;
		default:		//ConnectBlueStruct.Connect_Number=100;
			break;
	}
}

extern char NAME[252];
extern char MAC[216];

void Read_NAME_AND_NAME(void)//从FLASH中读取表号和MAC地址
{
	uint8_t i;
	STMFLASH_Read(SAVE_ADDR_NAME_1,(u16*)tempt2_Read_NAME,70);
	STMFLASH_Read(SAVE_ADDR_MAC_1,(u16*)tempt1_Read_MAC,60);
	STMFLASH_Read(SAVE_ADDR_MAC_2,(u16*)tempt3_Read_MAC,60);
	STMFLASH_Read(SAVE_ADDR_NAME_2,(u16*)tempt4_Read_NAME,70);
	STMFLASH_Read(SAVE_ADDR_MAC_3,(u16*)tempt5_Read_MAC,60);
	STMFLASH_Read(SAVE_ADDR_NAME_3,(u16*)tempt6_Read_NAME,70);
	STMFLASH_Read(SAVE_ADDR_MAC_4,(u16*)tempt7_Read_MAC,60);
	STMFLASH_Read(SAVE_ADDR_NAME_4,(u16*)tempt8_Read_NAME,70);
	raw_printf("读到的表号和MAC地址:");
	raw_printf("\r\n");
	raw_printf("第1-10个：");
	USART3_Send_xbit(tempt2_Read_NAME,140,14);//252
	USART3_Send_xbit(tempt1_Read_MAC,120,12);//252
	raw_printf("\r\n");
	raw_printf("第11-20个：");
	USART3_Send_xbit(tempt4_Read_NAME,140,14);//252
	USART3_Send_xbit(tempt3_Read_MAC,120,12);//252
	raw_printf("\r\n");
	raw_printf("第21-30个：");
	USART3_Send_xbit(tempt6_Read_NAME,140,14);//252
	USART3_Send_xbit(tempt5_Read_MAC,120,12);//252
	raw_printf("\r\n");
	raw_printf("第31-40个：");
	USART3_Send_xbit(tempt6_Read_NAME,140,14);//252
	USART3_Send_xbit(tempt5_Read_MAC,120,12);//252
	for(i=0;i<140;i++){BlueNAME[i]=tempt2_Read_NAME[i];}
	for(i=0;i<120;i++){BlueMAC[i]=tempt1_Read_MAC[i];}	
	StringToBytes(BlueNAME,StringConvert16_BlueNAME,140);//蓝牙名称字符串转16进制
	ConnectBlueStruct.Read_Flag=1;
}

//CS校验，输入数组名，数组长度，数据开始校验的地方
u8  CS_Check(uint8_t DataBuff[100],u8 length,u8 StartBit)//char DataBuff[100]
{
	u8 i,j=0;
	for(i=0;i<length;i++)
	{
		j+=DataBuff[i+StartBit];
	}	
	__nop();
	return j;
}


//读表数据协议格式
void Read_Meter_19Protocol(uint8_t QQ)
{  
    u8 i;
	memset(ReadBuff,0,sizeof(ReadBuff));
	ReadBuff[0] = 0xfe;
	ReadBuff[1] = 0xfe;
	ReadBuff[2] = 0xfe;
	ReadBuff[3] = 0x68;//包头
	ReadBuff[4] = 0x10;//表类型
	for(i=0;i<7;i++)
	{ReadBuff[5+i] = StringConvert16_BlueNAME[QQ-i];}
	ReadBuff[12] = 0x01;//控制字
	ReadBuff[13] = 0x03;//数据长度
	ReadBuff[14] = 0x90;
	ReadBuff[15] = 0x2f;
	ReadBuff[16] = 0x00;//SER  
	ReadBuff[17] = CS_Check(ReadBuff,14,3);
	ReadBuff[18] = 0x16;
}



