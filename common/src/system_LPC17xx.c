
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------

#include <stdint.h>
#include "LPC17xx.h"
/*--------------------- Clock Configuration ----------------------------------
//
// <e> Clock Configuration
//   <h> System Controls and Status Register (SCS)
//     <o1.4>    OSCRANGE: Main Oscillator Range Select
//                     <0=>  1 MHz to 20 MHz
//                     <1=> 15 MHz to 24 MHz
//     <e1.5>       OSCEN: Main Oscillator Enable
//     </e>
//   </h>
//
//   <h> Clock Source Select Register (CLKSRCSEL)
//     <o2.0..1>   CLKSRC: PLL Clock Source Selection
//                     <0=> Internal RC oscillator
//                     <1=> Main oscillator
//                     <2=> RTC oscillator
//   </h>
//
//   <e3> PLL0 Configuration (Main PLL)
//     <h> PLL0 Configuration Register (PLL0CFG)
//                     <i> F_cco0 = (2 * M * F_in) / N
//                     <i> F_in must be in the range of 32 kHz to 50 MHz
//                     <i> F_cco0 must be in the range of 275 MHz to 550 MHz
//       <o4.0..14>  MSEL: PLL Multiplier Selection
//                     <6-32768><#-1>
//                     <i> M Value
//       <o4.16..23> NSEL: PLL Divider Selection
//                     <1-256><#-1>
//                     <i> N Value
//     </h>
//   </e>
//
//   <e5> PLL1 Configuration (USB PLL)
//     <h> PLL1 Configuration Register (PLL1CFG)
//                     <i> F_usb = M * F_osc or F_usb = F_cco1 / (2 * P)
//                     <i> F_cco1 = F_osc * M * 2 * P
//                     <i> F_cco1 must be in the range of 156 MHz to 320 MHz
//       <o6.0..4>   MSEL: PLL Multiplier Selection
//                     <1-32><#-1>
//                     <i> M Value (for USB maximum value is 4)
//       <o6.5..6>   PSEL: PLL Divider Selection
//                     <0=> 1
//                     <1=> 2
//                     <2=> 4
//                     <3=> 8
//                     <i> P Value
//     </h>
//   </e>
//
//   <h> CPU Clock Configuration Register (CCLKCFG)
//     <o7.0..7>  CCLKSEL: Divide Value for CPU Clock from PLL0
//                     <3-256><#-1>
//   </h>
//
//   <h> USB Clock Configuration Register (USBCLKCFG)
//     <o8.0..3>   USBSEL: Divide Value for USB Clock from PLL1
//                     <0-15>
//                     <i> Divide is USBSEL + 1
//   </h>
//
//   <h> Peripheral Clock Selection Register 0 (PCLKSEL0)
//     <o9.0..1>    PCLK_WDT: Peripheral Clock Selection for WDT
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.2..3>    PCLK_TIMER0: Peripheral Clock Selection for TIMER0
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.4..5>    PCLK_TIMER1: Peripheral Clock Selection for TIMER1
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.6..7>    PCLK_UART0: Peripheral Clock Selection for UART0
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.8..9>    PCLK_UART1: Peripheral Clock Selection for UART1
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.12..13>  PCLK_PWM1: Peripheral Clock Selection for PWM1
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.14..15>  PCLK_I2C0: Peripheral Clock Selection for I2C0
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.16..17>  PCLK_SPI: Peripheral Clock Selection for SPI
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.20..21>  PCLK_SSP1: Peripheral Clock Selection for SSP1
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.22..23>  PCLK_DAC: Peripheral Clock Selection for DAC
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.24..25>  PCLK_ADC: Peripheral Clock Selection for ADC
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o9.26..27>  PCLK_CAN1: Peripheral Clock Selection for CAN1
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 6
//     <o9.28..29>  PCLK_CAN2: Peripheral Clock Selection for CAN2
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 6
//     <o9.30..31>  PCLK_ACF: Peripheral Clock Selection for ACF
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 6
//   </h>
//
//   <h> Peripheral Clock Selection Register 1 (PCLKSEL1)
//     <o10.0..1>   PCLK_QEI: Peripheral Clock Selection for the Quadrature Encoder Interface
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.2..3>   PCLK_GPIO: Peripheral Clock Selection for GPIOs
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.4..5>   PCLK_PCB: Peripheral Clock Selection for the Pin Connect Block
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.6..7>   PCLK_I2C1: Peripheral Clock Selection for I2C1
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.10..11> PCLK_SSP0: Peripheral Clock Selection for SSP0
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.12..13> PCLK_TIMER2: Peripheral Clock Selection for TIMER2
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.14..15> PCLK_TIMER3: Peripheral Clock Selection for TIMER3
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.16..17> PCLK_UART2: Peripheral Clock Selection for UART2
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.18..19> PCLK_UART3: Peripheral Clock Selection for UART3
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.20..21> PCLK_I2C2: Peripheral Clock Selection for I2C2
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.22..23> PCLK_I2S: Peripheral Clock Selection for I2S
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.26..27> PCLK_RIT: Peripheral Clock Selection for the Repetitive Interrupt Timer
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.28..29> PCLK_SYSCON: Peripheral Clock Selection for the System Control Block
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//     <o10.30..31> PCLK_MC: Peripheral Clock Selection for the Motor Control PWM
//                     <0=> Pclk = Cclk / 4
//                     <1=> Pclk = Cclk
//                     <2=> Pclk = Cclk / 2
//                     <3=> Pclk = Hclk / 8
//   </h>
//
//   <h> Power Control for Peripherals Register (PCONP)
//     <o11.1>      PCTIM0: Timer/Counter 0 power/clock enable
//     <o11.2>      PCTIM1: Timer/Counter 1 power/clock enable
//     <o11.3>      PCUART0: UART 0 power/clock enable
//     <o11.4>      PCUART1: UART 1 power/clock enable
//     <o11.6>      PCPWM1: PWM 1 power/clock enable
//     <o11.7>      PCI2C0: I2C interface 0 power/clock enable
//     <o11.8>      PCSPI: SPI interface power/clock enable
//     <o11.9>      PCRTC: RTC power/clock enable
//     <o11.10>     PCSSP1: SSP interface 1 power/clock enable
//     <o11.12>     PCAD: A/D converter power/clock enable
//     <o11.13>     PCCAN1: CAN controller 1 power/clock enable
//     <o11.14>     PCCAN2: CAN controller 2 power/clock enable
//     <o11.15>     PCGPIO: GPIOs power/clock enable
//     <o11.16>     PCRIT: Repetitive interrupt timer power/clock enable
//     <o11.17>     PCMC: Motor control PWM power/clock enable
//     <o11.18>     PCQEI: Quadrature encoder interface power/clock enable
//     <o11.19>     PCI2C1: I2C interface 1 power/clock enable
//     <o11.21>     PCSSP0: SSP interface 0 power/clock enable
//     <o11.22>     PCTIM2: Timer 2 power/clock enable
//     <o11.23>     PCTIM3: Timer 3 power/clock enable
//     <o11.24>     PCUART2: UART 2 power/clock enable
//     <o11.25>     PCUART3: UART 3 power/clock enable
//     <o11.26>     PCI2C2: I2C interface 2 power/clock enable
//     <o11.27>     PCI2S: I2S interface power/clock enable
//     <o11.29>     PCGPDMA: GP DMA function power/clock enable
//     <o11.30>     PCENET: Ethernet block power/clock enable
//     <o11.31>     PCUSB: USB interface power/clock enable
//   </h>
//
//   <h> Clock Output Configuration Register (CLKOUTCFG)
//     <o12.0..3>   CLKOUTSEL: Selects clock source for CLKOUT
//                     <0=> CPU clock
//                     <1=> Main oscillator
//                     <2=> Internal RC oscillator
//                     <3=> USB clock
//                     <4=> RTC oscillator
//     <o12.4..7>   CLKOUTDIV: Selects clock divider for CLKOUT
//                     <1-16><#-1>
//     <o12.8>      CLKOUT_EN: CLKOUT enable control
//   </h>
//
// </e>
*/


