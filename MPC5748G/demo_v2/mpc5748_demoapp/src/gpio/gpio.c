/*
 * gpio.c
 *
 *  Created on: 2018年12月19日
 *      Author: Hjian
 */
#include "derivative.h"
#include "gpio.h"
#include "project.h"



void generalGpioInit(void)
{
//	unsigned long count = 0 ;
	gpioTypeInit(CHIP_POWER_CONTROL,OUTPUT,1);

	gpioTypeInit(FLEXLIN_GPIO_CONFIG_1,OUTPUT,0);
	gpioTypeInit(FLEXLIN_GPIO_CONFIG_2,OUTPUT,1);
	gpioTypeInit(FLEXLIN_GPIO_CONFIG_3,OUTPUT,0);
	gpioTypeInit(FLEXLIN_GPIO_CONFIG_4,OUTPUT,0);

#if defined(NONE_BOOTLD_MODE)
#ifdef LAC
    #if LAC == LAC_V21
        // do nothing
    #elif LAC == LAC_V_OLD
        gpioTypeInit(PI0,OUTPUT,1);
        gpioTypeInit(PI1,OUTPUT,1);
        gpioTypeInit(PI2,OUTPUT,1);
    #else
        gpioTypeInit(PI0,OUTPUT,0);
        gpioTypeInit(PI1,OUTPUT,0);
        gpioTypeInit(PI2,OUTPUT,0); //LAC_HARDWARE
    #endif
#else
    gpioTypeInit(PI1,OUTPUT,1); //AC(OLD)_HARDWARE
#endif
#endif


}

void gpio_uart0_cfg(void)
{

	    gpioTypeInit(26,OUTPUT,0);
		gpioTypeInit(27,OUTPUT,1);
		gpioTypeInit(28,OUTPUT,0);
		gpioTypeInit(29,OUTPUT,1);
}


/**********************************************************************
 description 			: GPIO初始化函数
 @argument gNum			: 引脚GPIO功能对应的SIUL2.MSCR寄存器编号
 @argument type			: 输入or输出
 @argument init_status	: 初始化为高或低（若为输出）
 @return : none
 **********************************************************************/
void gpioTypeInit(int gNum,GPIO_TYPE type,int init_status)
{
	SIUL2.MSCR[gNum].B.SSS = 0;              /* choose I/O function */
	if(type == OUTPUT)
	{
		SIUL2.MSCR[gNum].B.PUE = 1;
		SIUL2.MSCR[gNum].B.OBE = 1;          /* Output Buffer Enable on */
		SIUL2.MSCR[gNum].B.IBE = 0;          /* Input Buffer Enable off */
		SIUL2.GPDO[gNum].R = init_status;
	}
	else
	{
		SIUL2.MSCR[gNum].B.OBE = 0;          /* Output Buffer Enable on */
		SIUL2.MSCR[gNum].B.IBE = 1;          /* Input Buffer Enable off */
	}

}


/**********************************************************************
 description 		: 操作GPIO输出高或低
 @argument gNum		: 引脚GPIO功能对应的SIUL2.MSCR寄存器编号
 @argument opt		: 0(低） or 1（高）
 @return : none
 **********************************************************************/
void gpioOperation(int gNum,uint8_t opt)
{
	SIUL2.GPDO[gNum].R = opt;
}


/**********************************************************************
 description : GPIO输出电平翻转
 @argument gNum		: 引脚GPIO功能对应的SIUL2.MSCR寄存器编号
 @return : none
 **********************************************************************/
void gpioToggle(int gNum)
{
	if(SIUL2.GPDO[gNum].R & 0x1)
	{
		SIUL2.GPDO[gNum].R = 0;
	}
	else
	{
		SIUL2.GPDO[gNum].R = 1;
	}
}

void gpio_intc_config(void)
{
	/*SIUL2.MSCR[gNum].B.OBE = 0;
	SIUL2.MSCR[gNum].B.IBE = 1;
	SIUL2.IMCR[].B.SSS = 1 ;
	SIUL2.IREER0.B.IREE0 = 1;
	SIUL2.IRER0.B.EIRE0= 1;


	WKPU.WISR.R = 0x40;  //0x02000000
	WKPU.WIREER.R = 0x40; //WKPU[6] PE0
	WKPU.WRER.R = 0x40;


	*/


}


