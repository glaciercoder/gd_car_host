/*
 * user_udp.c
 *
 *  Created on: 2018��8��16��
 *      Author: hjian
 */
#include "user_udp.h"
#include "RingBuffer.h"
#include "ByteQueue.h"
#include "udp_handle.h"

#include "common.h"
#include "project.h"



unsigned char MCUInfo_sendbuf[256];



/*����UDP���ܶ˿ڼ���ص�����*/
/*����������б�����Ӷ˿ڼ�����ص�����Ҫ�ı�UDP_RX_PORT_HANDLE_NUM��Ӧ��ֵ*/
udprx_handler_t udprx_handle_list[UDP_RX_PORT_HANDLE_NUM] = {
		{1088,udpPortRxHandle_1088},
		{4626,udpPortRxHandle_4626},
		{1090,udpPortRxHandle_1090},
		{1081,udpPortRxHandle_1081},//
		{1091,udpPortRxHandle_1091},//
		{1092,udpPortRxHandle_1092}, // 
		{1093,udpPortRxHandle_1093}, // 
		{1066,udpPortRxHandle_1066},  //
		{1067,udpPortRxHandle_1067},  //
		{1055,udpPortRxHandle_1055} //



};


/*����UDP���͵�ַ���ɶ�������ַ*/
NET_ADDRESS_t udpSendDstAddr_ctrlNode;
NET_ADDRESS_t udpSendDstAddr_planNode;
NET_ADDRESS_t udpSendDstAddr_broadcast;

/**************************************************************************
description:	udp���͵�ַ���˿ڳ�ʼ��
@para none
@return : 0
**************************************************************************/
int udpDstAddrInit(void)
{
	memset(&udpSendDstAddr_ctrlNode,0,sizeof(NET_ADDRESS_t));

	/*����UDP����Ŀ���mac��ַ*/
	udpSendDstAddr_ctrlNode.dst_mac[0] = 0x00;
	udpSendDstAddr_ctrlNode.dst_mac[1] = 0x00;
	udpSendDstAddr_ctrlNode.dst_mac[2] = 0x00;
	udpSendDstAddr_ctrlNode.dst_mac[3] = 0x00;
	udpSendDstAddr_ctrlNode.dst_mac[4] = 0x00;
	udpSendDstAddr_ctrlNode.dst_mac[5] = 0x00;

	/*����UDP����Ŀ���ip��ַ*/
	udpSendDstAddr_ctrlNode.dst_ip[0]  = CTRL_IP1;
	udpSendDstAddr_ctrlNode.dst_ip[1]  = CTRL_IP2;
	udpSendDstAddr_ctrlNode.dst_ip[2]  = CTRL_IP3;
	udpSendDstAddr_ctrlNode.dst_ip[3]  = CTRL_IP4;

	/*����UDP����Ŀ��Ķ˿�*/
	udpSendDstAddr_ctrlNode.dst_port  = 1086;

	memset(&udpSendDstAddr_planNode,0,sizeof(NET_ADDRESS_t));

	/*����UDP����Ŀ���mac��ַ*/
	udpSendDstAddr_planNode.dst_mac[0] = 0x00;
	udpSendDstAddr_planNode.dst_mac[1] = 0x00;
	udpSendDstAddr_planNode.dst_mac[2] = 0x00;
	udpSendDstAddr_planNode.dst_mac[3] = 0x00;
	udpSendDstAddr_planNode.dst_mac[4] = 0x00;
	udpSendDstAddr_planNode.dst_mac[5] = 0x00;

	/*����UDP����Ŀ���ip��ַ*/
	udpSendDstAddr_planNode.dst_ip[0]  = PLAN_IP1;
	udpSendDstAddr_planNode.dst_ip[1]  = PLAN_IP2;
	udpSendDstAddr_planNode.dst_ip[2]  = PLAN_IP3;
	udpSendDstAddr_planNode.dst_ip[3]  = PLAN_IP4;

	/*����UDP����Ŀ��Ķ˿�*/
	udpSendDstAddr_planNode.dst_port  = 1086;

	memset(&udpSendDstAddr_broadcast,0,sizeof(NET_ADDRESS_t));

	/*����UDP����Ŀ���mac��ַ*/
	udpSendDstAddr_broadcast.dst_mac[0] = 0xFF;
	udpSendDstAddr_broadcast.dst_mac[1] = 0xFF;
	udpSendDstAddr_broadcast.dst_mac[2] = 0xFF;
	udpSendDstAddr_broadcast.dst_mac[3] = 0xFF;
	udpSendDstAddr_broadcast.dst_mac[4] = 0xFF;
	udpSendDstAddr_broadcast.dst_mac[5] = 0xFF;

	/*����UDP����Ŀ���ip��ַ*/
	udpSendDstAddr_broadcast.dst_ip[0]  = BROADCAST_IP1;
	udpSendDstAddr_broadcast.dst_ip[1]  = BROADCAST_IP2;
	udpSendDstAddr_broadcast.dst_ip[2]  = BROADCAST_IP3;
	udpSendDstAddr_broadcast.dst_ip[3]  = BROADCAST_IP4;

	/*����UDP����Ŀ��Ķ˿�*/
	udpSendDstAddr_broadcast.dst_port  = UDP_PORT_NMEA;
	return 0;
}


/**************************************************************************
description:	ENET��UDP��ģ���ʼ��
@para priority: ���籨�Ľ������ȼ�
@return : 0
**************************************************************************/
int udpEnetDriverInit(uint8_t priority)
{
	ENET_UDP_Driver_Init();
	ENET_Driver_Rx_Init(priority);
	udpDstAddrInit();

	return 0;
}



int udpmuticoretest(char* buf,uint16_t len)
{
	udpSendDstAddr_ctrlNode.dst_port = UDP_PORT_MUTICORE_TEST;
	udpSendOut(&udpSendDstAddr_ctrlNode,(char*)buf,len);

/*	udpSendDstAddr_ctrlNode.dst_port = 1100;
	udpSendOut(&udpSendDstAddr_ctrlNode,(char*)test_rcvbuf,strlen(test_rcvbuf));
*/
	return 0;
}


/**************************************************************************
description: UDP���ͺ���
@para data : ���������׵�ַ
@para data : �������ݳ���
@return  0  : ���ͳɹ�
@return  -1 : ����ʧ��
**************************************************************************/
int udpSendOut(NET_ADDRESS_t* addr,char* data,int len)
{
	int ret = -1;
	if(addr != NULL)
		ret = User_UDP_Send(addr->dst_mac,addr->dst_ip,DEFAULT_UDP_LOCAL_PORT,addr->dst_port,(uint8_t*)data,len);

	return ret;
}


int isMacAvailable(NET_ADDRESS_t* addr)
{
	if(addr->dst_mac[0] | addr->dst_mac[1] | addr->dst_mac[2] | addr->dst_mac[3] | addr->dst_mac[4] | addr->dst_mac[5])
		return 1;
	else
		return 0;
}
