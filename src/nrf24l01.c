#define __SRC_NRF
#include "main.h"
#undef __SRC_NRF

#define NRF_SPI_WR				SSP1_WR

#define RX_ADR_WIDTH 3
#define RX_PLOAD_WIDTH 8
unsigned char rssi;
unsigned char TAG_ch;
#if 0
unsigned char tempsum[3]={0};
unsigned char const TX_ADDRESS[5]  = {0x34,0x43,0x10,0x10,0x02}; // Define a static TX address
unsigned char const RX_ADDRESS[5] ={0xF2,0xA2,0xEF,0x10,0x01};//{0x34,0x43,0x10,0x10,0x01};//
const unsigned char Key[6] = {0xAC,0x11,0x69,0xFD,0xFC,0xDF};//{0xAB,0x01,0x89,0xCD,0x45,0xEF};
#else
unsigned char tempsum[3]={0};
unsigned char const TX_ADDRESS[5]  = {0x34,0x43,0x10,0x10,0x02}; // Define a static TX address
unsigned char const RX_ADDRESS[5] ={0xF2,0xA2,0xEF,0x10,0x01};//{0x34,0x43,0x10,0x10,0x01};//04号段
const unsigned char Key[6] = {0xAC,0x11,0x69,0xFD,0xFC,0xDF};
#endif
 const unsigned char CheckTab[256]=
{
  0x78,0x79,0x7A,0x7B,0x74,0x75,0x76,0x77,0x70,0x71,0x72,0x73,0x6C,0x6D,0x6E,0x6F,
  0x68,0x69,0x6A,0x6B,0x64,0x65,0x66,0x67,0x60,0x61,0x62,0x63,0x1C,0x1D,0x1E,0x1F,
  0x18,0x19,0x1A,0x1B,0x14,0x15,0x16,0x17,0x10,0x11,0x12,0x13,0x0C,0x0D,0x0E,0x0F,
  0x08,0x09,0x0A,0x0B,0x04,0x05,0x06,0x07,0x00,0x01,0x02,0x03,0x3C,0x3D,0x3E,0x3F,
  0x38,0x39,0x3A,0x3B,0x34,0x35,0x36,0x37,0x30,0x31,0x32,0x33,0x2C,0x2D,0x2E,0x2F,
  0x28,0x29,0x2A,0x2B,0x24,0x25,0x26,0x27,0x20,0x21,0x22,0x23,0xDC,0xDD,0xDE,0xDF,
  0xD8,0xD9,0xDA,0xDB,0xD4,0xD5,0xD6,0xD7,0xD0,0xD1,0xD2,0xD3,0xCC,0xCD,0xCE,0xCF,
  0xC8,0xC9,0xCA,0xCB,0xC4,0xC5,0xC6,0xC7,0xC0,0xC1,0xC2,0xC3,0xFC,0xFD,0xFE,0xFF,
  0xF8,0xF9,0xFA,0xFB,0xF4,0xF5,0xF6,0xF7,0xF0,0xF1,0xF2,0xF3,0xEC,0xED,0xEE,0xEF,
  0xE8,0xE9,0xEA,0xEB,0xE4,0xE5,0xE6,0xE7,0xE0,0xE1,0xE2,0xE3,0x9C,0x9D,0x9E,0x9F,
  0x98,0x99,0x9A,0x9B,0x94,0x95,0x96,0x97,0x90,0x91,0x92,0x93,0x8C,0x8D,0x8E,0x8F,
  0x88,0x89,0x8A,0x8B,0x84,0x85,0x86,0x87,0x80,0x81,0x82,0x83,0xBC,0xBD,0xBE,0xBF,
  0xB8,0xB9,0xBA,0xBB,0xB4,0xB5,0xB6,0xB7,0xB0,0xB1,0xB2,0xB3,0xAC,0xAD,0xAE,0xAF,
  0xA8,0xA9,0xAA,0xAB,0xA4,0xA5,0xA6,0xA7,0xA0,0xA1,0xA2,0xA3,0x5C,0x5D,0x5E,0x5F,
  0x58,0x59,0x5A,0x5B,0x54,0x55,0x56,0x57,0x50,0x51,0x52,0x53,0x4C,0x4D,0x4E,0x4F,
  0x48,0x49,0x4A,0x4B,0x44,0x45,0x46,0x47,0x40,0x41,0x42,0x43,0x7C,0x7D,0x7E,0x7F
};

 const unsigned char CheckTab2[256]=
{
  0xD2,0x78,0xF3,0xD6,0x32,0x9A,0x98,0x31,0xDD,0xF8,0x73,0xD9,0x90,0x80,0xC4,0xA0,
  0xC2,0x68,0xE3,0xC6,0x22,0x8A,0x88,0x21,0xCD,0xE8,0x63,0xC9,0xE0,0xF0,0xB4,0xD0,
  0xB2,0x18,0x93,0xB6,0x52,0xFA,0xF8,0x51,0xBD,0x98,0x13,0xB9,0xF0,0xE0,0xA4,0xC0,
  0xA2,0x08,0x83,0xA6,0x42,0xEA,0xE8,0x41,0xAD,0x88,0x03,0xA9,0xC0,0xD0,0x94,0xF0,
  0x92,0x38,0xB3,0x96,0x72,0xDA,0xD8,0x71,0x9D,0xB8,0x33,0x99,0xD0,0xC0,0x84,0xE0,
  0x82,0x28,0xA3,0x86,0x62,0xCA,0xC8,0x61,0x8D,0xA8,0x23,0x89,0x20,0x30,0x74,0x10,
  0x72,0xD8,0x53,0x76,0x92,0x3A,0x38,0x91,0x7D,0x58,0xD3,0x79,0x30,0x20,0x64,0x00,
  0x62,0xC8,0x43,0x66,0x82,0x2A,0x28,0x81,0x6D,0x48,0xC3,0x69,0x00,0x10,0x54,0x30,
  0x52,0xF8,0x73,0x56,0xB2,0x1A,0x18,0xB1,0x5D,0x78,0xF3,0x59,0x10,0x00,0x44,0x20,
  0x42,0xE8,0x63,0x46,0xA2,0x0A,0x08,0xA1,0x4D,0x68,0xE3,0x49,0x60,0x70,0x34,0x50,
  0x32,0x98,0x13,0x36,0xD2,0x7A,0x78,0xD1,0x3D,0x18,0x93,0x39,0x70,0x60,0x24,0x40,
  0x22,0x88,0x03,0x26,0xC2,0x6A,0x68,0xC1,0x2D,0x08,0x83,0x29,0x40,0x50,0x14,0x70,
  0x12,0xB8,0x33,0x16,0xF2,0x5A,0x58,0xF1,0x1D,0x38,0xB3,0x19,0x50,0x40,0x04,0x60,
  0x02,0xA8,0x23,0x06,0xE2,0x4A,0x48,0xE1,0x0D,0x28,0xA3,0x09,0xA0,0xB0,0xF4,0x90,
  0xF2,0x58,0xD3,0xF6,0x12,0xBA,0xB8,0x11,0xFD,0xD8,0x53,0xF9,0xB0,0xA0,0xE4,0x80,
  0xE2,0x48,0xC3,0xE6,0x02,0xAA,0xA8,0x01,0xED,0xC8,0x43,0xE9,0x80,0x90,0xD4,0xB0
};

	
void NrfComPortInit(void)			//通信端口初始化
{

}

