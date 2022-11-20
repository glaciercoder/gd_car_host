/*
 * handler.c
 *
 *  Created on: 2019年1月8日
 *      Author: hjian
 */

#include "derivative.h" /* include peripheral declarations */
#include "can.h"
#include "project.h"
#include "user_can.h"
#if defined(DEBUG_REALTIME)
volatile uint32_t can_int_ticks_max = 0;
volatile uint32_t can_int_ticks_pre = 0;
volatile uint32_t can_int_ticks_curr = 0;
volatile uint32_t can_int_cnt = 0;
#endif
static uint8_t _MbIndex;

// 不封装成函数能提高运行速度
#define GET_MB_INDEX_4(can_n, offset) 									\
	if (CAN_##can_n.IFLAG1.R & _BIT_(0 + offset)) /* MB[0+offset] */	\
		_MbIndex = 0 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(1 + offset))					\
		_MbIndex = 1 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(2 + offset))					\
		_MbIndex = 2 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(3 + offset))					\
		_MbIndex = 3 + offset;

#define GET_MB_INDEX_16(can_n, offset) 									\
	if (CAN_##can_n.IFLAG1.R & _BIT_(0 + offset)) /* MB[0+offset] */	\
		_MbIndex = 0 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(1 + offset))					\
		_MbIndex = 1 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(2 + offset))					\
		_MbIndex = 2 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(3 + offset))					\
		_MbIndex = 3 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(4 + offset)) 					\
		_MbIndex = 4 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(5 + offset))					\
		_MbIndex = 5 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(6 + offset))					\
		_MbIndex = 6 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(7 + offset))					\
		_MbIndex = 7 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(8 + offset)) 					\
		_MbIndex = 8 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(9 + offset))					\
		_MbIndex = 9 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(10 + offset))					\
		_MbIndex = 10 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(11 + offset))					\
		_MbIndex = 11 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(12 + offset)) 				\
		_MbIndex = 12 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(13 + offset))					\
		_MbIndex = 13 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(14 + offset))					\
		_MbIndex = 14 + offset;											\
	else if (CAN_##can_n.IFLAG1.R & _BIT_(15 + offset))					\
		_MbIndex = 15 + offset;

#define PUSH_MSG_CAN(x)             										\
	static can_frame_t msg = {.type = CAN_STD, .recv_com = CAN_COM_##x};	\
	msg.id = (CAN_##x.MB[_MbIndex].ID.R) >> 18;								\
	*(double *)&msg.data[0] = *(double *)&CAN_##x.MB[_MbIndex].DATA.B[0];  	\
	CanMsgRecvBuffPush(&msg);                                           	\
	CAN_##x.IFLAG1.R = 0xFFFFFFFF; // clear

#define IRQ_PROCESSCAN(x) IrqProcessCan##x(_MbIndex,&msg)

/**********************************************************************
 description : FlexCan0 MB0-MB3接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan0ReceiveInterruptHandle_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(0, 0)

	PUSH_MSG_CAN(0)

	IRQ_PROCESSCAN(0);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}


void flexCan0MB_4_7_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(0, 4)

	PUSH_MSG_CAN(0)

	IRQ_PROCESSCAN(0);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}

/***********************************************************************//*flexcan1 interrupt*/
void flexCan1ReceiveInterruptHandle_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(1, 0)

	PUSH_MSG_CAN(1)

	IRQ_PROCESSCAN(1);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}

/**********************************************************************
 description : FlexCan0 MB0-MB3接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan1MB_4_7_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(1, 4)

	PUSH_MSG_CAN(1)

	IRQ_PROCESSCAN(1);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}/* Vector # 581 FlexCAN1_3 FlexCAN1_BUF_04_07 */

void flexCan1MB_8_11_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(1, 8)

	PUSH_MSG_CAN(1)

	IRQ_PROCESSCAN(1);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}/* Vector # 582 FlexCAN1_4 FlexCAN1_BUF_08_11 */

void flexCan1MB_12_15_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(1, 12)

	PUSH_MSG_CAN(1)

	IRQ_PROCESSCAN(1);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}/* Vector # 583 FlexCAN1_5 FlexCAN1_BUF_12_15 */
void flexCan1MB_16_31_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_16(1, 16)

	PUSH_MSG_CAN(1)

	IRQ_PROCESSCAN(1);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}/* Vector # 584 FlexCAN1_6 FlexCAN1_BUF_16_31 */

/***********************************************************************//*flexcan1 interrupt*/

/**********************************************************************
 description : FlexCan0 MB0-MB3接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan2ReceiveInterruptHandle_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(2, 0)

	PUSH_MSG_CAN(2)

	IRQ_PROCESSCAN(2);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}

void flexCan2MB_4_7_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(2, 4)

	PUSH_MSG_CAN(2)

	IRQ_PROCESSCAN(2);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}

void flexCan2MB_8_11_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(2, 8)

	PUSH_MSG_CAN(2)

	IRQ_PROCESSCAN(2);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}
/**********************************************************************
 description : FlexCan0 MB0-MB3接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan3ReceiveInterruptHandle_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(3, 0)

	PUSH_MSG_CAN(3)

	IRQ_PROCESSCAN(3);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}

void flexCan3MB_4_7_isr(void)
{
#if defined(DEBUG_REALTIME)
	can_int_cnt++;
	can_int_ticks_pre = PIT.TIMER[2].CVAL.R;
#endif
	GET_MB_INDEX_4(3, 4)

	PUSH_MSG_CAN(3)

	IRQ_PROCESSCAN(3);
#if defined(DEBUG_REALTIME)
	can_int_ticks_curr = PIT.TIMER[2].CVAL.R;
	if (can_int_ticks_pre - can_int_ticks_curr > can_int_ticks_max)
		can_int_ticks_max = can_int_ticks_pre - can_int_ticks_curr;
#endif
}
/**********************************************************************
 description : FlexCan4 MB0-MB1接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan4ReceiveInterruptHandle_isr(void)
{
	
}

/**********************************************************************
 description : FlexCan5 MB0-MB1接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan5ReceiveInterruptHandle_isr(void)
{
	
}

/**********************************************************************
 description : FlexCan6 MB0-MB1接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan6ReceiveInterruptHandle_isr(void)
{
	
}

/**********************************************************************
 description : FlexCan6 MB0-MB1接收中断
 @argument: none
 @return : none
 **********************************************************************/
void flexCan7ReceiveInterruptHandle_isr(void)
{
	
}
