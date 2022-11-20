/*
 * udp_handle.h
 *
 *  Created on: 2019��4��15��
 *      Author: luxin
 */

#ifndef ENET_UDP_HANDLE_H_
#define ENET_UDP_HANDLE_H_

typedef struct{
	    unsigned long  seq;
	    unsigned long  stamp_sec;
	    unsigned long  stamp_nsec;
	    unsigned short points_num;
	    short lon_acc; // 0.01m/s2
	    float x[50]; // cm
	    float y[50]; // cm
	    short speed[50]; // 0.01 km/h
	    unsigned short heading[50]; // 0.01 deg, east=0, north=90, 0~360
}trajectory_t;

typedef struct{
unsigned char HAD_Dig_Ord_LowBeam; //�����  0:������ 1:�أ�2:�������¾�����ͬ���ã�
unsigned char HAD_Dig_Ord_HighBeam;//Զ���
unsigned char HAD_Dig_Ord_RearFogLamp;//�����
unsigned char HAD_Dig_Ord_LeftTurningLamp;//��ת���
unsigned char HAD_Dig_Ord_RightTurningLamp;//��ת���
unsigned char HAD_Dig_Ord_DangerAlarmLamp;//˫��
unsigned char HAD_Dig_Ord_BrakeLamp;//ɲ����
unsigned char HAD_Dig_Ord_BackupLamp;//������
unsigned char HAD_Dig_Ord_DoorLock;//��������
unsigned char HAD_Dig_Ord_Horn;//���ȿ���
}body_control_t;


/*#define test_format "{\
\"speed\":%d,\
\"heading\":%d,\
\"float_x\":%.2f,\
\"float_y\":%.2f,\
\"lonacc\":%d,\
\"status\":%d,\
\"curr_speed\":%.2f,\
\"curr_heading\":%.2f,\
\"in_times\":%d,\
\"in_timems\":%d,\
\"out_times\":%d,\
\"out_timems\":%d,\*/
#define test_format "{\
\"goal_x\":%.2f,\
\"goal_y\":%.2f,\
\"goal_id\":%d,\
\"points_num\":%d\
}}"



void vehicle_status_switch(void);
int udpPortRxHandle_1088(uint8_t* data,uint16_t len);
int udpPortRxHandle_4626(uint8_t* data,uint16_t len);
int udpPortRxHandle_1090(uint8_t* data,uint16_t len);
int udpPortRxHandle_1091(uint8_t* data,uint16_t len);
int udpPortRxHandle_1092(uint8_t* data,uint16_t len);
int udpPortRxHandle_1093(uint8_t* data,uint16_t len);
int udpPortRxHandle_1081(uint8_t* data,uint16_t len);
int udpPortRxHandle_1066(uint8_t* data,uint16_t len);
int udpPortRxHandle_1055(uint8_t* data,uint16_t len);
int udpPortRxHandle_1067(uint8_t* data,uint16_t len);

#endif /* ENET_UDP_HANDLE_H_ */
