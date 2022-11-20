/*******************************************************************************
* Freescale Semiconductor Inc.
* (c) Copyright 2010 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
********************************************************************************
Services performed by FREESCALE in this matter are performed AS IS and without
any warranty. CUSTOMER retains the final decision relative to the total design
and functionality of the end product. FREESCALE neither guarantees nor will be
held liable by CUSTOMER for the success of this project.
FREESCALE DISCLAIMS ALL WARRANTIES, EXPRESSED, IMPLIED OR STATUTORY INCLUDING,
BUT NOT LIMITED TO, IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR
A PARTICULAR PURPOSE ON ANY HARDWARE, SOFTWARE ORE ADVISE SUPPLIED 
TO THE PROJECT BY FREESCALE, AND OR NAY PRODUCT RESULTING FROM FREESCALE 
SERVICES. IN NO EVENT SHALL FREESCALE BE LIABLE FOR INCIDENTAL OR CONSEQUENTIAL 
DAMAGES ARISING OUT OF THIS AGREEMENT.
CUSTOMER agrees to hold FREESCALE harmless against any and all claims demands 
or actions by anyone on account of any damage, or injury, whether commercial,
contractual, or tortuous, rising directly or indirectly as a result 
of the advise or assistance supplied CUSTOMER in connection with product, 
services or goods supplied under this Agreement.
********************************************************************************
* File:             uart.c
* Owner:            Martin Kovar
* Version:          1.0
* Date:             Dec-09-2015
* Classification:   General Business Information
* Brief:            terminal IO. implements CodeWarrior MSL library calls
*                   MSL_C library printf() output to MPC5607B's LINFlex0
********************************************************************************
* Detailed Description: 
* 
* implements CW MSL library function calls to implement printf() on LINFlex0
* header file UART.h is taken from the CW as is
* global functions defined in this module replace the ones from the library
* so finally we send printf() to LINFlex0
*
* ------------------------------------------------------------------------------
* Test HW:  XPC5607B 176LQFP, XPC56XX EVB MOTHEBOARD Rev.C
* Target :  PPC5607BMLUAM03Y
* Terminal: 19200-8-no parity-1 stop bit-no flow control on LINFlexD_0
* Fsys:     64/48 MHz
*
********************************************************************************
Revision History:
1.0     Dec-21-2015     Martin Kovar  Initial Version
*******************************************************************************/

#include "MPC5748G.h"
#include "uart.h"
#include "stdarg.h"
#include "derivative.h"

/*******************************************************************************
* Global variables
*******************************************************************************/

/*******************************************************************************
* Constants and macros
*******************************************************************************/
#define CRYSTAL_Hz   40000000
#define UART_RECV_BYTE_TIME_OUT 1000
/*******************************************************************************
* Local types
*******************************************************************************/

/*******************************************************************************
* Local function prototypes
*******************************************************************************/
//static void Init_LINFlexD_0(void);
static void TransmitData(const char * pBuf, const uint32_t cnt);
static int32_t ReceiveData(char * const pBuf);

extern uint8_t LinFlex_UartRX_Buf[MAX_UART_RECV_BYTE];
extern unsigned char flag_uartsend[7];

/*******************************************************************************
* Local variables
*******************************************************************************/

/*******************************************************************************
* Local functions
*******************************************************************************/ 

