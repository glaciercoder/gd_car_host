/*
 * main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "ENET_Config.h"
#include "user_udp.h"
#include "udp_handle.h"
#include "pit.h"
#include "uart.h"
#include "gpio.h"
#include "stdio.h"
#include "stdlib.h"
#include "cJSON.h"
#include "user_can.h"
#include "common.h"
#include "ByteQueue.h"

#define KEY_VALUE1 0x5AF0ul
#define KEY_VALUE2 0xA50Ful

extern void xcptn_xmpl(void);

extern int global_flag_send_arp_request;


extern unsigned char udp_test_flag;

unsigned char ElectricAcPedal;
unsigned short BrakeAccleration, wheelangle;
volatile int global_control_algo_flag;


unsigned char global_security_flag = 0;

unsigned long lat_in_times,lat_in_timems;
unsigned long lat_out_times,lat_out_timems;

volatile extern struct time_s 	sys_local_time;
static void peri_clock_gating(void)
{
	MC_ME.RUN_PC[0].R = 0x00000000; /* gate off clock for all RUN modes */
	MC_ME.RUN_PC[1].R = 0x000000FE; /* config. peri clock for all RUN modes */

	MC_ME.PCTL[15].B.RUN_CFG = 0x1; /* ENET 0: select peri. cfg. RUN_PC[1] */
	MC_ME.PCTL[91].B.RUN_CFG = 0x1; /* PIT: select peripheral configuration RUN_PC[1] */
	MC_ME.PCTL[71].B.RUN_CFG = 0x1; /*FLEXCAN 1:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[70].B.RUN_CFG = 0x1; /*Flexcan 0:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[72].B.RUN_CFG = 0x1; /*Flexcan 2:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[73].B.RUN_CFG = 0x1; /*Flexcan 3:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[74].B.RUN_CFG = 0x1; /*FLEXCAN 1:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[75].B.RUN_CFG = 0x1; /*Flexcan 0:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[76].B.RUN_CFG = 0x1; /*Flexcan 2:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[77].B.RUN_CFG = 0x1; /*Flexcan 3:select peri. configuration RUN_PC[1] */
	MC_ME.PCTL[50].B.RUN_CFG = 0x1; /* Flexlin-uart:select peri. configuration RUN_PC[1] */
}

__attribute__((unused)) static void stop_mode_config(void)
{
	MC_ME.ME.B.STOP0 = 1;
	MC_ME.STOP_MC.R = 0x001101BF;

	MC_ME.LP_PC[1].R = 0x2400;
	MC_ME.PCTL[15].B.LP_CFG = 1;
	MC_ME.PCTL[91].B.LP_CFG = 1;
	MC_ME.PCTL[71].B.LP_CFG = 1;
	MC_ME.PCTL[70].B.LP_CFG = 1;
	MC_ME.PCTL[72].B.LP_CFG = 1;
	MC_ME.PCTL[73].B.LP_CFG = 1;
	MC_ME.PCTL[74].B.LP_CFG = 1;
	MC_ME.PCTL[75].B.LP_CFG = 1;
	MC_ME.PCTL[76].B.LP_CFG = 1;
	MC_ME.PCTL[77].B.LP_CFG = 1;
	MC_ME.PCTL[50].B.LP_CFG = 1;

	MC_ME.MCTL.R = 0xA0005AF0;
	MC_ME.MCTL.R = 0xA000A50F;
}