/*--------------------- Flash Accelerator Configuration ----------------------
//
// <e> Flash Accelerator Configuration
//   <o1.0..1>   FETCHCFG: Fetch Configuration
//               <0=> Instruction fetches from flash are not buffered
//               <1=> One buffer is used for all instruction fetch buffering
//               <2=> All buffers may be used for instruction fetch buffering
//               <3=> Reserved (do not use this setting)
//   <o1.2..3>   DATACFG: Data Configuration
//               <0=> Data accesses from flash are not buffered
//               <1=> One buffer is used for all data access buffering
//               <2=> All buffers may be used for data access buffering
//               <3=> Reserved (do not use this setting)
//   <o1.4>      ACCEL: Acceleration Enable
//   <o1.5>      PREFEN: Prefetch Enable
//   <o1.6>      PREFOVR: Prefetch Override
//   <o1.12..15> FLASHTIM: Flash Access Time
//               <0=> 1 CPU clock (for CPU clock up to 20 MHz)
//               <1=> 2 CPU clocks (for CPU clock up to 40 MHz)
//               <2=> 3 CPU clocks (for CPU clock up to 60 MHz)
//               <3=> 4 CPU clocks (for CPU clock up to 80 MHz)
//               <4=> 5 CPU clocks (for CPU clock up to 100 MHz)
//               <5=> 6 CPU clocks (for any CPU clock)
// </e>
*/
//-------- <<< end of configuration section >>> ------------------------------




