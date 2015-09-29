/*------------------------------------------------------------------------------
 *
 *    Copyright (C) 1998 : Space Systems Finland Ltd.
 *
 * Space Systems Finland Ltd (SSF) allows you to use this version of
 * the DEBIE-I DPU software for the specific purpose and under the
 * specific conditions set forth in the Terms Of Use document enclosed
 * with or attached to this software. In particular, the software
 * remains the property of SSF and you must not distribute the software
 * to third parties without written and signed authorization from SSF.
 *
 *    System Name:   DEBIE DPU SW
 *    Module     :   isr_ctrl.h
 *
 * Macros and operations to control and handle interrupts.
 *
 * Based on the SSF DHI file isr_ctrl.h, rev 1.16, Sun Jul 25 15:02:08 1999.
 *      
 *      
 *- * --------------------------------------------------------------------------
 */


#ifndef ISR_CTRL_H
#define ISR_CTRL_H

#include "target_isr_ctrl.h"

/* Function prototypes */

extern void AttachInterrupt(unsigned char ISR_VectorNumber);

extern void EnableInterrupt(unsigned char ISR_VectorNumber);

extern void DisableInterrupt(unsigned char ISR_VectorNumber);

extern signed char SetInterruptMask(unsigned char ISR_MaskNumber);

extern signed char ResetInterruptMask(unsigned char ISR_MaskNumber);

extern void WaitInterrupt (unsigned char ISR_VectorNumber, unsigned char timer);

#endif
