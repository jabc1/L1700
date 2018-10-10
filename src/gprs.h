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
	pwr_off,							//�ػ������GPRS�ϵ��־�򿪣���򿪵�Դ���ȴ�5��������һ״̬��
	pwr_on,
	at_test,							//����ATE0 ָ��������ģ�鷵��OK�������ģ��׼����״̬
	at_cpin,							//���SIM��
	at_csq,								//����ź�ǿ��
//	at_cnum,							//��������
	at_cgsn,							//ȡģ�����к�
	at_creg,							//�������ע��״̬
	at_cgatt,							//���GPRS����
	at_cstt,							//����APN��Ĭ��"CMNET"
	at_ciicr,							//����������·
	at_cifsr,							//��ȡ����IP
	at_ciphead,
	at_cipstart,							//����CIPSTART				
	connecting,						//�����յ�CONNECT OK�������һ״̬�����������һ״̬
	connected,						//�Ѿ�������TCP������
	ip_send,
	ip_rcv,
	at_cipclose,
	at_cipshut,
	restart,
	close,
	gprs_idle							//����̬�������κ�����
}GPRS_STATE;
EXTERN GPRS_STATE GprsState;

EXTERN unsigned char GprsRcvListenEn;

typedef enum 
{
	send,							//����״̬
	waiting						//�ȴ�
}GPRS_AT_STATE;

EXTERN GPRS_AT_STATE GprsAtState;

EXTERN unsigned char GprsPwrEn;		//GPRS�ϵ�
EXTERN unsigned int GprsRssi;			//�ź�ǿ��
EXTERN unsigned char GprsSimValidFlag;
EXTERN unsigned char GprsTcpConStatus;	// ��0λ��ʾ����״̬
EXTERN unsigned char GprsTcpBusy;			//TCP����æµ״̬��1��ʾ���������ڷ��ͻ����
EXTERN unsigned char GprsTcpRcvStatus;	//TCP����״̬��־��0��ʾ���ڽ��գ�1��ʾ�յ����ݰ���2��ʾ��ʱ
EXTERN unsigned char GprsTcpSendStatus;	//TCP����״̬��־��0��ʾ���ڷ��ͣ�1 ��ʾ���ͳɹ���2��ʾ���ͳ���

EXTERN unsigned char GprsTcpRcvBuf[GPRS_MAX_PACK_LEN+16];			//����������
EXTERN unsigned short GprsTcpRcvLen;				//�������ݳ���
EXTERN unsigned char GprsTcpSendBuf[GPRS_MAX_PACK_LEN+16];	//����������,�û�����ֻ����Э�鴦����������װ����GPRS����
											//ȡ�������������ֹ�������
EXTERN unsigned short GprsTcpSendLen;			//�������ݳ���

EXTERN unsigned short GprsAtTimeoutCnt;
EXTERN unsigned char GprsErrorCnt;		//AT�����������
EXTERN unsigned short GprsConnectingTimeCnt;		//���ӹ����еļ�ʱ����Ҫ��������һ��ʱ���ز�����״��

EXTERN unsigned char GprsGetRssiRequest;
EXTERN unsigned char GprsGetTcpStatusRequest;
EXTERN unsigned short GprsRcvTimeout;

//EXTERN char DebugStr[100];

EXTERN char GprsSim[12];
EXTERN char GprsIMEI[16];

EXTERN unsigned char GprsUartLockFlag;		//�����������˱�־��λʱ�������������ڽ������ݰ���
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

