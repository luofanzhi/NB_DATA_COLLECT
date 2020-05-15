#include "communication_to_app.h" 
#include "key.h"
#include "BlueTooth.h"
#include "BlueData_Deal.h"
#include "NB.h"
#include "delay.h"
#include "SimplePrintf.h"
#include "usart.h"
#include  "flash.h"
#include  "string.h"
#include "stdio.h"
#include "wakeup.h"
void KEY_Set_Blue_S_Mode(void)//按键设置蓝牙模式为从模式
{
	if(KEY_Scan(1)==2)
	{
		Work_Struct.Mode_type.Weakup.WeakUP_Time=100;
		Work_Struct.Mode_type.Sleep_Mode=0;
		KEYStruct.KEY_Time++;
		Work_Struct.Mode_type.Weakup.WeakUP_Time=0;
		Work_Struct.Mode_type.Weakup.WeakUP_Over_Time =0;//&&NB_Struct.NB_Init==1
		if(KEYStruct.KEY_Time==50)
		{
			weak_BLUE_init();
			Sleep_Mode_Struct.WeakUP=0;
			BlueDataRceiveModeStruct.Blue_AT_Mode=1;
			ConnectBlueStruct.ConnectBlueReceive_AT=0;
			BLUE_POWER_OPEN();
			BLUE_EN_ENABLE();	
			CommunicationStruct.S_Mode_Start=1;
			KEYStruct.KEY_Time=0;
		}
	}
	if(CommunicationStruct.S_Mode_Start)
	{
		Work_Struct.Mode_type.Sleep_Mode=0;
		Blue_S_AT_Config();
	}
}

void Read_Device_information(void)//返回设备信息
{
	u8 i;
	uint8_t Tuff[30];
	char information[18];
	memset(Device_information_u8_Buff,0,sizeof(Device_information_u8_Buff));
	STMFLASH_Read(FLASH_SAVE_ADDR+4096,(u16*)information,18);
	StringToBytes(information,Device_information_u8_Buff,18);
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x32;
	Tuff[10]=0x0d;
	Tuff[11]=0x90;
	Tuff[12]=0x2f;
	Tuff[13]=0x00;
	Tuff[14]=Device_information_u8_Buff[0];
	Tuff[15]=Device_information_u8_Buff[1];
	Tuff[16]=Device_information_u8_Buff[2];
	Tuff[17]=Device_information_u8_Buff[3];
	Tuff[18]=Device_information_u8_Buff[4];
	Tuff[19]=Device_information_u8_Buff[5];
	Tuff[20]=Device_information_u8_Buff[6];
	Tuff[21]=Device_information_u8_Buff[7];
	//for(j=0;j<8;j++){Tuff[14+j]=Device_information_u8_Buff[j];}
	Tuff[22]=Device_information_u8_Buff[8];
	Tuff[23]=0x00;//SER
	Tuff[24] = CS_Check(Tuff,23,0);
	Tuff[25]=0x16;
//	USart_CLR_RecvBuf(3);
//	USART3_Send_Arr(Tuff,25);
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,26);
	CommunicationStruct.Send_Device_information=0;
}
void Scan_Answer(void)//集中器扫描回复
{
	u8 i;
	uint8_t Tuff[30];
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x33;
	Tuff[10]=0x05;
	Tuff[11]=0x90;
	Tuff[12]=0x3f;
	Tuff[13]=0x00;
	Tuff[14]=0xbb;
	Tuff[15]=0x00;//SER
	Tuff[16]=CS_Check(Tuff,16,0);
	Tuff[17]=0x16;
//	USart_CLR_RecvBuf(3);
//	USART3_Send_Arr(Tuff,18);
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,18);
}

