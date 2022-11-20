/*
 * pit.h
 *
 *  Created on: 2018Äê8ÔÂ10ÈÕ
 *      Author: luxin
 */

#ifndef PIT_TIMER_PIT_H_
#define PIT_TIMER_PIT_H_


typedef enum{
	PIT_0,
	PIT_1,
	PIT_2
}PIT_TIMER_t;

void pitTimerInit(PIT_TIMER_t instance,uint32_t LDVAL,uint8_t priority);

#endif /* PIT_TIMER_PIT_H_ */