/*******************************************************************************
Function Name : Init_LINFlexD_0
Engineer      : Martin Kovar
Date          : Dec-09-2015
Parameters    : NONE
Modifies      : NONE
Returns       : NONE
Notes         : initializes MPC5644P's Init_LINFlexD_0 module for UART mode
Issues        : expecting system clock is 100MHz
*******************************************************************************/
void LINFlexD_0_Init(UART_COM_t instance, UARTBaudRate baudRate, unsigned char priority, unsigned char enableTIE)
{
	unsigned int BaudRate;
	unsigned int Fraction;
	unsigned int Integer;
	float LFDIV = 0;
	switch (instance)
	{
	case 0:
		/* enter INIT mode */
		LINFlexD_0.LINCR1.R = 0x0081; /* SLEEP=0, INIT=1 */

		/* wait for the INIT mode */
		while (0x1000 != (LINFlexD_0.LINSR.R & 0xF000))
		{
		}

		/* configure pads */
		SIUL2.MSCR[18].B.SSS = 1;  /* Pad PB2: Source signal is LIN0_TXD  */
		SIUL2.MSCR[18].B.OBE = 1;  /* Pad PB2: Output Buffer Enable */
		SIUL2.MSCR[18].B.SRC = 3;  /* Pad PB2: Maximum slew rate */
		SIUL2.IMCR[200].B.SSS = 2; /* LIN0_RXD: connected to pad PB3 */
		/* Configure pad PB3 for LIN0RX */
		SIUL2.MSCR[19].B.IBE = 1; /* Pad PB3: Enable pad for input - LIN0_RXD */

		/* configure for UART mode */
		LINFlexD_0.UARTCR.R = 0x0001; /* set the UART bit first to be able to write the other bits */

		LINFlexD_0.UARTCR.R = 0x0033; /* 8bit data, no parity, Tx and Rx enabled, UART mode */
									  /* Transmit buffer size = 1 (TDFL = 0 */
									  /* Receive buffer size = 1 (RDFL = 0) */
		LINFlexD_0.UARTSR.B.DRFRFE = 1;
		/* configure baudrate 19200 */
		/* assuming 64 MHz peripheral set 1 clock (fsys below)*/
		/* LFDIV = fsys / (16 * desired baudrate)
			   LINIBRR = integer part of LFDIV
			   LINFBRR = 16 * fractional part of LFDIV (after decimal point)

			   for instance:
			   LFDIV = 100e6/(16*19200) = 325.52
			   LINIBRR = 325
			   LINFBRR = 16*0.52 = 8.32
			*/
		LFDIV = (float)CRYSTAL_Hz / (16 * baudRate);
		LINFlexD_0.LINIBRR.R = (int)LFDIV;
		LFDIV -= (int)LFDIV;
		LINFlexD_0.LINFBRR.R = (int)(LFDIV * 16);

		//		LINFlexD_0.LINFBRR.R = 11;//8;
		//		LINFlexD_0.LINIBRR.R = 21;//325;
		INTC.PSR[376].B.PRC_SELN0 = 1;
		INTC.PSR[376].B.PRIN = priority;
		LINFlexD_0.LINIER.B.DRIE = 1;

		/* enter NORMAL mode */
		LINFlexD_0.LINCR1.R = 0x0080; /* INIT=0 */
		break;
	case 1:
		/* enter INIT mode */
		//LINFlexD_1.LINCR1.R = 0x0081; /* SLEEP=0, INIT=1 */
		LINFlexD_1.LINCR1.B.INIT = 1;
		LINFlexD_1.LINCR1.B.SLEEP = 0;

		/* wait for the INIT mode */
		while (0x1000 != (LINFlexD_1.LINSR.R & 0xF000))
		{
		}

		/* configure pads */
		SIUL2.MSCR[38].B.SSS = 1;  /* Pad PB2: Source signal is LIN0_TXD  */
		SIUL2.MSCR[38].B.OBE = 1;  /* Pad PB2: Output Buffer Enable */
		SIUL2.MSCR[38].B.SRC = 3;  /* Pad PB2: Maximum slew rate */
		SIUL2.IMCR[201].B.SSS = 1; /* LIN0_RXD: connected to pad PB3 */
		/* Configure pad PB3 for LIN0RX */
		SIUL2.MSCR[39].B.IBE = 1; /* Pad PB3: Enable pad for input - LIN0_RXD */

		/* configure for UART mode */
		LINFlexD_1.UARTCR.R = 0x0001; /* set the UART bit first to be able to write the other bits */

		LINFlexD_1.UARTCR.R = 0x0033; /* 8bit data, no parity, Tx and Rx enabled, UART mode */
									  /* Transmit buffer size = 1 (TDFL = 0 */
									  /* Receive buffer size = 1 (RDFL = 0) */
		LINFlexD_1.UARTSR.B.SZF = 1;  /* Clear the Zero status bit */
		LINFlexD_1.UARTSR.B.DRFRFE = 1;
		/* configure baudrate 19200 */
		/* assuming 64 MHz peripheral set 1 clock (fsys below)*/
		/* LFDIV = fsys / (16 * desired baudrate)
					   LINIBRR = integer part of LFDIV
					   LINFBRR = 16 * fractional part of LFDIV (after decimal point)

					   for instance:
					   LFDIV = 100e6/(16*19200) = 325.52
					   LINIBRR = 325
					   LINFBRR = 16*0.52 = 8.32
					*/
		BaudRate = (80 * 1000000) / baudRate;
		Integer = BaudRate / 16;
		Fraction = BaudRate - (Integer * 16);

		LINFlexD_1.LINIBRR.R = Integer;
		LINFlexD_1.LINFBRR.R = Fraction;
		//					LFDIV = (float)CRYSTAL_Hz/(16*baudRate);
		//					LINFlexD_1.LINIBRR.R = (int)LFDIV;
		//					LFDIV -= (int)LFDIV;
		//					LINFlexD_1.LINFBRR.R = (int)(LFDIV*16);

		//		LINFlexD_0.LINFBRR.R = 11;//8;
		//		LINFlexD_0.LINIBRR.R = 21;//325;
		INTC.PSR[379].B.PRC_SELN0 = 1;
		INTC.PSR[379].B.PRIN = priority;
		LINFlexD_1.LINIER.B.DRIE = 1;

		/* enter NORMAL mode */
		LINFlexD_1.LINCR1.R = 0x00; //0x0080; /* INIT=0 */
		break;

	case 3:
		LINFlexD_3.LINCR1.B.INIT = 1;
		LINFlexD_3.LINCR1.B.SLEEP = 0;

		/* wait for the INIT mode */
		while (0x1000 != (LINFlexD_3.LINSR.R & 0xF000))
		{
		}

		/* configure pads */
		SIUL2.MSCR[74].B.SSS = 1;  /* Pad PB2: Source signal is LIN0_TXD  */
		SIUL2.MSCR[74].B.OBE = 1;  /* Pad PB2: Output Buffer Enable */
		SIUL2.MSCR[74].B.SRC = 3;  /* Pad PB2: Maximum slew rate */
		SIUL2.IMCR[203].B.SSS = 2; /* LIN0_RXD: connected to pad PB3 */
		/* Configure pad PB3 for LIN0RX */
		SIUL2.MSCR[75].B.IBE = 1; /* Pad PB3: Enable pad for input - LIN0_RXD */

		/* configure for UART mode */
		LINFlexD_3.UARTCR.R = 0x0001; /* set the UART bit first to be able to write the other bits */

		LINFlexD_3.UARTCR.R = 0x0033; /* 8bit data, no parity, Tx and Rx enabled, UART mode */
									  /* Transmit buffer size = 1 (TDFL = 0 */
									  /* Receive buffer size = 1 (RDFL = 0) */
		LINFlexD_3.UARTSR.B.SZF = 1;  /* Clear the Zero status bit */
		LINFlexD_3.UARTSR.B.DRFRFE = 1;
		/* configure baudrate 19200 */
		/* assuming 64 MHz peripheral set 1 clock (fsys below)*/
		/* LFDIV = fsys / (16 * desired baudrate)
		 					   LINIBRR = integer part of LFDIV
		 					   LINFBRR = 16 * fractional part of LFDIV (after decimal point)

		 					   for instance:
		 					   LFDIV = 100e6/(16*19200) = 325.52
		 					   LINIBRR = 325
		 					   LINFBRR = 16*0.52 = 8.32
		 					*/
		BaudRate = (80 * 1000000) / baudRate;
		Integer = BaudRate / 16;
		Fraction = BaudRate - (Integer * 16);

		LINFlexD_3.LINIBRR.R = Integer;
		LINFlexD_3.LINFBRR.R = Fraction;
		//					LFDIV = (float)CRYSTAL_Hz/(16*baudRate);
		//					LINFlexD_1.LINIBRR.R = (int)LFDIV;
		//					LFDIV -= (int)LFDIV;
		//					LINFlexD_1.LINFBRR.R = (int)(LFDIV*16);

		//		LINFlexD_0.LINFBRR.R = 11;//8;
		//		LINFlexD_0.LINIBRR.R = 21;//325;
		INTC.PSR[385].B.PRC_SELN0 = 1;
		INTC.PSR[385].B.PRIN = priority;
		LINFlexD_3.LINIER.B.DRIE = 1;

		/* enter NORMAL mode */
		LINFlexD_3.LINCR1.R = 0x00; //0x0080; /* INIT=0 */
		break;
	case 6:
		LINFlexD_6.LINCR1.B.INIT = 1;
		LINFlexD_6.LINCR1.B.SLEEP = 0;

		/* wait for the INIT mode */
		while (0x1000 != (LINFlexD_6.LINSR.R & 0xF000))
		{
		}

		/* configure pads */
		SIUL2.MSCR[102].B.SSS = 2; /* Pad PB2: Source signal is LIN0_TXD  */
		SIUL2.MSCR[102].B.OBE = 1; /* Pad PB2: Output Buffer Enable */
		SIUL2.MSCR[102].B.PUS = 1;
		SIUL2.MSCR[102].B.PUE = 1;
		SIUL2.MSCR[102].B.SRC = 3; /* Pad PB2: Maximum slew rate */
		SIUL2.IMCR[206].B.SSS = 1; /* LIN0_RXD: connected to pad PB3 */
		/* Configure pad PB3 for LIN0RX */
		SIUL2.MSCR[103].B.IBE = 1; /* Pad PB3: Enable pad for input - LIN0_RXD */

		/* configure for UART mode */
		LINFlexD_6.UARTCR.R = 0x0001; /* set the UART bit first to be able to write the other bits */

		LINFlexD_6.UARTCR.R = 0x0033; /* 8bit data, no parity, Tx and Rx enabled, UART mode */
									  /* Transmit buffer size = 1 (TDFL = 0 */
									  /* Receive buffer size = 1 (RDFL = 0) */
		LINFlexD_6.UARTSR.B.SZF = 1;  /* Clear the Zero status bit */
		LINFlexD_6.UARTSR.B.DRFRFE = 1;
		/* configure baudrate 19200 */
		/* assuming 64 MHz peripheral set 1 clock (fsys below)*/
		/* LFDIV = fsys / (16 * desired baudrate)
				 					   LINIBRR = integer part of LFDIV
				 					   LINFBRR = 16 * fractional part of LFDIV (after decimal point)

				 					   for instance:
				 					   LFDIV = 100e6/(16*19200) = 325.52
				 					   LINIBRR = 325
				 					   LINFBRR = 16*0.52 = 8.32
				 					*/
		BaudRate = (80 * 1000000) / baudRate;
		Integer = BaudRate / 16;
		Fraction = BaudRate - (Integer * 16);

		LINFlexD_6.LINIBRR.R = Integer;
		LINFlexD_6.LINFBRR.R = Fraction;
		//					LFDIV = (float)CRYSTAL_Hz/(16*baudRate);
		//					LINFlexD_1.LINIBRR.R = (int)LFDIV;
		//					LFDIV -= (int)LFDIV;
		//					LINFlexD_1.LINFBRR.R = (int)(LFDIV*16);

		//		LINFlexD_0.LINFBRR.R = 11;//8;
		//		LINFlexD_0.LINIBRR.R = 21;//325;
		INTC.PSR[394].B.PRC_SELN0 = 1;
		INTC.PSR[394].B.PRIN = priority;
		LINFlexD_6.LINIER.B.DRIE = 1;

		if (enableTIE)
		{
			INTC.PSR[395].B.PRC_SELN0 = 1;
			INTC.PSR[395].B.PRIN = priority - 1;
			LINFlexD_6.LINIER.B.DTIE = 1;
		}
		/* enter NORMAL mode */
		LINFlexD_6.LINCR1.R = 0x00; //0x0080; /* INIT=0 */
		break;

	default:
		break;
	}
}