void Read_name_mac(u8 ser)//集中器返回表号和MAC
{
	u8 i;
	uint8_t Tuff[200];
	uint8_t BlueNAME_u8[72];
	uint8_t BlueMAC_u8[62];
	char name[142];
	char mac[122];
	memset(BlueNAME_u8,0,sizeof(BlueNAME_u8));
	memset(BlueMAC_u8,0,sizeof(BlueMAC_u8));
	memset(name,0,sizeof(name));
	memset(mac,0,sizeof(mac));
	
	switch(ser)
	{
		case 0x01:
				{
					STMFLASH_Read(SAVE_ADDR_NAME_1,(u16*)name,142);
					STMFLASH_Read(SAVE_ADDR_MAC_1,(u16*)mac,122);	
				}
			break;
		case 0x02:	
				{
					STMFLASH_Read(SAVE_ADDR_NAME_2,(u16*)name,142);
					STMFLASH_Read(SAVE_ADDR_MAC_2,(u16*)mac,122);
				}
			break;
		case 0x03:	
				{
					STMFLASH_Read(SAVE_ADDR_NAME_3,(u16*)name,142);
					STMFLASH_Read(SAVE_ADDR_MAC_3,(u16*)mac,122);
				}
			break;
		case 0x04:	
				{
					STMFLASH_Read(SAVE_ADDR_NAME_4,(u16*)name,142);
					STMFLASH_Read(SAVE_ADDR_MAC_4,(u16*)mac,122);
				}
		case 0x05:	
				{
//					for(i=0;i<142;i++){name[i]='0';}
//					for(i=0;i<122;i++){mac[i]='0';}
					memset(name,'0',sizeof(name));
					memset(mac,'0',sizeof(mac));
				}		
			break;
		default:
			break;
	}
	StringToBytes(name,BlueNAME_u8,142);
	StringToBytes(mac,BlueMAC_u8,122);
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x35;
	Tuff[10]=0x86;
	Tuff[11]=0x90;
	Tuff[12]=0x5f;
	Tuff[13]=0x00;
	for(i=0;i<70;i++){Tuff[14+i]=BlueNAME_u8[i];}
	for(i=0;i<60;i++){Tuff[84+i]=BlueMAC_u8[i];}
	Tuff[144]=ser;//SER
	Tuff[145]=CS_Check(Tuff,144,0);
	Tuff[146]=0x16;
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,147);
}
void write_name_mac(u8 ser)//写表号后集中器返回
{
	u8 i;
	uint8_t Tuff[30];
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x34;
	Tuff[10]=0x06;
	Tuff[11]=0x90;
	Tuff[12]=0x4f;
	Tuff[13]=0x00;
	Tuff[14]=0x80;
	Tuff[15]=0x00;
	Tuff[16]=ser;//SER
	Tuff[17]=CS_Check(Tuff,16,0);
	Tuff[18]=0x16;
//	USart_CLR_RecvBuf(3);
//	USART3_Send_Arr(Tuff,18);
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,19);
}
void Set_JZQ_IP_Return(void)//设置集中器IP及端口返回
{
	u8 i;
	uint8_t Tuff[30];
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x95;
	Tuff[10]=0x03;
	Tuff[11]=0xB0;
	Tuff[12]=0x19;
	Tuff[13]=0x00;//SER
	Tuff[14]=CS_Check(Tuff,14,0);
	Tuff[15]=0x16;
//	USart_CLR_RecvBuf(3);
//	USART3_Send_Arr(Tuff,18);
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,16);
}

void Read_JZQ_IP_COM(void)//读取集中器IP及端口返回
{
	u8 i;
	uint8_t Tuff[50];
	STMFLASH_Read(FLASH_SAVE_ADDR+2048,(u16*)CommunicationStruct.IP_COM_u8,20);
	HEXArrayToStringArray((u8*)CommunicationStruct.IP_COM_u8,CommunicationStruct.IP_COM_char,20);
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x83;
	Tuff[10]=0x1a;
	Tuff[11]=0x91;
	Tuff[12]=0x0b;
	Tuff[13]=0x00;
	Tuff[14]=0x2c;
	Tuff[15]=CommunicationStruct.IP_COM_u8[0];
	Tuff[16]=CommunicationStruct.IP_COM_u8[1];
	Tuff[17]=CommunicationStruct.IP_COM_u8[2];
	Tuff[18]=CommunicationStruct.IP_COM_u8[3];
	Tuff[19]=0x2c;
	for(i=0;i<15;i++){Tuff[20+i]=CommunicationStruct.IP_COM_u8[i+5];}
	Tuff[35]=0x00;
	Tuff[36]=0x00;//SER
	Tuff[37]=CS_Check(Tuff,35,0);
	Tuff[38]=0x16;
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,39);

}
void Clear_NAME_MAC(void)//清表节点返回
{
	u8 i;
	uint8_t Tuff[50];
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0xa6;
	Tuff[10]=0x05;
	Tuff[11]=0xaa;
	Tuff[12]=0xbb;
	Tuff[13]=0x00;
	Tuff[14]=0x00;
	Tuff[15]=0x00;//SER
	Tuff[16]=CS_Check(Tuff,16,0);
	Tuff[17]=0x16;
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,18);
	//68 20 AA AA AA AA AA AA AA A6 05 AA BB 00 00 00 3E 16
}
void Error_Return(void)
{
	u8 i;
	uint8_t Tuff[50];
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x00;
	Tuff[10]=0x05;
	Tuff[11]=0xff;
	Tuff[12]=0xff;
	Tuff[13]=0xff;
	Tuff[14]=0xff;
	Tuff[15]=0xff;//SER
	Tuff[16]=CS_Check(Tuff,16,0);
	Tuff[17]=0x16;
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,18);
}
void Determine_Deveice(void)
{
	if(CommunicationStruct.Receive_Over_Time)
	{
		if(CommunicationStruct.CommunicationBuff[CommunicationStruct.CommunicationBuff[10]+12]!=0x16)
		{
			OLED_Clear();
			OLED_Display_GB2312_string(1,6,"接收数据错误！！"); 
			Error_Return();
			CommunicationStruct.Communication_RX_Cont=0;
			memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
			CommunicationStruct.Receive_Time=0;
			CommunicationStruct.Receive_Over_Time=0;
			
		}
		
	}
}
void APP_To_BlueScan(uint32_t time)//APP启动扫描
{
	u8 i;
	if(CommunicationStruct.Start)
	{
		BlueDataRceiveModeStruct.Blue_AT_Mode=2;
		ConnectBlueStruct.ConnectBlueReceive_AT=1;
		BlueDataRceiveModeStruct.Blue_To_APP=0;
		BlueDataRceiveModeStruct.Blue_Data_Mode=0;
		ConnectBlueStruct.connect_flag=0;
		CommunicationStruct.Start_Scan1=1;
		BLUE_POWER_CLOSE();
		BLUE_EN_DISABLE();
		delay_ms(500);
		BLUE_POWER_OPEN();
		BLUE_EN_ENABLE();
		ConnectBlueStruct.ScanTime=time;
		Uart2_SendAtCommand("AT+ROLE=M");
		delay_ms(200);
		Uart2_SendAtCommand("AT+SCAN:RSSI=-80");
		CommunicationStruct.Start=0;	
	}
	 if(CommunicationStruct.Start_Scan1)
	{
		Take_Out_NAME_MAC(80);
	}
	if(ConnectBlueStruct.Scan_OverTime_Flag)
	{
		Uart2_SendAtCommand("AT+STOPSCAN");
		delay_ms(100);
		Uart3_SendLR();
		printf("扫描结束,扫描到的表端数量：");
		printf("%d",strlen(BlueNAME)/14);
		CommunicationStruct.Start_Scan1=0;
		for(i=0;i<(252-strlen(BlueNAME));i++){BlueNAME[strlen(BlueNAME)+i]='0';}
		for(i=0;i<(216-strlen(BlueMAC));i++){BlueMAC[strlen(BlueMAC)+i]='0';}
		STMFLASH_Write(SAVE_ADDR_NAME_1,(u16*)BlueNAME,140);
		STMFLASH_Write(SAVE_ADDR_MAC_1,(u16*)BlueMAC,120);
		for(i=0;i<112;i++){BlueNAME[i]=BlueNAME[i+140];}
		for(i=0;i<96;i++){BlueMAC[i]=BlueMAC[i+120];}
		STMFLASH_Write(SAVE_ADDR_NAME_2,(u16*)BlueNAME,112);
		STMFLASH_Write(SAVE_ADDR_MAC_2,(u16*)BlueMAC,96);
		Blue_Flag_Init();
		ConnectBlueStruct.Scan_OverTime_Flag=0;
	}
}

