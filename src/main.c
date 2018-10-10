/*********************************************************************************************

MAIN程序区地址 0x5000



2016年12月8日，将编译器优化等级变为2，减少代码。没做老化测试，常规版本建议改回为0.
**********************************************************************************************/
#define __SRC_MAIN
#include "main.h"
#include "math.h"
#undef __SRC_MAIN



unsigned int BLstate;
unsigned char Passwardtate;
unsigned int Res_Flash;
unsigned int  Serversize;

void EnableIrq()
{
    __enable_irq();
}

void DisableIrq()
{
    __disable_irq();
}

void WdtInit(void)
{
    LPC_WDT->WDTC = 8000000;// 8s
    LPC_WDT->WDMOD = 0x03;
    LPC_WDT->WDFEED = 0xAA;
    LPC_WDT->WDFEED = 0x55;
}

void WdtFeed(void)
{
    LPC_WDT->WDFEED = 0xAA;
    LPC_WDT->WDFEED = 0x55;
}
void EX1_Init(void)
{
    LPC_PINCON->PINSEL4 |= (1<<22);         //P2.10口
    LPC_SC->EXTMODE &= 0x0C;                // 电平触发方式
    LPC_SC->EXTPOLAR &= 0x0C;               // 低电平
    LPC_SC->EXTINT |= (1<<1);
    NVIC_EnableIRQ(EINT1_IRQn);
}

void StartBeep(unsigned int time)
{
    if(ParameterList.ReadIndicate & 0x02)
        SET_BEEP();
    if(ParameterList.ReadIndicate & 0x01)
        SET_LED();
    BeepOn=1;
    BeepDelayCnt=0;
    BeepDelayPeriod=time;

}

void Var_init()
{
    Uart0TimeoutEn = 0;
    Uart2TimeoutEn = 0;

    BeepOn = 0;
    GprsSimValidFlag = 0;
    GprsTcpBusy = 0;

    //ReadIndicate = 1;
    BleRcvListenEn = 0;
    BleAuthFlag=0;
    BleUpdateFlag=0;

//    NETParaValidFlag=0xff;
//    ParameterList.Valid_Flag=0xff;
	
	BtRandomAddr = 0xFF;

    ServerUpdateFlag = 0;
	DebugEn = 1;
}

void PinDirInit()
{
    LPC_GPIO0->FIODIR |= (NRF_CSN_D_PIN|FLASH_CS_PIN);

    LPC_GPIO1->FIODIR |= (NRF_CE_A_PIN|NRF_CSN_A_PIN|NRF_CE_B_PIN|NRF_CSN_B_PIN);
    LPC_GPIO1->FIODIR |= (NRF_CE_C_PIN|LED_PIN|BEEP_PIN|BT_WAKEUP_PIN|BT_RST_PIN);
    LPC_GPIO1->FIODIR |= GSM_PWR_EN;
    LPC_GPIO2->FIODIR |= (GSM_PWR_KEY|RF_TX_EN_PIN|RF_RX_EN_PIN);
    LPC_GPIO4->FIODIR |= (NRF_CSN_C_PIN|NRF_CE_D_PIN);

	LPC_PINCON->PINMODE3 |=(1<<13);
	LPC_PINCON->PINMODE_OD1 |=GSM_PWR_EN;
    CLR_BEEP();
    CLR_LED();
    RF_TX_DISABLE();
    RF_RX_ENABLE();
}