/*******************************************************************************
Function Name : TransmitData
Engineer      : b05111
Date          : Apr-14-2011
Parameters    : pBuf - input string. won't be modified by the function
              : cnt  - input number of characters to be transmitted
Modifies      : NONE
Returns       : NONE
Notes         : Tx data on LINFlexD_0. polled mode.
Issues        : NONE 
*******************************************************************************/
void TransmitData(const char * const pBuf, const uint32_t cnt)
{
    uint8_t	j = 0; // Dummy variable
    
    for (j=0; j< cnt; j++) 
    {  // Loop for character string  
   	    LINFlexD_0.BDRL.B.DATA0 = *(pBuf+j);
   	      //write character to transmit buffer
	    while (1 != LINFlexD_0.UARTSR.B.DTFTFF) {}
	      // Wait for data transmission completed flag
	    LINFlexD_0.UARTSR.R = 0x0002;
	      // clear the DTF flag and not the other flags
    }

    
}

void Transmit1Data(const char * const pBuf, const uint32_t cnt)
{
    uint8_t	j = 0; // Dummy variable

    for (j=0; j< cnt; j++)
    {  // Loop for character string
   	    LINFlexD_1.BDRL.B.DATA0 = *(pBuf+j);
   	      //write character to transmit buffer
	    while (1 != LINFlexD_1.UARTSR.B.DTFTFF) {}
	      // Wait for data transmission completed flag
	    LINFlexD_1.UARTSR.R = 0x0002;
	      // clear the DTF flag and not the other flags
    }


}

