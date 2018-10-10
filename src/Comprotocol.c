#define __SRC_PROTOCAL
#include "main.h"
#undef __SRC_PROTOCAL


unsigned char FlashTempBuff[8];
unsigned short PacketSn;
extern unsigned char rssi;
extern unsigned char Passwardtate;

void UartResponse(unsigned char len)
{
int i;
if(CurrentCommPort==3)
{	
BtRandomAddr++;
if(BtRandomAddr == 0xFF)
BtRandomAddr = 0x00;
pResponse_Frame->Addr = BtRandomAddr;

}
else
pResponse_Frame->Addr = 0XFF;//ADDR
pResponse_Frame->SOF = 0x5A;
pResponse_Frame->Len = len;
pResponse_Frame->Status = rssi ;
pResponse_Frame->Response[len - 2] = CheckSum((unsigned char *)(&(pResponse_Frame->SOF)), len + 2);
if (CurrentCommPort == 0)
Uart0_Send(len + 3);
else if (CurrentCommPort == 3)
if(BleAuthFlag==0x01 && Passwardtate==0x01)
	{
		Uart3_Query_Send(len + 3);

}
else
{
   return;
}
}

void UartNoDataResponse(unsigned status)
{
int i;
pResponse_Frame->SOF = 0x5A;
if(CurrentCommPort==3)
{
BtRandomAddr++;
if(BtRandomAddr == 0xFF)
BtRandomAddr = 0x0;
pResponse_Frame->Addr = BtRandomAddr;

}
else
pResponse_Frame->Addr = 0XFF;//ADDR
pResponse_Frame->Len = 2;
pResponse_Frame->Status = status;
pResponse_Frame->Response[0] = CheckSum(&(pResponse_Frame->SOF), 4);
if (CurrentCommPort == 0)
Uart0_Query_Send(5);
else if (CurrentCommPort == 3)
Uart3_Query_Send(5);
else
{
}
}
void Uart3DataResponse()
{
int i;
pResponse_Frame->SOF = 0x5A;
if(CurrentCommPort==3)
{
BtRandomAddr++;
if(BtRandomAddr == 0xFF)
BtRandomAddr = 0x0;
pResponse_Frame->Addr = 0xFF;

}
else
pResponse_Frame->Addr = 0XFF;//ADDR
pResponse_Frame->Len = 2;
pResponse_Frame->Status = 00;
pResponse_Frame->Response[0]=0xC8;
Uart3_Query_Send(5);

}


void CommProtocol(void)
{
unsigned char cmd_type;
cmd_type = pCmd_Frame->Cmd & 0xF0;
switch (cmd_type)
{
case 0x10:
case 0x30:
	if((CurrentCommPort==3)&&(BtAddrMatchFlag == 0))
			return;
	SysProcess();
	break;
case 0x40:
	if((CurrentCommPort==3)&&(BtAddrMatchFlag == 0))
			return;
	BufProcess();
case 0x50:
	if((CurrentCommPort==3)&&(BtAddrMatchFlag == 0))
			return;
	UpgradeProcess();
	break;
case 0x60:
	BleProcess();
	break;
default:
	UartNoDataResponse(ERR_CMD_ERR);
	break;
}
}



void BleProcess(void)
{
unsigned char i;
 int t;

unsigned char blename_lenth;
switch(pCmd_Frame->Cmd)
{
case CMD_MODIFY_BLEPARA :
	if((CurrentCommPort==3)&&(BtAddrMatchFlag == 0))
			return;
	memset(&ParameterBle.BleName[0],0,16);
	memcpy(&ParameterBle.BleName[0],&pCmd_Frame->Parameters[1],pCmd_Frame->Parameters[0]);
	blename_lenth=strlen(ParameterBle.BleName);
	memcpy(&ParameterBle.BlePassWord[0],&pCmd_Frame->Parameters[blename_lenth+1],6);
	ParameterBle.ValidCode=0xAA;
	FlashSetParam(BASIC_PARA_ADDR,BLE_PARA_ADDR, &ParameterBle.BleName[0],23);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_AUTH_BLE:

	if(memcmp(ParameterBle.BlePassWord,pCmd_Frame->Parameters,6) != 0)
            {
                BtRandomAddr = 0xFF;
                DebugStateList.BTState[1]=0;
                UartNoDataResponse(ERR_AUTH_FAILED);
            }
            else
            {
//                BtRandomAddr = SysTick1sCnt;            //伪随机数
//                DebugStateList.BTState[1]=1;
//                memcpy(pResponse_Frame->Response,&DebugStateList.McuState,12);
							  Passwardtate=1;
                Uart3DataResponse();
            }
            break;
default:
	break;
}
}

