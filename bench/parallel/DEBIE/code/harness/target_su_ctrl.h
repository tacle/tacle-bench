/*------------------------------------------------------------------------------

      Copyright (C) 1998 : Space Systems Finland Ltd.

   Space Systems Finland Ltd (SSF) allows you to use this version of
   the DEBIE-I DPU software for the specific purpose and under the
   specific conditions set forth in the Terms Of Use document enclosed
   with or attached to this software. In particular, the software
   remains the property of SSF and you must not distribute the software
   to third parties without written and signed authorization from SSF.

      System Name:   DEBIE DPU SW
      Module     :   target_su_ctrl.h

   Macros, functions, constants and types for controlling Sensor Units.

   This version uses the harness functions for I/O and kernel simulation.

   Based on the SSF DHI file su_ctrl.h, rev 1.28, Tue Sep 14 14:51:26 1999.

  - * --------------------------------------------------------------------------
*/


#ifndef TARGET_SU_CTRL_H
#define TARGET_SU_CTRL_H

/* Simulation of counter access */

extern unsigned char Get_MSB_Counter   ( void );
extern unsigned char Get_LSB1_Counter  ( void );
extern unsigned char Get_LSB2_Counter  ( void );
extern unsigned char Rise_Time_Counter ( void );

/* Counter access macros */

#define GET_MSB_COUNTER   Get_MSB_Counter ()
#define GET_LSB1_COUNTER  Get_LSB1_Counter ()
#define GET_LSB2_COUNTER  Get_LSB2_Counter ()
#define RISE_TIME_COUNTER Rise_Time_Counter ()

#define SET_COUNTER_RESET(LEVEL) {}

/* Simulation of trigger signal */

extern unsigned char Trigger_Source_0 ( void );
extern unsigned char Trigger_Source_1 ( void );
extern unsigned char V_Down           ( void );

/* Macros to access trigger signals */

#define TRIGGER_SOURCE_0 Trigger_Source_0 ()
#define TRIGGER_SOURCE_1 Trigger_Source_1 ()
#define V_DOWN           V_Down ()

/* Simulation of SU Self Test functions */

extern void Set_SU_Self_Test_Ch ( unsigned char value );
extern void Set_Test_Pulse_Level ( unsigned char level );

#define SET_SU_SELF_TEST_CH(value)  Set_SU_Self_Test_Ch (value)
#define SET_TEST_PULSE_LEVEL(level) Set_Test_Pulse_Level(level)

#endif


