/*------------------------------------------------------------------------------

   Copyright (C) 1998 : Space Systems Finland Ltd.

   Space Systems Finland Ltd (SSF) allows you to use this version of
   the DEBIE-I DPU software for the specific purpose and under the
   specific conditions set forth in the Terms Of Use document enclosed
   with or attached to this software. In particular, the software
   remains the property of SSF and you must not distribute the software
   to third parties without written and signed authorization from SSF.

      System Name:   DEBIE DPU SW, test harness for WCET analysis
      Subsystem  :   DNI (DEBIE Null Interface)
      Module     :   target.c

   Target-specific implementations of the DNI operations, specifically
   for the intel/linux target.

   Options: if the preprocessor symbol TRACE_TARGET is defined,
   these operations generate trace message on stdout.

   Based, with extensive changes, on parts of the SSF file rtx_if.c,
   rev 1.13, Fri May 21 00:14:00 1999.

  - * --------------------------------------------------------------------------
*/

/* This file contains one set of operations, as follows:

   > memory operations: dpu_ctrl.h

*/


#if defined(TRACE_TARGET)
#include <stdio.h>
#endif

#include "keyword.h"


/*    Memory operations : dpu_ctrl.h    */


#include "dpu_ctrl.h"


static char data_memory[ 65536 ];
/* Simulated 80C32 external data RAM. */


unsigned char *Data_Pointer ( uint16_t address )
{
  return &data_memory[ address ];
}


void Set_Data_Byte ( data_address_t addr, unsigned char value )
{
  #if defined(TRACE_TARGET)
  printf ( "Set_Data_Byte 0x%x to %d = 0x%x\n", addr, value, value );
  #endif
  data_memory[ addr ] = value;
}


unsigned char Get_Data_Byte ( data_address_t addr )
{
  unsigned char value = data_memory[ addr ];
  #if defined(TRACE_TARGET)
  printf ( "Get_Data_Byte 0x%x is %d = 0x%x\n", addr, value, value );
  #endif
  return value;
}


unsigned char Get_Code_Byte ( code_address_t addr )
{
  #if defined(TRACE_TARGET)
  /* printf ("Get_Code_Byte 0x%x\n", addr); */
  /* This would be to much output. Skip. */
  #endif
  return 0;
}
