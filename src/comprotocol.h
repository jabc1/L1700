#ifdef __SRC_PROTOCAL
#define EXTERN
#else
#define EXTERN  extern
#endif

#define BASIC_PARA_ADDR 0
#define BLE_PARA_ADDR 16
#define NET_PARA_ADDR 48

#define WORK_MODE_ACTIVE	0
#define WORK_MODE_PASSIVE	1

typedef struct
{
    unsigned char SOF;
    unsigned char Addr;
    unsigned char Len;
    unsigned char Cmd;
    unsigned char Parameters[120];
} Struct_Cmd_Frame;

typedef struct
{
    unsigned char SOF;
    unsigned char Addr;
    unsigned char Len;
    unsigned char Status;
    unsigned char Response[250];
} Struct_Response_Frame;

typedef	struct
{
    unsigned char LocalAddr;
	unsigned char AddrValid;
    unsigned char ReadIndicate;
	unsigned char IndicateValid;
    unsigned char FilterUse;
	unsigned char FilterValid;
    unsigned char FilterWindow;
    unsigned char WindowValid;
	unsigned char WorkMode;
	unsigned char WorkModeValid;
} Struct_Parameter_List;

typedef struct
{
    char DistDNS[32];
    uint16_t DistPort;
    uint16_t ValidCode;
} Struct_Parameter_Net;


typedef struct
{
    char BleName[16];
    char BlePassWord[6];
    char ValidCode;
} Struct_Parameter_Ble;

EXTERN Struct_Parameter_List ParameterList;
EXTERN Struct_Parameter_Net ParameterNet;
EXTERN Struct_Parameter_Ble ParameterBle;


typedef struct
{
    unsigned char McuState;
    unsigned char RFState[4];
    unsigned char BTState[2];
    unsigned char GPRState;
    unsigned char SIMState;
    unsigned char FlashState;
    unsigned char TCPState;
    unsigned char DateState;
} Struct_Debug_State;
EXTERN Struct_Debug_State DebugStateList;


EXTERN unsigned int FlashAddr;
EXTERN unsigned int FlashCount;


EXTERN Struct_Cmd_Frame * pCmd_Frame;
EXTERN Struct_Response_Frame *pResponse_Frame;

#define CMD_RST_READER              0x10
#define CMD_GET_FIRMWARE            0x11
#define CMD_SET_LOCAL_ADDR          0x12
#define CMD_GET_LOCAL_ADDR          0x13
#define CMD_SET_BEEP                0x14
#define CMD_GET_BEEP                0x15
#define CMD_SET_LED                 0x16
#define CMD_GET_LED                 0x17
#define CMD_SET_WORK_MODE           0x18
#define CMD_GET_WORK_MODE           0x19

#define CMD_SET_SERVER_NETPARA           0x1A
#define CMD_GET_SERVER_NETPARA           0x1B

#define CMD_PARAM_DEFAULT			0x1F		//所有参数恢复出厂设置，所有数据清空

#define CMD_SET_FILTER_EN           0x36
#define CMD_GET_FILTER_EN           0x37
#define CMD_SET_FILTER_WIN          0x38
#define CMD_GET_FILTER_WIN          0x39

#define CMD_CLEAR_BUF               0x40
#define CMD_GET_BUF_CNT             0x41
#define CMD_GET_BUF_DATA            0x42


#define CMD_SET_ATTENUATION         0x3A
#define CMD_GET_ATTENUATION         0x3B

#define CMD_AUTHOR_UPDATE           0x50
#define CMD_START_UPDATE            0x51
#define CMD_DOWNLOAD_FILE           0x52
#define CMD_END_UPDATE              0x53


//#define CMD_SET_BLEPASSPWD             0x60
#define CMD_MODIFY_BLEPARA          0x61
#define CMD_AUTH_BLE                0x60



#define ERR_NONE                        0x00
#define ERR_NO_ERR                      0x00
#define ERR_GENERAL_ERR             0x01
#define ERR_PAR_SET_FAILED              0x02
#define ERR_PAR_GET_FAILED              0x03
#define ERR_NO_TAG                      0x04
#define ERR_READ_FAILED             0x05
#define ERR_WRITE_FAILED                0x06
#define ERR_AUTH_FAILED                 0x07
#define ERR_ERASE_FAILED                0x08
#define ERR_CMD_ERR                     0xFE
#define ERR_UNDEFINED                   0xFF


EXTERN unsigned char UpdateFlag;
EXTERN unsigned char BleParmeter[23];
EXTERN unsigned char BleUpdateFlag;


EXTERN unsigned char BlePassWord[6];
EXTERN unsigned char BleAuthFlag;

EXTERN unsigned char  DebugData[12];

EXTERN __align(4) unsigned char IapDataBuf[256];

EXTERN unsigned char  NETParaValidFlag;


//EXTERN unsigned char BleNameStr[23];
//EXTERN unsigned char BlePwdStr[14];
EXTERN void UartResponse(unsigned char len);
EXTERN void UartNoDataResponse(unsigned status);
EXTERN void CommProtocol(void);
EXTERN void SysProcess(void);
EXTERN void BufProcess(void);
EXTERN void UpgradeProcess(void);
void BleProcess(void);

#undef EXTERN

