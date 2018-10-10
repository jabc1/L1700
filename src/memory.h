#ifdef __SRC_MEMERY
#define EXTERN
#else
#define     EXTERN      extern
#endif

#define WRITE       0x02  /* Write to Memory instruction */
#define READ        0x03  /*Read from Memory instruction */
#define FSTREAD     0x0B  /* FAST Read from Memory instruction */
#define RDID        0x90  /* Read ??ID */
#define SE          0x20  /* Sector Erase instruction */
#define BE          0xD8  /* Bulk Erase instruction */
#define CE          0xC7  /* ????*/
#define WIP_Flag    0x01  /* Write In Progress (WIP) flag */
#define Dummy_Byte  0x00  /* Dummy Byte The data on the line not care*/

#define SPI_FLASH_PageSize 256


#define         WREN         0x06         // Set Write Enable Latch
#define         WRDI         0x04         // Write Disable
#define         RDSR         0x05         // Read Status Register
#define         WRSR         0x01         // Write Status Register
#define         RDMD         0x03         // Read Memory Data
#define         WRMD         0x02         // Write Memory Data




/*----- Low layer function -----*/
void SPI_FLASH_ChipErase(void);
unsigned char SPI_FLASH_ReadByte(void);
unsigned char SPI_FLASH_SendByte(uint8_t byte);
unsigned short SPI_FLASH_SendHalfWord(uint16_t HalfWord);
void SPI_FLASH_WriteEnable(void);
void SPI_FLASH_WriteDisable(void);
void SPI_FLASH_WaitForWriteEnd(void);




void SPI_FLASH_Write_Protection(uint8_t value);

uint16_t SPI_FLASH_ReadID(uint8_t ID_Addr);

void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_FLASH_SectorErase(uint32_t SectorAddr);



void GetWorkParameters(void);

void FlashSetParam(uint32_t base_addr,uint8_t offset,uint8_t *param,uint8_t len);
void FlashGetParam(uint32_t base_addr,uint8_t offset,uint8_t *param,uint8_t len);


#undef EXTERN

