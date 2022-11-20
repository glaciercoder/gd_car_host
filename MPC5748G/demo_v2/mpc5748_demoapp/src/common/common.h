/*
 * common.h
 *
 *  Created on: 2019年8月25日
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
 * 避免多控制器之间IP和MAC冲突，此处定义控制器ID
 * CONTROLLER_ID=0时，MCU IP为192.168.1.200，Xavier1和Xavier2分别为101和102
 * CONTROLLER_ID=1时，分别为201、103、104
 * CONTROLLER_ID=2时，分别为202、105、106
 * 后面若有增加，以此类推
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
	DATA_STATE_RECV,		/*可接收最新数据*/
	DATA_STATE_AWAIT,		/*等待发送*/
	DATA_STATE_SENDING		/*发送中*/
};

#endif /* COMMON_H_ */