static __inline void NRF_CS_LOW(unsigned char ch)
{
	switch(ch)
	{
		case NRF_A:
			CLR_NRF_A_CSN();                   // CSN low, init SPI transaction
			break;
		case NRF_B:
			CLR_NRF_B_CSN();                   // CSN low, init SPI transaction
			break;
		case NRF_C:
			CLR_NRF_C_CSN();                   // CSN low, init SPI transaction
			break;
		case NRF_D:
			CLR_NRF_D_CSN();                   // CSN low, init SPI transaction
			break;
		default:
			break;

	}
}

static __inline void NRF_CS_HIGH(unsigned char ch)
{
	switch(ch)
	{
		case NRF_A:
			SET_NRF_A_CSN();                   // CSN low, init SPI transaction
			break;
		case NRF_B:
			SET_NRF_B_CSN();                   // CSN low, init SPI transaction
			break;
		case NRF_C:
			SET_NRF_C_CSN();                   // CSN low, init SPI transaction
			break;
		case NRF_D:
			SET_NRF_D_CSN();                   // CSN low, init SPI transaction
			break;
		default:
			break;

	}
}

static __inline void NRF_CE_LOW(unsigned char ch)
{
	switch(ch)
	{
		case NRF_A:
			CLR_NRF_A_CE();                   // CSN low, init SPI transaction
			break;
		case NRF_B:
			CLR_NRF_B_CE();                   // CSN low, init SPI transaction
			break;
		case NRF_C:
			CLR_NRF_C_CE();                   // CSN low, init SPI transaction
			break;
		case NRF_D:
			CLR_NRF_D_CE();                   // CSN low, init SPI transaction
			break;
		default:
			break;

	}
}
static __inline void NRF_CE_HIGH(unsigned char ch)
{
	switch(ch)
	{
		case NRF_A:
			SET_NRF_A_CE();                   // CSN low, init SPI transaction
			break;
		case NRF_B:
			SET_NRF_B_CE();                   // CSN low, init SPI transaction
			break;
		case NRF_C:
			SET_NRF_C_CE();                   // CSN low, init SPI transaction
			break;
		case NRF_D:
			SET_NRF_D_CE();                   // CSN low, init SPI transaction
			break;
		default:
			break;

	}
}

