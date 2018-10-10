#define __SRC_SSP
#include "main.h"
#undef __SRC_SSP

void  SSP1_Init(void)
{  

	LPC_SC->PCONP 	|= (1 << 10);			    			                    /* ʹ��SSP1����                 */

	LPC_PINCON->PINSEL0 = (LPC_PINCON->PINSEL0	| (0xA8<<12));   			//P0.7,P0.8,P0.9,��10b			                  
 	
	LPC_GPIO0->FIOSET = (1<<9);
	LPC_SSP1->CR0  = (7 << 0) |                                            /* ÿ֡���ݳ���Ϊ8λ            */
				(0 << 4) |                                            /* ����֡��ʽΪSPI              */
				(0<<5)|
			   (0 << 6) |	                                        /* ����ʱSCKΪ�ߵ�ƽ��Ч        */
			   (0<< 7) |	                                        /* ������SCK�ĵ�1��ʱ���ز���   */
			   (0 << 8);		                                        /* ��SSP1ͨѶλ����ΪĬ��ֵ     */					
	LPC_SSP1->CPSR = 20;			    				                    /* ����SSP1��Pclk���ʱ�ӷ�Ƶֵ */
	                                                                    						/* ��ģʽ����Ч, ��СֵΪ0x02   */
	LPC_SSP1->CR1  = (0 << 3)                                              /* SOD  �ӻ��������,0=����	    */
             | (0 << 2)                                              /* MS   ����ѡ��,0=����,1=�ӻ�  */
             | (1 << 1)                                              /* SSE  SSPʹ��                 */
             | (0 << 0);                                             /* LBM  ��дģʽ	            */
	                                                                    /* ��ʹ�û�дģʽ,��ʹ��        */
																																				
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
	LPC_SC->PCONP 	|= (1 << 21);			    			                    /* ʹ��SSP0����                 */	

	LPC_PINCON->PINSEL0 = (LPC_PINCON->PINSEL0 | (0x80000000));				    /* ����P0.15,P0.17,P0.18ΪSSP0����          */
	LPC_PINCON->PINSEL1 = (LPC_PINCON->PINSEL1 | (0x0A<<2));	
	LPC_SSP0->CR0  = (0x07 << 0) |                                            /* ÿ֡���ݳ���Ϊ8λ            */
				(0x00 << 4) |                                            /* ����֡��ʽΪSPI              */
			   (0x00 << 6) |	                                        /* ����ʱSCKΪ�ߵ�ƽ��Ч        */
			   (0x01<< 7) |	                                        /* ������SCK�ĵ�1��ʱ���ز���   */
			   (0x00 << 8);		                                        /* ��SSP0ͨѶλ����ΪĬ��ֵ     */					
	LPC_SSP0->CPSR = 100;			    				                    /* ����SSP0��Pclk���ʱ�ӷ�Ƶֵ */
	                                                                    /* ��ģʽ����Ч, ��СֵΪ0x02   */
	LPC_SSP0->CR1  = (0x00 << 3)                                              /* SOD  �ӻ��������,0=����	    */
             | (0x00 << 2)                                              /* MS   ����ѡ��,0=����,1=�ӻ�  */
             | (0x01 << 1)                                              /* SSE  SSPʹ��                 */
             | (0x00 << 0);                                             /* LBM  ��дģʽ	            */
	                                                                    /* ��ʹ�û�дģʽ,��ʹ��        */	
}

unsigned char SSP0_WR(unsigned char dat)
{
	LPC_SSP0->DR = dat;
	while((LPC_SSP0->SR & 0x01) == 0);
	DelayUs(10);
	return (LPC_SSP0->DR);
}
