/*
 * RingBuffer.h
 *
 *  Created on: 2021年7月17日
 *      Author: cxt
 */

#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

typedef enum
{
    rbsSuccess      = 0, // 成功
    rbsInvalidParam = 1, // 无效参数
    rbsFull         = 2, // 满
    rbsEmpty        = 3, // 空
    rbsOthers,
} TRingBufferState;

typedef struct {
    unsigned int Capacity; // 容量
    unsigned int Size;     // 大小
    unsigned int DataSize; // 单个数据大小
    volatile unsigned int Front;
    volatile unsigned int Tail;
    char Data[0];
} TRingBuffer;


#define BufferHead(x) (*(TRingBuffer*)x)
#define RingBuffer_IsEmpty(x) (BufferHead(x).Size==0)
#define RingBuffer_IsFull(x) (BufferHead(x).Size==BufferHead(x).Capacity)

int RingBuffer_Init(const char *aBuff, unsigned int aCapacity, unsigned int aDataSize);
int RingBuffer_Push(const char* aBuff, const void * const aData);
int RingBuffer_Pop(const char* aBuff, const void* aData);
int RingBuffer_Clear(const char* aBuff);

#endif 