void write_mac_name(uint8_t ser,uint32_t name_addr,uint32_t mac_addr)
{
	u8 i;
	uint8_t tempt_name[142];
	uint8_t tempt_mac[122];
	char name_222[142];
	char mac_222[122];
	memset(name_222,0,sizeof(name_222));
	memset(mac_222,0,sizeof(mac_222));
	memset(tempt_name,0,sizeof(tempt_name));
	memset(tempt_mac,0,sizeof(tempt_mac));
	for(i=0;i<70;i++){tempt_name[i]=CommunicationStruct.CommunicationBuff[14+i];}
	for(i=0;i<60;i++){tempt_mac[i]=CommunicationStruct.CommunicationBuff[84+i];}
	HEXArrayToStringArray(tempt_name,name_222,71);
	HEXArrayToStringArray(tempt_mac,mac_222,61);
	STMFLASH_Write(name_addr,(u16*)name_222,142);
	STMFLASH_Write(mac_addr,(u16*)mac_222,122);
	CommunicationStruct.Communication_RX_Cont=0;
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	write_name_mac(ser);//写表号返回
	CommunicationStruct.write_ser=0x00;
}
void JZQ_Write_NAME_MAC(void)
{
	switch(CommunicationStruct.write_ser)
	{
		case 0x01:write_mac_name(0x01,SAVE_ADDR_NAME_1,SAVE_ADDR_MAC_1);
			break;
		case 0x02:write_mac_name(0x02,SAVE_ADDR_NAME_2,SAVE_ADDR_MAC_2);
			break;
		case 0x03:write_mac_name(0x03,SAVE_ADDR_NAME_3,SAVE_ADDR_MAC_3);
			break;
		case 0x04:write_mac_name(0x04,SAVE_ADDR_NAME_4,SAVE_ADDR_MAC_4);
			break;
		default:
			break;
	}
}

void APP_TO_JZQ_CB_RETURN(void)
{
	u8 i;
	uint8_t Tuff[50];
	Tuff[0]=0x68;
	Tuff[1]=0x20;
	for(i=0;i<7;i++){Tuff[2+i]=0xaa;}
	Tuff[9]=0x36;
	Tuff[10]=0x04;
	Tuff[11]=0xc1;
	Tuff[12]=0xb1;
	Tuff[13]=0x11;
	Tuff[14]=0x00;
	Tuff[15]=CS_Check(Tuff,15,0);
	Tuff[16]=0x16;
	memset(CommunicationStruct.CommunicationBuff,0,sizeof(CommunicationStruct.CommunicationBuff));
	USART2_Send_Arr(Tuff,17);
}

