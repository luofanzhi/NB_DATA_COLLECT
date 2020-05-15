#ifndef _BlueTooth_h__
#define _BlueTooth_h__
#include "sys.h"
#include "usart.h"
#include "led.h"

#define BLUE_POWER PAout(1)
#define BLUE_EN PAout(5)
#define BLUE_RST PAout(7)
#define BLUE_DATA_INT       GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_6)
#define BLUE_CONNECT_COND   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4)

typedef struct 
{

	uint16_t 	BlueConnect_time;
	uint8_t 	BlueConnect_fail;
	uint8_t		Connect;
}BlueConnect;
extern BlueConnect BlueConnectStruct;
extern u16 Blue_time;
extern u8 Blue_SendData_Flag;
extern u8 Blue_ReceveData_Flag;
extern u8 Blue_AT_num;
extern u8 Blue_ReceveData_fail_Flag;
extern 	char CONNECT_MAC[12];
extern uint8_t CONNECT_NAME[14];
extern uint8_t	CONNECT_NAME_uint[7];
void BLUE_GPIO_Init(void);
void BLUE_POWER_OPEN(void);
void BLUE_POWER_CLOSE(void);
void BLUE_EN_ENABLE(void);
void BLUE_EN_DISABLE(void);
void BLUE_RST_ENABLE(void);
void BLUE_RST_DISABLE(void);
void Blue_Flag_Init(void);
void Blue_S_AT_Config(void);//从模式的AT配置
void Blue_M_AT_Config(void);//启动扫描

void itoi(char *str, unsigned long num);

void BlueTooth_S_AT_Send(void);
void Check_MAC(void);
void BLE_StopScan_Set(void);//停止扫描


void BLE_CLRBOND(void);//取消绑定





#endif




