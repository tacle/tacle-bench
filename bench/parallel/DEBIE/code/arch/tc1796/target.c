/*------------------------------------------------------------------------------
 *
 * Copyright (C) 1998 : Space Systems Finland Ltd.
 *
 * Space Systems Finland Ltd (SSF) allows you to use this version of
 * the DEBIE-I DPU software for the specific purpose and under the
 * specific conditions set forth in the Terms Of Use document enclosed
 * with or attached to this software. In particular, the software
 * remains the property of SSF and you must not distribute the software
 * to third parties without written and signed authorization from SSF.
 *
 *    System Name:   DEBIE DPU SW, test harness for WCET analysis
 *    Subsystem  :   DNI (DEBIE Null Interface)
 *    Module     :   target.c
 *
 * Target-specific implementations of the DNI operations, specifically
 * for the iF-DEV-LPC kit and the LPC2138 processor.
 *
 * Based, with extensive changes, on parts of the SSF file rtx_if.c,
 * rev 1.13, Fri May 21 00:14:00 1999.
 *
 *- * --------------------------------------------------------------------------
 */

/* This file contains one set of operations, as follows:
 *
 * > memory operations: dpu_ctrl.h
 *
 */


#include "keyword.h"


/*    Memory operations : dpu_ctrl.h    */


#include "dpu_ctrl.h"


#define DATA_MEM_BASE 0xc0000000L
/* The SRAM starts at this address. */


unsigned char *Data_Pointer (uint16_t address)
{
   return (unsigned char *)(DATA_MEM_BASE + (uint32_t)address);
}


void Set_Data_Byte (data_address_t addr, unsigned char value)
{
   /* *(Data_Pointer (addr)) = value; */
   /* Safer to do nothing. */
}


unsigned char Get_Data_Byte (data_address_t addr)
{
   return *(Data_Pointer (addr));
}


unsigned char Get_Code_Byte (code_address_t addr)
{
   return 0;
}
