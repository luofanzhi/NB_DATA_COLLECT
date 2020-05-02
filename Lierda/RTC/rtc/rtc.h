#ifndef __RTC_H
#define __RTC_H	    
#include "sys.h"


//ʱ��ṹ��
typedef struct 
{
	vu8 hour;
	vu8 min;
	vu8 sec;			
	//������������
	vu16 w_year;
	vu8  w_month;
	vu8  w_date;
	vu8  week;		 
}_calendar_obj;					 
extern _calendar_obj calendar;	//�����ṹ��

typedef struct 
{
	uint8_t ARM_year;
	uint8_t ARM_month;
	uint8_t ARM_dey;
	uint8_t ARM_hour;
	uint8_t ARM_min;
	uint8_t ARM_sec;
}ARM_Typedef;
extern ARM_Typedef ARM_Struct;
typedef struct
{
	 u32 x_sec;
	uint8_t Wakeup_Flag;
}Sleep_Jishi_Typedef;
extern Sleep_Jishi_Typedef Sleep_Jishi_Struct;//˯�߼�ʱ�ṹ��
extern u8 const mon_table[12];	//�·��������ݱ�

void RTC_NVIC_Config(void);
void Disp_Time(u8 x,u8 y,u8 size);//���ƶ�λ�ÿ�ʼ��ʾʱ��
void Disp_Week(u8 x,u8 y,u8 size,u8 lang);//��ָ��λ����ʾ����
void RTC_EXTI_INITIAL(FunctionalState interrupt_en_or_dis);
void RTC_AWU_SET(void);
u8 RTC_Init(void);        //��ʼ��RTC,����0,ʧ��;1,�ɹ�;
u8 Is_Leap_Year(u16 year);//ƽ��,�����ж�
u8 RTC_Alarm_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);
u8 RTC_Get(void);         //����ʱ��   
u8 RTC_Get_Week(u16 year,u8 month,u8 day);
u8 RTC_Set(u16 syear,u8 smon,u8 sday,u8 hour,u8 min,u8 sec);//����ʱ��			 
#endif








