//#ifndef __UART_H 
//#define __UART_H
#ifdef __SRC_COMM
#define EXTERN
#else
#define EXTERN  extern
#endif

#define UART0SEL     ((1<<4)|(1<<6)) 				//P0.2,P0.2
#define UART2SEL     ((2<<16)|(2<<18))				//P2.8,P2.9
#define UART3SEL	((2)|(2<<2))					//p0.0,p0.1

#define UART0IRQEnable   1
#define UART2IRQEnable   1
#define UART3IRQEnable   1

#define __UART_H

#define UART_MAX_FIFO_LEN				16

#define UART0_FIFO_LENGTH				1
#define UART2_FIFO_LENGTH				1
#define UART3_FIFO_LENGTH				1
#define UART_BUF_LENGTH				(520)//255


#define IER_RBR		0x01
#define IER_THRE    0x02
#define IER_RLS		0x04

#define IIR_PEND	0x01
#define IIR_RLS		0x03
#define IIR_RDA		0x02
#define IIR_CTI		0x06
#define IIR_THRE	0x01

#define LSR_RDR		0x01
#define LSR_OE		0x02
#define LSR_PE		0x04
#define LSR_FE		0x08
#define LSR_BI		0x10
#define LSR_THRE	0x20
#define LSR_TEMT	0x40
#define LSR_RXFE	0x80

#define StartUart0Timeout()      Uart0TimeoutEn = 1;Uart0TimeoutCnt = 100
#define StartUart2Timeout()     Uart2TimeoutEn = 1;Uart2TimeoutCnt = 100
#define StartUart3Timeout()     Uart3TimeoutEn = 1;Uart3TimeoutCnt = 300


#define PACKET_SOF 0xA5

typedef struct
{
	unsigned char send_buf[UART_BUF_LENGTH];
	unsigned char rcv_buf[UART_BUF_LENGTH];     
	unsigned short rcv_index;               
	unsigned short rcv_len;
	unsigned char rcv_state;
	unsigned char rcv_finish_flag;         
	unsigned char be_sending_flag ;
	unsigned short send_len;
	unsigned short send_index;
	unsigned char overtime_cnt;
}STRU_UART_PARAM;
EXTERN STRU_UART_PARAM Uart0;
EXTERN STRU_UART_PARAM Uart2;  
EXTERN STRU_UART_PARAM Uart3;  

EXTERN unsigned char Uart0TimeoutEn;
EXTERN unsigned int Uart0TimeoutCnt;
EXTERN unsigned char Uart2TimeoutEn;
EXTERN unsigned int Uart2TimeoutCnt; 
EXTERN unsigned char Uart3TimeoutEn;
EXTERN unsigned int Uart3TimeoutCnt;


EXTERN uint32_t UART0Status;
EXTERN uint32_t UART2Status;
EXTERN uint32_t UART3Status;


EXTERN unsigned char  BtRandomAddr;
EXTERN unsigned char  BtAddrMatchFlag;			//µØÖ·Æ¥Åä±êÖ¾


void EnableUart0intr(void);
void EnableUart2intr(void);
void EnableUart3intr(void);


void Uart0OvertimeError(void);
void Uart2OvertimeError(void);
void Uart3OvertimeError(void);


uint8_t  CheckSum(uint8_t *uBuff, uint8_t uBuffLen);

void Uart0Init(void);
void Uart2Init(void);
void Uart3Init(void);



void Set_Uart0_Baud_Rate(uint32_t Baudrate);
void Set_Uart2_Baud_Rate(uint32_t Baudrate);
void Set_Uart3_Baud_Rate(uint32_t Baudrate);


void UART0_IRQHandler (void) ;
void UART2_IRQHandler (void) ;
void UART3_IRQHandler (void) ;

void Uart0_Send(uint16_t Length)  ;
void Uart2_Send(uint16_t Length)  ;
void Uart3_Send(uint16_t Length)  ;

void Uart0_Query_Send(uint16 Length)  ;
void Uart2_Query_Send(uint16 Length)  ;
void Uart3_Query_Send(uint16 Length)  ;

void DebugMessage(char *str);
void Uart3_SendStr(char *str);


void GsmUartRcvHandle(unsigned char dat);
void GprsFlushRx(void);
void BleUartRcvHandle(STRU_UART_PARAM *uart, unsigned char dat);
void Uart0RcvHandle(STRU_UART_PARAM *uart, unsigned char dat);

#undef EXTERN