__attribute__((unused)) static void sysclk_init_RUN0_MODE(void)
{
	MC_ME.ME.R = 0x000005FF;
	MC_ME.RUN_PC[0].R = 0x000000FE;

	/* Connect XOSC to PLL */
	MC_CGM.AC5_SC.B.SELCTL = 1;
	/* Configure PLL0 Dividers - 160MHz from 40Mhx XOSC */
	PLLDIG.PLLDV.B.PREDIV = 2;
	PLLDIG.PLLDV.B.MFD = 32;
	PLLDIG.PLLDV.B.RFDPHI = 1;
	PLLDIG.PLLCAL3.R = 0x09C3C000;
	PLLDIG.PLLFD.B.SMDEN = 1; /* Sigma delta modulation disabled */
	/* switch to PLL */
	MC_ME.RUN_MC[0].R = 0x00130172; /* FLASH in normal mode, PLLON, FXOSC ON, Use PLL_PHI_0 */
	MC_ME.RUN_MC[0].B.SIRCON = 1;

	MC_ME.MCTL.R = 0x40005AF0; /* DRUN target mode + KEY */
	MC_ME.MCTL.R = 0x4000A50F; /* DRUN target mode + KEY Inverted */

	while (MC_ME.GS.B.S_MTRANS == 1)
		; /* Wait for mode transition complete */
	while (MC_ME.GS.B.S_CURRENT_MODE != 4)
		;

	/* Enable CLKOUT signal */
	/* System clock (160MHz divided by 20 = 8MHz) will be visible on pin PG[7] */
	/* Pin PG[7] is configured in SIUL2_Init() function */
	MC_CGM.AC6_DC0.B.DE = 1; /* Enable output clock devided */
	MC_CGM.AC6_DC0.B.DIV = 0x13; /* Divide output clock by 20 */
	MC_CGM.AC6_SC.B.SELCTL = 0x2; /* PLL_CLKOUT1 */

}

static void Sysclk_Init_160MHz(void)
{
	/* enable all modes, enable all peripherals */
	MC_ME.ME.R = 0x000005FF;
	MC_ME.RUN_PC[0].R = 0x000000FE;

	/* Connect XOSC to PLL */
	MC_CGM.AC5_SC.B.SELCTL = 1;
	/* Configure PLL0 Dividers - 160MHz from 40Mhx XOSC */
	PLLDIG.PLLDV.B.PREDIV = 2;
	PLLDIG.PLLDV.B.MFD = 32;
	PLLDIG.PLLDV.B.RFDPHI = 1;
	PLLDIG.PLLCAL3.R = 0x09C3C000;
	PLLDIG.PLLFD.B.SMDEN = 1; /* Sigma delta modulation disabled */
	/* switch to PLL */
	MC_ME.DRUN_MC.R = 0x00130172; /* FLASH in normal mode, PLLON, FXOSC ON, Use PLL_PHI_0 */
	MC_ME.DRUN_MC.B.SIRCON = 1;

	MC_ME.MCTL.R = 0x30005AF0; /* DRUN target mode + KEY */
	MC_ME.MCTL.R = 0x3000A50F; /* DRUN target mode + KEY Inverted */

	while (MC_ME.GS.B.S_MTRANS == 1)
		; /* Wait for mode transition complete */

	/* Enable CLKOUT signal */
	/* System clock (160MHz divided by 20 = 8MHz) will be visible on pin PG[7] */
	/* Pin PG[7] is configured in SIUL2_Init() function */
	MC_CGM.AC6_DC0.B.DE = 1; /* Enable output clock devided */
	MC_CGM.AC6_DC0.B.DIV = 0x13; /* Divide output clock by 20 */
	MC_CGM.AC6_SC.B.SELCTL = 0x2; /* PLL_CLKOUT1 */
}

void SWT_Init(void)
{
	MC_RGM.DRET.B.DRET = 0; /*锟斤�? Destructive Reset锟侥达拷锟斤拷锟斤拷�?*/
	while (MC_ME.DRUN_MC.B.SIRCON == 0)
		; /*should open SIRC if use watchdog*/
	SWT_0.SR.R = 0xC520U;
	SWT_0.SR.R = 0xD928U;

	while (SWT_0.CR.B.SLK == 0x1U)
		;

	SWT_0.TO.R = 128000-1; // 1s
	SWT_0.CR.R = 0xFF000101U;
	SWT_0.CR.B.WEN = 0x1U;

}

void SWT_Service(void)
{

	SWT_0.SR.R = 0xC520U; /*ulock*/
	SWT_0.SR.R = 0xD928U;
	while (SWT_0.CR.B.SLK == 0x1U)
		;

	SWT_0.SR.R = 0xA602U;
	SWT_0.SR.R = 0xB480U;
}

void rcv_replydata(void)
{

}

