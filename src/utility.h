/*****************************************************
Project : EPC&ISO18000 Series interrogator  
Version  : 1.0   
File name: "utility.h"
Date     : 2005-12-10
Author   : Wu zuowen
Company  : Technology
Comments : 
*****************************************************/
#ifdef  __SRC_UTILITY
#define EXTERN 
#else
#define EXTERN extern
#endif

#define uint8 unsigned char 
#define uint16 unsigned short
#define uint32 unsigned int

#define int8 char
#define int16 short
EXTERN unsigned char Log10Tab[360];
EXTERN void Delay(unsigned int i);
EXTERN void Delay_us(unsigned short iTime);
EXTERN void Delay_10us(unsigned int iTime);
EXTERN void Delay_100us(unsigned int iTime);
EXTERN void Delay_100us_WDT(unsigned int iTime);
EXTERN void Delay_ms(unsigned int iTime);
EXTERN unsigned char CheckSum(unsigned char *uBuff, unsigned char uBuffLen) ;//reentrant;

EXTERN unsigned char XOR_Cal(unsigned char *uBuf,unsigned char num);
EXTERN unsigned char HexToASCII(unsigned char data_hex);
EXTERN unsigned char ASCIIToHex(unsigned char uData);
EXTERN unsigned short CRC16(unsigned char *ptr, unsigned char len);

unsigned int DecCH2Num(unsigned char *ascBuf,unsigned char c_cnt);
unsigned char StrSearch(unsigned char * src,char * ch_str,unsigned short src_len,unsigned char ch_len);
void DelayUs(unsigned int us);
unsigned int  Ascii2Dec(char *ascBuf, unsigned char num);

void StrToHex(uint8_t *pbDest, char *pbSrc, int nLen);
unsigned char* StrSplit(unsigned char* srcbuf,unsigned char *splitbuf,unsigned char ch);


#undef EXTERN
