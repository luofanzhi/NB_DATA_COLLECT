

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h" 
#include "led.h"
uint16_t  TimingDelay;

void SysTickInit(uint32_t HzPreSecond)
{	
    //if (SysTick_Config(SystemCoreClock / HzPreSecond))//SYSCLK_FREQ_HSE
	  if (SysTick_Config( 48000000L/ HzPreSecond))
    {   
        while (1);
    }
}
void TimingDelay_Decrement(void)
{
		if(TimingDelay != 0x00)
		{ 
			TimingDelay--;	
		}
}
void NMI_Handler(void)
{
}
 
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}
 
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

 
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}
 
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}
 
void SVC_Handler(void)
{
}
 
void DebugMon_Handler(void)
{
}
 
void PendSV_Handler(void)
{
}
 
void SysTick_Handler(void)
{
	TimingDelay_Decrement();
	LED1=!LED1;
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/
