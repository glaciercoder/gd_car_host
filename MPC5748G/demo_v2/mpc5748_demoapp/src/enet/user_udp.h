/*
 * user_udp.h
 *
 *  Created on: 2018Äê8ÔÂ16ÈÕ
 *      Author: luxin
 */

#ifndef USER_UDP_USER_UDP_H_
#define USER_UDP_USER_UDP_H_

#include "ENET_UDP_Driver.h"


#define UDP_PORT_VEHICLE_INFO  1087
#define UDP_PORT_VEHICLE_CMD   1088
#define UDP_PORT_DEVICE_SPAN   1089
#define UDP_PORT_DELAY_CHECK   1090
#define UDP_PORT_DEVICE_OBU    4626
#define UDP_PORT_DEVICE_OBU2   4627

#define UDP_PORT_ULTRASONIC   1085
#define UDP_PORT_MIWAVE   1084
#define UDP_PORT_MUTICORE_TEST 1099

#define UDP_PORT_NMEA          30000


enum vehicle_mode_t{
	epb_mode,
	run_mode,
	brake_mode
};


typedef struct {
	uint8_t 	dst_mac[6];
	uint8_t 	dst_ip[4];
	uint16_t 	dst_port;
}NET_ADDRESS_t;

typedef int (*udprx_func)(uint8_t*,uint16_t);

typedef struct{
	uint16_t rx_port;
	udprx_func handle;
}udprx_handler_t;

#define DEFAULT_UDP_LOCAL_PORT				1086
#define PACKET_LEN_BEFORE_ICMP_PAYLOAD  	42
#define UDP_RX_PORT_HANDLE_NUM				9

typedef struct{
	uint8_t 		des_mac[6];
	uint8_t 		src_mac[6];
	uint8_t 		des_ip[4];
	uint8_t 		src_ip[4];
	uint16_t 		des_port;
	uint16_t 		src_port;
	uint16_t 		len;
	uint16_t 		checksum;
	uint8_t* 		data;
}UDP_Message_t;


extern NET_ADDRESS_t udpSendDstAddr_ctrlNode;
extern NET_ADDRESS_t udpSendDstAddr_planNode;

extern udprx_handler_t udprx_handle_list[UDP_RX_PORT_HANDLE_NUM];

int udpEnetDriverInit(uint8_t priority);
int udpSendOut(NET_ADDRESS_t* addr,char* data,int len);

int isMacAvailable(NET_ADDRESS_t* addr);
int udpmuticoretest(char* buf,uint16_t len);

#endif /* USER_UDP_USER_UDP_H_ */
