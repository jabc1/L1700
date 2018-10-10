/***************************************************************************************


****************************************************************************************/
#define FIRMWARE_VERSION 1

#define FIRMWARE_MAJOR	 	0
#define FIRMWARE_MINOR		3

#define Tag_MAJOR	 	0
#define Tag_MINOR		3
#define TEST_MULTI_TAG
#define __COMPLETE_DEBUG			//完整调试信息
#define __SIMPLE_DEBUG				//精简调试信息
#define SSP0		0
#define SSP1		1

#define Fpclk				100000000


////////////////////////////////// 24R1 接口//////////////////////////////////////


#define NRF_A			0
#define NRF_B			1
#define NRF_C			2
#define NRF_D			3

#define NRF_CE_A_PIN		(1<<4)		//p1.4
#define SET_NRF_A_CE()		(LPC_GPIO1->FIOSET = NRF_CE_A_PIN)
#define CLR_NRF_A_CE()		(LPC_GPIO1->FIOCLR = NRF_CE_A_PIN)

#define NRF_CSN_A_PIN		(1<<8)	//P1.8
#define SET_NRF_A_CSN()	(LPC_GPIO1->FIOSET = NRF_CSN_A_PIN)
#define CLR_NRF_A_CSN()	(LPC_GPIO1->FIOCLR = NRF_CSN_A_PIN)

#define NRF_IRQ_A_PIN		(1)		//P2.0
#define GET_NRF_A_IRQ()		(LPC_GPIO2->FIOPIN&NRF_IRQ_A_PIN)

#define NRF_CE_B_PIN		(1<<9)		//p1.9
#define SET_NRF_B_CE()		(LPC_GPIO1->FIOSET = NRF_CE_B_PIN)
#define CLR_NRF_B_CE()		(LPC_GPIO1->FIOCLR = NRF_CE_B_PIN)

#define NRF_CSN_B_PIN		(1<<10)		//P1.10
#define SET_NRF_B_CSN()	(LPC_GPIO1->FIOSET = NRF_CSN_B_PIN)
#define CLR_NRF_B_CSN()	(LPC_GPIO1->FIOCLR = NRF_CSN_B_PIN)

#define NRF_IRQ_B_PIN		(1<<1)		//p2.1
#define GET_NRF_B_IRQ()		(LPC_GPIO2->FIOPIN&NRF_IRQ_B_PIN)


#define NRF_CE_C_PIN		(1<<15)		//p1.15
#define SET_NRF_C_CE()		(LPC_GPIO1->FIOSET = NRF_CE_C_PIN)
#define CLR_NRF_C_CE()		(LPC_GPIO1->FIOCLR = NRF_CE_C_PIN)

#define NRF_CSN_C_PIN		(1<<29)		//P4.29
#define SET_NRF_C_CSN()	(LPC_GPIO4->FIOSET = NRF_CSN_C_PIN)
#define CLR_NRF_C_CSN()	(LPC_GPIO4->FIOCLR = NRF_CSN_C_PIN)

#define NRF_IRQ_C_PIN		(1<<3)		//P2.3
#define GET_NRF_C_IRQ()		(LPC_GPIO2->FIOPIN&NRF_IRQ_C_PIN)

#define NRF_CE_D_PIN		(1<<28)		//p4.28
#define SET_NRF_D_CE()		(LPC_GPIO4->FIOSET = NRF_CE_D_PIN)
#define CLR_NRF_D_CE()		(LPC_GPIO4->FIOCLR = NRF_CE_D_PIN)

#define NRF_CSN_D_PIN		(1<<6)		//P0.6
#define SET_NRF_D_CSN()	(LPC_GPIO0->FIOSET = NRF_CSN_D_PIN)
#define CLR_NRF_D_CSN()	(LPC_GPIO0->FIOCLR = NRF_CSN_D_PIN)