void Transmit3Data(const char * const pBuf, const uint32_t cnt)
{
    uint8_t	j = 0; // Dummy variable

    for (j=0; j< cnt; j++)
    {  // Loop for character string
   	    LINFlexD_3.BDRL.B.DATA0 = *(pBuf+j);
   	      //write character to transmit buffer
	    while (1 != LINFlexD_3.UARTSR.B.DTFTFF) {}
	      // Wait for data transmission completed flag
	    LINFlexD_3.UARTSR.R = 0x0002;
	      // clear the DTF flag and not the other flags
    }


}

void Transmit6Data(const char * const pBuf, const uint32_t cnt)
{
    uint8_t	j = 0; // Dummy variable

    for (j=0; j< cnt; j++)
    {  // Loop for character string
   	    LINFlexD_6.BDRL.B.DATA0 = *(pBuf+j);
   	      //write character to transmit buffer
	    while (1 != LINFlexD_6.UARTSR.B.DTFTFF) {}
	      // Wait for data transmission completed flag
	    LINFlexD_6.UARTSR.R = 0x0002;
	      // clear the DTF flag and not the other flags
    }


}



/*******************************************************************************
Function Name : ReceiveData
Engineer      : b05111
Date          : Apr-14-2011
Parameters    : pBuf - address of a char where the received char is written to
                       the address (pBuf) doesn't change in the function
Modifies      : NONE
Returns       : NONE
Notes         : Rx data on LINFlexD_0. polled mode.
Issues        : NONE 
*******************************************************************************/
int32_t ReceiveData(char * const pBuf)
{
    
    int32_t rx_data;
    uint32_t counter = 0;
    	/* wait for DRF */
	while (1 != LINFlexD_0.UARTSR.B.DRFRFE) {
		counter++;
		if(counter > UART_RECV_BYTE_TIME_OUT)
		{
			return kUARTNoData;
		}
	}  /* Wait for data reception completed flag */
		
	/* wait for RMB */
	while (1 != LINFlexD_0.UARTSR.B.RMB) {}  /* Wait for Release Message Buffer */
	
	/* get the data */
	
	rx_data = LINFlexD_0.BDRM.R; // read whole register due to erratum e4897PS
    
	*pBuf = (uint8_t)rx_data; // take 

	/* clear the DRF and RMB flags by writing 1 to them */
	LINFlexD_0.UARTSR.R = 0x0204;

	return kUARTNoError;
}


