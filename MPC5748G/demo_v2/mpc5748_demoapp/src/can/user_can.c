/*
 * user_can.c
 *
 *  Created on: 2018骞�8鏈�22鏃�
 *      Author: luxin
 */


#include "derivative.h"
#include "user_can.h"
#include "user_udp.h"
#include "common.h"
#include "RingBuffer.h"
#include "InterruptCtrl.h"


#ifndef NULL
	#define NULL 0L
#endif /* NULL */






extern CanTSET_buff can_testbuff;





/*******************************************************************************
* Global variables
*******************************************************************************/




/* CAN鐜舰缂撳瓨 */
#define CAN_RB_CAPACITY 512
static char CanMsgRecvBuff[2][sizeof(TRingBuffer) + sizeof(can_frame_t) * CAN_RB_CAPACITY];
volatile uint8_t _FlagBufferSelect = 0;
static const TCanRecvExcuteMsg _CanRecvExcuteMsg[] = 
{
	NULL,
	HandlerSendCan0Message,
	HandlerSendCan1Message,
	HandlerSendCan2Message,
	HandlerSendCan3Message
};




#if defined(DEBUG_REALTIME)
volatile uint16_t buffer_maxcnt = 0;
volatile uint32_t t2;
volatile uint16_t buffer_curr_cnt = 0;
volatile uint32_t tick_can_msg_exe;
volatile uint8_t can_exe_flag = 0;
volatile uint32_t canmsgexe_can_int_cnt;
volatile uint32_t can_intcnt_pre;
volatile uint32_t can_intcnt_curr;

extern volatile uint32_t enet_udp_int_cnt ;
extern volatile uint32_t enet_int_cnt ;
extern volatile uint32_t enet_ip4_int_cnt ;
extern volatile uint32_t can_int_cnt ;
#endif

int CanMsgRecvBuffInit(void)
{
	RingBuffer_Init(CanMsgRecvBuff[0], CAN_RB_CAPACITY, sizeof(can_frame_t));
	RingBuffer_Init(CanMsgRecvBuff[1], CAN_RB_CAPACITY, sizeof(can_frame_t));

	return 0;
}

int CanMsgRecvBuffPush(const can_frame_t* aFrame)
{
#if !defined(DEBUG_REALTIME)
	return RingBuffer_Push(CanMsgRecvBuff[_FlagBufferSelect], aFrame);
#else
	RingBuffer_Push(CanMsgRecvBuff[_FlagBufferSelect], aFrame);
	if ((*(TRingBuffer*)CanMsgRecvBuff[_FlagBufferSelect]).Size > buffer_maxcnt)
		buffer_maxcnt = (*(TRingBuffer*)CanMsgRecvBuff[_FlagBufferSelect]).Size;
	return 0;
#endif

}



int CanRecvExcuteMsg(void)
{
	can_frame_t Frame;
	TRingBufferState volatile State;
	uint8_t PopBuff = (_FlagBufferSelect + 1) % 2;
	uint32_t t1 = PIT.TIMER[2].CVAL.R;
	
#if defined(DEBUG_REALTIME)
	can_intcnt_pre = can_int_cnt;
#endif
	
	while (1)
	{
		State = RingBuffer_Pop(CanMsgRecvBuff[PopBuff], &Frame);

		if (State == rbsEmpty)
		{
			_FlagBufferSelect = PopBuff;
			break;
		}
		else if (State != rbsSuccess)
			break;
		if (Frame.recv_com >= CAN_COM_0 && Frame.recv_com <= CAN_COM_7)
		{
#if defined(DEBUG_REALTIME)
			can_exe_flag = 1;
			buffer_curr_cnt = (*(TRingBuffer*)CanMsgRecvBuff[_FlagBufferSelect]).Size;
#endif
			_CanRecvExcuteMsg[Frame.recv_com](&Frame);
		}

		if (t1 - PIT.TIMER[2].CVAL.R > 40000 * 2) // 2ms
			break; // 释放CPU
	}
#if defined(DEBUG_REALTIME)
	t2 = PIT.TIMER[2].CVAL.R;
	can_intcnt_curr = can_int_cnt;
	tick_can_msg_exe = t1 - t2;
	canmsgexe_can_int_cnt = can_intcnt_curr - can_intcnt_pre;

	can_exe_flag = 0;
#endif
	return 0;
}

void IrqProcessCan0(uint32_t _index, can_frame_t* _msg)
{

}

int HandlerSendCan0Message(can_frame_t* msg)
{
    return 0;
}

void IrqProcessCan1(uint32_t _index, can_frame_t* _msg)
{
	switch(_msg->id)
	  {
	
	  case 0x224:
		  memcpy(can_testbuff.Can_buff, _msg->data, 8);
		  break;
	  case 0x223:
		  memcpy(can_testbuff.Can_buff+8, _msg->data, 8);
		  break;
	  }
	  

}

int HandlerSendCan1Message(can_frame_t* msg)
{
    return 0;
}

void IrqProcessCan2(uint32_t _index, can_frame_t* _msg)
{

}

int HandlerSendCan2Message(can_frame_t* msg)
{

    return 0;
}

void IrqProcessCan3(uint32_t _index, can_frame_t* _msg)
{

}

int HandlerSendCan3Message(can_frame_t* msg)
{

    return 0;
}


