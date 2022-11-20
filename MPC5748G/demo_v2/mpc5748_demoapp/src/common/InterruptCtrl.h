/*
 * InterruptCtrl.h
 *
 *  Created on: 2021年7月20日
 *      Author: cxt
 */

#ifndef _INTERRUPTCTRL_H_
#define _INTERRUPTCTRL_H_

#ifndef _INLINE_
#define _INLINE_ inline __attribute__(( always_inline ))
#endif

#define INTC_CPR_ADDR                      0xFC040010
#define INTC_CPR  (*(volatile unsigned int *)(INTC_CPR_ADDR))

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configMAX_API_CALL_INTERRUPT_PRIORITY    (15)

typedef long BaseType_t;
typedef unsigned long UBaseType_t;


#define DISABLE_INTERRUPTS()                MaskInterrupts()
#define ENABLE_INTERRUPTS()                 UnmaskInterrupts(0)

void MaskInterrupts(void);
void UnmaskInterrupts(UBaseType_t priority);

#endif /* INTERRUPTCTRL_H_ */
