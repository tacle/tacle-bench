/*------------------------------------------------------------------------------

      Copyright (C) 1998 : Space Systems Finland Ltd.

   Space Systems Finland Ltd (SSF) allows you to use this version of
   the DEBIE-I DPU software for the specific purpose and under the
   specific conditions set forth in the Terms Of Use document enclosed
   with or attached to this software. In particular, the software
   remains the property of SSF and you must not distribute the software
   to third parties without written and signed authorization from SSF.

      System Name:   DEBIE DPU SW
      Module     :   ad_conv.h

   Macros, functions, types and constants for controlling AD converter.

   Based on the SSF DHI file ad_conv.h, revision 1.6, Tue Jun 01 12:35:44 1999.


  - * --------------------------------------------------------------------------
*/


#ifndef AD_CONV_H
#define AD_CONV_H

#include "keyword.h"

/* Constant definitions */

#define AD_CHANNELS     0x28
/* Number of possible ADC channels (includes GND channels) */

#define  BP_UP           0x40
#define  BP_DOWN         0xBF

#include "target_ad_conv.h"

extern unsigned char EXTERNAL ADC_channel_register;
/* Holds value of the ADC Channel HW register       */
/* Is used by Hit Trigger ISR task and Health       */
/* Monitoring task.                                 */
/* Updating must be atomic in the Health Monitoring */
/* task, because Hit Trigger can preempt it.        */

#endif
