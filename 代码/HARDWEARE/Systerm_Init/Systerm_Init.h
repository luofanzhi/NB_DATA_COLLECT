#ifndef _Systerm_Init_h__
#define _Systerm_Init_h__
#include "sys.h"
#include "usart.h"
#include "delay.h"
#include "NB_time.h"
#include "led.h"
#include "key.h"
#include "NB.h"
#include "wdg.h"
#include "BlueTooth.h"
#include "wakeup.h"
#include "SleepMode.h"
#include "rtc.h"
#include "Blue_time.h"
#include "SimplePrintf.h"
#include "BlueData_Deal.h"
#include "flash.h"
#include "BlueData_Deal.h"
#include "string.h"
#include "flash.h"
#include "communication_to_app.h"
void SYSTERM_INIT(void);
void LED_Init(void);
void NB_ALL_Flag_Init(void);
void Deveice_Display(void);


#endif