void McuInit(void)
{
    SystemInit();   //系统时钟初始化 pllclock=384MHz,cclk=96MHz,pclk=96MHz
    RTCInit();
    DelayUs(50000);
    PinDirInit();//IO口方向初始化
    CLR_BEEP();   //关蜂鸣器
    SysTickInit();//系统滴答计时器初始化
    Uart0Init();//串口0初始化
    Set_Uart0_Baud_Rate(4);
    Uart2Init();
    Set_Uart2_Baud_Rate(4);     //GPRS 115200
    Uart3Init();
    Set_Uart3_Baud_Rate(4);     //GPRS 115200
    SSP0_Init();//SPI1初始化
    SSP1_Init();
}
void Ble_Init()
{

    unsigned char blestr[24];
    unsigned char blename_lenth;
    Uart3.rcv_len = 0;
    Uart3.rcv_index = 0;
    CLR_BT_RST();
    Delay_ms(50);
    SET_BT_RST();
    CLR_BT_WAKEUP();
    Delay_ms(30);
    // memset(blestr,0,23);

    Uart3_SendStr("AT+ADVIN0");
    Delay_100us(3000);
    Uart3_SendStr("AT+HOSTEN0");
    Delay_100us(3000);
    Uart3_SendStr("AT+NEIN1");
    Delay_100us(3000);
    Uart3_SendStr("AT+POWR0");
    Delay_100us(3000);
    Uart3_SendStr("AT+ISCEN0");
    Delay_100us(3000);
    blename_lenth=strlen(ParameterBle.BleName);
    memcpy((char*)blestr,"AT+NAME",7);
    memcpy((char*)&blestr[7],ParameterBle.BleName,blename_lenth);
    Uart3_SendStr(blestr);
    if(StrSearch(Uart3.rcv_buf,"OK",Uart3.rcv_len,2) == 0)
    {
#ifdef __SIMPLE_DEBUG
        DebugMessage(MESSAGE_BT_OK);
        DebugStateList.BTState[0]=1;
#endif
        BleRcvListenEn = 1;
    }
}

void Systick10msHandler(void)
{
	uint8_t buf[16];
	
    if(Com3TimeOutCnt)
        Com3TimeOutCnt--;
    if(Uart0TimeoutEn)
    {
        Uart0TimeoutCnt --;
        if(!Uart0TimeoutCnt)
        {
            Uart0OvertimeError();
            Uart0TimeoutEn=0;
        }
    }
    if(Uart3TimeoutEn)
    {
        Uart3TimeoutCnt --;
        if(!Uart3TimeoutCnt)
        {
            Uart3OvertimeError();
            Uart3TimeoutEn=0;
        }
    }
    SysTick10msCnt++;
#ifdef TEST_MULTI_TAG
		if(OutPutBufQueue.cnt)
		{
			if(Uart0.be_sending_flag == 0)
			{
				BufDeQueue(&OutPutBufQueue,buf,buf);
				memcpy(pResponse_Frame->Response,buf+2,6);
				UartResponse(8);
			}

		}
#else
    ServerProtocalTask();
    GprsTask();
#endif

    if(SysTick10msCnt >= 10)
    {
        SysTick10msCnt = 0;
        SysTick100msCnt++;

        if(SysTick100msCnt >= 10)
        {
            SysTick100msCnt = 0;
            SysTick1sCnt++;             //在这里处理1秒钟的事件
            RTCGetTime(&SystemTime);
            FlushRepeatBuf();

        }
    }
}

void GetWorkParameters(void)
{
//61.130.176.206:9000
     char str[40];
    unsigned char i,dns_lenth;
    Struct_Parameter_List param;
    DistinIp[0] = 61;
    DistinIp[1] = 130;
    DistinIp[2] = 176;
    DistinIp[3] = 214;
//   DistinIp[0] = 183;
//   DistinIp[1] = 14;
//   DistinIp[2] = 175;
//   DistinIp[3] = 105;
    DistinPort = 6800;

    FlashGetParam(BASIC_PARA_ADDR,0,(uint8*)(&param),8);//读参数

    if(param.AddrValid == 0xAA)
    {
        ParameterList.LocalAddr = param.LocalAddr;
        ParameterList.AddrValid = 0xAA;
    }
    else
    {
        ParameterList.LocalAddr = 0xFF;
    }
    if(param.IndicateValid == 0xAA)
    {
        ParameterList.ReadIndicate = param.ReadIndicate;
        ParameterList.IndicateValid = 0xAA;
    }
    else
    {
        ParameterList.ReadIndicate = 0x01;
    }
    if(param.FilterValid == 0xAA)
    {
        ParameterList.FilterUse = param.FilterUse;
        ParameterList.FilterValid = 0xAA;
    }
    else
    {
        ParameterList.FilterUse = 1;
    }
    if(param.WindowValid == 0xAA)
    {
        ParameterList.FilterWindow = param.FilterWindow;
        ParameterList.WindowValid = 0xAA;
    }
    else
    {
        ParameterList.FilterWindow = 180;
    }
    if(param.WorkModeValid == 0xAA)
    {
        ParameterList.WorkMode = param.WorkMode;
        ParameterList.WorkModeValid = 0xAA;
    }
    else
    {
        ParameterList.WorkMode = WORK_MODE_ACTIVE;
    }
    FlashGetParam(BASIC_PARA_ADDR,NET_PARA_ADDR,(uint8_t*)(&ParameterNet),40);
    if(ParameterNet.ValidCode == 0x55AA)
    {
        strcpy(MainServerDNS,ParameterNet.DistDNS);
        MainServerPort = ParameterNet.DistPort;
    }
    else
    {
			strcpy(MainServerDNS,"system.zjlhtr.com");
        MainServerPort=3000;
//        strcpy(MainServerDNS,"183.14
//        strcpy(MainServerDNS,"61.153.64.8");
//        MainServerPort=45005;
//        strcpy(MainServerDNS,"183.14.174.226");
//        MainServerPort = 4500;
    }

    FlashGetParam(BASIC_PARA_ADDR,BLE_PARA_ADDR+22,&ParameterBle.ValidCode,1);
    if(ParameterBle.ValidCode==0xAA)
    {
        FlashGetParam(BASIC_PARA_ADDR,BLE_PARA_ADDR,(uint8_t*)(&ParameterBle),23);
    }
    else
    {
        memset(ParameterBle.BleName,0,16);
        memcpy(ParameterBle.BleName,"TEST",4);
        for(i=0; i<6; i++)
        {
            ParameterBle.BlePassWord[i]=0x88;
        }
    }


    pCmd_Frame = (Struct_Cmd_Frame *)(Uart0.rcv_buf);
    pResponse_Frame = (Struct_Response_Frame *)(Uart0.send_buf);
	CurrentCommPort=0;
}

