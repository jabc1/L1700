/*****************************************************

******************************************************/
#define __SRC_UTILITY
#include "main.h"
#include "math.h"
#include "ctype.h"

#undef __SRC_UTILITY

unsigned short const crc16_table[256]=   //* CRC余式表
{
    0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
    0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
    0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
    0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
    0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
    0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
    0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
    0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
    0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
    0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
    0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
    0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
    0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
    0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
    0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
    0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
    0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
    0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
    0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
    0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
    0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
    0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
    0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
    0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
    0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
    0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
    0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
    0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
    0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
    0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
    0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
    0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

void Delay(unsigned int i)
{
    while(i--);
}
void Delay_us(unsigned short iTime)
{
    unsigned int i,j;
    for(i=0; i<iTime; i++)
    {
        for (j=0; j<8; j++);
    }
}

void Delay_10us(unsigned int iTime)
{
    unsigned int i,j;
    for(i=0; i<iTime; i++)
    {
        for (j=0; j<200; j++);
    }
}

void Delay_100us(unsigned int  iTime)
{
    unsigned int i,j;
    for(i=0; i<iTime; i++)
    {
        for (j=0; j<3000; j++);
    }
}
void Delay_100us_WDT(unsigned int  iTime)
{
    unsigned int i,j;
    for(i=0; i<iTime; i++)
    {
        for (j=0; j<2000; j++)WdtFeed();;
    }
}
void DelayUs(unsigned int us)
{
    LPC_TIM2->TC = 0;
    LPC_TIM2->PR = 0;
    LPC_TIM2->MCR = 0x03;
    LPC_TIM2->MR0 = (Fpclk*us)/1000000;
    LPC_TIM2->TCR = 0x01;
    while (!((LPC_TIM2->IR) & 0x01));
    (LPC_TIM2->IR) = 0x01;
    LPC_TIM2->TCR = 0x00;
}

void Delay_ms(unsigned int iTime)
{
    //DelayCnt = iTime;
}

unsigned char Byte_Parity_Cal(unsigned char Data)
{
    Data = (Data & 0x0F) ^ (Data >> 4);
    Data = (Data & 0x03) ^ (Data >> 2);
    Data = (Data & 0x01) ^ (Data >> 1);
    return Data;
}

unsigned char  CheckSum(unsigned char *uBuff, unsigned char uBuffLen) //reentrant
{
    unsigned char i,uSum=0;
    for(i=0; i<uBuffLen; i++)
    {
        uSum = uSum + uBuff[i];
    }
    uSum = (~uSum) + 1;
    return uSum;
}

unsigned char XOR_Cal(unsigned char *uBuf,unsigned char num)
{
    unsigned char i;
    unsigned char result=0;
    for(i=0; i<num; i++)
        result=result^uBuf[i];
    return  result;
}

unsigned char HexToASCII(unsigned char data_hex)
{
    unsigned char ASCII_Data;
    ASCII_Data=data_hex & 0x0F;
    if(ASCII_Data<10)
        ASCII_Data=ASCII_Data+0x30;
    else
        ASCII_Data=ASCII_Data+0x37;
    return ASCII_Data;
}


void Ascii2Hex(unsigned char *hexBuf, char *ascBuf, unsigned char hexbufLen)
{
    unsigned char i,index;
    unsigned char temp;

    index = 0;
    for(i = 0; i < hexbufLen; i++)
    {
        if((ascBuf[index] <= 0x39)&&(ascBuf[index] >= 0x30))
        {
            temp = ascBuf[index] - 0x30;
        }
        else if((ascBuf[index] <= 'F')&&(ascBuf[index] >= 'A'))
            temp = ascBuf[index] - 0x37;
        else
            temp = 0;

        hexBuf[i] = temp<<4;

        index++;

        if((ascBuf[index] <= 0x39)&&(ascBuf[index] >= 0x30))
        {
            temp = ascBuf[index] - 0x30;
        }
        else if((ascBuf[index] <= 'F')&&(ascBuf[index] >= 'A'))
            temp = ascBuf[index] - 0x37;

        hexBuf[i] += temp;
        index++;
    }
}


unsigned int  Ascii2Dec(char *ascBuf, unsigned char num)
{
    unsigned char i,t;
    unsigned int d = 0;

    for(i = 0; i < num; i++)
    {
        d*=10;
        if((ascBuf[i] <= 0x39)&&(ascBuf[i] >= 0x30))
        {
            t = ascBuf[i] - 0x30;
        }
        else
            t = 0;

        d+=t;

    }
    return d;
}


unsigned short CRC16(unsigned char * Data,unsigned char Length)
{
    unsigned short crc;
    unsigned char da;

    crc = 0xFFFF;
    while(Length--!=0)
    {
        da=(unsigned char) (crc/256);
        crc <<= 8;
        crc ^= crc16_table[da^*Data];
        Data++;
    }

    return crc;
}


unsigned int DecCH2Num(unsigned char *ascBuf,unsigned char c_cnt)
{
    unsigned char i,t;
    unsigned int d = 0;

    for(i = 0; i < c_cnt; i++)
    {

        if((ascBuf[i] <= 0x39)&&(ascBuf[i]>=0x30))
        {
            t = ascBuf[i] - 0x30;
        }
        else if(ascBuf[i] == ',')
        {
            break;
        }
        else
        {
            t=0;
        }
        d*=10;
        d+=t;

    }
    return d;
}

unsigned char Hex2Dec(unsigned char hex)    //???100???????
{
    unsigned char l,h;

    l = hex%10;
    h = hex/10;

    l += (h<<4);

    return l;
}

unsigned char StrSearch(unsigned char * src,char * ch_str,unsigned short src_len,unsigned char ch_len)
{
    unsigned short i,j;

    if(src_len < ch_len)
        return 2;
    for(i = 0; i <= (src_len-ch_len); i++)
    {
        for(j = 0; j < ch_len; j++)
        {
            if(src[i+j] != ch_str[j])
                break;
        }
        if(j == ch_len)
            return 0;           //???
    }
    return 1;

}

/*
// C prototype : void StrToHex(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 输出缓冲区
//  [IN] pbSrc - 字符串
//  [IN] nLen - 16进制数的字节数(字符串的长度/2)
// return value:
// remarks : 将字符串转化为16进制数
*/
void StrToHex(uint8_t *pbDest, char *pbSrc, int nLen)
{
    char h1,h2;
    uint8_t s1,s2;
    int i;

    for (i=0; i<nLen; i++)
    {
        h1 = pbSrc[2*i];
        h2 = pbSrc[2*i+1];

        s1 = toupper(h1) - 0x30;
        if (s1 > 9)
            s1 -= 7;

        s2 = toupper(h2) - 0x30;
        if (s2 > 9)
            s2 -= 7;

        pbDest[i] = s1*16 + s2;
    }
}


unsigned char* StrSplit(unsigned char* srcbuf,unsigned char *splitbuf,unsigned char ch)
{
    unsigned char *p;
	  unsigned int i;
    p=srcbuf;
    
    while(*p!=ch)
    {
        splitbuf[i++]=*p;
        p++;
        if(i==15)//前30字节没有，号
        {
            return NULL;
        }
    }
    p=p+1;
    return p;

}
