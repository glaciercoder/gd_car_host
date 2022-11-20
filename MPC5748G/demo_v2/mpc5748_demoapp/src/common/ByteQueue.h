#ifndef BYTE_QUEUE_H_
#define BYTE_QUEUE_H_

#define MAX_BYTE_QUEUE_MSG_NUM 1024

typedef struct{
	int front;
	int rear;
	char data[MAX_BYTE_QUEUE_MSG_NUM];
}byte_queue_t;

enum{
	BYTE_QUEUE_NORMAL = 0,
	BYTE_QUEUE_EMPTY,
	BYTE_QUEUE_FULL
};

int ByteQueueInit(byte_queue_t* qu);
int GetByteQueueStatus(byte_queue_t* qu);
int PushInByteQueue(byte_queue_t* qu, char* data);
int PopOutByteQueue(byte_queue_t* qu, char* data);

#endif
