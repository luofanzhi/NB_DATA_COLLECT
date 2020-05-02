#ifndef _NB_H__
#define _NB_H__
#include "sys.h"
#define NB_Switch PAout(8)
#define NB_Power_Control PAout(12)
#define AT_Mode 1
#define Data_Mode 2

typedef struct
{
	uint8_t Mode;
	char Recevie_Data_Buff[255];
	char Send_Data_Buff[255];
	u8 Recevie_Buff_Cont;
	u8 Send_Buff_Cont;
	uint8_t NB_Init;
	uint8_t NB_1;
	uint8_t Updata_End;
	uint8_t Net_time;
}NB_Typedef;
extern NB_Typedef NB_Struct;

extern u16 NB_time;
extern u8 NB_SendData_flag;
extern u8 NB_ReceveData_flag;
extern u8 NB_AT_Num;
extern u8 NB_ReceveData_Failed_Flag;
extern u16 NB_Close_Power_time;
extern char Device_information_char_Buff[50];
extern uint8_t Device_information_u8_Buff[50];
extern u8 CurrentTim[6];//年，月，日，时，分。各一个字节


void NB_Flag_Init(void);
void NB_NO_Rest(void);
void NB_Rest(void);
void NB_GPIO_Init(void);
uint8_t NB_Bard_AT_Send(u8 a);
void StringArrayToHEXArray(char *inputdata,char *output,u16 length);
void AscillToString(char *InputData,char *OutputData,u8 length);
void NB_Update_Data(void);
void NB_Iot_AT_Send(u8 b);
void NB_UPDATE_DATA(void);
void NB_INIT(void);

#endif

