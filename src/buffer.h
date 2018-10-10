/*****************************************************

*****************************************************/
#ifdef __SRC_BUFFER
#define EXTERN 
#else
#define EXTERN extern
#endif

//#define MAX_BUF_SIZE		0x1000
//#define MAX_EX_BUF_SIZE	0x7F00
#define MAX_REPEAT_BUF_SIZE	0x1400
#define MAX_OUTPUT_BUF_SIZE	0x1400
//EXTERN unsigned char TagBuf[MAX_BUF_SIZE];
EXTERN unsigned char RepeatTagBuf[MAX_REPEAT_BUF_SIZE];		// ���ڹ������ظ���ǩ�ݴ�
EXTERN unsigned char OutputTagBuf[MAX_OUTPUT_BUF_SIZE];		//�����������
typedef struct
{
	uint8 * p_buf;
	uint8 *p_head;			//����
	uint8 *p_tail;				//��β
	uint16 cnt;				//��������
	uint16 buf_size;
}BUF_QUEUE;						//ѭ�����У��Ƚ��ȳ�

EXTERN BUF_QUEUE BufQueue;
EXTERN BUF_QUEUE RepeatBufQueue,OutPutBufQueue;

void BufInitQueue(BUF_QUEUE * queue,uint8 *buf,uint32 size);
unsigned char BufIsFull(BUF_QUEUE * queue);
unsigned char BufIsEmpty(BUF_QUEUE * queue);
void BufDelTailData(BUF_QUEUE * queue);
unsigned char BufEnQueue(BUF_QUEUE * queue,unsigned char len,unsigned char *data);
unsigned char  BufDeQueue(BUF_QUEUE * queue,unsigned char *dat_len,unsigned char *dat);


unsigned char AutoReadDataHandle(unsigned char len,unsigned char *data);
void FlushRepeatBuf(void);

unsigned char GetNextDataLength(BUF_QUEUE *queue);
#undef EXTERN