#define NRF_IRQ_D_PIN		(1<<2)		//P2.2
#define GET_NRF_D_IRQ()		(LPC_GPIO2->FIOPIN&NRF_IRQ_D_PIN)

#define NRF_SPI			SSP1

#define RF_TX_EN_PIN		(1<<4)
#define RF_TX_EANBLE()	(LPC_GPIO2->FIOSET = RF_TX_EN_PIN)
#define RF_TX_DISABLE()	(LPC_GPIO2->FIOCLR = RF_TX_EN_PIN)

#define RF_RX_EN_PIN		(1<<5)
#define RF_RX_ENABLE()	(LPC_GPIO2->FIOSET = RF_RX_EN_PIN)
#define RF_RX_DISABLE()	(LPC_GPIO2->FIOCLR = RF_RX_EN_PIN)

/////////////////////////////////   存储器   ////////////////////////////////////
#define FLASH_CS_PIN		(1<<16)			//P0.16
#define SET_FLASH_CS()	(LPC_GPIO0->FIOSET = FLASH_CS_PIN)
#define CLR_FLASH_CS()	(LPC_GPIO0->FIOCLR = FLASH_CS_PIN)


/////////////////////////////////   GPRS  ////////////////////////////////////
#define GSM_PWR_KEY		(1<<7)			//P2.7
#define GSM_PWR_KEY_DOWN()	(LPC_GPIO2->FIOSET = GSM_PWR_KEY)
#define GSM_PWR_KEY_UP()	(LPC_GPIO2->FIOCLR = GSM_PWR_KEY)

#define GSM_PWR_STATUS	(1<<6)
#define GET_GSM_PWR_STATUS()		(LPC_GPIO2->FIOPIN&GSM_PWR_STATUS)

#define GSM_PWR_EN		(1<<22)
#define GSM_PWR_ON()    (LPC_GPIO1->FIOCLR = GSM_PWR_EN)
#define GSM_PWR_OFF()	  (LPC_GPIO1->FIOSET = GSM_PWR_EN)

/////////////////////////////////指示灯和蜂鸣器 ////////////////////////////////////
#define LED_PIN	   (1<<24)		//p1.24
#define SET_LED()		(LPC_GPIO1->FIOSET = LED_PIN)
#define CLR_LED()		(LPC_GPIO1->FIOCLR = LED_PIN)

#define BEEP_PIN			(1<<20)		//p1.20
#define SET_BEEP()		(LPC_GPIO1->FIOSET = BEEP_PIN)
#define CLR_BEEP()		(LPC_GPIO1->FIOCLR = BEEP_PIN)


/////////////////////////////////蓝牙//////////////////////////
#define BT_WAKEUP_PIN		(1<<26)		//p1.26
#define SET_BT_WAKEUP()		(LPC_GPIO2->FIOSET = BT_WAKEUP_PIN)
#define CLR_BT_WAKEUP()		(LPC_GPIO2->FIOCLR = BT_WAKEUP_PIN)

/**************************蓝牙状态检测************************/
#define BT_STATU_PIN		(1<<19)
#define GET_BT_STATU()	(LPC_GPIO1->FIOPIN&BT_STATU_PIN)

#define BT_RST_PIN		(1<<28)//p1.28
#define SET_BT_RST()		(LPC_GPIO1->FIOSET = BT_RST_PIN)
#define CLR_BT_RST()		(LPC_GPIO1->FIOCLR = BT_RST_PIN)

////////////////////////////////////////////////  工作参数定义////////////////////////////////////////////////

//eeprom config parameters
#define ADDR_PRODUCT_SN				0x10		//产品序列号,总共6字节，一字节产品型号，3字节生产日期，2字节流水号
#define ADDR_LOCAL_ADDR				0x11		//地址码,等同IP地址末字节
#define ADDR_READ_INDICATE			0x12		//读卡声光指示

#define ADDR_DIST_IP					0x20
#define ADDR_DIST_PORT					0x24


