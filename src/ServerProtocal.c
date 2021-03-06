/***************************************************************************************************




***************************************************************************************************/
#define __SRC_SERVERPROTOCAL
#include "main.h"
#undef __SRC_SERVERPROTOCAL

#define HEART_BEAT_INTERVAL     (30*100)                //心跳包发送间隔，30S

enum
{
    get_time,
    get_time_rsp,
    send_data,
    send_data_rsp,
    heartbeat,
    heartbeat_rsp,
    update_req,
    update_rsp,
    update_confirm,
    update_confirm_rsp,
    upgrade_req,
    upgrade_rsp
} SERVER_COMM_STATE;

enum
{
    update_fail,
    update_succeed
} UPDATE_STATE;

static uint16_t GprsPreSendLen;

enum
{
    req,
    confirm,
    upgrade_data,
    upgrade_end
} UPDATE_NEXT_PACK; //下一包数据类型的指示

static char ConfirmContext[32];
unsigned char UpdateStrLenth;
unsigned char UpdateServerDNSLen;
unsigned char UpdateServerPortLen;
unsigned int UpgradeFileSize;
unsigned int BakeFileSize;
unsigned char CRCVal[2];
unsigned char UpgradeFlag;

static unsigned char FileData[256];

/*****************************************************************************

******************************************************************************/
void ServerFillDataPack(uint8_t *data)
{
    char str[16];

    memcpy(GprsTcpSendBuf+GprsTcpSendLen,"A2 1000,",8);
    GprsTcpSendLen += 8;
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,GprsIMEI,14);//DeviceID
    GprsTcpSendLen += 14;

    sprintf(str,",%02X%02X,",FIRMWARE_MAJOR,FIRMWARE_MINOR);    //设备软件版本
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,6);
    GprsTcpSendLen +=6;

    sprintf(str,"%02X%02X%02X%02X%02X,",data[0],data[1],data[2],data[3],data[4]);   //标签ID
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,11);
    GprsTcpSendLen +=11;

    sprintf(str,"%02X%02X,",Tag_MAJOR,Tag_MINOR);    //标签软件版本
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,5);
    GprsTcpSendLen +=5;

    sprintf(str,"00,");
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,3);                                   //时间
    GprsTcpSendLen +=3;

    sprintf(str,",,,,0000,\r\n");
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,11);
    GprsTcpSendLen += 11;

}

/*****************************************************************************

******************************************************************************/
void ServerFillHeartbeatPack(void)
{
    char str[16];
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,"send 4000,",10);
//    GprsTcpSendLen += 10;
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,GprsIMEI,14);//DeviceID
//    GprsTcpSendLen += 14;

//    sprintf(str,",%02X%02X,",FIRMWARE_MAJOR,FIRMWARE_MINOR);
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,6);
//    GprsTcpSendLen +=6;

//    sprintf(str,"0000000000,");
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,11);
//    GprsTcpSendLen +=11;

//    sprintf(str,"0001,00,");
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,8);
//    GprsTcpSendLen +=8;

//    sprintf(str,"%02X%02X%02X%02X%02X,",SystemTime.RTC_Mon,SystemTime.RTC_Mday,SystemTime.RTC_Hour,SystemTime.RTC_Min,SystemTime.RTC_Sec);
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,11);
//    GprsTcpSendLen +=11;

//    sprintf(GprsTcpSendBuf+GprsTcpSendLen,",,,0000,\r\n");

//    GprsTcpSendLen += 10;

    memcpy(GprsTcpSendBuf+GprsTcpSendLen,"A2 4000,",8);
    GprsTcpSendLen += 8;
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,GprsIMEI,14);//DeviceID
    GprsTcpSendLen += 14;

    sprintf(str,",,");
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,2);
    GprsTcpSendLen +=2;

    sprintf(str,",");
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,1);
    GprsTcpSendLen +=1;

    sprintf(str,",,");
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,2);
    GprsTcpSendLen +=2;

//    sprintf(str,",,,,");
//    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,4);
//    GprsTcpSendLen +=4;

    sprintf(str,",,,,,\r\n");
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,7);
    GprsTcpSendLen += 7;
  


}

/////////////////////////// 更新请求数据包/////////////////////////////
void ServerUpdateReqPack(void)          //2016-10-18 GH
{
    char str[16];
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,"send 4000,",10);
    GprsTcpSendLen += 10;
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,GprsIMEI,14);//DeviceID
    GprsTcpSendLen += 14;
    sprintf(str,",%02X%02X,",FIRMWARE_MAJOR,FIRMWARE_MINOR);    //设备软件版本
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,6);
    GprsTcpSendLen +=6;
}