#define CLOCK_SETUP           1
#define SCS_Val               0x00000020   //主振荡器使能，主振频率范围1-20MHz
#define CLKSRCSEL_Val         0x00000001   //选中主晶振作为时钟源
#define PLL0_SETUP            1
#define PLL0CFG_Val           0x00050063   //锁相环0 12倍频
#define PLL1_SETUP            0
#define PLL1CFG_Val           0x00000023   //锁相环1 36倍频
#define CCLKCFG_Val           0x00000003   //CPU时钟4分频
#define USBCLKCFG_Val         0x00000000   //USB时钟不分频
#define PCLKSEL0_Val          0x01511054   //外设时钟4分频
#define PCLKSEL1_Val          0x00011404
#define PCONP_Val             0x4160975E   // 部分外设功率输出使能
#define CLKOUTCFG_Val         0x00000100   //选择CPU时钟作为CLKOUT的时钟源

#define FLASH_SETUP           1
#define FLASHCFG_Val          0x0000303A  //访问数据不缓冲加速禁止




/*----------------------------------------------------------------------------
  Check the register settings
 *----------------------------------------------------------------------------*/
#define CHECK_RANGE(val, min, max)                ((val < min) || (val > max))
#define CHECK_RSVD(val, mask)                     (val & mask)

/* Clock Configuration -------------------------------------------------------*/
#if (CHECK_RSVD((SCS_Val),       ~0x00000030))
   #error "SCS: Invalid values of reserved bits!"
#endif

#if (CHECK_RANGE((CLKSRCSEL_Val), 0, 2))
   #error "CLKSRCSEL: Value out of range!"
#endif

#if (CHECK_RSVD((PLL0CFG_Val),   ~0x00FF7FFF))
   #error "PLL0CFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PLL1CFG_Val),   ~0x0000007F))
   #error "PLL1CFG: Invalid values of reserved bits!"
#endif

#if ((CCLKCFG_Val != 0) && (((CCLKCFG_Val - 1) % 2)))
   #error "CCLKCFG: CCLKSEL field does not contain only odd values or 0!"
#endif

#if (CHECK_RSVD((USBCLKCFG_Val), ~0x0000000F))
   #error "USBCLKCFG: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCLKSEL0_Val),   0x000C0C00))
   #error "PCLKSEL0: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCLKSEL1_Val),   0x03000300))
   #error "PCLKSEL1: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((PCONP_Val),      0x10100821))
   #error "PCONP: Invalid values of reserved bits!"
#endif

#if (CHECK_RSVD((CLKOUTCFG_Val), ~0x000001FF))
   #error "CLKOUTCFG: Invalid values of reserved bits!"
#endif

/* Flash Accelerator Configuration -------------------------------------------*/
#if (CHECK_RSVD((FLASHCFG_Val), ~0x0000F07F))
   #error "FLASHCFG: Invalid values of reserved bits!"
#endif



#define XTAL        (12000000UL)        //晶振频率
#define OSC_CLK     (      XTAL)        //主振频率
#define RTC_CLK     (   32000UL)        //RTC时钟
#define IRC_OSC     ( 4000000UL)        //内部时钟



uint32_t SystemFrequency = IRC_OSC; 