unsigned char GET_NRF_IRQ(unsigned char ch)
{
	switch(ch)
	{
		case NRF_A:
			return(GET_NRF_A_IRQ()!=0);                   // 
		case NRF_B:
			return(GET_NRF_B_IRQ()!=0);                   // 
		case NRF_C:
			return(GET_NRF_C_IRQ()!=0);                   // 
		case NRF_D:
			return(GET_NRF_D_IRQ()!=0);                   // 
		default:
			return 0;

	}	
}

unsigned char  SPI_RW_Reg(unsigned char ch,unsigned char reg, unsigned char value)
{
	unsigned char  status;
	NRF_CS_LOW(ch);
  	status = NRF_SPI_WR(reg);      // select register
  	NRF_SPI_WR(value);             // ..and write value to it..
	NRF_CS_HIGH(ch);
  	return(status);            // return nRF24L01 status byte
}

/**************************************************/
 
void SPI_Write(unsigned char ch,unsigned char c)
{ 

    NRF_CS_LOW(ch);               // CSN low, initialize SPI communication...
	NRF_SPI_WR(c);    // ..then read registervalue
	NRF_CS_HIGH(ch);               // CSN high, terminate SPI communication

}   
/**************************************************
Function: SPI_Read();

Description:
  Read one byte from nRF24L01 register, 'reg'
**************************************************/
unsigned char SPI_Read(unsigned char ch,unsigned char reg)
{
	unsigned char reg_val;

  	NRF_CS_LOW(ch);               // CSN low, initialize SPI communication...
  	NRF_SPI_WR(reg);            // Select register to read from..
  	reg_val = NRF_SPI_WR(0);    // ..then read registervalue
  	NRF_CS_HIGH(ch);              // CSN high, terminate SPI communication

  	return(reg_val);        // return register value
}
/**************************************************/

/**************************************************
Function: SPI_Read_Buf();

Description:
  Reads 'bytes' #of bytes from register 'reg'
  Typically used to read RX payload, Rx/Tx address
**************************************************/
unsigned char  SPI_Read_Buf(unsigned char ch,unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
	unsigned char  status,byte_ctr;

  	NRF_CS_LOW(ch);                   		// Set CSN low, init SPI tranaction
  	status = NRF_SPI_WR(reg);       		// Select register to write to and read status byte

  	for(byte_ctr=0;byte_ctr<bytes;byte_ctr++)
    	pBuf[byte_ctr] = NRF_SPI_WR(0);    // Perform SPI_RW to read byte from nRF24L01

  	NRF_CS_HIGH(ch);                         // Set CSN high again

  	return(status);                    // return nRF24L01 status byte
}
/**************************************************/