///////////////////////////更新确认回复/////////////////////////////

void ServerConfirmPack(char*confirm_str)          //2016-10-18 GH
{
    int len;

    memcpy(GprsTcpSendBuf+GprsTcpSendLen,"send ",5);
    GprsTcpSendLen += 5;
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,GprsIMEI,14);//DeviceID
    GprsTcpSendLen += 14;
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,",",1);
    GprsTcpSendLen += 1;

    if(UpgradeFlag)
    {
        if(UPDATE_STATE)
        {
            sprintf(confirm_str,"%s SUCC",Firmware_Ver);
        }
        else
        {
            sprintf(confirm_str,"%s FAIL",Firmware_Ver);
        }

    }
    len = strlen(confirm_str);
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,confirm_str,len);
    GprsTcpSendLen += len;

}

void ServerUpgradeReqPack(void)
{
    static
    char str[32];
    char str_lenth;
    sprintf(str,"send Get_upgrade_file,%d",Packet_Sn);
    str_lenth=strlen(str);
    memcpy(GprsTcpSendBuf+GprsTcpSendLen,str,str_lenth);
    //Paket_Sn++;
    GprsTcpSendLen += 17;
}

static __INLINE void SendBackup(void)
{
    GprsPreSendLen = GprsTcpSendLen;
}

static __INLINE void ResendLastFrame(void)
{
    GprsTcpSendLen = GprsPreSendLen;
}

/*****************************************************************************

******************************************************************************/
void ServerGetTimePack(void)
{
    //char str[16];
    memcpy(GprsTcpSendBuf,"send get_datetime\r\n",19);
    GprsTcpSendLen = 19;
}

uint8_t ServerRcvTimeHandle(void)
{

    return 0;
}

uint8_t ServerDataRspHandle(void)
{
    return 0;
}

//服务器若有数据更新，返回 A0 gateway.google.com,2020, END_END
uint8_t ServerHeartbeatRspHandle(void)
{
    //unsigned char i;

    if(GprsTcpRcvBuf[0] == '1')
        return 0;

    else if(memcmp((char*)GprsTcpRcvBuf,"A0",2) == 0)      //需要更新
    {
        memset(UpdateServerDNS,0,32);
        sscanf((char*)GprsTcpRcvBuf+2,"%[^,],%d,",UpdateServerDNS,(int*)(&UpdateServerPort));// 取出更新服务器的域名和端口
        ServerUpdateFlag = 1;                           // 通知GPRS 断开网络，再进入更新服务器
        UPDATE_NEXT_PACK = req;
    }
    return 0;
}

