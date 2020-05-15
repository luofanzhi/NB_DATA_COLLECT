#ifndef _beep__h_
#define _beep__h_
#include "sys.h"
#include "delay.h"
#define BEEP PBout(3)
void BEEP_GPIO_Init(void);
void BEEP_ON(uint8_t times);

#endif



