#define __SRC_SSP
#include "main.h"
#undef __SRC_SSP

void  SSP1_Init(void)
{  

	LPC_SC->PCONP 	|= (1 << 10);			    			                    /* 使能SSP1外设                 */

	LPC_PINCON->PINSEL0 = (LPC_PINCON->PINSEL0	| (0xA8<<12));   			//P0.7,P0.8,P0.9,设10b			                  
 	
	LPC_GPIO0->FIOSET = (1<<9);
	LPC_SSP1->CR0  = (7 << 0) |                                            /* 每帧数据长度为8位            */
				(0 << 4) |                                            /* 设置帧格式为SPI              */
				(0<<5)|
			   (0 << 6) |	                                        /* 空闲时SCK为高电平有效        */
			   (0<< 7) |	                                        /* 数据在SCK的第1个时钟沿采样   */
			   (0 << 8);		                                        /* 设SSP1通讯位速率为默认值     */					
	LPC_SSP1->CPSR = 20;			    				                    /* 设置SSP1从Pclk获得时钟分频值 */
	                                                                    						/* 主模式下有效, 最小值为0x02   */
	LPC_SSP1->CR1  = (0 << 3)                                              /* SOD  从机输出禁能,0=允许	    */
             | (0 << 2)                                              /* MS   主从选择,0=主机,1=从机  */
             | (1 << 1)                                              /* SSE  SSP使能                 */
             | (0 << 0);                                             /* LBM  回写模式	            */
	                                                                    /* 不使用回写模式,并使能        */
																																				
}

unsigned char SSP1_WR(unsigned char dat)
{
	while ( !(LPC_SSP1->SR & SSPSR_TNF) );
	LPC_SSP1->DR = dat;
	while ( LPC_SSP1->SR & SSPSR_BSY );
	return (LPC_SSP1->DR);	
}

void SSP0_Init(void)
{
	LPC_SC->PCONP 	|= (1 << 21);			    			                    /* 使能SSP0外设                 */	

	LPC_PINCON->PINSEL0 = (LPC_PINCON->PINSEL0 | (0x80000000));				    /* 设置P0.15,P0.17,P0.18为SSP0功能          */
	LPC_PINCON->PINSEL1 = (LPC_PINCON->PINSEL1 | (0x0A<<2));	
	LPC_SSP0->CR0  = (0x07 << 0) |                                            /* 每帧数据长度为8位            */
				(0x00 << 4) |                                            /* 设置帧格式为SPI              */
			   (0x00 << 6) |	                                        /* 空闲时SCK为高电平有效        */
			   (0x01<< 7) |	                                        /* 数据在SCK的第1个时钟沿采样   */
			   (0x00 << 8);		                                        /* 设SSP0通讯位速率为默认值     */					
	LPC_SSP0->CPSR = 100;			    				                    /* 设置SSP0从Pclk获得时钟分频值 */
	                                                                    /* 主模式下有效, 最小值为0x02   */
	LPC_SSP0->CR1  = (0x00 << 3)                                              /* SOD  从机输出禁能,0=允许	    */
             | (0x00 << 2)                                              /* MS   主从选择,0=主机,1=从机  */
             | (0x01 << 1)                                              /* SSE  SSP使能                 */
             | (0x00 << 0);                                             /* LBM  回写模式	            */
	                                                                    /* 不使用回写模式,并使能        */	
}

unsigned char SSP0_WR(unsigned char dat)
{
	LPC_SSP0->DR = dat;
	while((LPC_SSP0->SR & 0x01) == 0);
	DelayUs(10);
	return (LPC_SSP0->DR);
}
