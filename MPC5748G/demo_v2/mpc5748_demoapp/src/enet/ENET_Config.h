/*
 * ENET_Config.h
 *
 *  Created on: Sept 13, 2014
 *      Author: B34981
 *
 *  Modified: April, 2015
 *      Author: B46845
 */

#ifndef ENET_CONFIG_H_
#define ENET_CONFIG_H_

#include "common/common.h"

/**********************************************************************************************
* ENET Driver Configuration Macros
**********************************************************************************************/
/* DESTINATION MAC ADDRESS */		/* 此处设置无效 */
#define DESTMAC1 0xf4
#define DESTMAC2 0x4d
#define DESTMAC3 0x30
#define DESTMAC4 0x7b
#define DESTMAC5 0x87
#define DESTMAC6 0x6b


/* DESTINATION IP ADDRESS */		/* 设置目标IP */
#define CTRL_IP1 192
#define CTRL_IP2 168
#define CTRL_IP3 1
#define CTRL_IP4 8

#define PLAN_IP1 192
#define PLAN_IP2 168
#define PLAN_IP3 1
#define PLAN_IP4 (102 + CONTROLLER_ID * 2)

#define BROADCAST_IP1 192
#define BROADCAST_IP2 168
#define BROADCAST_IP3 1
#define BROADCAST_IP4 255

/* ORIGIN MAC ADDRESS */			/* 此处设置本机mac地址 */
#define ORIGMAC1 0x00
#define ORIGMAC2 0x04
#define ORIGMAC3 0x9F
#define ORIGMAC4 0x22
#define ORIGMAC5 0x0E
#define ORIGMAC6 (0xB0 + CONTROLLER_ID)

/* ORIGIN IP ADDRESS */				/* 此处设置本机IP地址 */
#define ORIGIP1 192
#define ORIGIP2 168
#define ORIGIP3 1
#define ORIGIP4 (200 + CONTROLLER_ID)

/* DESTINATION PORT */				/* 此处设置无效 */
#define DESTPORT 1086

/* ORIGIN PORT */
#define ORIGPORT 1086

/**********************************************************************************************
* Constants
**********************************************************************************************/


/**********************************************************************************************
* Macros
**********************************************************************************************/


/**********************************************************************************************
* Types
**********************************************************************************************/


/**********************************************************************************************
* Variables
**********************************************************************************************/


/**********************************************************************************************
* Global Variables
**********************************************************************************************/


/**********************************************************************************************
* Global Functions
**********************************************************************************************/

/***********************************************************************************************
*
* @brief    none
* @param    none
* @return   none
*
************************************************************************************************/


#endif /* ENET_CONFIG_H_ */
