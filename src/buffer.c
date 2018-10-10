
#define __SRC_BUFFER
#include "main.h"
#undef __SRC_BUFFER

void BufInitQueue(BUF_QUEUE * queue,uint8 *buf,uint32 size)
{
	queue->cnt = 0;
	queue->p_buf = buf;
	queue->p_head = buf;
	queue->p_tail = buf;
	queue->buf_size = size;
}

 unsigned char BufIsFull(BUF_QUEUE * queue)
{
	if(queue->p_head >= queue->p_tail)			//ͷ��ǰ��β�ں�
	{
		if(queue->p_head-queue->p_tail > (queue->buf_size -16))		// 16�ֽڰ�ȫ����
		{
			return 1;
		}
		else
			return 0;
	}
	else										// ͷ�ں�β��ǰ
	{
		if( queue->p_tail - queue->p_head < 16)
			return 1;
		else
			return 0;
	}
}

unsigned char BufIsEmpty(BUF_QUEUE * queue)
{
	if (queue->cnt == 0)
		return 0x01;
	else
		return 0x00;
}

void BufDelTailData(BUF_QUEUE * queue)
{
	unsigned char len;
	len = *(queue->p_tail);
	queue->p_tail += (len+1);
	if(queue->p_tail > (queue->p_buf+queue->buf_size-16))		// ���ﶥ��
	{
		queue->p_tail = queue->p_buf;
	}
	queue->cnt--;
}
unsigned char BufEnQueue(BUF_QUEUE * queue,unsigned char len,unsigned char *data)
{
	unsigned char i;
	
	if(BufIsFull(queue) == 1)		//���ˣ������һ��
		BufDelTailData(queue);
	*(queue->p_head) = len;
	for(i = 0;i < len;i++)
	{
		(queue->p_head)[i+1] = data[i];
	}
	queue->p_head+= (len+1);
	if(queue->p_head > (queue->p_buf+queue->buf_size)-16)		// ���ﶥ��
	{
		queue->p_head = queue->p_buf;
	}
	queue->cnt++;
	return 0;
}

unsigned char  BufDeQueue(BUF_QUEUE * queue,unsigned char *dat_len,unsigned char *dat)
{
	unsigned char len,i;
	
	if(BufIsEmpty(queue))
		return false;
	len = (queue->p_tail)[0];
	dat[0] = len;
	*dat_len = len;
	for(i = 0;i < len;i++)
		dat[i+1] = (queue->p_tail)[i+1];
	
	queue->p_tail+= (len+1);
	if(queue->p_tail > (queue->p_buf+queue->buf_size-16))		// ���ﶥ��
	{
		queue->p_tail = queue->p_buf;
	}	
	queue->cnt--;
	return true;
}

unsigned char GetNextDataLength(BUF_QUEUE *queue)
{
	unsigned char len;

	if(BufIsEmpty(queue))
		return 0;	
	len = ((queue->p_tail)[0]);

	return len;
}


// data�е�һ�ֽ������ߣ����������
unsigned char AutoReadDataHandle(unsigned char len,unsigned char *data)
{
	uint16 i;
	uint8 *p;
	uint8 l;
	if(ParameterList.FilterUse)						//ʹ���ظ���ǩ������
	{
		p = RepeatBufQueue.p_tail;
		for(i = 0;i < RepeatBufQueue.cnt;i++)
		{
			if(memcmp(data+1,p+2,len-1) == 0)	//������һ���ֽ�
			{
				return 0;			//�ظ�����
			}
			l = *p;
			p += (l+1);
			if(p > RepeatTagBuf + MAX_REPEAT_BUF_SIZE - 16)
			{
				p = RepeatTagBuf;
			}
		}
		if(i == RepeatBufQueue.cnt)	//û���ظ���
		{
			memcpy(PublicBuf,data,len);
			PublicBuf[len] = SysTick1sCnt>>8;
			PublicBuf[len+1] = SysTick1sCnt;
			BufEnQueue(&RepeatBufQueue, len+2,PublicBuf);
			BufEnQueue(&OutPutBufQueue,len, data);
		}
	}
	else
	{
		BufEnQueue(&OutPutBufQueue,len, data);		
	}
	return 1;
}

void FlushRepeatBuf(void)
{
	uint16 i;
	uint8 *p;
	uint8 len;
	uint16 cnt,tim;
	uint8 t;

	cnt = RepeatBufQueue.cnt;
	if(cnt == 0)
		return;	
	
	for(i = 0;i < RepeatBufQueue.cnt;i++)
	{
		p = RepeatBufQueue.p_tail;
		len = *p;
		tim = p[len-1]*256 + p[len];
		t = SysTick1sCnt - tim;//p[len]
		if(t > ParameterList.FilterWindow)	//���ڵ�����
		{
			RepeatBufQueue.p_tail += len+1;
			if(RepeatBufQueue.p_tail >(RepeatTagBuf + MAX_REPEAT_BUF_SIZE - 16))
				RepeatBufQueue.p_tail = RepeatTagBuf;
			cnt--;
		}
		else
			break;
	}
	RepeatBufQueue.cnt = cnt;
}

