#ifndef _led_h__
#define _led_h__
#include "sys.h"

#define LED1 PBout(12)
#define LED2 PBout(13)
#define LED3 PBout(14)
void LED_GPIO_Init(void);
void LEDn_ON(u8 num);
void LEDn_OFF(u8 num);


#endif

