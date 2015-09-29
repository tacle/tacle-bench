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
 *    Module     :   target_dpu_ctrl.h
 *
 * Operations and macros for low-level control of the Data Processing
 * Unit, the 80C32 computer on which the DEBIE DPU software runs.
 * This includes accessing data and code memories by address; boot
 * and reset operations; watchdog handling; memory patch and test.
 *
 * This version uses the harness functions for I/O and kernel simulation.
 *
 * Based on the SSF DHI file dpu_ctrl.h, rev 1.23, Fri May 28 14:59:30 1999.
 *      
 *- * --------------------------------------------------------------------------
 */

#ifndef TARGET_DPU_CTRL_H
#define TARGET_DPU_CTRL_H

#include "keyword.h"

extern unsigned char Event_Flag (void);

#define EVENT_FLAG Event_Flag()
/* Event storage condition signal. */


/* Access simulated memory */

extern void          Set_Data_Byte (data_address_t addr, unsigned char value);
extern unsigned char Get_Data_Byte (data_address_t addr);
extern unsigned char Get_Code_Byte (code_address_t addr);


/* Functions and macros to access external memory by numerical address. */

#define SET_DATA_BYTE(ADDR,VALUE) Set_Data_Byte (ADDR, VALUE)
#define GET_DATA_BYTE(ADDR)       Get_Data_Byte (ADDR)
#define GET_CODE_BYTE(ADDR)       Get_Code_Byte (ADDR)


/* Controlling the watchdog and the memory mapping: */

#define SET_WD_RESET_HIGH  {}
#define SET_WD_RESET_LOW   {}
#define SET_MEM_CONF_PROM  {}
#define SET_MEM_CONF_SRAM  {}


/* macro used in healthmonitoring */

extern unsigned char Check_Current (unsigned char bits);

#define CHECK_CURRENT(BIT_NUMBERS) Check_Current (BIT_NUMBERS)
/* Checks whether given bit in the HV Status Register is HIGH or LOW. */


#endif
