#include "common/ByteQueue.h"
#include <stdio.h>
#include <derivative.h>

int ByteQueueInit(byte_queue_t* qu)
{
	memset(qu,0,sizeof(byte_queue_t));
	return 0;
}

int GetByteQueueStatus(byte_queue_t* qu)
{
	if(qu->front == qu->rear)
		return BYTE_QUEUE_EMPTY;
	else if(((qu->rear + 1)%MAX_BYTE_QUEUE_MSG_NUM) == qu->front)
		return BYTE_QUEUE_FULL;
	else
		return BYTE_QUEUE_NORMAL;
}

int PushInByteQueue(byte_queue_t* qu,char* data)
{
	if(GetByteQueueStatus(qu) == BYTE_QUEUE_FULL)
		return -1;
	qu->data[qu->rear] = *data;
	qu->rear = (qu->rear + 1)%MAX_BYTE_QUEUE_MSG_NUM;
	return 0;
}

int PopOutByteQueue(byte_queue_t* qu, char* data)
{
	if(GetByteQueueStatus(qu) == BYTE_QUEUE_EMPTY)
		return -1;
	*data = qu->data[qu->front];
	qu->front = (qu->front+1)%MAX_BYTE_QUEUE_MSG_NUM;
	return 0;
  }





