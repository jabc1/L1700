#ifdef __SRC_MAIN
#define EXTERN 
#else
#define EXTERN  extern
#endif

#define ISO18000_6C  0
#define ISO18000_6B  1

EXTERN unsigned char BeepOn;
EXTERN unsigned char BeepDelayCnt;
EXTERN unsigned char BeepDelayPeriod;

/* SSP Status register */
#define SSPSR_TFE		1 << 0
#define SSPSR_TNF		1 << 1 
#define SSPSR_RNE		1 << 2
#define SSPSR_RFF		1 << 3 
#define SSPSR_BSY		1 << 4

/* SSP0 CR0 register */
#define SSPCR0_DSS		1 << 0
#define SSPCR0_FRF		1 << 4
#define SSPCR0_SPO		1 << 6
#define SSPCR0_SPH		1 << 7
#define SSPCR0_SCR		1 << 8

/* SSP0 CR1 register */
#define SSPCR1_LBM		1 << 0
#define SSPCR1_SSE		1 << 1
#define SSPCR1_MS		1 << 2
#define SSPCR1_SOD		1 << 3

/* SSP0 Interrupt Mask Set/Clear register */
#define SSPIMSC_RORIM	1 << 0
#define SSPIMSC_RTIM	1 << 1
#define SSPIMSC_RXIM	1 << 2
#define SSPIMSC_TXIM	1 << 3

/* SSP0 Interrupt Status register */
#define SSPRIS_RORRIS	1 << 0
#define SSPRIS_RTRIS	1 << 1
#define SSPRIS_RXRIS	1 << 2
#define SSPRIS_TXRIS	1 << 3

/* SSP0 Masked Interrupt register */
#define SSPMIS_RORMIS	1 << 0
#define SSPMIS_RTMIS	1 << 1
#define SSPMIS_RXMIS	1 << 2
#define SSPMIS_TXMIS	1 << 3

/* SSP0 Interrupt clear register */
#define SSPICR_RORIC	1 << 0
#define SSPICR_RTIC		1 << 1

#define COMM_RESUME_TIME	1000  //10s
#define BREAK_HOPTABLE_TIME	3000 //30s




#define MESSAGE_MCU_OK			"00\r\n"
#define MESSAGE_RF1_OK			"01\r\n"
#define MESSAGE_RF2_OK			"02\r\n"
#define MESSAGE_RF3_OK			"03\r\n"
#define MESSAGE_RF4_OK			"04\r\n"
#define MESSAGE_BT_OK			"10\r\n"
#define MESSAGE_BT_PAIR			"14\r\n"
#define MESSAGE_GPRS_OK			"11\r\n"
#define MESSAGE_SIM_OK			"12\r\n"
#define MESSAGE_FLASH_OK		"13\r\n"
#define MESSAGE_TCP_CONNECT		"26\r\n"
#define MESSAGE_DATA_OK			"29\r\n"
#define Version_OK					"--RF-08-04-04--\r\n"


EXTERN unsigned char SysTick10msFlag;
EXTERN unsigned char SysTick10msCnt;
EXTERN unsigned char SysTick100msFlag;
EXTERN unsigned char SysTick100msCnt;
EXTERN unsigned char SysTick1sFlag;
EXTERN unsigned short SysTick1sCnt;

EXTERN char DebugStr[64];
EXTERN unsigned char PublicBuf[255];
EXTERN unsigned char OutDataBuf[255];

EXTERN unsigned short FlashID;
EXTERN char DeviceID[14];
/////////////////////////////////////////////////// 工作参数变量/////////////////////////////////////////////////////////

EXTERN unsigned char ProductSN[10];	//产品序列号
//EXTERN unsigned char LocalAddr;		//地址码

//EXTERN unsigned char ReadIndicate;	//读卡声光指示
EXTERN unsigned char WorkMode;		//工作模式
EXTERN unsigned char DebugEn;


EXTERN unsigned char OutputPort;		// 自动数据输出接口

EXTERN unsigned char CurrentCommPort;                //Uart0 or Uart1

EXTERN unsigned char BleRcvListenEn;


EXTERN unsigned short Com3TimeOutCnt;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#undef EXTERN

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lpc17xx.h"
#include "type.h"
#include "gpiocfg.h"
#include "uart.h"
#include "rtc.h"
#include "delay.h"
//#include "cmdDef.h"
#include "core_cm3.h"
#include <string.h> 
#include "delay.h" 
#include "memory.h"
#include "timer.h"
#include "IAP.h"
#include "utility.h"
#include "buffer.h"
#include "ssp.h"
#include "memory.h"
#include "Nrf24l01.h"
#include "GPRS.h"
#include "ServerProtocal.h"
#include "comprotocol.h"

void EnableIrq(void);
void DisableIrq(void);
void StartBeep(unsigned int time);
void TagAutoRead(void);
void EtherNetInit(void);
void WdtFeed(void);
//Struct_Uart_Parameters uart0;
//Struct_Uart_Parameters uart1;


