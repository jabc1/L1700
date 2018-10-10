#define __SRC_RTC
#include "LPC17xx.h"
#include "rtc.h"
#undef	__SRC_RTC

/*****************************************************************************
** Function name:		RTCInit
**
** Descriptions:		Initialize RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCInit(void)
{

  /*--- Initialize registers ---*/    
 	LPC_RTC->CCR = CCR_CLKEN;
  	LPC_RTC->AMR = 0xFF;
	return;
}

/*****************************************************************************
** Function name:		RTCStart
**
** Descriptions:		Start RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStart(void) 
{
  	LPC_RTC->CCR |= CCR_CLKEN;

  	LPC_SC->PCONP |= (1 << 9);

  	return;
}

/*****************************************************************************
** Function name:		RTCStop
**
** Descriptions:		Stop RTC timer
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCStop(void)
{   
  LPC_SC->PCONP |= (1 << 9);
  /*--- Stop RTC counters ---*/
  LPC_RTC->CCR &= ~CCR_CLKEN;
  return;
} 

/*****************************************************************************
** Function name:		RTCGetTime
**
** Descriptions:		Get RTC timer value
**
** parameters:			None
** Returned value:		The data structure of the RTC time table
** 
*****************************************************************************/
void  RTCGetTime(RTCTime *LocalTime) 
{
  	LocalTime->RTC_Sec = LPC_RTC->SEC;
  	LocalTime->RTC_Min = LPC_RTC->MIN;
  	LocalTime->RTC_Hour = LPC_RTC->HOUR;
  	LocalTime->RTC_Mday = LPC_RTC->DOM;
  	LocalTime->RTC_Wday = LPC_RTC->DOW;
  	LocalTime->RTC_Yday = LPC_RTC->DOY;
  	LocalTime->RTC_Mon = LPC_RTC->MONTH;
  	LocalTime->RTC_Year = LPC_RTC->YEAR;
}

/*****************************************************************************
** Function name:		RTCSetTime
**
** Descriptions:		Setup RTC timer value
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/
void RTCSetTime(RTCTime Time) 
{
  	LPC_RTC->SEC = Time.RTC_Sec;
  	LPC_RTC->MIN = Time.RTC_Min;
  	LPC_RTC->HOUR = Time.RTC_Hour;
  	LPC_RTC->DOM = Time.RTC_Mday;
  	LPC_RTC->DOW = Time.RTC_Wday;
  	LPC_RTC->DOY = Time.RTC_Yday;
  	LPC_RTC->MONTH = Time.RTC_Mon;
  	LPC_RTC->YEAR = Time.RTC_Year;    
  	return;
}

/*****************************************************************************
**                            End Of File
******************************************************************************/

