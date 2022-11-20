/*
 * uart_interrupt.c
 *
 *  Created on: 2019年4月15日
 *      Author: luxin
 */


#include "MPC5748G.h"
#include "uart.h"
#include "common.h"
#include "RingBuffer.h"
#include "ByteQueue.h"
#include "project.h"
uint16_t uart_recv_len = 0;
uint8_t uart_recv_compelet_flag = 0;
uint8_t LinFlex_UartRX_Buf[MAX_UART_RECV_BYTE] = {0};

unsigned char flag_uartsend[7]={0};

#if INS_CHOICE == GENAWEI
volatile unsigned char recv_count = 0;
volatile extern int flag_MEMSDataProcess;
#endif

/**********************************************************************
 description : uart接收中断函数，每接收一个字节进入一次
 @argument: none
 @return : none
 **********************************************************************/
void uartRxInterrupt_isr(void)
{
	uint8_t rx_data = 0;
	while (1 != LINFlexD_0.UARTSR.B.DRFRFE){}	/* Wait for data reception completed flag */
	while (1 != LINFlexD_0.UARTSR.B.RMB) {}  	/* Wait for Release Message Buffer */
	LINFlexD_0.LINIER.B.DRIE = 0;
	/* get the data */
	rx_data = LINFlexD_0.BDRM.R; // read whole register due to erratum e4897PS


#if INS_CHOICE == GENAWEI

	if(flag_MEMSDataProcess == DATA_STATE_SENDING)
		goto exit;
	switch(recv_count)
	{
	case 0:
		if(rx_data != 0x01)
			goto exit;
		LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
		break;
	case 1:
		if(rx_data != 0xce)
		{
			recv_count = 0;
			goto exit;
		}

		LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
		break;
	case 2:
		if(rx_data != 0x3a)
		{
			recv_count = 0;
			goto exit;
		}
		LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
		break;
	case 3:
		if(rx_data != 0x55)
		{
			recv_count = 0;
			goto exit;
		}
		LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
		break;
	default:
		LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
		if(recv_count >= MEMS_MSG_LENGTH)
		{
			flag_MEMSDataProcess = DATA_STATE_SENDING;
//			dealwithMemsRecvMsg(LinFlex_UartRX_Buf,recv_count);
			recv_count = 0;
		}
		break;
	}
exit:
#elif INS_CHOICE == DAOYUAN
	switch(recv_count)
		{
		case 0:
			if(rx_data != 0xBD)
				goto exit;
			LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
			break;
		case 1:
			if(rx_data != 0xDB)
			{
				recv_count = 0;
				goto exit;
			}

			LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
			break;
		case 2:
			if(rx_data != 0x0B)
			{
				recv_count = 0;
				goto exit;
			}
			LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
			break;
		default:
			LinFlex_UartRX_Buf[recv_count++] = (uint8_t)rx_data;
			if(recv_count >= INS_550D_MSG_LENGTH)
			{
				//dealwith550dRecvMsg

				recv_count = 0;
			}
			break;
		}
#elif INS_CHOICE == DAOYUAN_CAN
	LinFlex_UartRX_Buf[0]=rx_data;
	   flag_uartsend[0]=1;

#endif

//exit:
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFlexD_0.UARTSR.R = 0x0204;
	LINFlexD_0.LINIER.B.DRIE = 1;
}

void uart1RxInterrupt_isr(void)
{

	uint8_t rx_data = 0;
	while (1 != LINFlexD_1.UARTSR.B.DRFRFE){}	/* Wait for data reception completed flag */
	while (1 != LINFlexD_1.UARTSR.B.RMB) {}  	/* Wait for Release Message Buffer */
	LINFlexD_1.LINIER.B.DRIE = 0;
	/* get the data */
	rx_data = LINFlexD_1.BDRM.R; // read whole register due to erratum e4897PS

//	LinFlex_UartRX_Buf[0]=rx_data;
//	flag_uartsend[1]=1;

//exit:
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFlexD_1.UARTSR.R = 0x0204;
	LINFlexD_1.LINIER.B.DRIE = 1;


}

void uart3RxInterrupt_isr(void)
{

	uint8_t rx_data = 0;
	while (1 != LINFlexD_3.UARTSR.B.DRFRFE){}	/* Wait for data reception completed flag */
	while (1 != LINFlexD_3.UARTSR.B.RMB) {}  	/* Wait for Release Message Buffer */
	LINFlexD_3.LINIER.B.DRIE = 0;
	/* get the data */
	rx_data = LINFlexD_3.BDRM.R; // read whole register due to erratum e4897PS

//	LinFlex_UartRX_Buf[0]=rx_data;
//	flag_uartsend[3]=1;

//exit:
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFlexD_3.UARTSR.R = 0x0204;
	LINFlexD_3.LINIER.B.DRIE = 1;


}

#if defined(USE_GPRMC_FROM_UART)
#define NMEA_BUFF_SIZE    128
#define NMEA_SIZE_MAX 100
char nmeaBuff[NMEA_BUFF_SIZE];
uint8_t nmeaIdx;
int isGprmcGet;
#endif

