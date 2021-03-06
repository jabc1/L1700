#define   __SRC_GPRS
#include "main.h"
#undef __SRC_GPRS

#define GPRS_UART           Uart2
#define GPRS_SEND         Uart2_Query_Send
//#define GPRS_SEND           Uart2_Send

#define CALL_INTERVAL       10         //   100ms调用周期, 1 秒调用10次
#define GPRS_TIME_100ms     10
#define GPRS_TIME_1S        100

uint32_t t;
uint32_t sim;
uint32_t gprs;
uint32_t netstate;
uint32_t GPRS_state;
uint32_t CGATT;
uint32_t CREG;
uint32_t ip_rev;
uint32_t conet_state;
void SendSimCmd(char *str)  //UART3??GPRS??
{
    int i;

    for(i = 0; i < 100; i++)
    {
        if(*str)
            GPRS_UART.send_buf[i] = *str;
        else
            break;
        str++;
    }
    GPRS_SEND(i);

}

/****************************************************************************
在接收数据中查找指定的字符串，查找成功返回0，查不到返回1
*****************************************************************************/
unsigned char GprsRcvSearch(char *str,unsigned char len)
{
    unsigned char i;
    for(i = 0; i<=20; i++)
    {
        if(memcmp(str,(char *)(GPRS_UART.rcv_buf+i),len)==0)
        {
            return 0;
        }
    }
    return 1;
}

/************************************************************************
在字符串中寻找指定特征串，返回匹配字符串的下一个地址
*************************************************************************/
unsigned char * DataSeek(unsigned char * src,char * ch_str,unsigned short src_len)
{
    unsigned short i,j;
    unsigned short ch_len;
    unsigned char *ptr;

    ch_len = strlen(ch_str);
    if(src_len < ch_len)
    {
#ifdef __COMPLETE_DEBUG
        DebugMessage((char*)src);
#endif
        return 0;
    }
    for(i = 0; i <= (src_len-ch_len); i++)
    {
        for(j = 0; j < ch_len; j++)
        {
            if(src[i+j] != ch_str[j])
                break;
        }
        if(j == ch_len)
        {
            ptr = src+i+j;  //指针指向后面匹配字符后面一个数
            return ptr;         //匹配上
        }
    }
    return 0;

}

/********************************************************************************************
AT指令发送和接收函数
分发送、等待和接收完成三个阶段，函数需在时间片轮询中多次调用
返回，最终返回接收完成的状态
**********************************************************************************************/
unsigned char GprsAtCmd(char* cmd,char* match_str,unsigned short waiting_time)
{
    unsigned char status;

    switch(GprsAtState)
    {
        case send:
            GprsFlushRx();
            GprsSetRcvMatch(match_str);
            SendSimCmd(cmd);
#ifdef __COMPLETE_DEBUG
            DebugMessage(cmd);
#endif
            GprsUartLockFlag = 1;
            GprsAtState = waiting;
            GprsAtTimeoutCnt = waiting_time;        //开始超时倒计时
            return AT_RCV_WAITING;
        case waiting:
            if(GprsAtTimeoutCnt)
            {
                GprsAtTimeoutCnt--;
                if(GprsAtTimeoutCnt == 0)
                {
                    GprsAtState = send;
                    return AT_RCV_TIMEOUT;
                }
                else
                {
                    status = GprsCmdRspMatchWait();
                    if(status != AT_RCV_NONE)
                    {
                        GprsAtState = send;
                    }
                    return status;
                }
            }
            break;
        default:
            GprsAtState = send;
            return 4;
    }
}

