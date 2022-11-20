/*
 * pit.c
 *
 *  Created on: 2018年8月10日
 *      Author: luxin
 */
#include "derivative.h"
#include "pit.h"



/**********************************************************************
 description : PIT定时器初始化配置函数
 @argument instance			: PIT 0,1,2
 @argument can_baudrate_t 	: 定时间隔计数器， 40000000为1s，可按比例换算成ms或us
 @argument priority			: 接受中断优先级，0-15 (15为最高优先级)，>15则不开中断
 @return : none
 **********************************************************************/
void pitTimerInit(PIT_TIMER_t instance,uint32_t LDVAL,uint8_t priority)
{
	PIT.MCR.B.MDIS 				= 0;
	PIT.MCR.B.FRZ 				= 0;  		/* Freeze PIT timers in debug mode */
	switch(instance)
	{
	case 0:
		PIT.TIMER[0].LDVAL.R 		= LDVAL - 1;  	/* Load # Pit clocks to count */
		
		if (priority <= 15)
		{
			PIT.TIMER[0].TCTRL.B.TIE 	= 1;      	/* Enable interrupt */
			INTC.PSR[226].B.PRC_SELN0 	= 1;  		/* IRQ sent to Core 2 */
			INTC.PSR[226].B.PRIN 		= priority;       /* IRQ priority = 10 (15 = highest) */			
		}
		else
		{
			PIT.TIMER[0].TCTRL.B.TIE 	= 0;
		}
		
		PIT.TIMER[0].TCTRL.B.TEN 	= 1;      	/* enable channel */
		break;
	case 1:
		PIT.TIMER[1].LDVAL.R 			= LDVAL - 1;
		
		if (priority <= 15)
		{
			PIT.TIMER[1].TCTRL.B.TIE 		= 1;
			INTC.PSR[227].B.PRC_SELN0 		= 1;
			INTC.PSR[227].B.PRIN 			= priority;
		}
		else
		{
			PIT.TIMER[1].TCTRL.B.TIE 		= 0;
		}
		
		PIT.TIMER[1].TCTRL.B.TEN 		= 1;
		break;
	case 2:
		PIT.TIMER[2].LDVAL.R 		= LDVAL - 1;
		
		if (priority <= 15)
		{
			PIT.TIMER[2].TCTRL.B.TIE 	= 1;
			INTC.PSR[228].B.PRC_SELN0 	= 1;
			INTC.PSR[228].B.PRIN 		= priority;
		}
		else
		{
			PIT.TIMER[2].TCTRL.B.TIE 	= 0;
		}
		
		PIT.TIMER[2].TCTRL.B.TEN 	= 1;
		break;
	default:
		break;
	}
}