void PrintfParameters(void)
{
    char str[40];
    sprintf(str,"ReadIndicate:%02X%02X\r\n",ParameterList.ReadIndicate,ParameterList.IndicateValid);
    DebugMessage(str);
    sprintf(str,"FilterUse:%02X%02X\r\n",ParameterList.FilterUse,ParameterList.FilterValid);
    DebugMessage(str);
    sprintf(str,"FilterUse:%02X%02X\r\n",ParameterList.FilterWindow,ParameterList.WindowValid);
    DebugMessage(str);

    sprintf(str,"Net:%s,%d,%04X\r\n",MainServerDNS,MainServerPort,ParameterNet.ValidCode);
    DebugMessage(str);
	sprintf(str,"5:%s,%x%x%x%x%x%x\r\n",ParameterBle.BleName,ParameterBle.BlePassWord[0],ParameterBle.BlePassWord[1],ParameterBle.BlePassWord[2],\
    ParameterBle.BlePassWord[3],ParameterBle.BlePassWord[4],ParameterBle.BlePassWord[5],ParameterBle.BlePassWord[6]);
    DebugMessage(str);
}

void Blestate()
{
		unsigned int Blescheck=0x00080000;
		unsigned int Blesdischeck=0x00000000;
		BLstate = GET_BT_STATU();
		if(BLstate!=Blescheck)
		{
		BleAuthFlag=1;
		Com3TimeOutCnt=3000;
		}
		if(BLstate ==Blescheck)
		{
		BleAuthFlag=0;
		Passwardtate=0;
		Com3TimeOutCnt=3000;
		}
}