/*******************************************************************************
* Global functions
*******************************************************************************/ 

/*
Methods called by MW MSL libraries to perform console IO:
*/

/**********************************************************************
 description : UART初始化配置函数
 @argument instance			: UART_COM
 @argument can_baudrate_t 	: 波特率
 @argument priority			: 接受中断优先级，0-15 (15为最高优先级)
 @return : none
 **********************************************************************/
UARTError flexLinUartInit(UART_COM_t instance,UARTBaudRate baudRate,unsigned char priority, unsigned char enableTIE)
{
	LINFlexD_0_Init (instance,baudRate,priority,enableTIE);
	return kUARTNoError;
}


#if 0
UARTError ReadUARTN(void* bytes, unsigned long limit)
{
	int count;
	UARTError err;

	for (count = 0, err = kUARTNoError;
		count < limit && err == kUARTNoError;
		count++)
        {
			err = ReceiveData( (char *)bytes + count );
			if(err == kUARTNoData )
				count = 0;
//			if(err == kUARTNoError)
//				LOG("count = %d %d \n",count,*(uint8_t*)(bytes+count));
        }

	return count;//return err;
}
#endif

UARTError ReadUARTN(void* bytes,unsigned short* recv_len,unsigned short expect_len)
{
	int count;
	UARTError err;
	unsigned short *p = recv_len;
	*p = 0;
	for (count = 0, err = kUARTNoError;
		count < expect_len;
		count++)
        {
			err = ReceiveData( (char *)bytes + count );
			if(err == kUARTNoError )
				*p += 1;
			else
				return err;
			if((count+1) == expect_len)
			{
				return kUARTNoError;
			}
//			if(err == kUARTNoError)
//				LOG("count = %d %d \n",count,*(uint8_t*)(bytes+count));
        }

	return kUARTBufferOverflow;//return err;
}

