#ifndef _BlueData_Deal_h__
#define _BlueData_Deal_h__
#include "sys.h"

typedef struct
{
	uint8_t Connect_Number;
	uint8_t ConnectBlueReceive_AT;
	uint8_t BlueScanSuccess;
	uint8_t Read_Flag;
	uint8_t StopScan;
	uint8_t ReadBlueData_Success;
	uint8_t ReadBlueData_fail;
	uint8_t NB_Send_WaterMeterData_Flag;
	uint16_t Connect_Time;
	uint16_t ReadData_Time;
	uint8_t OK_num;
	uint8_t Connect_OverTime_Flag;
	uint8_t Connect_Next_Flag;
	uint8_t OK_flag;
	uint8_t Disconnect_OverTime;
	uint8_t Connect_fail_Num;
	uint8_t ReceiveData_Flag;
	uint8_t connect_flag;
	uint32_t ScanTime;
	uint8_t Scan_OverTime_Flag;
	uint8_t Scan_STOP_FLAG;
	uint8_t Connect_quality;
	uint8_t Connect_lenth;
}ConnectBlueTypedef;
extern ConnectBlueTypedef ConnectBlueStruct;
typedef struct 
{
	uint8_t Blue_To_APP;
	uint8_t Blue_AT_Mode;
	uint8_t Blue_Data_Mode;
	uint8_t NB_UpdateBlueData_Success;
	uint8_t Fail_Name_Cont;
	uint8_t Fail_Mac_Cont;
	char Connect_Fail_Name_Buff[252];
	char Connect_Fail_Mac_Buff[216];
	struct Data
	{
		uint8_t BlueDataBuff[255];//读表数据缓存
		char BlueDataStringBuff[130];//表数据转字符数组缓存
		u8 BlueDataCount;
		uint8_t Blue_68_Data;
		uint8_t NB_SendBlueData_Flag;
		uint8_t Start_NB;
		uint8_t UART_Cont;
	}DataMode;
}BlueDataRceiveMode;

typedef struct
{
	uint8_t Fail_Num;
	uint8_t Fail_Lenth;
}Connect_Fail_typedef;
extern Connect_Fail_typedef Connect_Fail_Struct;
extern BlueDataRceiveMode BlueDataRceiveModeStruct;
extern u8 ReceivMAC_Flag;

extern char tempt1_Read_MAC[230];
extern char tempt2_Read_NAME[230];
extern char tempt3_Read_MAC[120];
extern char tempt4_Read_NAME[140];
extern char tempt5_Read_MAC[120];
extern char tempt6_Read_NAME[140];
extern char tempt8_Read_MAC[120];
extern char tempt7_Read_NAME[140];
extern uint8_t StringConvert16_BlueNAME[216];
extern char BlueNAME[255];//14*16
extern char BlueMAC[216];//12*16
extern char CurrentNAME_String[14];
extern char CurrentMAC_String[12];
extern char Connect_Fail_NAME_Buff[255];
extern char Connect_Fail_MAC_Buff[255];
extern char Fail_NAME[14];
extern char Fail_MAC[12];
extern u8 T;
extern u8 k;


void AscillToString(char *InputData,char *OutputData,u8 length);//ASCII转字符
void DectoBCD(int Dec,char *Bcd, int length);//十进制转BCD,length为BCD长度
void HEXArrayToStringArray(uint8_t *inputdata,char *output,u16 length);//16进制数组转字符串
void StringArrayToHEXArray(char *inputdata,char *output,u16 length);//16进制字符串转数组
uint16_t StringToBytes( char* pSrc, uint8_t* pDst, uint16_t nSrcLength);  //字符转16进制
uint8_t find_data(char *dat , char F_data , uint8_t num );
u8 FindStringToString(char *src, char *sub);
char FindStringToStringPlace(char *src, char *sub,u8 StartBit);
u8 FindSameData(char *input,char*output,u8 startbit,u8 length);
void BLUE_AT_M_Init(u8 Power);
void Check_MAC(void);
extern u8 CheckHead(char Data[100]);//判断包头
void Take_Out_NAME_MAC(u8 Power);
void Read_Meter_19Protocol(uint8_t QQ);//读表数据协议格式

extern u8 CS_Check(uint8_t DataBuff[100],u8 length,u8 StartBit);//CS校验，输入数组名，数组长度，数据开始校验的地方
void Connect_Blue_Read_Data(u8 jiedianshu);
void SendCONNECT(uint8_t a,uint8_t b);
void Jishi_Scan(void );
void Read_NAME_AND_NAME(void);
void Connect_Fail_Deal(void);
#endif