////////////////////////// 更新返回数据处理/////////////////////////////
//域名: A1 gateway01.baojutong.com,2020, END_END
//参数: A2 OFF,ON,180, END_END
//查询: AA  ALL END_END
//升级: FF 000A,1024, END_END
void ServerUpdateRspHandle(void)            //2016-10-18 GH
{
    char buf_beep[4]= {0};
    char buf_filter[4]= {0};
    uint32_t filter_window;
    //unsigned char str_serverport[2];
    //unsigned char dns_lenth=0;


    UpgradeFlag=0;
    if(memcmp(GprsTcpRcvBuf,"A1",2) == 0)
    {
        memset(MainServerDNS,0,32);
        sscanf((char*)GprsTcpRcvBuf+2,"%[^,],%d,",MainServerDNS,(int*)(&MainServerPort));
        sprintf(ConfirmContext,"A1 %s,%d, SUCC",MainServerDNS,MainServerPort);

       // dns_lenth=strlen(MainServerDNS);//存网络参数到FLASH中
        //str_serverport[0]=MainServerPort>>8;
       // str_serverport[1]=MainServerPort&0xff;

        memcpy(ParameterNet.DistDNS,MainServerDNS,32);
        ParameterNet.DistPort = MainServerPort;
        ParameterNet.ValidCode = 0x55AA;
        FlashSetParam(BASIC_PARA_ADDR,NET_PARA_ADDR, (uint8_t *)(&ParameterNet),40);

        UPDATE_NEXT_PACK = confirm;
    }
    else if(memcmp(GprsTcpRcvBuf,"A2",2) == 0)
    {
        sscanf((char*)GprsTcpRcvBuf+2,"%[^,],%[^,],%d,",buf_beep,buf_filter,&filter_window);
        sprintf(ConfirmContext,"A2 %s,%s,%d, SUCC",buf_beep,buf_filter,filter_window);

        //在这里保存参数
        if(memcmp((char*)buf_beep,"ON",2) == 0)
        {
            ParameterList.ReadIndicate |= 0x02;
        }
        else
        {
            ParameterList.ReadIndicate &= ~0x02;
        }
        if(memcmp((char*)buf_filter,"ON",2) == 0)
        {
            ParameterList.FilterUse=1;
        }
        else
        {
            ParameterList.FilterUse=0;
        }
        ParameterList.FilterWindow=filter_window;

        ParameterList.FilterValid = 0xAA;
        ParameterList.IndicateValid = 0xAA;
        ParameterList.WindowValid = 0xAA;

        FlashSetParam(BASIC_PARA_ADDR,0,(uint8_t *)(&ParameterList),8);
        UPDATE_NEXT_PACK = confirm;
    }
    else if(memcmp(GprsTcpRcvBuf,"AA",2) == 0)
    {
        if(ParameterList.ReadIndicate&0x02)
        {
            strcpy(buf_beep,"ON");
        }
        else
            strcpy(buf_beep,"OFF");
        if(ParameterList.FilterUse == 0)
        {
            strcpy(buf_filter,"OFF");
        }
        else
            strcpy(buf_filter,"ON");

        sprintf(ConfirmContext,"AA ALL %s,%d,%s,%s,%d,%d, SUCC",MainServerDNS,MainServerPort,buf_beep,buf_filter,\
                ParameterList.FilterWindow,FIRMWARE_VERSION);
        UPDATE_NEXT_PACK = confirm;
    }
    else if(memcmp(GprsTcpRcvBuf,"FF",2) == 0)
    {

        //获取文件基本信息
        UpgradeFlag=1;
        Packet_Sn=0;
        Update_Finish_Flag=0;
        sscanf((char*)GprsTcpRcvBuf+2,"%[^,],%d,",Firmware_Ver,&UpgradeFileSize);
        BakeFileSize=UpgradeFileSize;
        UPDATE_NEXT_PACK = upgrade_data;
    }
    else
    {

    }
}


////////////////////////// 服务器收到确认回复后应答处理/////////////////////////////

void ServerConfirmRspHandle(void)          //2016-10-18 GH
{
    if(GprsTcpRcvBuf[0]=='1')//收到1则表示应用更新
    {
        if(UpgradeFlag&&UPDATE_STATE)
        {
            UpgradeFlag=0;
            UPDATE_STATE=update_fail;
            UpdateUserFlag();
            NVIC_SystemReset();
        }
    }
    ServerUpdateFlag = 0;
}

uint8_t InterFlashEraseBackup(void)
{
    uint8_t status;
    status = SelSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
    if ( status == CMD_SUCCESS)
    {
        status = EraseSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
    }
    return status;
}

uint8_t InterFlashWrite(uint32_t addr,uint8_t *dat,uint16_t len)
{
    uint8_t status;

    status = SelSector(BAKEUP_START_SECTOR, BAKEUP_END_SECTOR);
    if (status == CMD_SUCCESS)
    {
        memcpy((char*)IapDataBuf,dat,256);
        status = RamToFlash((uint32_t)IapDataBuf, (int)dat, len);
    }
    return status;

}