void hw_init(void)
{
#if defined(DEBUG_SECONDARY_CORES)
	uint32_t mctl = MC_ME.MCTL.R;
#if defined(TURN_ON_CPU1)
	/* enable core 1 in all modes */
	MC_ME.CCTL[2].R = 0x00FE;
	/* Set Start address for core 1: Will reset and start */
#if defined(START_FROM_FLASH)
	MC_ME.CADDR[2].R = 0x1040000 | 0x1;
#else
	MC_ME.CADDR[2].R = 0x40040000 | 0x1;
#endif /* defined(START_FROM_FLASH) */

#endif
#if defined(TURN_ON_CPU2)
	/* enable core 2 in all modes */
	MC_ME.CCTL[3].R = 0x00FE;
	/* Set Start address for core 2: Will reset and start */
#if defined(START_FROM_FLASH)
	MC_ME.CADDR[3].R = 0x1540000 | 0x1;
#else
	MC_ME.CADDR[3].R = 0x40080000 | 0x1;
#endif /* defined(START_FROM_FLASH) */

#endif
	MC_ME.MCTL.R = (mctl & 0xffff0000ul) | KEY_VALUE1;
	MC_ME.MCTL.R = mctl; /* key value 2 always from MCTL */
#endif /* defined(DEBUG_SECONDARY_CORES) */
}

void PowerPinProcess(void);
int send_Udp_Testdata(void);


void Cpu_DelayMS(uint32_t ms)
{
    int32_t Cpu_Freq;
    while(ms--)
    {
        for(Cpu_Freq = 6206;Cpu_Freq>0;)
            Cpu_Freq--;
    }
}

#if LAC == LAC_V21 && defined(NONE_BOOTLD_MODE)
static void _PowerPinInit(void)
{
    char step = 0;
    uint32_t count = 0;
    gpioTypeInit(PI0, OUTPUT, 0);
    gpioTypeInit(PI6, OUTPUT, 0);
    gpioTypeInit(PI7, OUTPUT, 0);
    gpioTypeInit(PI8, OUTPUT, 0);
    gpioTypeInit(PI9, OUTPUT, 0);

    for (;;)
    {
        Cpu_DelayMS(1);
        count++;
        if (step == 0 && count > 30u)
        {
            gpioOperation(PI6, 1u);
            gpioOperation(PI7, 1u);
            step++;
        }
        else if (step == 1 && count > 120u)
        {
            gpioOperation(PI8, 1u);
            gpioOperation(PI9, 1u);
            step++;
        }
        else if (step == 2)
            break;
    }
}
#endif

#if defined(DEBUG_REALTIME)
volatile uint32_t localpose_tick_last;
volatile uint32_t algo_tick1, algo_tick2, algo_ticks;
#endif

int main(void)
{
	volatile int counter = 0;
	xcptn_xmpl(); /* Configure and Enable Interrupts */
	Sysclk_Init_160MHz();
	peri_clock_gating();

	//sysclk_init_RUN0_MODE(); // RUNO mode config

	gpio_uart0_cfg();
	/*application init */
#if LAC == LAC_V21 && defined(NONE_BOOTLD_MODE)
	_PowerPinInit();
#endif

	flexLinUartInit(UART_COM_0, kBaud115200, 12,0); // FOR TEST USE by
	flexLinUartInit(UART_COM_1, kBaud115200, 12,0);
	flexLinUartInit(UART_COM_3, kBaud115200, 12,0);
	flexLinUartInit(UART_COM_6, kBaud9600, 13,1);

	generalGpioInit();

	flexCanInitConfig(CAN_COM_1, canBaud500k, 10);
	
	pitTimerInit(PIT_0, 40000, 10); /* PIT0 interrupt  1 ms */
	pitTimerInit(PIT_1, 40000, 10); /* PIT1 interrupt  1 ms */
	pitTimerInit(PIT_2,0,0xff);		/* PIT2 no interrupt */

	udpEnetDriverInit(12);
#if !defined(DEBUG_WDG_OFF)
	SWT_Init();
#endif


	for (;;)
	{
		counter++;
#if !defined(DEBUG_WDG_OFF)
		SWT_Service();
#endif
		
		CanRecvExcuteMsg();
	   	uart_send();//test_rcvuart

		if (global_flag_send_arp_request)
		{
			ENET_ARP_Request_Tx(udpSendDstAddr_ctrlNode.dst_ip);
			ENET_ARP_Request_Tx(udpSendDstAddr_planNode.dst_ip);
			global_flag_send_arp_request = 0;
		}

		
		if (ENET_ARP_RequestFlag)
		{
			ENET_ARP_Response();
		}

		if (ENET_ICMP_RequestFlag)
		{
			ENET_ICMP_Response();
		}
		
//		if (udp_test_flag == 1)
//		{
//			send_Udp_Testdata();
//			udp_test_flag = 0;
//		}


#if LAC >= LAC_V_OLD && !defined(NONE_BOOTLD_MODE)
//		PowerPinProcess();
#endif

	}
}