typedef unsigned char TUart6Data;
#define CAPACITY (1024)
char _Uart6Buff[sizeof(TRingBuffer) + sizeof(TUart6Data) * CAPACITY];
volatile char _FlagUart6Sending = 0;
volatile char uart6_send_idx = 0;
volatile char uart6_send_len = 0;
byte_queue_t _uart6queue;
#if defined(DEBUG_REALTIME)
volatile uint32_t uart6IntTick1;
volatile uint32_t uart6IntTick2;
volatile uint32_t uart6IntTicks;
volatile uint32_t uart6IntTicksMax = 0;
#endif
void uart6RxInterrupt_isr(void)
{
#if defined(USE_GPRMC_FROM_UART)
    static union
    {
        struct
        {
            int8_t isEnd1; // '\r'
            int8_t isEnd2; // '\n'
        };
        uint32_t val;
    } chk;
#endif

#if defined(DEBUG_REALTIME)
	uart6IntTick1 = PIT.TIMER[2].CVAL.R;
#endif
	LINFlexD_6.LINIER.B.DRIE = 0;
	char data = LINFlexD_6.BDRM.B.DATA4;
#if defined(USE_GPRMC_FROM_UART)
	if (!isGprmcGet)
	{
		if (nmeaIdx == 0)
		{
			if (data == '$')
				nmeaBuff[nmeaIdx++] = data;
		}
		else if (nmeaIdx == 3)
		{
			if (data == 'R')
				nmeaBuff[nmeaIdx++] = data;
			else
				nmeaIdx = 0;
		}
		else if (nmeaIdx == 4)
		{
			if (data == 'M')
				nmeaBuff[nmeaIdx++] = data;
			else
				nmeaIdx = 0;
		}
		else if (nmeaIdx == 5)
		{
			if (data == 'C')
				nmeaBuff[nmeaIdx++] = data;
			else
				nmeaIdx = 0;
		}
		else
		{
			nmeaBuff[nmeaIdx++] = data;

			if (data == '\r')
			{
				chk.isEnd1 = 1;
			}
			else if (data == '\n')
			{
				chk.isEnd2 = 1;
			}
			if (chk.isEnd1 && chk.isEnd2)
			{
	//          nmeaIdx = 0; // 清除isGprmcGet的时候清零nmeaIdx
				chk.val = 0; // clear
				isGprmcGet = 1;
			}
			if (nmeaIdx >= NMEA_SIZE_MAX)
			{
				nmeaIdx = 0;
				chk.val = 0;
			}
		}
	}
#endif
//	/* 将收到的数据发出去 */
//	if (BufferHead(_Uart6Buff).Capacity == 0)
//	{
//		RingBuffer_Init(_Uart6Buff, CAPACITY, sizeof(TUart6Data));
//	}
//
//
//	RingBuffer_Push(_Uart6Buff, &data);
//	PushInByteQueue(&_uart6queue, &data);

#if defined(DEBUG_REALTIME)
	uart6IntTick2 = PIT.TIMER[2].CVAL.R;
	uart6IntTicks = uart6IntTick1 - uart6IntTick2;
	if (uart6IntTicks > uart6IntTicksMax)
		uart6IntTicksMax = uart6IntTicks;
#endif
	//exit:
	/* clear the DRF and RMB flags by writing 1 to them */
	LINFlexD_6.UARTSR.R = 0x0204;
	LINFlexD_6.LINIER.B.DRIE = 1;
}

// used for debug
// #undef CAPACITY
// #define CAPACITY (1024*8)
int StartUart6TransIT(char* _data, uint16_t _len)
{
	int i;

	if (_data == NULL || _len > CAPACITY)
	{
		return -1;
	}
	if (BufferHead(_Uart6Buff).Capacity == 0)
	{
		RingBuffer_Init(_Uart6Buff, CAPACITY, sizeof(TUart6Data));
	}
	if (RingBuffer_IsFull(_Uart6Buff))
	{
		return -2;
	}
	LINFlexD_6.LINIER.B.DTIE = 0;
	for (i = 0; i < _len; i++)
	{
		RingBuffer_Push(_Uart6Buff, &_data[i]);
	}
	LINFlexD_6.LINIER.B.DTIE = 1;

	if (!_FlagUart6Sending)
	{
		_FlagUart6Sending = 1;
		RingBuffer_Pop(_Uart6Buff, (char*)&LINFlexD_6.BDRL.R + 3); // DATA0
	}

	return 0;
}

void uart6TxInterrupt_isr(void)
{
	LINFlexD_6.LINIER.B.DTIE = 0;
	
	LINFlexD_6.UARTSR.B.DTFTFF = 1;

//	_FlagUart6Sending = 0;
	if(uart6_send_idx < uart6_send_len)
		LINFlexD_6.BDRL.B.DATA0 = nmeaBuff[uart6_send_idx++];



	LINFlexD_6.LINIER.B.DTIE = 1;
}
