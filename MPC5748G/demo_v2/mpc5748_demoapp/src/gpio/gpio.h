/*
 * gpio.h
 *
 *  Created on: 2018年12月19日
 *      Author: Hjian
 */

#ifndef GPIO_GPIO_H_
#define GPIO_GPIO_H_
#include "project.h"

#define CHIP_POWER_CONTROL			PQ0
#define FLEXLIN_GPIO_CONFIG_1		PC12
#define FLEXLIN_GPIO_CONFIG_2		PC13
#define FLEXLIN_GPIO_CONFIG_3		PC14
#define FLEXLIN_GPIO_CONFIG_4		PC15


#define LAC_V_LAC   0
#define LAC_V_OLD   1
#define LAC_V21     21
/**
 * 为了兼容多个硬件版本，合并到master分支之前必须注释掉LAC，编译的时候再反注释，防止程序员忘记改对应的硬件版本
 * */
//#define LAC         LAC_V_LAC
#define LAC         LAC_V_OLD
//#define LAC         LAC_V21
#ifndef LAC
#error 未定义LAC
#endif

typedef enum{
	INPUT,
	OUTPUT
}GPIO_TYPE;


void generalGpioInit(void);
void gpioTypeInit(int gNum,GPIO_TYPE type,int init_status);
void gpioOperation(int gNum,uint8_t opt);
void gpioToggle(int gNum);
void gpio_uart0_cfg(void);
#endif /* GPIO_GPIO_H_ */
