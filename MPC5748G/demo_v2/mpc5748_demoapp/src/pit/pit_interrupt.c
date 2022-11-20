/*

 * pit_interrupt.c
 *
 *  Created on: 2019��4��15��
 *      Author: luxin
 */
#include "derivative.h" /* include peripheral declarations */
#include "math.h"
#include "pit.h"
#include "can.h"
#include "user_udp.h"
#include "uart.h"
#include "common.h"
#include "user_can.h"
#include "udp_handle.h"
#include "gpio.h"

extern CanTSET_buff can_testbuff;






volatile extern struct time_s sys_local_time;




unsigned long can3_velinfo_time_cnt;
unsigned long can3_velinfo_time_cnt1;

unsigned short delay_ms = 0;

volatile int global_flag_send_arp_request = 0;

unsigned char rcv_core1data_flag = 0;
unsigned char udp_test_flag = 0;

unsigned short current_brake = 0;




#if defined(DEBUG_REALTIME)
volatile uint32_t pit0_10ms_tsk_cnt = 0;
volatile uint32_t pit0_10ms_tsk_tick_max_interval = 0;
volatile uint32_t pit0_10ms_tsk_tick_last = 0;
#endif
/**********************************************************************
 description : PIT0 ��ʱ���жϺ���
 @argument: none
 @return : none
 **********************************************************************/
void PIT0_isr(void)
{
	static uint16_t counter = 0; /* Increment ISR counter */
	counter++;
	delay_ms++;

	sys_local_time.msec = counter;

	if (counter > 999)
	{
		sys_local_time.sec += 1;
		counter = 0;
	}
#if LAC >= LAC_V_OLD
	if(isMacAvailable(&udpSendDstAddr_ctrlNode) && isMacAvailable(&udpSendDstAddr_planNode))
#else
	if (isMacAvailable(&udpSendDstAddr_ctrlNode))
#endif
	{
		if (sys_local_time.sec % 120 == 7)
			if (sys_local_time.msec == 333)
				global_flag_send_arp_request = 1;
	}
	else
	{
		if (sys_local_time.msec == 333)
			global_flag_send_arp_request = 1;
	}

	

//	if (counter % 10 == 1)
//	{
//
//		    can_frame_t cantest ={ 0 };
//		    can_frame_t cantest1 ={ 0 };
//			cantest.id = 0x223;
//		    cantest.dlc = 8;
//		    cantest.send_com = CAN_COM_1;
//		    cantest.type = CAN_STD;
//			memcpy(cantest.data, can_testbuff.Can_buff+8, 8);
//			flexCanMsgTransmit(&cantest); //  10ms
//
//		    cantest1.id = 0x224;
//		    cantest1.dlc = 8;
//		    cantest1.send_com = CAN_COM_1;
//		    cantest1.type = CAN_STD;
//			memcpy(cantest1.data, can_testbuff.Can_buff, 8);
//			flexCanMsgTransmit(&cantest1); //  10ms
//	}
	
//	if (counter % 100 == 10)
//	{
//		udp_test_flag = 1;
//	}


	PIT.TIMER[0].TFLG.R = 1; /* Clear interrupt flag */
}

/**********************************************************************
 description : PIT1 ��ʱ���жϺ���
 @argument: none
 @return : none
 **********************************************************************/
void PIT1_isr(void)
{

	PIT.TIMER[1].TFLG.R = 1; /* Clear interrupt flag */
}

/**********************************************************************
 description : PIT2 ��ʱ���жϺ���
 @argument: none
 @return : none
 **********************************************************************/
void PIT2_isr(void)
{
	PIT.TIMER[2].TFLG.R = 1; /* Clear interrupt flag */
}

void del_ms(unsigned short ms)
{
	delay_ms = 0;

	while (delay_ms < ms)
		;

}
