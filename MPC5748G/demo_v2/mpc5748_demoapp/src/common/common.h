/*
 * common.h
 *
 *  Created on: 2019��8��25��
 *      Author: Hjian
 */

#ifndef COMMON_H_
#define COMMON_H_

/* Version Format: [PROJECT_VERSION].[MAJOR_VERSION].[MINOR_VERSION]_[PATCH_VERSION] */
#define PROJECT_VERSION 1
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define PATCH_VERSION 78

/*
 * ����������֮��IP��MAC��ͻ���˴����������ID
 * CONTROLLER_ID=0ʱ��MCU IPΪ192.168.1.200��Xavier1��Xavier2�ֱ�Ϊ101��102
 * CONTROLLER_ID=1ʱ���ֱ�Ϊ201��103��104
 * CONTROLLER_ID=2ʱ���ֱ�Ϊ202��105��106
 * �����������ӣ��Դ�����
*/
#define CONTROLLER_ID 0

#define DAOYUAN 1
#define GENAWEI 2
#define DAOYUAN_CAN 3

#define INS_CHOICE  DAOYUAN_CAN


struct time_s{
	uint32_t sec;
	uint16_t msec;
};

struct timer_s{
	struct time_s tv_start;
	unsigned int  tv_length;		//ms
	int  valid;
};


enum InsDataProcess{
	DATA_STATE_RECV,		/*�ɽ�����������*/
	DATA_STATE_AWAIT,		/*�ȴ�����*/
	DATA_STATE_SENDING		/*������*/
};

#endif /* COMMON_H_ */