void SysProcess(void)
{
int i;
unsigned char dns_lenth;
unsigned char str_port[2];
uint8_t buf[8];
Struct_Parameter_Net net_param;
switch (pCmd_Frame->Cmd)
{
case CMD_RST_READER:
	UartNoDataResponse(ERR_NO_ERR);
	Delay_100us(3000);
	//MCUResetToBoot();
	NVIC_SystemReset();
	break;
case CMD_GET_FIRMWARE:
	pResponse_Frame->Response[0] = FIRMWARE_MAJOR;
	pResponse_Frame->Response[1] = FIRMWARE_MINOR;
	UartResponse(4);
	break;
case CMD_SET_LOCAL_ADDR:
	ParameterList.LocalAddr = pCmd_Frame->Parameters[0];
	buf[0]= ParameterList.LocalAddr;
	buf[1] = 0xAA;
	FlashSetParam(BASIC_PARA_ADDR,0,buf,2);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_LOCAL_ADDR:
	pResponse_Frame->Response[0] = ParameterList.LocalAddr;
	UartResponse(3);
	break;
case CMD_SET_BEEP:
	if(pCmd_Frame->Parameters[0])
			ParameterList.ReadIndicate |= 0x02;
	else
			ParameterList.ReadIndicate &= ~0x02;
	buf[0]= ParameterList.ReadIndicate;
	buf[1] = 0xAA;
	FlashSetParam(BASIC_PARA_ADDR,2,buf,2);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_BEEP:
	if(ParameterList.ReadIndicate&0x02)
	{
			pResponse_Frame->Response[0]=0x01;
	}
	else
	{
			pResponse_Frame->Response[0]=0x00;
	}
	UartResponse(3);
	break;
case CMD_SET_LED:
	SPI_FLASH_BufferRead(&ParameterList.LocalAddr,BASIC_PARA_ADDR, 6);
	if(pCmd_Frame->Parameters[0])
			ParameterList.ReadIndicate |= 0x01;
	else
			ParameterList.ReadIndicate &= ~0x01;
	buf[0]= ParameterList.ReadIndicate;
	buf[1] = 0xAA;
	FlashSetParam(BASIC_PARA_ADDR,2,buf,2);

	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_LED:
	if(ParameterList.ReadIndicate&0x01)
	{
			pResponse_Frame->Response[0]=0x01;
	}
	else
	{
			pResponse_Frame->Response[0]=0x00;
	}
	UartResponse(3);
	break;

case CMD_SET_FILTER_EN:
	ParameterList.FilterUse = pCmd_Frame->Parameters[0];
	buf[0]= ParameterList.FilterUse;
	buf[1] = 0xAA;
	FlashSetParam(BASIC_PARA_ADDR,4,buf,2);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_FILTER_EN:
	pResponse_Frame->Response[0] = ParameterList.FilterUse;
	UartResponse(3);
	break;
case CMD_SET_FILTER_WIN:
	ParameterList.FilterWindow = pCmd_Frame->Parameters[0];
	buf[0]= ParameterList.FilterWindow;
	buf[1] = 0xAA;
	FlashSetParam(BASIC_PARA_ADDR,6,buf,2);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_FILTER_WIN:
	pResponse_Frame->Response[0] = ParameterList.FilterWindow;
	UartResponse(3);
	break;

case CMD_SET_WORK_MODE:
	ParameterList.WorkMode = pCmd_Frame->Parameters[0];
	buf[0]= ParameterList.WorkMode;
	buf[1] = 0xAA;
	FlashSetParam(BASIC_PARA_ADDR,8,buf,2);
	UartNoDataResponse(ERR_NO_ERR);
	break;

case CMD_GET_WORK_MODE:
	pResponse_Frame->Response[0] = ParameterList.WorkMode;
	UartResponse(3);
	break;

case CMD_SET_SERVER_NETPARA:
	memset(MainServerDNS,0,32);
	memcpy((char*)MainServerDNS,&pCmd_Frame->Parameters[1],pCmd_Frame->Parameters[0]);
	MainServerPort=(pCmd_Frame->Parameters[pCmd_Frame->Parameters[0]+1]<<8)+pCmd_Frame->Parameters[pCmd_Frame->Parameters[0]+2];
	memcpy(net_param.DistDNS,MainServerDNS,32);
	net_param.DistPort = MainServerPort;
	net_param.ValidCode = 0x55AA;
	FlashSetParam(BASIC_PARA_ADDR,NET_PARA_ADDR, (uint8_t *)(&net_param),40);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_SERVER_NETPARA:
	dns_lenth=strlen(MainServerDNS);
	memcpy(&pResponse_Frame->Response[0],&dns_lenth,1);
	memcpy(&pResponse_Frame->Response[1],MainServerDNS,dns_lenth);
	str_port[0]=MainServerPort>>8;
	str_port[1]=MainServerPort&0xff;
	memcpy(&pResponse_Frame->Response[dns_lenth+1],str_port,2);
	UartResponse(pResponse_Frame->Response[0]+5);
	break;
case CMD_PARAM_DEFAULT:			
SPI_FLASH_ChipErase();
	UartNoDataResponse(ERR_NO_ERR);
	Delay_100us(3000);
	NVIC_SystemReset();

break;
default:
	UartNoDataResponse(ERR_CMD_ERR);
	break;
}
}

