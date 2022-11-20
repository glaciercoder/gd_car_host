/*
 * udprx_handler.c
 *
 *  Created on: 2019年4月15日
 *      Author: luxin
 */

#include "user_udp.h"
#include "udp_handle.h"
#include "stdio.h"
#include "stdlib.h"
#include "../cJSON.h"


#include "can.h"

#include <math.h>

unsigned char test_rcvbuf[1024];
unsigned char auto_gear,auto_brake,auto_haden,auto_vcuen,auto_escen,auto_epsen,auto_epben;

volatile float aebBrakeAcc;

unsigned long global_epbtorun_timeout = 0;
unsigned long global_braketorun_timeout = 0;
unsigned long global_braketoepb_timeout = 0;
unsigned long global_runtobrake_timeout = 0;
unsigned long global_brakeinepb_timeout = 0;

unsigned char Pedestrian_Avoidance_flag = 0;

enum vehicle_mode_t vehicle_mode = epb_mode;


unsigned long auto_drive_timeout = 0;
unsigned short goal_nums;




extern CanTSET_buff can_testbuff;

/**************************************************************************
description:收到UDP数据的处理函数
@para data : 数据首个字节地址
@para len  : 数据长度
@return : 0
**************************************************************************/
int udpPortRxHandle_1088(uint8_t* data,uint16_t len)
{
	uint32_t id = 0;
	id = (data[8] << 24) | (data[9] << 16) | (data[10] << 8) | (data[11]);
    can_frame_t can_data ={ 0 };
	can_data.id = id;
    can_data.dlc = 8;
    can_data.send_com = CAN_COM_1;
    can_data.type = CAN_STD;
    int i = 0;
    for(i = 0;i < 8;i++){
    	can_data.data[i] = data[i];
    }
	flexCanMsgTransmit(&can_data);

	return 0;
}



/**************************************************************************
description:收到UDP数据的处理函数
@para data : 数据首个字节地址
@para len  : 数据长度
@return : 0
**************************************************************************/
int udpPortRxHandle_4626(uint8_t* data,uint16_t len)
{
	return 0;
}

int udpPortRxHandle_1090(uint8_t* data,uint16_t len)
{
	return 0;
}

int udpPortRxHandle_1091(uint8_t* data,uint16_t len)
{
		return 0;

}

int udpPortRxHandle_1092(uint8_t* data,uint16_t len)
{
		return 0;

}

int udpPortRxHandle_1093(uint8_t* data,uint16_t len)
{
		return 0;

}

int udpPortRxHandle_1081(uint8_t* data,uint16_t len)
{
	 return 0;
}

void vehicle_status_switch(void)
{

}


int udpPortRxHandle_1066(uint8_t* data,uint16_t len)
{
	return 0;

}

int udpPortRxHandle_1067(uint8_t* data,uint16_t len)
{
    return 0;
}

int udpPortRxHandle_1055(uint8_t* data,uint16_t len)
{


	return 0;
}