void SystemInit (void)
{
	#if (CLOCK_SETUP)                      //启动时钟
  	LPC_SC->SCS  = SCS_Val;
  	if (SCS_Val & (1 << 5))
	{             
    	while ((LPC_SC->SCS & (1<<6)) == 0);//等待主振稳定
  	}

  	LPC_SC->CCLKCFG   = CCLKCFG_Val;      //CPU时钟4分频

	#if (PLL0_SETUP)
  	LPC_SC->CLKSRCSEL = CLKSRCSEL_Val;    //选定选择主振作为PLL0的时钟源
  	LPC_SC->PLL0CFG   = PLL0CFG_Val;		//锁相环倍频值和分频值选择
  	LPC_SC->PLL0CON   = 0x01;             // 锁相环0使能
  	LPC_SC->PLL0FEED  = 0xAA;
  	LPC_SC->PLL0FEED  = 0x55;
  	while (!(LPC_SC->PLL0STAT & (1<<26)));//等待频率锁定

  	LPC_SC->PLL0CON   = 0x03;            //锁相环0使能与连接
  	LPC_SC->PLL0FEED  = 0xAA;
  	LPC_SC->PLL0FEED  = 0x55;
	#endif

	#if (PLL1_SETUP)
  	LPC_SC->PLL1CFG   = PLL1CFG_Val;
  	LPC_SC->PLL1CON   = 0x01;            
 	 LPC_SC->PLL1FEED  = 0xAA;
  	LPC_SC->PLL1FEED  = 0x55;
  	while (!(LPC_SC->PLL1STAT & (1<<10)));

  	LPC_SC->PLL1CON   = 0x03;            
  	LPC_SC->PLL1FEED  = 0xAA;
  	LPC_SC->PLL1FEED  = 0x55;

	#else
  	LPC_SC->USBCLKCFG = USBCLKCFG_Val;    //USB时钟分频值 
	#endif

  	LPC_SC->PCLKSEL0  = PCLKSEL0_Val;     //外设时钟选择
  	LPC_SC->PCLKSEL1  = PCLKSEL1_Val;

  	LPC_SC->PCONP     = PCONP_Val;       //设置外设功率控制寄存器

  	LPC_SC->CLKOUTCFG = CLKOUTCFG_Val;    //CLKOUT时钟配置
	#endif

	if (((LPC_SC->PLL0STAT >> 24)&3)==3)  //PLL0处于使能并连接状态
    switch (LPC_SC->CLKSRCSEL & 0x03) //时钟源判断
	{
      case 0:                           //PLL0以内部RC作为时钟源
      case 3:                           //时钟源为保留位时，系统时钟以内部RC作为时钟源
             SystemFrequency = (IRC_OSC * 
                          (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                          (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /
                          ((LPC_SC->CCLKCFG & 0xFF)+ 1));
             break;
      case 1:                          //主振作为PLL0的时钟源时
             SystemFrequency = (OSC_CLK * 
                          (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                          (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /
                          ((LPC_SC->CCLKCFG & 0xFF)+ 1));
             break;
      case 2:                          //RTC作为PLL0的时钟源时
             SystemFrequency = (RTC_CLK * 
                          (((2 * ((LPC_SC->PLL0STAT & 0x7FFF) + 1))) /
                          (((LPC_SC->PLL0STAT >> 16) & 0xFF) + 1))   /
                          ((LPC_SC->CCLKCFG & 0xFF)+ 1));
             break;
    }
  	else 	   //PLL0未激活时系统时钟
  	{
    	switch (LPC_SC->CLKSRCSEL & 0x03) 
		{
      		case 0:                           
      		case 3:                           
        			SystemFrequency = IRC_OSC / ((LPC_SC->CCLKCFG & 0xFF)+ 1);
        			break;
      		case 1:                          
        			SystemFrequency = OSC_CLK / ((LPC_SC->CCLKCFG & 0xFF)+ 1);
        			break;
      		case 2:                           
        			SystemFrequency = RTC_CLK / ((LPC_SC->CCLKCFG & 0xFF)+ 1);
       			    break;
    }
  }

	#if (FLASH_SETUP == 1)                  //存储器加速模块配置
  	LPC_SC->FLASHCFG  = FLASHCFG_Val;
	#endif
}