UARTError ReadUARTPoll(char* c)
{
    int32_t rx_data;
    rx_data = LINFlexD_0.BDRM.R; // read whole register due to erratum e4897PS

    if (LINFlexD_0.UARTSR.B.RMB == 0)
    {
        return  kUARTNoData;  // return no data
    }
    else
    {
        LINFlexD_0.UARTSR.R = 0x0204;
        *c =(unsigned char) rx_data; // read byte of Data
        return kUARTNoError;  // return no error
    }
}



UARTError WriteUARTN(const void * bytes, unsigned long length)
{ 
	TransmitData ((const char * const)bytes,length);
  	return kUARTNoError;
}


void uart_printf_format(char* format,...)
{
	char buf[512] = {0};
	va_list argv;
	va_start(argv,format);
	vsprintf(buf,format,argv);
	va_end(argv);

	TransmitData(buf,strlen(buf));
}

void uart_send(void)
{
    unsigned char i =0;

    for(i=0;i<7;i++)
    {
       if(flag_uartsend[i]==0)
    	   continue;
       switch(i)
       {
          case 0:
        	  TransmitData((char*)&LinFlex_UartRX_Buf[0],1);
        	  break;
          case 1:
        	  Transmit1Data((char*)&LinFlex_UartRX_Buf[0],1);
        	   break;
          case 3:
        	  Transmit3Data((char*)&LinFlex_UartRX_Buf[0],1);
        	   break;
          case 6:
        	  Transmit6Data((char*)&LinFlex_UartRX_Buf[0],1);
        	   break;
          default:
        	  break;

       }

       flag_uartsend[i]=0;
    }
}
