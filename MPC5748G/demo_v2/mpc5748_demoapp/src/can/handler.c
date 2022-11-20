/*
 * can_interrupt.c
 *
 *  Created on: 2019��3��20��
 *      Author: luxin
 */

#include "derivative.h" /* include peripheral declarations */
#include "can.h"
#include "uart.h"
#include "user_can.h"


can_mmwave_frame_t can_mmwave_frame_buffer[CAN3_BUFFER_NUM][50];
volatile unsigned char can_mmwave_frame_counter[CAN3_BUFFER_NUM]={0};
volatile unsigned char flag_mmwave_send_udp_task[CAN3_BUFFER_NUM]={0};


/**********************************************************************
 description 	: FlexCan0 �������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN0(can_frame_t* msg)
{
	//canFunction_Ultrasonic(msg);
	return 0;
}


/**********************************************************************
 description 	: FlexCan1 �������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN1(can_frame_t* msg)
{

	Handler_Can1_Message(msg);
	return 0;
}

/**********************************************************************
 description 	: FlexCan2 �������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN2(can_frame_t* msg)
{

	Handler_Can2_Message(msg);
	return 0;
}


/**********************************************************************
 description 	: FlexCan3�������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN3(can_frame_t* msg)
{
	canFunction_Ultrasonic(msg);
	return 0;

}

/**********************************************************************
 description 	: FlexCan3�������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN4(can_frame_t* msg)
{
	return 0;
}


/**********************************************************************
 description 	: FlexCan3�������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN5(can_frame_t* msg)
{
	return 0;
}


/**********************************************************************
 description 	: FlexCan3�������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN6(can_frame_t* msg)
{
	return 0;
}


/**********************************************************************
 description 	: FlexCan3�������ݴ�����
 @argument msg 	: һ֡can_frame_t�ṹ�����͵�can����
 @return : none
 **********************************************************************/
int flexCanRecvExcuteMsg_CAN7(can_frame_t* msg)
{
	return 0;
}

