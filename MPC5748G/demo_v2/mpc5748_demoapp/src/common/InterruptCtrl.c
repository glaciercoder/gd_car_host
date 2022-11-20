/*
 * InterruptCtrl.c
 *
 *  Created on: 2021年7月20日
 *      Author: cxt
 */

#include "InterruptCtrl.h"
#include "compiler_api.h"

_INLINE_ void MaskInterrupts(void)
{
	BaseType_t msr;

	// See MPC5746C RM Rev 3   section 21.8.5.2  "Ensuring coherency" (most restrictive rules)
	// See MPC5646C RM Rev 5   section 19.10.4.2 "Ensuring coherency"
	// See MPC5607B RM Rev 7.2 section 18.7.5.2  "Ensuring coherency"
	// See MPC5604B RM Rev 8.2 section 16.7.5.2  "Ensuring coherency"
	PPCASM volatile
	(
		"mfmsr  %0 \n\t"
		"wrteei  0 \n\t" // disable interrupts
		: "=r" (msr)
	);

	/* Set current interrupt priority to max API priority */
	INTC_CPR = configMAX_API_CALL_INTERRUPT_PRIORITY;

	PPCASM volatile
	(
		"mbar     \n\t" // ensure INTC_CPR write completes before re-enabling interrupts
		"wrtee %0 \n\t" // re-enable interrupts if they were previously enabled
		"se_isync \n\t" // re-fetch Processor pipeline
		: : "r" (msr)
	);
}

_INLINE_ void UnmaskInterrupts(UBaseType_t priority)
{
    BaseType_t msr;

    // See MPC5746C RM Rev 3   section 21.8.5.2  "Ensuring coherency" (most restrictive rules)
    // See MPC5646C RM Rev 5   section 19.10.4.2 "Ensuring coherency"
    // See MPC5607B RM Rev 7.2 section 18.7.5.2  "Ensuring coherency"
    // See MPC5604B RM Rev 8.2 section 16.7.5.2  "Ensuring coherency"
    PPCASM volatile
    (
        "mbar      \n\t" // flush out writes from store buffer
        "mfmsr  %0 \n\t"
        "wrteei  0 \n\t" // disable interrupts
        : "=r" (msr)
    );

    // Restore current interrupt priority
    INTC_CPR = priority;

    PPCASM volatile
    (
        "wrtee %0 \n\t" // re-enable interrupts if they were previously enabled. no mbar needed: OK if INTC_CPR write takes a few cycles to show up.
        : : "r" (msr)
    );
}
