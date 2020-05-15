#ifndef _flash_h_
#define _flash_h_
#include "sys.h"
#define FLASH_SAVE_ADDR (0X08000000+58*1024)
#define SAVE_ADDR_NAME_1 (0X08000000+50*1024)
#define SAVE_ADDR_NAME_2 (0X08000000+51*1024)
#define SAVE_ADDR_NAME_3 (0X08000000+52*1024)
#define SAVE_ADDR_NAME_4 (0X08000000+53*1024)
#define SAVE_ADDR_MAC_1 (0X08000000+54*1024)
#define SAVE_ADDR_MAC_2 (0X08000000+55*1024)
#define SAVE_ADDR_MAC_3 (0X08000000+56*1024)
#define SAVE_ADDR_MAC_4 (0X08000000+57*1024)

//#define FLASH_STARTADDR             (0x08000000+54*1024)

//#define FLASH_PAGE_SIZE		2048	//FLASH������С	2K
//#define FLASH_PAGE_COUNT	128		//FLASH��������
///*  FLASHBUFFER  */
//#define FlashBufferLength  								400

typedef struct
{
	uint8_t SAVE_MAC_Buff[230];
	uint8_t SAVE_NAME_Buff[255];
	uint8_t Write_Success;
	struct TEMPT
	{
		uint8_t MAC_Cont;
		uint8_t NAME_Cont;
	}Type;
}FLASH_TYPEDEF;
extern FLASH_TYPEDEF FLASH_Struct;

//void Read_Flash_NBtye(uint32_t ReadAddress,uint8_t *ReadData,uint32_t ReadNum);
//void Write_Flash_NBtye(uint32_t WriteAddress,uint8_t *WriteData,uint32_t WriteNum); 




//�û������Լ�����Ҫ����
#define STM32_FLASH_SIZE 64 	 		//��ѡSTM32��FLASH������С(��λΪK)
#define STM32_FLASH_WREN 1              //ʹ��FLASHд��(0��������;1��ʹ��)
//////////////////////////////////////////////////////////////////////////////////////////////////////

//FLASH��ʼ��ַ
#define STM32_FLASH_BASE 0x08000000 	//STM32 FLASH����ʼ��ַ
//FLASH������ֵ
 

u16 STMFLASH_ReadHalfWord(u32 faddr);		  //��������  
void STMFLASH_WriteLenByte(u32 WriteAddr,u32 DataToWrite,u16 Len);	//ָ����ַ��ʼд��ָ�����ȵ�����
u32 STMFLASH_ReadLenByte(u32 ReadAddr,u16 Len);						//ָ����ַ��ʼ��ȡָ����������
void STMFLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite);		//��ָ����ַ��ʼд��ָ�����ȵ�����
void STMFLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead);   		//��ָ����ַ��ʼ����ָ�����ȵ�����

//����д��
void Test_Write(u32 WriteAddr,u16 WriteData);								   



#endif


