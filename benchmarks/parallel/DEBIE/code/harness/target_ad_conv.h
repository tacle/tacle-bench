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
 *    Module     :   target_ad_conv.h
 * 
 * Macros, functions, types and constants for controlling AD converter.
 *
 * This version uses the harness functions for I/O and kernel simulation.
 *
 * Based on the SSF DHI file ad_conv.h, revision 1.6, Tue Jun 01 12:35:44 1999.
 *
 *      
 *- * --------------------------------------------------------------------------
 */


#ifndef TARGET_AD_CONV_H
#define TARGET_AD_CONV_H

#include "keyword.h"

/* AD converter control functions, simulated in harness.c */

extern void          Update_ADC_Channel_Reg (unsigned char channel);
extern void          Start_Conversion       (void);
extern unsigned char End_Of_ADC             (void);
extern unsigned char Get_Result             (void);
extern void          Set_DAC_Output         (unsigned char level);

/* AD converter control macros */

#define UPDATE_ADC_CHANNEL_REG Update_ADC_Channel_Reg (ADC_channel_register)
#define START_CONVERSION       Start_Conversion ()
#define END_OF_ADC             End_Of_ADC ()
#define GET_RESULT             Get_Result ()
#define SET_DAC_OUTPUT(LEVEL)  Set_DAC_Output (LEVEL)

#endif