void ServerUpgradeDataHandle(void)
{
    static unsigned char UpgradeState=0;
    unsigned char status;
    unsigned short crcval;
    unsigned char crc[2];
    char *p;
    unsigned char first_str[15];
    int paket_sn_rsp;
    switch (UpgradeState)
    {
        case 0://第一包数据处理
            if(memcmp((char*)GprsTcpRcvBuf,"Begin_Begin",11)==0)//第一帧数据
            {
                status = InterFlashEraseBackup();//成功收到起始符则擦除FLASH
                if(status != CMD_SUCCESS)//失败则直接返回
                {
                    Update_Finish_Flag=1;
                    UPDATE_STATE=update_fail;
                    return;
                }
                memset(first_str,0,15);
                p=StrSplit(&GprsTcpRcvBuf[12], first_str, ',');//在TCP接收缓存中查找第一个"，"字符；保存"，"前面的字符串并返回"，"字符后的字符地址
                if(p!=NULL)
                {
                    sscanf(first_str,"%*s%d",&paket_sn_rsp);//提取字符中的数字
                    StrToHex(FileData,p,256);//明码转HEX
                }
                else
                {
                    Update_Finish_Flag=1;
                    UPDATE_STATE=update_fail;
                    return;
                }
                if(Packet_Sn==paket_sn_rsp)//收到的序列号与发送的序列号匹配
                {
                    Packet_Sn++;
                }
                else//直接返回失败
                {
                    Update_Finish_Flag=1;
                    UPDATE_STATE=update_fail;
                    return;
                }
                FlashAddr=BAKEUP_START_SECTOR;
                status = InterFlashWrite(FlashAddr,FileData,256);//写第一包数据到FLASH
                if (status == CMD_SUCCESS)
                {
                    FlashAddr += 256;
                    UpgradeFileSize=UpgradeFileSize-256;
                    UpgradeState=1;//跳转到中间包接收
                }
                else
                {
                    Update_Finish_Flag=1;
                    UPDATE_STATE=update_fail;
                    return;
                }

            }
            else
            {
                Update_Finish_Flag=1;
                UPDATE_STATE=update_fail;
                return;
            }
            break;
        case 1:
            if(UpgradeFileSize<256)//最后一包字节数不满256字节按照256字节写入
            {
                UpgradeFileSize=256;
            }
            memset(first_str,0,15);
            p=StrSplit(&GprsTcpRcvBuf[0], first_str, ',');//分割出升级数据
            if(p!=NULL)
            {
                sscanf(first_str,"%*s%d",&paket_sn_rsp);//提取字符中的数字
                StrToHex(FileData,p,256);//明码转HEX
            }
            else
            {
                Update_Finish_Flag=1;
                UpgradeState=0;
                UPDATE_STATE=update_fail;
                return;
            }

            if(Packet_Sn==paket_sn_rsp)
            {
                Packet_Sn++;
            }
            else
            {
                Update_Finish_Flag=1;
                UpgradeState=0;
                UPDATE_STATE=update_fail;
                return;
            }
            status = InterFlashWrite(FlashAddr,FileData,256);//写FLASH操作
            if (status == CMD_SUCCESS)
            {
                FlashAddr += 256;
                UpgradeFileSize = UpgradeFileSize-256;
                if(UpgradeFileSize==0)
                {
                    UpgradeState=2;//跳转到数据校验
                }
            }
            else
            {
                Update_Finish_Flag=1;
                UpgradeState=0;
                UPDATE_STATE=update_fail;
                return;
            }
            break;
        case 2:
            StrToHex(crc,GprsTcpRcvBuf,2);//将前CRC四字节明码转十六进制
            crcval=(crc[0]<<8)|crc[1];//提取CRC值
            if(crcval!=CRC16((unsigned char *)BAKEUP_START_SECTOR,BakeFileSize))//crc值与本地计算的值相同
            {
                UpgradeState=0;
                UPDATE_STATE=update_fail;
                return;
            }
            else
            {
                UPDATE_STATE=update_succeed;//校验成功
            }
            Update_Finish_Flag=1;
            break;
        default:
            break;
    }
}

void ServerProtocalInit(void)
{
    SERVER_COMM_STATE = send_data;
}


