#define __SRC_MEMERY
#include "main.h"
#undef __SRC_MEMERY

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.(24??bit??)4K
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
   __disable_irq();
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
  /* Sector Erase */ 
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  /* Send Sector Erase instruction */
  SSP0_WR(SE);
  /* Send SectorAddr high nibble address byte */
  SSP0_WR((SectorAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SSP0_WR((SectorAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SSP0_WR(SectorAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
  __enable_irq();

}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : BulkAddr:Bulk???(24??bit??)
* Output         : None
* Return         : None
*******************************************************************************/
//void SPI_FLASH_BulkErase(uint32_t BulkAddr)
void SPI_FLASH_BulkErase(uint8_t pages)
{
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();

  /* Bulk Erase */ 
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  /* Send Bulk Erase instruction  */
  SSP0_WR(BE);
   /* Send SectorAddr high nibble address byte */
  SSP0_WR(pages);//((BulkAddr & 0xFF0000) >> 16);
  /* Send SectorAddr medium nibble address byte */
  SSP0_WR(0);//((BulkAddr & 0xFF00) >> 8);
  /* Send SectorAddr low nibble address byte */
  SSP0_WR(0);//(BulkAddr & 0xFF);
  
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}
/*****************************************************************
*****???:????
****??:????FLASH
****????:?
****???:?
******************************************************************/

void SPI_FLASH_ChipErase(void)
{
  SPI_FLASH_WriteEnable();
  CLR_FLASH_CS();
  SSP0_WR(CE);
  SET_FLASH_CS();
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************

????,?????????????page?,????????????page????
?0,????256?byte,?????page???????0xff

* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value. 
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();
  
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  /* Send "Write to Memory " instruction */
  SSP0_WR(WRITE);
  /* Send WriteAddr high nibble address byte to write to */
  SSP0_WR((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SSP0_WR((WriteAddr & 0xFF00) >> 8);  
  /* Send WriteAddr low nibble address byte to write to */
  SSP0_WR(WriteAddr & 0xFF);
  
  /* while there is data to be written on the FLASH */
  while(NumByteToWrite--) 
  {
    /* Send the current byte */
    SSP0_WR(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++; 
  }
  
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();
  
  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be 
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
       
	Addr = WriteAddr % SPI_FLASH_PageSize;
	count = SPI_FLASH_PageSize - Addr;
	NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
	NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
       __disable_irq();
	if(Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
	{
		if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */ 
		{
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;  
			}
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
		}
	}
	else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
	{
		if(NumOfPage== 0) /* NumByteToWrite < SPI_FLASH_PageSize */
		{
			if(NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
			{
				temp = NumOfSingle - count;
			  
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
				WriteAddr +=  count;
				pBuffer += count; 
				
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
			}
			else
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
			}
		}
		else /* NumByteToWrite > SPI_FLASH_PageSize */
		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
			NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
		  
			SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
			WriteAddr +=  count;
			pBuffer += count;  
		 
			while(NumOfPage--)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
				WriteAddr +=  SPI_FLASH_PageSize;
				pBuffer += SPI_FLASH_PageSize;
			}
		  
			if(NumOfSingle != 0)
			{
				SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
			}
		}
	}
	__enable_irq();
}


/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read 
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  __disable_irq();
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /* Send "Read from Memory " instruction */
  SSP0_WR(FSTREAD);
  
  /* Send ReadAddr high nibble address byte to read from */
  SSP0_WR((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SSP0_WR((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SSP0_WR(ReadAddr & 0xFF);
  SSP0_WR(Dummy_Byte);//??????!!!!

  
  while(NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer= SSP0_WR(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }
  
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();
  __enable_irq();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadID
* Description    : Reads FLASH ??ID,???0xEF 0x16
* Input          : None
* Output         : None
* Return         : FLASH identification
*******************************************************************************/
uint16_t SPI_FLASH_ReadID(uint8_t ID_Addr)
{
  uint16_t Temp = 0;

  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /*??W25X64PDF30??Device ID instruction*/
  SSP0_WR(0x90);
  /*3?dummy*/
  SSP0_WR(Dummy_Byte);
  SSP0_WR(Dummy_Byte);
  SSP0_WR(ID_Addr);               /* send W25Pxx selectable ID address 00H or 01H */
  Temp = SSP0_WR(Dummy_Byte)<<8;  /* receive Manufature or Device ID byte */  
  Temp|=SSP0_WR(Dummy_Byte);
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();        
  return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_StartReadSequence
* Description    : Initiates a read data byte (READ) sequence from the Flash.
*                  This is done by driving the /CS line low to select the device,
*                  then the READ instruction is transmitted followed by 3 bytes
*                  address. This function exit and keep the /CS line low, so the
*                  Flash still being selected. With this technique the whole
*                  content of the Flash is read with a single READ instruction.
* Input          : - ReadAddr : FLASH's internal address to read from.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /* Send "Read from Memory " instruction */
  SSP0_WR(READ);

/* Send the 24-bit address of the address to read from -----------------------*/  
  /* Send ReadAddr high nibble address byte */
  SSP0_WR((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte */
  SSP0_WR((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte */
  SSP0_WR(ReadAddr & 0xFF);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_ReadByte
* Description    : Reads a byte from the SPI Flash.
*                  This function must be used only if the Start_Read_Sequence
*                  function has been previously called.
* Input          : None
* Output         : None
* Return         : Byte Read from the SPI Flash.
*******************************************************************************/
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SSP0_WR(Dummy_Byte));
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /* Send "Write Enable" instruction */
  SSP0_WR(WREN);
  
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();
}
/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteDisable(void)
{
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /* Send "Write Enable" instruction */
  SSP0_WR(WRDI);
  
  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the  
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.  
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t FLASH_Status = 0;
  
  /* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /* Send "Read Status Register" instruction */
  SSP0_WR(RDSR);
  
  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH 
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SSP0_WR(Dummy_Byte);
	//WDR();
  } while((FLASH_Status & WIP_Flag) == 1); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  SET_FLASH_CS();
}
/***************************************************************************** 
* 
*    Function name : SPI_FLASH_Write_Protection 
* 
*    Returns :        
* 
*    Parameters :   
* 
*    Purpose :        
* 
******************************************************************************/
void SPI_FLASH_Write_Protection(unsigned char value)
{
  //SSP0_WR(WREN);
  SPI_FLASH_WriteEnable();

	/* Select the FLASH: Chip Select low */
  CLR_FLASH_CS();
  
  /* Send "Write Enable" instruction */
  SSP0_WR(WRSR);
	if(value==1)
		SSP0_WR(0x98);
	else
		SSP0_WR(0x00);
  /* Deselect the FLASH: Chip Select high */
	SET_FLASH_CS();

	//SSP0_WR(WRDI);
	SPI_FLASH_WriteDisable();
}


void FlashSetParam(uint32_t base_addr,uint8_t offset,uint8_t *param,uint8_t len)
{
	uint8_t temp_buf[128];
	uint8_t i;

	if(offset +len >= 128)
		return;
	SPI_FLASH_BufferRead(temp_buf,base_addr,128);
	SPI_FLASH_SectorErase(base_addr);

	for(i = 0;i < len;i++)
	{
		temp_buf[offset+i] = param[i];
	}
	SPI_FLASH_BufferWrite(temp_buf,base_addr,128);
}

void FlashGetParam(uint32_t base_addr,uint8_t offset,uint8_t *param,uint8_t len)
{
	SPI_FLASH_BufferRead(param,base_addr+offset,len);
}