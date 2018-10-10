#define __SRC_TIMER
#include "main.h"
#undef __SRC_TIMER

void SysTickInit(void)
{
	SysTick->LOAD = 999999;
	SysTick->CTRL = 0x07;
//	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void)
{
	SysTick10msFlag = 1;
	if(Uart0TimeoutEn)
	{
		Uart0TimeoutCnt --;
		if(!Uart0TimeoutCnt)
		{
			Uart0.rcv_state = 0;
			Uart0TimeoutEn=0;
		}
	}
	if(BeepOn)
	{
		BeepDelayCnt++;
		if(BeepDelayCnt > BeepDelayPeriod)
		{
			CLR_BEEP();		
			CLR_LED();
			BeepOn=0;
			BeepDelayCnt=0;
		}
	}
	
}

