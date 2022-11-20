/* EWL
 * Copyright 锟� 1995-2009 Freescale Corporation.  All rights reserved.
 *
 * $Date: 2012/06/01 15:44:21 $
 * $Revision: 1.1 $
 */

#ifndef UART_H
#define UART_H

#if !_EWL_CONSOLE_SUPPORT
#error _EWL_CONSOLE_SUPPORT must not be defined to 1.
#endif

#define MAX_UART_RECV_BYTE 256

typedef int UARTError;


typedef enum{
	UART_COM_0,
	UART_COM_1,
	UART_COM_2,
	UART_COM_3,
	UART_COM_4,
	UART_COM_5,
	UART_COM_6
}UART_COM_t;

enum {
	kUARTNoError = 0,
	kUARTUnknownBaudRate,
	kUARTConfigurationError,
	kUARTBufferOverflow,				/* specified buffer was too small */
	kUARTNoData							/* no data available from polling */
};

typedef enum {
	kBaudHWSet = -1,					/* use HW settings such as DIP switches */
	kBaud300 = 300,						/* valid baud rates */
	kBaud600 = 600,
	kBaud1200 = 1200,
	kBaud1800 = 1800,
	kBaud2000 = 2000,
	kBaud2400 = 2400,
	kBaud3600 = 3600,
	kBaud4800 = 4800,
	kBaud7200 = 7200,
	kBaud9600 = 9600,
	kBaud19200 = 19200,
	kBaud38400 = 38400,
	kBaud57600 = 57600,
	kBaud115200 = 115200,
	kBaud230400 = 230400
} UARTBaudRate;

UARTError flexLinUartInit(UART_COM_t instance,UARTBaudRate baudRate,unsigned char priority, unsigned char enableTIE);
UARTError WriteUARTN(const void* bytes, unsigned long length);



void uart_printf_format(char* format,...);
void uart_send(void);



#define __UART_DEBUG__

#ifdef __UART_DEBUG__
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)
#define uart_printf(format,...) uart_printf_format("[%s %d] "format, __FILENAME__, __LINE__,##__VA_ARGS__)
#else
#define uart_printf(format,...)
#endif


#endif /* UART_H */
