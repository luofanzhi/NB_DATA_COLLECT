#ifndef __OLED_H
#define __OLED_H 

#include "sys.h"
#include "stdlib.h"	

typedef struct 
{
	uint8_t		oled_clear;
}OLEDTypedef;
extern OLEDTypedef OLEDStruct;
//-----------------测试LED端口定义---------------- 

#define LED_ON GPIO_ResetBits(GPIOA,GPIO_Pin_15)
#define LED_OFF GPIO_SetBits(GPIOA,GPIO_Pin_15)

//------------------OLED端口定义------------------ 

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_9)//SCLK

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_8)//DIN
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define OLED_DC_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//DC
#define OLED_DC_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

#define OLED_CS_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//CS
#define OLED_CS_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define ROM_OUT 		  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)

#define OLED_ROM_CS_Clr()  GPIO_ResetBits(GPIOB,GPIO_Pin_4)//ROM_CS
#define OLED_ROM_CS_Set()  GPIO_SetBits(GPIOB,GPIO_Pin_4)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define u8 unsigned char
#define u32 unsigned int
#define u8long unsigned long 

void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u32 dat,u8 mode);
void OLED_Clear(void);
void OLED_address(u8 page,u8 column);
void OLED_Display_128x64(u8 *dp);
void OLED_Display_16x16(u32 column,u32 page,u8 *dp);
void OLED_Display_8x16(u8 column,u32 page,u8 *dp);
void OLED_Display_5x7(u8 column,u32 page,u8 *dp);
void Send_Command_to_ROM(u8 dat);
u8 Get_data_from_ROM(void);
void OLED_get_data_from_ROM(u8 addrHigh,u8 addrMid,u8 addrLow,u8 *pbuff,u8 DataLen);
void OLED_Display_GB2312_string(u8 x,u8 y,u8 *text);
void OLED_Display_string_5x7(u8 x,u8 y,u8 *text);
void OLED_ShowNum(u8 x,u8 y,float num,u8 len);
void OLED_Init(void);

#endif