/********************************************************************************************

**********************************************************************************************/
unsigned char GprsAtSendData(unsigned char *buf,unsigned short len,unsigned short waiting_time)
{
    unsigned char status;
//    char* match_str = "ok";
    switch(GprsAtState)
    {
        case send:
            GprsFlushRx();
            GprsSetRcvMatch("SEND OK");
            memcpy(Uart2.send_buf,buf,len+1);
            Uart2_Query_Send(len);
            //  GPRS_SEND(len);//len+1
#ifdef __COMPLETE_DEBUG
            memcpy(Uart0.send_buf,buf,len+1);           //调试用
            Uart0_Query_Send(len);//len+1
#endif
            GprsAtState = waiting;
            GprsUartLockFlag = 1;
            GprsAtTimeoutCnt = waiting_time;        //开始超时倒计时
            return AT_RCV_WAITING;
        case waiting:
            if(GprsAtTimeoutCnt)
            {
                GprsAtTimeoutCnt--;
                if(GprsAtTimeoutCnt == 0)
                {
                    GprsAtState = send;
                    return AT_RCV_TIMEOUT;
                }
                else
                {
                    status = GprsCmdRspMatchWait();
                    if(status != AT_RCV_NONE)
                    {
                        GprsAtState = send;
                    }
                    return status;
                }
            }
            break;
        default:
            GprsAtState = send;
            return 4;
    }
}

/*********************************************************************************************************
设置GPRS接收匹配字符串，匹配的过程将在串口接收中同步完成，匹配字符串转入到专用全局变量中
*********************************************************************************************************/
void GprsSetRcvMatch(char *match_str)
{
    unsigned char i;

    GprsRcvMatchLen = strlen((const char *)match_str);  //取匹配字符长度
    if(GprsRcvMatchLen > 20)              //长度大于20只取前20位
        GprsRcvMatchLen = 20;
    for(i = 0; i < GprsRcvMatchLen; i++) //将匹配字符传到串口接收中
    {
        GprsRcvMatchBuf[i] = match_str[i];
    }
}


/*************************************************************************************************************
接收等待符合特征码出现，出现返回AT_RCV_OK
**************************************************************************************************************/
unsigned char GprsCmdRspMatchWait(void)
{
    unsigned char ret;

    if(GPRS_UART.rcv_len >= GprsRcvMatchLen)
    {
        ret = StrSearch(GPRS_UART.rcv_buf, GprsRcvMatchBuf,GPRS_UART.rcv_len , GprsRcvMatchLen);
        if(ret == 0)            //找到
        {
            return AT_RCV_OK;
        }
        else if(GPRS_UART.rcv_len >= 7)
        {
            ret = StrSearch(GPRS_UART.rcv_buf, "ERROR",GPRS_UART.rcv_len ,5);
            if(ret == 0)
                return AT_RCV_ERROR;
            else
                return AT_RCV_NONE;
        }
        else
        {
            return AT_RCV_NONE;
        }
    }
    else if(GPRS_UART.rcv_len >= 7)
    {
        ret = StrSearch(GPRS_UART.rcv_buf, "ERROR",GPRS_UART.rcv_len ,5);
        if(ret == 0)
            return AT_RCV_ERROR;
        else
            return AT_RCV_NONE;
    }
    return AT_RCV_NONE;
}

/*************************************************************************************************************
接收等待符合特征码出现，出现返回AT_RCV_OK
**************************************************************************************************************/
unsigned char GprsDataRcvWait(void)
{
    unsigned char *ptr_len;
    unsigned char *ptr_dat;
    unsigned char i;
    unsigned short len,dat_len;

    if(GPRS_UART.rcv_len >= 8)
    {
        ptr_len = DataSeek(GPRS_UART.rcv_buf, (char*)"+IPD,",GPRS_UART.rcv_len);        //定位到长度
        if(ptr_len != 0)            //找到
        {
            len = GPRS_UART.rcv_buf + GPRS_UART.rcv_len - ptr_len;  // +IPD:之后的剩余长度
            if(len < 2)
                return AT_RCV_NONE;
            for(i = 0; i < len; i++)
            {
                if(*(ptr_len+i) == ':')
                {
                    dat_len = DecCH2Num(ptr_len,i);
                    if(len >= dat_len+i)        //全部数据已经收全
                    {
                        memcpy(GprsTcpRcvBuf,ptr_len+i+1,dat_len);
                        GprsTcpRcvLen = dat_len ;
                        return AT_RCV_OK;
                    }
                }
                if(i >= 5)
                    return AT_RCV_ERROR;
            }
            return AT_RCV_OK;
        }
        else
        {
            return AT_RCV_NONE;
        }
    }

    return AT_RCV_NONE;

}

