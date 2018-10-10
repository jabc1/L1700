#ifndef __IAP_H
#define __IAP_H


typedef void (*IAP)(uint32 param_tab[], uint32 result_tab[]);

#define		IAP_ENTER_ADR   0x1FFF1FF1

#define     IAP_SELECTOR        	50  
#define     IAP_RAMTOFLASH      	51 
#define     IAP_ERASESECTOR     	52  
#define     IAP_BLANKCHK        	53 
#define     IAP_READPARTID      	54  
#define     IAP_BOOTCODEID      	55  
#define     IAP_COMPARE         	56

#define     CMD_SUCCESS          	0
#define     INVALID_COMMAND      	1
#define     SRC_ADDR_ERROR       	2 
#define     DST_ADDR_ERROR       	3
#define     SRC_ADDR_NOT_MAPPED  	4
#define     DST_ADDR_NOT_MAPPED  	5
#define     COUNT_ERROR          	6
#define     INVALID_SECTOR       	7
#define     SECTOR_NOT_BLANK     	8
#define     SECTOR_NOT_PREPARED_FOR_WRITE_OPERATION 9

#define     COMPARE_ERROR        	10
#define     BUSY                 	11

#define		IAP_FCCLK            48000			   // In KHz

#define USER_CODE_FLAG_SECTOR			4
#define BAKEUP_START_SECTOR				11
#define BAKEUP_END_SECTOR					16
#define BAKEUP_START_ADDR					0xB000
#define USER_CODE_FLAG_ADDR				0x4000
#define MAIN_START_ADDR				0x5000
#define Start_Error_ADDR				4
#define Enddd_Error_ADDR				17
#define SECTOR_SIZE 0x1000
#define RAM_START 0x10000000
#define RAM_END	  0x10007FFF


#define SECTOR_SIZE 0x1000
#define RAM_START 0x10000000
#define RAM_END	  0x10007FFF

uint32  SelSector(uint8 sec1, uint8 sec2);
uint32  RamToFlash(uint32 dst, uint32 src, uint32 no);
uint32  EraseSector(uint8 sec1, uint8 sec2);
uint32  BlankCHK(uint8 sec1, uint8 sec2);
uint32  ReadParID(void);
unsigned char IsUserCodeValid(void);
uint32  BootCodeID(void);
uint32  Compare(uint32 dst, uint32 src, uint32 no);

unsigned char UpdateUserFlag(void);
unsigned char EraseUserFlag(void);
void CopyVectorTableToRAM(void);
void VectorRemapToRAM(void);
void ExecuteUserCode(void);
void MCUResetToBoot(void);

#endif
