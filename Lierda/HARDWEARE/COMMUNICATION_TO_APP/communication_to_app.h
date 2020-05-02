#ifndef _communication_to_app_h__
#define _communication_to_app_h__
#include "sys.h"

typedef struct	
{
	char IP_COM_char[50];
	char IP_COM_string[50];
	uint8_t IP_COM_u8[30];
	u8 CommunicationBuff[255];
	u8 Communication_RX_Cont;
	uint8_t S_Mode_Start;
	uint8_t Receive_Success;
	uint8_t Send_Device_information;
	uint8_t Start;
	uint8_t Start_Scan1;
	uint16_t Receive_Time;
	uint8_t Receive_Over_Time;
	uint8_t write_ser;
	uint8_t APP_To_JQZ_CB_Flag;
	uint8_t JQZ_CB_UPDATE;
}CommunicationTypedef;
extern CommunicationTypedef CommunicationStruct;
void KEY_Set_Blue_S_Mode(void);
void Read_Device_information(void);
void APP_To_BlueScan(uint32_t time);
void Scan_Answer(void);
void write_name_mac(u8 ser);
void Read_name_mac(u8 ser);//集中器返回表号和MAC
void Set_JZQ_IP_Return(void);
void Read_JZQ_IP_COM(void);
void Clear_NAME_MAC(void);
void Error_Return(void);
void Determine_Deveice(void);
void write_mac_name(uint8_t ser,uint32_t name_addr,uint32_t mac_addr);
void JZQ_Write_NAME_MAC(void);
void APP_TO_JZQ_CB_RETURN(void);
#endif