/********************************************************************************************************

注意:本函数中每个命令的执行，会有中间状态的返回值，遇到不确定的返回值时不进行任何处理，不能用else处理所有返回值

其中，csq,delay,和tcp_send这三个状态为过渡状态，他们的下一步状态在转到该状态
之前就定义了，保存在GprsNextState里。
*********************************************************************************************************/
void GprsTask(void)
{
unsigned char status,len,i;
    static unsigned short GprsDelayTime = 0;
    static unsigned char one_cmd_repeat_cnt;
    static unsigned char shut_err_cnt;
    unsigned char *ptr;
    char gprs_ver[32];
    char ip_addr[64];
    if(GprsDelayTime)
    {
        GprsDelayTime--;
        return;
    }
    switch(GprsState)
    {
        case pwr_off:
            if(GprsPwrEn)
            {
                if(GET_GSM_PWR_STATUS())
                {
                    GprsState = pwr_on;
#ifdef __COMPLETE_DEBUG
                    DebugMessage(">>GSM is already poweron\r\n");
#endif
                }
                else
                {
                    GSM_PWR_KEY_DOWN();
#ifdef __COMPLETE_DEBUG
                    DebugMessage(">>Press GSM pwrkey\r\n");
#endif
                    GprsDelayTime = GPRS_TIME_1S*3;
                }
                GprsState = pwr_on;
            }
            break;
        case pwr_on:
            GSM_PWR_KEY_UP();       //开电 ，延时5秒,下一状态去回显
#ifdef __COMPLETE_DEBUG
            DebugMessage(">>Gprs power on\r\n");
#endif
            GprsDelayTime = GPRS_TIME_1S*5;
            GprsState = at_test;
            one_cmd_repeat_cnt = 0;
            break;
        case at_test:
            status = GprsAtCmd("ATE0\r\n","OK",GPRS_TIME_1S*3);//去掉回显，下一状态查SIM卡
            if(status == AT_RCV_OK)
            {   GPRS_state++;
                //status ==AT_RCV_NONE;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>ATE0 OK\r\n");
#endif
#ifdef __SIMPLE_DEBUG
                DebugMessage(MESSAGE_GPRS_OK);
#endif
                GprsState = at_cpin;
                GprsDelayTime = GPRS_TIME_1S;
										if(GPRS_state>4)
							{
							  NVIC_SystemReset();
								sim=0;
							}
            }
            else if(status == AT_RCV_TIMEOUT)
            {   GPRS_state++;
                one_cmd_repeat_cnt++;
                if(one_cmd_repeat_cnt >= 3)
                {
                    GprsState = pwr_off;
                    one_cmd_repeat_cnt = 0;
                }
								if(GPRS_state>4)
							{
							  NVIC_SystemReset();
								sim=0;
							}
                else
                {
                    GprsState = at_test;
                }
								

            }
            break;
        case at_cpin:
            status = GprsAtCmd("AT+CPIN?\r\n","READY",GPRS_TIME_1S*3);//查SIM卡，下一状态查信号强度
            if(status == AT_RCV_OK)
            {
                //DebugMessage((char*)uart1.rcv_buf);
                GprsState = at_csq;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>SIM OK\r\n");
#endif
#ifdef __SIMPLE_DEBUG
                DebugMessage(MESSAGE_SIM_OK);
#endif

                GprsDelayTime = GPRS_TIME_100ms;
            }
            else if(status == AT_RCV_ERROR)
            {
									sim++;
							  GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S*10;
#ifdef __COMPLETE_DEBUG
                DebugMessage("SIM ERROR\r\n");
								if(sim>4)
							{
							  NVIC_SystemReset();
								sim=0;
							}
#endif
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;  //ADD BY:GAOHAO
                GprsDelayTime = GPRS_TIME_1S*10;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CPIN TIMEOUT\r\n");
#endif
            }
            break;
        case at_csq:       //获取信号强度，下一状态获取本机号码
            status = GprsAtCmd("AT+CSQ\r\n","OK",GPRS_TIME_1S*5);
            if(status == AT_RCV_OK)
            {
                ptr = DataSeek(GPRS_UART.rcv_buf,"+CSQ:",GPRS_UART.rcv_len);
                if(ptr != 0)
                {
                    GprsRssi = DecCH2Num(ptr,5);
                    GprsState = at_cgsn;
                    GprsDelayTime = GPRS_TIME_100ms;
				#ifdef __SIMPLE_DEBUG
					sprintf(DebugStr,"CSQ:%d.\r\n",GprsRssi);
					DebugMessage(DebugStr);
				#endif

                }
            }
            else if(status == AT_RCV_ERROR)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CSQ ERROR\r\n");
#endif
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;  //
                GprsDelayTime = GPRS_TIME_1S*10;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CSQ TIMEOUT\r\n");
#endif
            }
            break;
            /*
                    case at_cnum:                              //获取本机号码，下一状态查网络注册
                        status = GprsAtCmd("AT+CNUM\r\n","OK",GPRS_TIME_1S*3);
                        if(status == AT_RCV_OK)
                        {
                            ptr = DataSeek(GPRS_UART.rcv_buf,",",GPRS_UART.rcv_len);
                            if(ptr != 0)
                            {
                                memcpy(GprsSim,ptr+1,11);
                                GprsSimValidFlag = 1;
                                GprsSim[11] = 0;
                            }
                            GprsState = at_creg;
            #ifdef __COMPLETE_DEBUG
                            DebugMessage(GprsSim);
                            DebugMessage("\r\n");
            #endif
                            GprsDelayTime = 10;
                        }
                        else if(status == AT_RCV_ERROR)
                        {
                            GprsState = at_test;
                            GprsDelayTime = GPRS_TIME_1S;
            #ifdef __COMPLETE_DEBUG
                            DebugMessage(">>CNUM ERROR\r\n");
            #endif
                        }
                        else if(status == AT_RCV_TIMEOUT)
                        {
                            GprsState = at_test;
                            GprsDelayTime = GPRS_TIME_1S;
                        }
                        break;
            */
        case at_cgsn:                              //获取本机IMEI，下一状态查网络注册
            status = GprsAtCmd("AT+GSN\r\n","OK",GPRS_TIME_1S*3);
            if(status == AT_RCV_OK)
            {
                ptr = DataSeek(GPRS_UART.rcv_buf,"\n",GPRS_UART.rcv_len);
                if(ptr != 0)
                {
                    memcpy(GprsIMEI,ptr,15);
                    GprsSimValidFlag = 1;
                    GprsIMEI[16] = 0;
                }
                GprsState = at_creg;
#ifdef __COMPLETE_DEBUG
                DebugMessage(GprsIMEI);
                DebugMessage("\r\n");
#endif
#ifdef __SIMPLE_DEBUG
				DebugMessage(GprsIMEI);
				DebugMessage("\r\n");
#endif

                GprsDelayTime = 10;
            }
            else if(status == AT_RCV_ERROR)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>GSN ERROR\r\n");
#endif
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
            }
            break;

        case at_creg:                              //查网络注册，下一状态查GPRS附着
            status = GprsAtCmd("AT+CREG?\r\n","OK",GPRS_TIME_1S*3);
            if(status == AT_RCV_OK)
            {
                ptr = DataSeek(GPRS_UART.rcv_buf,",",GPRS_UART.rcv_len);
                if(*ptr == '1')
                {
                    GprsState = at_cgatt;
                    GprsDelayTime = GPRS_TIME_100ms;
                    one_cmd_repeat_cnt = 0;
                }
                else
                {
									  CREG++;
                    GprsDelayTime = GPRS_TIME_1S*3;
                    GprsState = at_creg;
									if(CREG >=4)
									{		CREG = 0;
                      GprsState = restart;
										if(CREG>=6)
										{
										   CREG = 0;
											 NVIC_SystemReset(); 
										}
									}
                }
            }
            else if(status == AT_RCV_ERROR)
            {
								
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CREG ERROR\r\n");

#endif
            }
            else if(status == AT_RCV_TIMEOUT)
            {   CREG++;
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
				     if(CREG==5)
							{		CREG=0;
								  NVIC_SystemReset(); 
							}
            }
            break;
        case at_cgatt:                              //查询GPRS附着
            status = GprsAtCmd("AT+CGATT?\r\n","OK",GPRS_TIME_1S*10);
            if(status == AT_RCV_OK)
            {
                ptr = DataSeek(GPRS_UART.rcv_buf,"CGATT:",GPRS_UART.rcv_len);

                if(*ptr == ' ')
                {
                    ptr++;
                }
                if(*ptr == '1')
                {
                    GprsState = at_cstt;
                    GprsDelayTime = GPRS_TIME_1S;
                    one_cmd_repeat_cnt = 0;
                }
                else
                {
                    GprsDelayTime = GPRS_TIME_1S*5;
                    one_cmd_repeat_cnt++;
                    if(one_cmd_repeat_cnt >= 6)
                    {
                        one_cmd_repeat_cnt = 0;
                        GprsState = restart;
                    }
                }
            }
            else if(status == AT_RCV_ERROR)
            {
              CGATT++; 
							GprsState = at_cgatt;
                GprsDelayTime = GPRS_TIME_1S;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CGATT ERROR\r\n");
							if(CGATT==5)
							{		CGATT=0;
								  NVIC_SystemReset(); 
							}
#endif
            }
            else if(status == AT_RCV_TIMEOUT)
            {
								CGATT++; 
                GprsState = at_creg;
                GprsDelayTime = GPRS_TIME_1S;
								if(CGATT==5)
							{		CGATT=0;
								  NVIC_SystemReset(); 
							}
            }
            break;
        case at_cstt:                   //设置APN
            status = GprsAtCmd("AT+CSTT=\"CMNET\",\"\",\"\"\r\n","OK",GPRS_TIME_1S*20);
            if(status == AT_RCV_OK)
            {
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CSTT OK\r\n");
#endif
                GprsState = at_ciicr;
            }
            else if(status == AT_RCV_ERROR)         //可能是哪些原因呢?
            {
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CSTT ERROR\r\n");
#endif
                one_cmd_repeat_cnt++;
                if(one_cmd_repeat_cnt < 2)
                {
                    GprsState = at_cstt;
                    GprsDelayTime = GPRS_TIME_1S*5;
                }
                else
                {
                    GprsState = restart;
                    one_cmd_repeat_cnt = 0;
                }
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
            }
            break;
        case at_ciicr:                  //建立无线链路
            status = GprsAtCmd("AT+CIICR\r\n","OK",GPRS_TIME_1S*30);
            if(status == AT_RCV_OK)
            {
                GprsState = at_cifsr;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIICR OK\r\n");
#endif
                GprsDelayTime = GPRS_TIME_1S;
            }
            else if(status == AT_RCV_ERROR)         //可能是哪些原因呢?
            {  
							 GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S*3;

            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S*3;
            }
            break;
        case at_cifsr:                  //获取本机IP
            status = GprsAtCmd("AT+CIFSR\r\n",".",GPRS_TIME_1S*5);
            if(status == AT_RCV_OK)
            {
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIFSR OK\r\n");
                DebugMessage((char*)(GPRS_UART.rcv_buf));
                DebugMessage("\r\n");
#endif
                GprsState = at_ciphead;
            }
            else if(status == AT_RCV_ERROR)         //可能是哪些原因呢?
            {
                GprsState = at_test;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIFST ERROR\r\n");
#endif
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIFST TIMEOUT\r\n");
#endif
            }
            break;

        case at_ciphead:
            status = GprsAtCmd("AT+CIPHEAD=1\r\n","OK",GPRS_TIME_1S*5);
            if(status == AT_RCV_OK)
            {
                GprsState = at_cipstart;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIICR OK\r\n");
#endif
                GprsDelayTime = GPRS_TIME_1S;
            }
            else if(status == AT_RCV_ERROR)         //可能是哪些原因呢?
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S*3;

            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S*3;
            }
            break;

        case at_cipstart:
            //status = GprsAtCmd("AT+CIPSTART=\"TCP\",\"gateway01.baojutong.com\",2021\r\n","OK",1500);
            //  sprintf(ip_addr,"AT+CIPSTART=\"TCP\",\"%d.%d.%d.%d\",%d\r\n",DistinIp[0],DistinIp[1],DistinIp[2],DistinIp[3],DistinPort);
            //  status = GprsAtCmd(ip_addr,"OK",GPRS_TIME_1S*15);       //218.080.194.102 9992

            if(ServerUpdateFlag == 0)                                                       // 2016-10-18 GH
            {
                sprintf(ip_addr,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",MainServerDNS,MainServerPort);      // 2016-10-18 GH
            }
            else
            {
                sprintf(ip_addr,"AT+CIPSTART=\"TCP\",\"%s\",%d\r\n",UpdateServerDNS,UpdateServerPort);// 2016-10-18 GH
            }
            status = GprsAtCmd(ip_addr,"OK",GPRS_TIME_1S*15);
            if(status == AT_RCV_OK)
            {
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIPSTART OK\r\n");
#endif
                GprsState = connecting;
                GprsDelayTime = GPRS_TIME_1S;
                GprsConnectingTimeCnt = 0;
            }
            else if(status == AT_RCV_ERROR)         //可能是哪些原因呢?
            {
                GprsState = at_cipclose;
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_cipshut;  //ADD BY:GAOHAO
            }
            break;

        case connecting:
            status = GprsAtCmd("AT+CIPSTATUS\r\n","CONNECT OK",GPRS_TIME_1S*5);
            if(status == 0)         //网络连接成功
            {
                GprsState = connected;
                GprsDelayTime = 20;
                GprsConnectingTimeCnt = 0;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>TCP connet OK\r\n");
#endif
#ifdef __SIMPLE_DEBUG
                DebugMessage(MESSAGE_TCP_CONNECT);
#endif

                GprsTcpConStatus = 1;
            }
            else if(status != AT_RCV_NONE)//????????
            {			 t++;
                if(GprsRcvSearch("CONNECTING",10) != 0)
                {
                    if(GprsConnectingTimeCnt > GPRS_TIME_1S*10)     //10秒钟还没有反应，就断开重连
                    {
                        GprsState = at_cipshut;
                        GprsTcpConStatus = 0;
                        break;
                    }
                }
                GprsConnectingTimeCnt++;        //????
                if(GprsConnectingTimeCnt>GPRS_TIME_1S*15)           //??15?
                {
                    GprsState = at_cipshut;
                    GprsTcpConStatus = 0;
                }
										if(t>=4)
								{
									NVIC_SystemReset();
									t=0;
								
								}
                else if(GprsConnectingTimeCnt&0x01)
                {
                    GprsState = connecting;
                    GprsDelayTime = GPRS_TIME_1S*3;
                }
            }
            break;
        case connected:                 //TCP连接状态，可能存在假连接，情况包括服务器断开，或是很久没传数据
            GprsConnectingTimeCnt++;
            if(GprsConnectingTimeCnt >= GPRS_TIME_1S*40)            //40秒重新査一次网络状态
            {		 
                GprsState = connecting;

            }
            if((GprsTcpSendLen)&&(GprsTcpBusy == 0))
            {
                sprintf(DebugStr,"AT+CIPSEND=%d\r\n",GprsTcpSendLen);
                status = GprsAtCmd(DebugStr,">",GprsTcpSendLen);
                if(status == AT_RCV_OK) //可以开始发送
                {
                    GprsState = ip_send;
#ifdef __COMPLETE_DEBUG
                    DebugMessage(">>CIPSEND OK\r\n");
#endif
                    GprsTcpBusy = 1;
                }
                else if((status == AT_RCV_ERROR)||(status == AT_RCV_TIMEOUT))//发送出错，表示连接出现问题
                {
                    shut_err_cnt = 0;
                    GprsState = at_cipclose;
#ifdef __COMPLETE_DEBUG
                    DebugMessage(">>Send Error!!!\r\n");
#endif
                    GprsTcpConStatus = 0;
                }
            }
            break;

        case ip_send:       //发送数据
            status = GprsAtSendData(GprsTcpSendBuf,GprsTcpSendLen,GPRS_TIME_1S*30);
            if(status == AT_RCV_OK)
            {
                GprsTcpBusy = 1;//  //通知主程序发送成功
                GprsTcpSendStatus = 1;
                GprsTcpSendLen = 0;             //清掉发送长度
                GprsRcvTimeout = GPRS_TIME_1S*20;               //设20s超时
                GprsTcpBusy = 0;
                GprsTcpRcvStatus = 0;
                GprsState = ip_rcv;         //发送完直接进入接收状态
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Send OK,enter rcv\r\n");
#endif
            }
            else if(status == AT_RCV_ERROR)
            {
                GprsState = at_cipclose;
                GprsTcpBusy = 0;
                //  GprsTcpSendLen = 0;         //不清发送长度，等待下次重发
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = at_cipshut;
                GprsTcpBusy = 0;
                //  GprsTcpSendLen = 0;
            }
            break;

        case ip_rcv:
            status = GprsDataRcvWait();
            if(status== AT_RCV_OK)
            {
                GprsTcpRcvStatus = 1;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Rcv Tcp Data0.\r\n");
#endif
#ifdef __SIMPLE_DEBUG
                DebugMessage(MESSAGE_DATA_OK);
#endif					
                GprsState = connected;
                break;
            }
            GprsRcvTimeout--;
            if(GprsRcvTimeout == 0)
            {		ip_rev++;
                GprsState = connected;
                GprsRcvListenEn = 0;
                GprsTcpRcvStatus = 2;           //置超时标志
								if(ip_rev==6)
								{
									ip_rev=0;
								 NVIC_SystemReset();

								}
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>Gprs no data rcved\r\n");               //*************************************%%%%%%%%%%% 接收超时
#endif

            }
            break;

        case at_cipclose:
            status = GprsAtCmd("AT+CIPCLOSE=1\r\n","CLOSE OK",GPRS_TIME_1S*10);
            if(status == AT_RCV_OK)
            {
                GprsState = at_cipstart;
                GprsDelayTime = GPRS_TIME_1S;
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>IPCLOSE OK\r\n");
#endif
                //  GprsNextState = pwr_off;
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = restart;
                GprsDelayTime = GPRS_TIME_1S;
            }
            else if(status == AT_RCV_ERROR)
            {
                GprsState = restart;
                GprsDelayTime = GPRS_TIME_1S;
            }
            break;
        case at_cipshut:
            status = GprsAtCmd("AT+CIPSHUT\r\n","OK",GPRS_TIME_1S*10);
            if(status == AT_RCV_OK)
            {
#ifdef __COMPLETE_DEBUG
                DebugMessage(">>CIPSHUT OK\r\n");
#endif
                GprsState = at_test;
                GprsDelayTime = GPRS_TIME_1S;
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                GprsState = restart;  //ADD BY:GAOHAO
                GprsDelayTime = GPRS_TIME_1S;
            }
            break;
        case restart:
            status = GprsAtCmd("AT+CPOWD=1\r\n","DOWN",GPRS_TIME_1S*10);
            if(status == AT_RCV_OK)
            {
                GprsDelayTime = GPRS_TIME_1S*4;
                GprsState = close;
            }
            else if(status == AT_RCV_TIMEOUT)
            {
                //GSM_PWR_KEY_DOWN();
                GSM_PWR_OFF();
                GprsDelayTime = GPRS_TIME_1S*10;
                GprsState = close;
            }
            break;
        case close:
            //GSM_PWR_KEY_UP();
            GSM_PWR_ON();
#ifdef __COMPLETE_DEBUG
            DebugMessage(">>GSM Close\r\n");
#endif
            GprsDelayTime = GPRS_TIME_1S*5;
            GprsState = pwr_off;
            break;
        case gprs_idle:
            break;
        default:
            break;

    }
  
  
}

uint8_t GprsBufIsFull(void)
{
    if(GprsTcpSendLen >= GPRS_MAX_PACK_LEN)
        return 1;
    else
        return 0;
}

uint8_t GprsSendIsBusy(void)
{
    return GprsTcpBusy;
}

