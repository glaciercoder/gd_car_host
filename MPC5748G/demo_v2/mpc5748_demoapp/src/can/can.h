/*
 * can.h
 *
 *  Created on: 2019Äê1ÔÂ4ÈÕ
 *      Author: Hjian
 */

#ifndef _CAN_H_
#define _CAN_H_

#define CAN0_USE_STD
#define CAN1_USE_STD

#define CAN3_BUFFER_NUM 5

typedef enum{
	CAN_COM_NONE = 0,
	CAN_COM_0,
	CAN_COM_1,
	CAN_COM_2,
	CAN_COM_3,
	CAN_COM_4,
	CAN_COM_5,
	CAN_COM_6,
	CAN_COM_7
}can_com_t;

typedef enum{
	UNKNOW = 0,
	CAN_STD,
	CAN_EXT
}can_type_t;

typedef struct{
	can_com_t send_com:8;
	can_com_t recv_com:8;
	can_type_t type:8;
	unsigned char dlc:8;
	unsigned int id;
	unsigned char data[8];
}can_frame_t;

typedef struct{
	unsigned int id;
	unsigned char data[8];
}can_mmwave_frame_t;

typedef struct {
    unsigned char Can_buff[16];
}CanTSET_buff;

typedef enum {
	canBaud50k = 50,
	canBaud100k = 100,
	canBaud125k = 125,
	canBaud250k = 250,
	canBaud500k = 500,
	canBaud800k = 800,
	canBaud1000k = 1000
}can_baudrate_t;

typedef int (*TCanRecvExcuteMsg)(can_frame_t*);

int flexCanInitConfig(can_com_t instance,can_baudrate_t baudrate,unsigned char priority);
int flexCanMsgTransmit(can_frame_t* msg);

int flexCanRecvExcuteMsg_CAN0(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN1(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN2(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN3(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN4(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN5(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN6(can_frame_t* msg);
int flexCanRecvExcuteMsg_CAN7(can_frame_t* msg);
#endif /* _CAN_H_ */