void BufProcess(void)
{
unsigned char cnt,i;
unsigned char len;
unsigned char buf[10];
switch (pCmd_Frame->Cmd)
{
case CMD_CLEAR_BUF:
	BufInitQueue(&RepeatBufQueue,RepeatTagBuf,MAX_REPEAT_BUF_SIZE);
	BufInitQueue(&OutPutBufQueue,OutputTagBuf,MAX_OUTPUT_BUF_SIZE);
	UartNoDataResponse(ERR_NO_ERR);
	break;
case CMD_GET_BUF_CNT:
	pResponse_Frame->Response[0] = (OutPutBufQueue.cnt>>8);
	pResponse_Frame->Response[1] = (OutPutBufQueue.cnt&0xFF);
	UartResponse(4);
	break;
case CMD_GET_BUF_DATA:
	cnt = pCmd_Frame->Parameters[0];
	for(i = 0; i < cnt; i++)
	{
			if(BufDeQueue(&OutPutBufQueue,&len,buf)== true)
			{
					memcpy(&(pResponse_Frame->Response[i*6+1]),buf+1,6);
			}
			else
					break;
	}
	UartResponse(i*6+3);
	break;
default:
	UartNoDataResponse(ERR_CMD_ERR);
	break;
}

}
void UpgradeProcess(void)
{
unsigned int status;
unsigned int t, i;
switch (pCmd_Frame->Cmd)
{
case CMD_AUTHOR_UPDATE:
	if((0x49==pCmd_Frame->Parameters[0])&&(0x96==pCmd_Frame->Parameters[1])&&(0x02==pCmd_Frame->Parameters[2])&&(0xd2==pCmd_Frame->Parameters[3]))
	{
			UpdateFlag=1;
WorkMode = 1;
DebugEn = 0;
			UartNoDataResponse(0);
	}
	else
			UartNoDataResponse(1);
	break;
case CMD_START_UPDATE:
	if(UpdateFlag)
	{
			FlashAddr = BAKEUP_START_ADDR;
			FlashCount = 0;
			status = SelSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
			if ( status == CMD_SUCCESS)
			{
					status = EraseSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
			}
			UartNoDataResponse(status);
	}
	else
	{
			UpdateFlag=0;
			UartNoDataResponse(0x01);
	}
	break;
case CMD_DOWNLOAD_FILE:
	if(UpdateFlag)
	{
			status = 0;
			if(((PacketSn>>8)==pCmd_Frame->Parameters[0])&&((PacketSn&0xff)==pCmd_Frame->Parameters[1]))
			{
					t = FlashCount % 4;
					for (i = 0; i < 64; i++)
					{
							IapDataBuf[t * 64 + i] = pCmd_Frame->Parameters[i+2];
					}
					FlashCount ++;
					if (( FlashCount % 4) == 0)
					{
							status = SelSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
							if (status == CMD_SUCCESS)
							{
									status = RamToFlash(FlashAddr, (int)&IapDataBuf[0], 256);
									FlashAddr += 256;
							}
					}
					PacketSn++;
					UartNoDataResponse(status);
			}
			else
			{
					UartNoDataResponse(0x01);
			}
	}
	else
	{
			UpdateFlag=0;
			UartNoDataResponse(0x02);
	}
	break;
case CMD_END_UPDATE:
	if(UpdateFlag)
	{
			if ((FlashCount % 4) != 0)//最后一次不满256字节
			{
					status = SelSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
					if (status == CMD_SUCCESS)
					{
							status = RamToFlash(FlashAddr, (int)&IapDataBuf[0], 256);
							FlashAddr += 256;
					}
			}
			else
					status = CMD_SUCCESS;
			if ( (status == CMD_SUCCESS) && UpdateUserFlag())//升级完成更新升级标志
			{

					UartNoDataResponse(0);
					Delay_100us(1000);
					DisableIrq();
					NVIC_SystemReset();

			}
			else
			{
					UartNoDataResponse(1);
			}
	}
	else
			UartNoDataResponse(1);
	UpdateFlag=0;
	break;
default:
	break;
}
}

