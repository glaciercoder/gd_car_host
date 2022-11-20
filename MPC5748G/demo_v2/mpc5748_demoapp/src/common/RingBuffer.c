/*
 * RingBuffer.c
 *
 *  Created on: 2021年7月17日
 *      Author: cxt
 */

#include "RingBuffer.h"
#ifndef NULL
#define NULL ((void*)0)
#endif

// 示例见文末

int RingBuffer_Init(const char *aBuff, unsigned int aCapacity, unsigned int aDataSize)
{
    int result = rbsOthers;
    TRingBuffer* Buff = NULL;

    if (aBuff == NULL || aCapacity == 0 || aDataSize == 0)
    {
        result = rbsInvalidParam;
    }
    else
    {
        Buff = (TRingBuffer*)aBuff;
        Buff->Capacity = aCapacity;
        Buff->DataSize = aDataSize;
        Buff->Size = 0;
        Buff->Front = 0;
        Buff->Tail = 0;
        result = rbsSuccess;
    }
    
    return result;
}

int RingBuffer_Push(const char* aBuff, const void * const aData)
{
    int result = rbsOthers;
    TRingBuffer* Buff;
    unsigned int i;
    char* PushAddr;
    char* Data;

    if (aBuff == NULL || aData == NULL)
    {
        result = rbsInvalidParam;
    }
    else
    {
        Buff = (TRingBuffer*)aBuff;
        if (Buff->Capacity == Buff->Size)
        {
            result = rbsFull;
        }
        else
        {
            Data = (char*)aData;            
            PushAddr = &Buff->Data[0] + Buff->Tail * Buff->DataSize;
            Buff->Tail++;
            if (Buff->Tail == Buff->Capacity)
            {
                Buff->Tail = 0;
            }       
            for (i = 0; i < Buff->DataSize; i++)
            {
                PushAddr[i] = Data[i];
            }
            Buff->Size++;
            result = rbsSuccess;
        }
    }

    return result;
}

int RingBuffer_Pop(const char* aBuff, const void* aData)
{
    int result = rbsOthers;
    TRingBuffer* Buff;
    unsigned int i;
    char* PopAddr;
    char* Data;

    if (aData == NULL)
    {
        result = rbsInvalidParam;
    }
    else
    {
        Buff = (TRingBuffer*)aBuff;
        if (Buff->Size == 0)
        {
            result = rbsEmpty;
        }
        else
        {
            Data = (char*)aData;
            PopAddr = &Buff->Data[0] + Buff->Front * Buff->DataSize;
            Buff->Front++;
            if(Buff->Front == Buff->Capacity)
            {
                Buff->Front = 0;
            }
            for (i = 0; i < Buff->DataSize; i++)
            {
                Data[i] = PopAddr[i];
            }
            Buff->Size--;
            result = rbsSuccess;
        }
    }

    return result;
}

int RingBuffer_Clear(const char* aBuff)
{
    int result = rbsOthers;

    if (aBuff == NULL)
    {
        result = rbsInvalidParam;
    }
    else
    {
        TRingBuffer* Buff = (TRingBuffer*)aBuff;
        Buff->Size = 0;
        Buff->Front = Buff->Tail;
        result = rbsSuccess;
    }
    
    return result;
}

#if 0
typedef struct
{
    int a;
    int b;
} TData;

#define CAPACITY 2
char Buff[sizeof(TRingBuffer) + sizeof(TData) * CAPACITY];

int main(int argc, char *argv[]) {
    TData Data = {.a = 1, .b = 2};
    RingBuffer_Init(Buff, CAPACITY, sizeof(TData));

    RingBuffer_Push(Buff, &Data);
    RingBuffer_Push(Buff, &Data);

    TData Data2;

    RingBuffer_Pop(Buff, &Data2);
    //RingBuffer_Pop(Buff, &Data2);
    //RingBuffer_Push(Buff, &Data);
    //RingBuffer_Push(Buff, &Data);
    //RingBuffer_Pop(Buff, &Data2);
    //RingBuffer_Clear(Buff);
    printf("a:%x,b:%x\r\n",Data2.a,Data2.b);
    int i;
    unsigned int* data = (unsigned int*)Buff;
    for (i = 0; i < sizeof(Buff) / 4; i++)
    {
        printf("%x\r\n", *data++);
    }
    
    return 0;
}
#endif