#ifndef _USART_H__
#define _USART_H__
#include "sys.h"
#include "stdio.h"
extern u8 PrintfFlag;
extern u16 StringTime;
extern u8 StringTimeTempt;

extern u8 USART1_RX_Count,USART2_RX_Count,USART3_RX_Count;
extern char USART1_RX_Buff[255],USART2_RX_Buff[255], USART3_RX_Buff[255];


void USART1_Init(uint32_t baud);
void USART2_Init(uint32_t baud);
void USART3_Init(uint32_t baud);
void USART1_char_send(char c);
void USART1_Send_String(char *dat);
void USART2_char_send(char c);
void USART2_Send_String(char *dat);
void USART3_char_send(char c);
void USART3_Send_String(char *dat);
void Uart1_SendLR(void);   //回车换行数据
void Uart2_SendLR(void);   //回车换行数据
void Uart3_SendLR(void);   //回车换行数据
void Uart1_SendAtCommand(char *cmd);//发送AT指令
void Uart2_SendAtCommand(char *cmd);//发送AT指令
void Uart3_SendAtCommand(char *cmd);//发送AT指令
void USart_CLR_RecvBuf(u8 i);//清除串口数据缓存
void Close_USART(uint8_t data);//关闭串口配置GPIO为浮空下拉输入
void Open_USART(uint8_t data);
void USART1_Send_Arr(char *dat,uint8_t len);
void USART2_Send_Arr(uint8_t *dat,uint8_t len);
void USART3_Send_Arr(char *dat,uint8_t len);
void USART3_Send_xbit(char *dat,uint8_t len,uint8_t q);
extern int fputc(int ch, FILE *f);
#endif


