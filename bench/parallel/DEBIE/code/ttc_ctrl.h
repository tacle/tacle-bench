/*------------------------------------------------------------------------------

      Copyright (C) 1998 : Space Systems Finland Ltd.

   Space Systems Finland Ltd (SSF) allows you to use this version of
   the DEBIE-I DPU software for the specific purpose and under the
   specific conditions set forth in the Terms Of Use document enclosed
   with or attached to this software. In particular, the software
   remains the property of SSF and you must not distribute the software
   to third parties without written and signed authorization from SSF.

      System Name:   DEBIE DPU SW
      Module     :   ttc_ctrl.h

   Macros and function prototypes for handling the Telecommand
   and Telemetry interface.

   Based on the SSF DHI file ttc_ctrl.h, rev 1.11,  Sun May 16 09:20:10 1999.

  - * --------------------------------------------------------------------------
*/


#ifndef TTC_CTRL_H
#define TTC_CTRL_H

#include "target_ttc_ctrl.h"

/* TM interrupt service handling */
#define TM_ISR_MASK  0x04

/* Error Status register bits concerning TM/TC interface */

#define PARITY_ERROR 2
#define TC_ERROR     1

#define TC_OR_PARITY_ERROR (TC_ERROR + PARITY_ERROR)

#endif