unsigned char CheckServerDns(void)
{ 

int count=0; 

char *delims = "//";
char *temp=NULL;
char res[29];
char DNSleng;
unsigned char DNSsize=0x20;
char Checktemp[5]={0x73,0x73,0x65,0x7A,0x6C};
//system.zjlhtr.com
char CheckServerDNS1[55]={"//s//y//s//t//e//m//.//z//j//l//h//t//r//.//c//o//m//"};
    temp = strtok(CheckServerDNS1, delims);  
    res[count] = *temp;     
    while (temp != NULL)  
    {  
        temp = strtok(NULL, delims);  
        res[++count] = *temp;  
    } 
		if((memcmp(res,MainServerDNS,17)!=0)  | (MainServerDNS[0] !=Checktemp[0]) | (MainServerDNS[2] !=Checktemp[1]) | (MainServerDNS[4] !=Checktemp[2]) | (MainServerDNS[7] !=Checktemp[3])| (MainServerDNS[9] !=Checktemp[4]))
		{
					 Res_Flash = SelSector(Start_Error_ADDR,Enddd_Error_ADDR);
			if (  Res_Flash == CMD_SUCCESS)
			{
					Res_Flash = EraseSector(Start_Error_ADDR, Enddd_Error_ADDR);
					NVIC_SystemReset();
			}
		  
		}
		
		else
			return 0;
}
int main()
{
    int i;
    unsigned char buf[16];
	  WdtInit();
    McuInit(); 	// MCU 初始化
	  GSM_PWR_OFF();
    Var_init();	//常用变量初始化
	  WdtFeed();
    Delay_100us(3000);
		DebugMessage(Version_OK);
#ifdef __COMPLETE_DEBUG
    DebugMessage("--RFID Reader Poweron.\r\n");
#endif
//	SPI_FLASH_ChipErase();/////////////////////////////////////////////////////////////////////
    GetWorkParameters();        //获取配置信息
#ifdef __COMPLETE_DEBUG
    PrintfParameters();
#endif
//    BufInitQueue(&BufQueue,TagBuf,MAX_BUF_SIZE);
		WdtFeed();
    BufInitQueue(&RepeatBufQueue,RepeatTagBuf,MAX_REPEAT_BUF_SIZE);
    BufInitQueue(&OutPutBufQueue,OutputTagBuf,MAX_OUTPUT_BUF_SIZE);

    StartBeep(5);//开蜂鸣器1S
#ifdef __COMPLETE_DEBUG
    DebugMessage("--Reader Init OK.\r\n");
#endif
#ifdef __SIMPLE_DEBUG
    DebugMessage(MESSAGE_MCU_OK);
    DebugStateList.McuState=1;
#endif
    
    WdtFeed();
    ServerProtocalInit();
    EnableIrq();
    GprsState = pwr_off;
    GprsPwrEn = 1;
#ifdef TEST_MULTI_TAG
   Delay_100us(2000);
	 GSM_PWR_OFF();
	 ParameterList.FilterUse = 0;
	// ParameterList.ReadIndicate |= 0x02;
	 ParameterList.FilterWindow = 7;
#else
    
#endif
    if(ParameterList.WorkMode != WORK_MODE_PASSIVE)
    {
        for(i = 0; i < 4; i++)
        {
            NrfRxMode(i,9);
        }
        Delay_100us(200);
    }
	
    for(i = 0; i < 4; i++)
    {
        if(NrfTestChip(i)== 0)          //接口验证成功
        {
#ifdef __SIMPLE_DEBUG
            sprintf(DebugStr,"%02d\r\n",i+1);
            DebugMessage(DebugStr);
            DebugStateList.RFState[i]=1;
#endif
            Delay_100us(200);
        }

    }
		Delay_100us_WDT(6000);
    GSM_PWR_ON();
    Ble_Init();
    FlashID = SPI_FLASH_ReadID(0);
#ifdef __SIMPLE_DEBUG
    if((FlashID & 0xFF00) == 0xEF00)
    {
        DebugMessage(MESSAGE_FLASH_OK);
        DebugStateList.FlashState=1;
    }
#endif

	FlashSetParam(0,0,"123456",6);
//	CheckServerDns();
	FlashGetParam(0,0,buf,6);
    while(1)
    {
        if (Uart0.rcv_finish_flag)
        {

            CurrentCommPort=0;
            Uart0.rcv_finish_flag = 0;
            pCmd_Frame = (Struct_Cmd_Frame *)(Uart0.rcv_buf);
            pResponse_Frame = (Struct_Response_Frame *)(Uart0.send_buf);
            CommProtocol();
        }
        if(Uart3.rcv_finish_flag)
        {
            Com3TimeOutCnt=60000;//
            CurrentCommPort=3;
            Uart3.rcv_finish_flag = 0;
            pCmd_Frame = (Struct_Cmd_Frame *)(Uart3.rcv_buf);
            pResponse_Frame = (Struct_Response_Frame *)(Uart3.send_buf);
            CommProtocol();
        }
         Blestate();
        if (SysTick10msFlag)
        {
            SysTick10msFlag = 0;
            Systick10msHandler();
        }
        if(ParameterList.WorkMode != WORK_MODE_PASSIVE)
        {
            Query24L01();
        }
        WdtFeed();
    }
}