/**************************************************
Function: SPI_Write_Buf();

Description:
  Writes contents of buffer '*pBuf' to nRF24L01
  Typically used to write TX payload, Rx/Tx address
**************************************************/
unsigned char  SPI_Write_Buf(unsigned char ch,unsigned char reg, unsigned char *pBuf, unsigned char bytes)
{
	unsigned char  status,byte_ctr;
	DisableIrq();

  	NRF_CS_LOW(ch);                   // Set CSN low, init SPI tranaction
  	status = NRF_SPI_WR(reg);    // Select register to write to and read status byte
  	for(byte_ctr=0; byte_ctr<bytes; byte_ctr++) // then write all byte in buffer(*pBuf)
    	NRF_SPI_WR(*pBuf++);
  	NRF_CS_HIGH(ch);                 // Set CSN high again
  	EnableIrq();
  	return(status);          // return nRF24L01 status byte
}

/**************************************************/

unsigned char NrfTestChip(unsigned char ch)
{
	unsigned char test;

	test = SPI_Read(ch,READ_REG + RF_CH);

	if((test != 0xFF)&&(test != 0x00))
		return 0;
	else
		return 1;
}

void EnterSleep(unsigned char ch)
{ 
    NRF_CE_LOW(ch); 
    SPI_RW_Reg(ch,WRITE_REG + CONFIG, 0x00);    
    NRF_CE_HIGH(ch);
}

void NrfRxMode(unsigned char ch,unsigned char rcv_len)
{
	DisableIrq();

	NRF_CE_LOW(ch); 
	SPI_Write_Buf(ch,WRITE_REG + TX_ADDR, (unsigned char *)TX_ADDRESS, RX_ADR_WIDTH); 
  	SPI_Write_Buf(ch,WRITE_REG + RX_ADDR_P0, (unsigned char *)RX_ADDRESS, RX_ADR_WIDTH); //
	SPI_RW_Reg(ch,WRITE_REG + EN_AA, 0x00);      // Enable Auto.Ack:Pipe0
  	SPI_RW_Reg(ch,WRITE_REG + EN_RXADDR, 0x01);  // Enable Pipe0
	SPI_RW_Reg(ch,WRITE_REG + SETUP_AW,0x01);		// 3Bytes ADDRESS
	
  	SPI_RW_Reg(ch,WRITE_REG + RF_CH, (50+ch*10));        // Select RF channel 30
  	SPI_RW_Reg(ch,WRITE_REG + RX_PW_P0, rcv_len); // Select same RX payload width as TX Payload width
  	SPI_RW_Reg(ch,WRITE_REG + RF_SETUP, 0x07);   // TX_PWR:0dBm, Datarate:1Mbps, LNA:HCURR
  	SPI_Write(ch,FLUSH_RX);
	SPI_RW_Reg(ch,WRITE_REG + STATUS,0xFF);
	SPI_RW_Reg(ch,WRITE_REG + CONFIG, 0x0F);     // Set PWR_UP bit, enable CRC(1 bytes) & Prim:RX. RX_DR enabled..
	
  	NRF_CE_HIGH(ch);
  	EnableIrq();
}

void EnDecrypt(unsigned char* key,unsigned char *dat,unsigned char length)
{
	unsigned int i;
	unsigned short k,d;

	k = key[0]<<8;
	k += key[1];
	for(i=0;i<length/2;i++)
	{
		k=(~k)*3;
		d = dat[2*i]<<8;
		d += dat[2*i+1];

		d = d^k;

		dat[2*i] = d>>8;
		dat[2*i+1] = d;
	}
}

