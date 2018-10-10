
#ifdef  __SRC_SERVERPROTOCAL
#define EXTERN

#else
#define EXTERN  extern
#endif

EXTERN char UpdateServerDNS[32];            //���·���������//2016-10-18 GH
EXTERN char Str_UpdateServerPort[5];
EXTERN uint32_t UpdateServerPort;           //���·������˿�

EXTERN char MainServerDNS[32];              //��������      //2016-10-18 GH
EXTERN uint32_t MainServerPort;
EXTERN char Str_MainServerPort[5];

EXTERN char Str_FilterWindow[4];

EXTERN int Packet_Sn;
EXTERN char Update_Finish_Flag;

EXTERN unsigned char Firmware_Ver[4];


EXTERN uint8_t ServerUpdateFlag;            //������Ҫ����µı�־



void ServerFillDataPack(uint8_t *data);
void ServerFillHeartbeatPack(void);
void ServerProtocalTask(void);
void ServerProtocalInit(void);
void ServerUpdateDataHandle(void);
void ServerUpgradeDataHandle(void);
void ServerUpgradeReqPack(void);
void ServerConfirmRspHandle(void);
void ServerUpdateFileRspHandle(void);
void ServerUpdateGenRspHandle(void);
void ServerUpdateReqPack(void);
void ServerConfirmPack(char*confirm_str);







#undef EXTERN
