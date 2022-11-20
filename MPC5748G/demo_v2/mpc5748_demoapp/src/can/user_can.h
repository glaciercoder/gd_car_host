/*
 * user_can.h
 *
 *  Created on: 2018��8��22��
 *      Author: luxin
 */

#ifndef USER_CAN_H_
#define USER_CAN_H_

#include "can.h"




typedef int (*canfunc)(can_frame_t*);
typedef int (*sendfunction)(void);

typedef struct{
	uint32_t sec;
	uint16_t msec;
}timestamp_t;

typedef struct{
	uint32_t can_id;
	canfunc func;
	uint8_t* flag_dataProcess;
	int (*sendfunc)(void);
	timestamp_t *stamp;

}can_func_t;



void printf_can_data(can_frame_t* msg);
int HandlerSendCan0Message(can_frame_t* msg);
int HandlerSendCan1Message(can_frame_t* msg);
int HandlerSendCan2Message(can_frame_t* msg);
int HandlerSendCan3Message(can_frame_t* msg);
int pollsingleCANUDP(int n);
int CanMsgRecvBuffInit(void);
int CanMsgRecvBuffPush(const can_frame_t* aFrame);
int CanRecvExcuteMsg(void);
void IrqProcessCan0();
void IrqProcessCan1();
void IrqProcessCan2();
void IrqProcessCan3();
#endif /* USER_CAN_H_ */