unsigned char CheckCRC(unsigned char *src,unsigned char len)
{
	unsigned char i;
	unsigned char sum = 0;
	for(i = 0;i < len;i++)
	{
		sum += src[i];
	}
	sum = CheckTab[sum];
	return sum;
}
		
unsigned char CheckCRC2(unsigned char *src,unsigned char len)
{
	unsigned char t;
	unsigned char sum2 = 0;
	for(t = 0;t < len;t++)
	{
		sum2 += src[t];
	}
	sum2 = CheckTab2[sum2];
	return sum2;
}
void send_re(unsigned char *data,unsigned char len)
{
	unsigned char ch;
	for(ch=0;ch<len;ch++)
	{
		LPC_UART0->THR = *data++;
		while((LPC_UART0->LSR & 0x40) == 0);		
	}
}

void Query24L01(void)
{
	static unsigned int count=0;
	unsigned char ch;
	unsigned char i;
	unsigned char check_rssi=0x68;
	unsigned char check_rssi2=0x69;
	unsigned char check_rssi3=0x6A;
	unsigned char check_rssi4=0x6B;
	unsigned char check_rssi5=0x6C;

	unsigned char Tagid [6]={0};
	
	for(ch = 0; ch < 4; ch++)
	{
		if(GET_NRF_IRQ(ch)==0)
		{
			StartBeep(10);
			DisableIrq();		
			i = SPI_Read(ch,7);
			if((i&0x40)== 0x40)
			{
				TAG_ch=ch;
				SPI_Read_Buf(ch,0x61,ActiveRcvBuf, 9);					//接收数据
				rssi = SPI_Read(ch,0x1E);
				SPI_Write(ch,FLUSH_RX);
				SPI_RW_Reg(ch,WRITE_REG + STATUS, 0xFF); 
			}				
			else
				SPI_RW_Reg(ch,WRITE_REG + STATUS, 0xFF); 
			EnableIrq();
			for(i = 0;i < 6;i++)
			{
				ActiveRcvBuf[i+1] ^= Key[i];
			}

			tempsum[0]=CheckCRC(&ActiveRcvBuf[1],6);
			tempsum[1]=CheckCRC2(&ActiveRcvBuf[1],6);

//			if(tempsum[0]==ActiveRcvBuf[7] & tempsum[1]==ActiveRcvBuf[8] & (rssi == check_rssi |  rssi ==check_rssi2 |  rssi ==check_rssi3 |  rssi ==check_rssi4 |  rssi ==check_rssi5))
//			if(tempsum[0]==ActiveRcvBuf[7] & tempsum[1]==ActiveRcvBuf[8] & rssi <= check_rssi)
			//if(tempsum[0]==ActiveRcvBuf[7] & tempsum[1]==ActiveRcvBuf[8] )
			{
				if((ActiveRcvBuf[0]==0xc9)&&(rssi <= 0x68))
				//if(rssi == 0x68)
				{
					ActiveRcvBuf[9] = rssi;
//					count++;
//					ActiveRcvBuf[9] = count %1000 / 1000;
//					ActiveRcvBuf[10] = count / 1000;
//					ActiveRcvBuf[11] = count/100 % 10 ;
//					ActiveRcvBuf[12] = count%100 /10;
//					ActiveRcvBuf[13] = count%10;
					send_re(ActiveRcvBuf,10);
					AutoReadDataHandle(9,ActiveRcvBuf);
				}
				
			}
			//else
				//memset(ActiveRcvBuf, 0xFF, 24);
			///////////////////////////////////////////////////////
//			rssi &= 0x07;
//			rssi <<= 2;
//			rssi += 50;
//			sprintf(DebugStr,"T:%02X%02X%02X%02X\r\n",ActiveRcvBuf[1],ActiveRcvBuf[2],ActiveRcvBuf[3],ActiveRcvBuf[4]);
//			DebugMessage(DebugStr);
			//CLR_LED();
		}	
	}	
//   memset(ActiveRcvBuf,0xFF,24);	
} 