void ServerProtocalTask(void)
{
    uint8_t buf[16];
    uint8_t len,ret;
    static uint16_t time_cnt = 2500;

    if(GprsTcpConStatus == 0)
        return;
    switch(SERVER_COMM_STATE)
    {
        case get_time:
            ServerGetTimePack();                    //数据填装后，GPRS的程序会自动发送出去
            SERVER_COMM_STATE = get_time_rsp;
            GprsTcpRcvStatus = 0;
            break;
        case get_time_rsp:
            if(GprsTcpRcvStatus == 1)               //正确返回
            {
                if(0 ==ServerRcvTimeHandle())
                {
                    SERVER_COMM_STATE = send_data;
                }
                else
                {
                    SERVER_COMM_STATE = get_time;
                }
                GprsTcpRcvStatus = 0;
            }
            else if(GprsTcpRcvStatus == 2)              //超时
            {
                SERVER_COMM_STATE = get_time;
                GprsTcpRcvStatus = 0;
            }
            break;
        case send_data:
            time_cnt++;
            if(time_cnt >= HEART_BEAT_INTERVAL)
            {
                time_cnt = 0;
                SERVER_COMM_STATE = heartbeat;
                break;
            }
            if(OutPutBufQueue.cnt)
            {
                BufDeQueue(&OutPutBufQueue,&len,buf);
                ServerFillDataPack(buf+2);
                //  do
                //  {
                //      if(GprsBufIsFull())
                //          break;
                //      BufDeQueue(&OutPutBufQueue,&len,buf);
                //      ServerFillDataPack(buf+2);
                //  }while(OutPutBufQueue.cnt);     //装载所有缓存数据
                SendBackup();
                SERVER_COMM_STATE = send_data_rsp;
            }
            break;
        case send_data_rsp:                         //不处理发送出错信息，因为发送出错GPRS程序会重发
            if(GprsTcpRcvStatus == 1)               //正确返回
            {
                GprsTcpRcvStatus = 0;
                ServerDataRspHandle();
                SERVER_COMM_STATE = send_data;      //不论服务器回什么，都要重新组新的数据发

            }
            else if(GprsTcpRcvStatus == 2)      //超时
            {
                ResendLastFrame();//重发上一包数据
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Refill\r\n");
#endif
                GprsTcpRcvStatus = 0;
            }
            break;
        case heartbeat:
            ServerFillHeartbeatPack();
            SERVER_COMM_STATE = heartbeat_rsp;
            break;
        case heartbeat_rsp:
            if(GprsTcpRcvStatus == 1)
            {
                GprsTcpRcvStatus = 0;
                if(0 == ServerHeartbeatRspHandle())      //这个处理里面会有很多结果
                {
                    if(ServerUpdateFlag)                    //2016-10-18 GH
                    {
                        SERVER_COMM_STATE = update_req;     //进入更新流程//2016-10-18 GH
                        GprsState = at_cipclose;          // 用close 还是shut 要测试2016-10-18 GH
                    }
                    else
                    {
                        SERVER_COMM_STATE = send_data;
                    }
                }
                else
                {
                    SERVER_COMM_STATE = send_data;//get_time;
                }
            }
            else if(GprsTcpRcvStatus == 2)
            {
                SERVER_COMM_STATE = send_data;
            }
            break;
        case update_req://发送更新请求包
            ServerUpdateReqPack();
            SERVER_COMM_STATE = update_rsp;
            GprsTcpRcvStatus = 0;
            SendBackup();
            break;
        case update_rsp://接收到服务器返回的更新信息需求
            if(GprsTcpRcvStatus == 1)
            {
                GprsTcpRcvStatus = 0;
                ServerUpdateRspHandle();
                if(UpgradeFlag)// 是固件升级请求
                {
                    SERVER_COMM_STATE = upgrade_req;
                }
                else//其他请求
                {
                    SERVER_COMM_STATE = update_confirm;
                }
            }
            else if(GprsTcpRcvStatus == 2)      //超时
            {
                ResendLastFrame();//重发上一包数据
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Refill\r\n");
#endif
                GprsTcpRcvStatus = 0;
            }
            break;
        case update_confirm://确认收到更新需求并返回数据接收情况给服务器
            ServerConfirmPack(ConfirmContext);
            SERVER_COMM_STATE = update_confirm_rsp;
            GprsTcpRcvStatus = 0;
            SendBackup();
            break;
        case update_confirm_rsp://接收服务器的更新应用确认包
            if(GprsTcpRcvStatus == 1)
            {
                GprsTcpRcvStatus = 0;
                ServerConfirmRspHandle();
                SERVER_COMM_STATE = heartbeat;
                GprsState = at_cipclose;
                ServerUpdateFlag = 0;

            }
            else if(GprsTcpRcvStatus == 2)      //超时
            {
                ResendLastFrame();//重发上一包数据
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Refill\r\n");
#endif
                GprsTcpRcvStatus = 0;
            }

            break;
        case upgrade_req:
            ServerUpgradeReqPack();
            SERVER_COMM_STATE=upgrade_rsp;
            GprsTcpRcvStatus = 0;
            SendBackup();
            break;
        case upgrade_rsp:
            if(GprsTcpRcvStatus == 1)
            {
                GprsTcpRcvStatus = 0;
                if(Update_Finish_Flag)
                {
                    SERVER_COMM_STATE=update_confirm;
                }
                else
                {
                    ServerUpgradeDataHandle();
                    SERVER_COMM_STATE=upgrade_req;
                }
            }
            else if(GprsTcpRcvStatus == 2)      //超时
            {
                ResendLastFrame();//重发上一包数据
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Refill\r\n");
#endif
                GprsTcpRcvStatus = 0;
            }
            break;
        default:
            break;
    }
}