int send_Udp_Testdata(void)
{
	
	uint8_t Udp_TestBUF[6] = {12, 34, 56, 78, 9, 10};
	udpmuticoretest(Udp_TestBUF, 6);
	return 0;

}

#if LAC == LAC_V21
/**
 * 鐢垫簮鎺у埗寮曡剼澶勭�? * 鏃跺簭濡備笅锛�
 *         PI0 PI6 PI7 PI8 PI9
 * t=0s    H   L   L   L   L
 * t=1s    L   L   L   L   L
 * t=1.03s L   H   H   L   L
 * t=1.12s L   H   H   H   H
*/
void PowerPinProcess(void)
{
    static uint8_t Step = 0;
    static uint32_t MsecsNoneMac = 0xffffffff;
    static uint32_t SecsNoneMac = 0xffffffff;

    if (Step == 0)
    {
        if (!isMacAvailable(&udpSendDstAddr_ctrlNode) || !isMacAvailable(&udpSendDstAddr_planNode))
        {
            if (SecsNoneMac == 0xffffffff)
            {
                SecsNoneMac = sys_local_time.sec;
            }
            else if (sys_local_time.sec - SecsNoneMac > 20)
            {
                gpioOperation(PI0, 1);
                gpioOperation(PI6, 0);
                gpioOperation(PI7, 0);
                gpioOperation(PI8, 0);
                gpioOperation(PI9, 0);
                Step = 1;
            }
        }
        else
        {
            Step = 0xff; // 閮芥湁MAC
        }
    }
    else if (Step == 1)
    {
        if (sys_local_time.sec - SecsNoneMac > 21)
        {
            gpioOperation(PI0, 0);
            MsecsNoneMac = PIT.TIMER[2].CVAL.R;
            Step = 2;
        }
    }
    else if (Step == 2)
    {
        if (MsecsNoneMac - PIT.TIMER[2].CVAL.R > 40000 * 30)
        {
            gpioOperation(PI6, 1);
            gpioOperation(PI7, 1);
            Step = 3;
        }
    }
    else if (Step == 3)
    {
        if (MsecsNoneMac - PIT.TIMER[2].CVAL.R > 40000 * 120)
        {
            gpioOperation(PI8, 1);
            gpioOperation(PI9, 1);
            Step = 0xff;
        }
    }
}
#elif LAC == LAC_V_OLD
/**
 * 鐢垫簮鎺у埗寮曡剼澶勭�? * 鏃跺簭濡備笅锛�
 *       PI0 PI1 PI2
 * t=0s  L   L   L
 * t=2s  H   H   L
 * t=3s  H   H   H
*/
void PowerPinProcess(void)
{
	static uint8_t Step = 0;
	static uint32_t SecsNoneMac = 0xffffffff;
	
	if (Step == 0)
	{
		if (!isMacAvailable(&udpSendDstAddr_ctrlNode) || !isMacAvailable(&udpSendDstAddr_planNode))
		{
			if (SecsNoneMac == 0xffffffff)
			{
				SecsNoneMac = sys_local_time.sec;
			}
			else if (sys_local_time.sec - SecsNoneMac > 20)
			{
				gpioOperation(PI0, 0);
				gpioOperation(PI1, 0);
				gpioOperation(PI2, 0);
				Step = 1;
			}
		}
		else
		{
			Step = 0xff; // 閮芥湁MAC
		}
	}
	else if (Step == 1)
	{
		if (sys_local_time.sec - SecsNoneMac > 22)
		{
			gpioOperation(PI0, 1);
			gpioOperation(PI1, 1);
			Step = 2;
		}
	}
	else if (Step == 2)
	{
		if (sys_local_time.sec - SecsNoneMac > 23)
		{
			gpioOperation(PI2, 1);
			Step = 0xff;
		}
	}
}
#endif
