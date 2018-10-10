#ifdef __SRC_GPRS
#define EXTERN
#else
#define EXTERN		extern
#endif


#define AT_RCV_OK						0
#define AT_RCV_WAITING					1
#define AT_RCV_ERROR				2
#define AT_RCV_TIMEOUT					3
#define AT_RCV_NONE					1
#define AT_RCV_OTHER					4


#define GPRS_CMD	0
#define GPRS_DATA 	1


#define GPRS_MAX_PACK_LEN			600

typedef enum 
{
	pwr_off,							//关机，如果GPRS上电标志打开，则打开电源并等待5秒后进入下一状态。
	pwr_on,
	at_test,							//发送ATE0 指令，测试如果模块返回OK，则进入模块准备好状态
	at_cpin,							//检查SIM卡
	at_csq,								//检查信号强度
//	at_cnum,							//本机号码
	at_cgsn,							//取模块序列号
	at_creg,							//检查网络注册状态
	at_cgatt,							//检查GPRS附着
	at_cstt,							//设置APN，默认"CMNET"
	at_ciicr,							//建立无线链路
	at_cifsr,							//获取本地IP
	at_ciphead,
	at_cipstart,							//发送CIPSTART				
	connecting,						//若接收到CONNECT OK则进入下一状态，否则进入上一状态
	connected,						//已经连接上TCP服务器
	ip_send,
	ip_rcv,
	at_cipclose,
	at_cipshut,
	restart,
	close,
	gprs_idle							//空闲态，不做任何事情
}GPRS_STATE;
EXTERN GPRS_STATE GprsState;

EXTERN unsigned char GprsRcvListenEn;

typedef enum 
{
	send,							//发送状态
	waiting						//等待
}GPRS_AT_STATE;

EXTERN GPRS_AT_STATE GprsAtState;

EXTERN unsigned char GprsPwrEn;		//GPRS上电
EXTERN unsigned int GprsRssi;			//信号强度
EXTERN unsigned char GprsSimValidFlag;
EXTERN unsigned char GprsTcpConStatus;	// 第0位表示连接状态
EXTERN unsigned char GprsTcpBusy;			//TCP连接忙碌状态，1表示有数据正在发送或接收
EXTERN unsigned char GprsTcpRcvStatus;	//TCP接收状态标志，0表示正在接收，1表示收到数据包，2表示超时
EXTERN unsigned char GprsTcpSendStatus;	//TCP发送状态标志，0表示正在发送，1 表示发送成功，2表示发送出错

EXTERN unsigned char GprsTcpRcvBuf[GPRS_MAX_PACK_LEN+16];			//接收数据区
EXTERN unsigned short GprsTcpRcvLen;				//接收数据长度
EXTERN unsigned char GprsTcpSendBuf[GPRS_MAX_PACK_LEN+16];	//发送数据区,该缓存区只能在协议处理任务里填装，在GPRS任务
											//取出，其它程序禁止对其操作
EXTERN unsigned short GprsTcpSendLen;			//发送数据长度

EXTERN unsigned short GprsAtTimeoutCnt;
EXTERN unsigned char GprsErrorCnt;		//AT质量出错计数
EXTERN unsigned short GprsConnectingTimeCnt;		//连接过程中的计时，主要用于连接一段时间重查网络状况

EXTERN unsigned char GprsGetRssiRequest;
EXTERN unsigned char GprsGetTcpStatusRequest;
EXTERN unsigned short GprsRcvTimeout;

//EXTERN char DebugStr[100];

EXTERN char GprsSim[12];
EXTERN char GprsIMEI[16];

EXTERN unsigned char GprsUartLockFlag;		//串口锁定，此标志置位时，表明串口正在接收数据包。
EXTERN unsigned char GprsRcvMatchLen;
EXTERN unsigned char GprsRcvMatchBuf[20];
EXTERN char GprsRcvMatchResult;

EXTERN unsigned short GprsIpFrameLen;

EXTERN unsigned char DistinIp[4];
EXTERN unsigned short DistinPort;

void GprsRcvListen(void);
unsigned char GprsRcvSearch(char *str,unsigned char len);
unsigned char * DataSeek(unsigned char * src,char * ch_str,unsigned short src_len);

void GprsSetRcvMatch(char *match_str);

unsigned char GprsCmdRspMatchWait(void);
unsigned char GprsDataRcvWait(void);
unsigned char GprsAtCmd(char* cmd, char* match_str,unsigned short waiting_time);
unsigned char GprsAtSendData(unsigned char *buf,unsigned short len,unsigned short waiting_time);

void GprsTask(void);
uint8_t GprsBufIsFull(void);

//unsigned char GprsGetTcpData(unsigned char * buf,unsigned short *len);
#undef EXTERN

