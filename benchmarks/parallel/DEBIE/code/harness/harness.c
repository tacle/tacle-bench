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
 *    Module     :   harness.c
 *
 * Implementations of the DNI operations, suitable for compiling
 * and linking the DEBIE I DPU Software as a standard C program,
 * on any processor that supports the required data types (eg. 8-bit
 * char, 32-bit long) and has enough program and data memory.
 *
 * This implementation is designed to support measurement-based
 * timing analysis by feeding the DEBIE SW with selected sets of
 * test data.
 *
 * Options: if the preprocessor symbol TRACE_HARNESS is defined,
 * the harness operations generate trace message on stdout.
 *
 * Based, with extensive changes, on the SSF file rtx_if.c, rev 1.13,
 * Fri May 21 00:14:00 1999.
 *
 *- * --------------------------------------------------------------------------
 */

/* This file contains several sets of operations, as follows:
 *
 * > processor registers as variables: reg52.h
 *
 * > kernel operations: taskctrl.h, isr_ctrl.h, msg_ctrl.h
 *
 * > some DPU operations: dpu_ctrl.h
 *
 * > telecommand and telemetry interface operations: ttc_ctrl.h, isr_ctrl.h
 *
 * > operations for the A/D converted: ad_conv.h
 *
 * > operations on the sensor units: su_ctrl.h, dpu_ctrl.h
 *
 * > accessing the VALUE_OF "unsigned short" telemetry items: keyword.h
 *
 * > "calling" a "patch function": keyword.h
 *
 * > checking the sizes of various types (for analysis only).
 *
 * For testing purposes the main operation is StartSystem(). The DEBIE
 * main function calls StartSystem(), which normally activates the
 * kernel and the DEBIE application tasks and interrupt handlers. In this
 * version, StartSystem() takes over the show and runs the test scenario.
 *
 * Target-specific harness operations are implemented in target.c. There
 * is a specific version of target.c for each target processor/compiler.
 *
 */


#if defined(TRACE_HARNESS)
#include <stdio.h>
#endif

#include "keyword.h"
#include "health.h"
#include "measure.h"
#include "tc_hand.h"
#include "telem.h"


/*   Processor registers : reg52.h    */


unsigned char EA;


/*   Kernel operations : taskctrl.h    */

#include "taskctrl.h"

void CreateTask (task_info_t EXTERNAL *new_task)
/* Purpose        : Task is created in the RTX.                              */
/* Interface      : input:   - new_task                                      */
/*                  output:  - telemetry_data.os_create_task_error           */
/* Preconditions  : none                                                     */
/* Algorithm      : -In case of an error, 'new_task' is stored to telemetry  */
/*                   as an error indication.                                 */
{
#if defined(TRACE_HARNESS)
   printf ("CreateTask %d\n", new_task->rtx_task_number);
#endif

   switch (new_task->rtx_task_number) {

   case TC_TM_INTERFACE_TASK:

      InitTelecommandTask ();

      break;

   case ACQUISITION_TASK:

      InitAcquisitionTask ();

      break;

   case HIT_TRIGGER_ISR_TASK:

      InitHitTriggerTask ();

      break;

   default:

#if defined(TRACE_HARNESS)
      printf ("CreateTask: unknown task number\n");
#endif

      break;
   }
}


void WaitInterval(unsigned char time)
/* Purpose        : Interval is waited with RTX.                             */
/* Interface      : input:   - time                                          */
/*                  output:  - telemetry_data.os_wait_error                  */
/* Preconditions  : none                                                     */
/* Postconditions : Interval for wait is set.                                */
/* Algorithm      : -In case of an error, 'K_IVL' is stored to telemetry     */
/*                   as an error indication and error bit is set in          */
/*                   software_error register.                                */
{
#if defined(TRACE_HARNESS)
   printf ("WaitInterval %d\n", time);
#endif
}


void WaitTimeout(unsigned char time) COMPACT REENTRANT_FUNC
/* Purpose        : Timeout is waited with RTX.                              */
/* Interface      : input:   - time                                          */
/*                  output:  - telemetry_data.os_wait_error                  */
/* Preconditions  : none                                                     */
/* Postconditions : Specified time has elapsed.                              */
/* Algorithm      : -In case of an error, 'K_TMO' is stored to telemetry     */
/*                   as an error indication and error bit is set in          */
/*                   software_error register.                                */
{
#if defined(TRACE_HARNESS)
   printf ("WaitTimeout %d\n", time);
#endif
}


void SetTimeSlice(unsigned int time_slice)
/* Purpose        : Time slice in the RTX is set.                            */
/* Interface      : input:   - time_slice                                    */
/*                  output:  - telemetry_data.os_set_slice_error             */
/* Preconditions  : none                                                     */
/* Postconditions : Timeslice which defines the time interval in number of   */
/*                  processor cycles is set.                                 */
/* Algorithm      :  In case of an error, indication bit is set in           */
/*                   the software_error register.                            */
{
#if defined(TRACE_HARNESS)
   printf ("SetTimeSlice %d\n", time_slice);
#endif
}


static unsigned int ad_converting = 0;
/* Whether the ADC is now busy with a conversion.
 * 0 = no, 1 = yes.
 * Declared here, because it is cleared by ShortDelay.
 */


void ShortDelay (uint_least8_t delay_loops)
{
#if defined(TRACE_HARNESS)
   printf ("ShortDelay %d\n", delay_loops);
#endif

   ad_converting = 0;
   /* Any on-going A/D conversion is assumed to end during the delay.
    * ShortDelay is sometimes used, instead of End_Of_ADC, when the
    * A/D converter is switched between unipolar and bipolar modes.
    */
}


unsigned char isr_send_message (
   unsigned char mailbox,
   uint16_t      message)
/* Purpose        : Send mail from ISR to a requested mailbox.              */
/* Interface      : input:   - mailbox, message                             */
/*                  output:  - telemetry_data.os_send_message_error         */
/* Preconditions  : Mailbox number should be a value 0 - 7                  */
/* Postconditions : Mail is send to a given mailbox.                        */
/* Algorithm      : - In case of an error, failed 'mailbox' is stored to    */
/*                    telemetry.                                            */
{
#if defined(TRACE_HARNESS)
   printf ("isr_send_message to %d, message %d = 0x%x\n",
      mailbox, message, message);
#endif
   SendTaskMail (mailbox, message, 0);
   return OK;
}


/*   Interrupt services : isr_ctrl.h   */


#include "isr_ctrl.h"


void AttachInterrupt(unsigned char ISR_VectorNumber)
/* Purpose        : Interrupt with a given number is assigned to a task in   */
/* Interface      : input:   - ISR_VectorNumber                              */
/*                  output:  - telemetry_data.os_attach_interrupt_error      */
/* Preconditions  : none                                                     */
/* Postconditions : Interrupt is attached to a calling task.                 */
/* Algorithm      : -In case of an error, 'ISR_VectorNumber' is stored to    */
/*                   telemetry as an error indication.                       */
{
#if defined(TRACE_HARNESS)
   printf ("AttachInterrupt %d\n", ISR_VectorNumber);
#endif
}


void EnableInterrupt(unsigned char ISR_VectorNumber)
/* Purpose        : Interrupt with a given number is enabled in the RTX.     */
/* Interface      : input:   - ISR_VectorNumber                              */
/*                  output:  - telemetry_data.os_enable_isr_error            */
/* Preconditions  : none                                                     */
/* Postconditions : Interrupt is enabled.                                    */
/* Algorithm      : -In case of an error, 'ISR_VectorNumber' is stored to    */
/*                   telemetry as an error indication.                       */
{
#if defined(TRACE_HARNESS)
   printf ("EnableInterrupt %d\n", ISR_VectorNumber);
#endif
}


void DisableInterrupt(unsigned char ISR_VectorNumber)
/* Purpose        : Interrupt with a given number is disabled in the RTX.    */
/* Interface      : input:   - ISR_VectorNumber                              */
/*                  output:  - telemetry_data.os_disable_isr_error           */
/* Preconditions  : none                                                     */
/* Postconditions : Interrupt is enabled.                                    */
/* Algorithm      : -In case of an error, 'ISR_VectorNumber' is stored to    */
/*                   telemetry as an error indication.                       */
{
#if defined(TRACE_HARNESS)
   printf ("DisableInterrupt %d\n", ISR_VectorNumber);
#endif
}


signed char SetInterruptMask(unsigned char ISR_MaskNumber)
/* Purpose        : Interrupt mask bit is set is in the RTX.                 */
/* Interface      : Return value, which describes the execution result, is   */
/*                  always zero as this function does no parameter checking. */
/*                  Used to manipulate special bits, which are part of the   */
/*                  interrupt enable registers or to modify interrupt enable */
/*                  bits from inside a C51 interrupt function.               */
/*                  Not to be used for interrupt sources attached to RTX51   */
/*                  tasks.                                                   */
/* Preconditions  :                                                          */
/* Postconditions : Interrupt mask is set.                                   */
/* Algorithm      : RTX syntax is used.                                      */
{
#if defined(TRACE_HARNESS)
   printf ("SetInterruptMask 0x%x\n", ISR_MaskNumber);
#endif
   return 0;  /* Success. */
}


signed char ResetInterruptMask(unsigned char ISR_MaskNumber)
/* Purpose        : Interrupt mask bit is reset is in the RTX.               */
/* Interface      : Return value, which describes the execution result, is   */
/*                  always zero as this function does no parameter checking. */
/*                  Used to manipulate special bits, which are part of the   */
/*                  interrupt enable registers or to modify interrupt enable */
/*                  bits from inside a C51 interrupt function.               */
/*                  Not to be used for interrupt sources attached to RTX51   */
/*                  tasks.                                                   */
/* Preconditions  :                                                          */
/* Postconditions : Interrupt mask is reset.                                 */
/* Algorithm      : RTX syntax is used.                                      */
{
#if defined(TRACE_HARNESS)
   printf ("ResetInterruptMask 0x%x\n", ISR_MaskNumber);
#endif
   return 0;  /* Success. */
}


void WaitInterrupt (unsigned char ISR_VectorNumber, unsigned char timer)
/* Purpose        : Interrupt is waited in the RTX.                          */
/* Interface      : input:   - ISR_VectorNumber,timer                        */
/*                  output:  - telemetry_data.os_wait_error                  */
/* Preconditions  : none                                                     */
/* Postconditions : Interrupt is waited.                                     */
/* Postconditions : Interrupt is enabled.                                    */
/* Algorithm      : -In case of an error, 'K_INT' is stored to telemetry     */
/*                   as an error indication and error bit is set in          */
/*                   software_error register.                                */
{
#if defined(TRACE_HARNESS)
   printf ("WaitInterrupt %d, time %d\n", ISR_VectorNumber, timer);
#endif
}

/* See below, Telecommand and telemetry interface, for the ISR */
/* operations related to the TC timer.                         */

/* See below for the ISR operations related to the Hit interrupt. */



/*   Message sending : msg_ctrl.h */


#include "msg_ctrl.h"


static uint16_t mail_message[8];
/* The last message in the mailbox. */

static int mail_count[8] = {0, 0, 0, 0, 0, 0, 0, 0};
/* The number of messages in the mailbox. */
/* Should be between 0 and 1.             */

static int mail_overflows = 0;
/* The number of times a mailbox has overflowed. */


void SendTaskMail (
   unsigned char mailbox,
   uint16_t      message,
   unsigned char timeout)
/* Purpose        : Send mail to a requested mailbox.                       */
/* Interface      : input:   - mailbox, message, timeout                    */
/*                  output:  - telemetry_data.os_send_message_error         */
/* Preconditions  : Mailbox number should be a value 0 - 7                  */
/*                  timeout should be a value 0 - 255                       */
/* Postconditions : Mail is send to a given mailbox.                        */
/* Algorithm      : - In case of an error, failed 'mailbox' is stored to    */
/*                    telemetry.                                            */
{
#if defined(TRACE_HARNESS)
   printf ("SendTaskMail to %d, message %d, timeout %d\n",
      mailbox, message, timeout);
#endif

   if (mail_count[mailbox] == 0)
   {
      mail_message[mailbox] = message;
   }
   else
   {
#if defined(TRACE_HARNESS)
      printf ("Overflow on mailbox %d, already %d message(s)\n",
         mailbox, mail_count[mailbox]);
#endif
      mail_overflows ++;
   }

   mail_count[mailbox] ++;
}


void WaitMail (incoming_mail_t EXTERNAL *message) COMPACT_DATA REENTRANT_FUNC
/* Purpose        : Mail is waited from the given mailbox.                   */
/* Interface      : Return value, which describes the execution result, is   */
/*                  stored in to a struct.                                   */
/* Preconditions  : Mailbox number should be a value 0 - 7                   */
/*                  Time-out should have a value 0 - 255.                    */
/* Postconditions : Message is received or timeout has occurred or error has */
/*                  occurred due to unvalid parameter values.                */
/* Algorithm      : -In case of an error, 'event_selector' is stored to      */
/*                   telemetry as an error indication and error bit is set   */
/*                   in software_error register.                             */
{
#if defined(TRACE_HARNESS)
   printf ("WaitMail from %d, timeout %d\n",
      message -> mailbox_number, message -> timeout);
#endif

   if (mail_count[message -> mailbox_number] > 0)
   {
      message -> wait_result      = MSG_RECEIVED;
      message -> execution_result = MSG_RECEIVED;
      *(message -> message)       = mail_message[message -> mailbox_number];

#if defined(TRACE_HARNESS)
      printf ("Message from %d is %d = 0x%x\n",
         message -> mailbox_number,
         *(message -> message),
         *(message -> message));
#endif

      mail_count[message -> mailbox_number] --;
   }
   else
   {
      message -> wait_result      = TIMEOUT_OCCURRED;
      message -> execution_result = TIMEOUT_OCCURRED;
      *(message -> message)       = 0;
   }
}


static void FlushMail (unsigned char mailbox)
/* Remove all mail from the mailbox. Harness use only. */
{
#if defined(TRACE_HARNESS)
   printf ("FlushMail from box %d, which had %d messages.\n",
      mailbox, mail_count[mailbox]);
#endif

   mail_count[mailbox] = 0;
}


/*   DPU operations : dpu_ctrl.h    */


#include "dpu_ctrl.h"


static unsigned int check_current_errors = 0;
/* The number of consecutive error indications to be returned
 * by the next calls of Check_Current.
 */


unsigned char Check_Current (unsigned char bits)
{
   unsigned char val;
#if defined(TRACE_HARNESS)
   printf ("Check_Current 0x%x\n", bits);
#endif
   switch (bits) {
   case   3: val =  1; break;
   case  12: val =  4; break;
   case  48: val = 16; break;
   case 192: val = 64; break;
   default : val =  0;
#if defined(TRACE_HARNESS)
             printf ("Check_Current param error\n");
             break;
#endif
   }

   if (check_current_errors > 0)
   {
      val = ~val;  /* Wrong value => alarm. */
      check_current_errors --;
   }

   return val;
}


/* The memory operations (Get_Data_Byte etc) are implemented in
 * target.c for each target processor/compiler.
 */


/* The following routines are implemented in das/hw_if.c:
 * Init_DPU
 * GetResetClass
 * SignalMemoryErrors
 * SetMemoryConfiguration
 * GetMemoryConfiguration
 * PatchCode
 */


void Reboot(reset_class_t boot_type)
{
#if defined(TRACE_HARNESS)
   printf ("Reboot %d\n", boot_type);
#endif

   if (boot_type == checksum_reset_e)
   {
      /* Make it not happen (at once) again: */
      reference_checksum = code_checksum;
   }

   TARGET_REBOOT;
}


/* Functions originally implemented in assembly-language (asmfuncs.a51): */


unsigned char TestMemBits (data_address_t address)
/*
 * Test each bit of a cell in RAM memory.
 * For each of the eight bits, the value with this bit ON and
 * all others bits OFF is written to the cell, and the written
 * value is read back and verified.
 * The original content of the cell is destroyed.
 * Parameters:
 *    address  Cell address.
 * TestMemBits returns zero for success, and otherwise the
 * first bit pattern for which the cell failed.
 *
 */
{
#if defined(TRACE_HARNESS)
   printf ("TestMemBits 0x%x\n", address);
#endif
   return 0;
}


unsigned char TestMemData (
   data_address_t start,
   uint_least8_t  bytes)
/*
 * Test an area of RAM memory.
 * For each RAM cell in the given area, the values AAh
 * and 55h are written to the cell, and the written value
 * is read back and verified.
 * The original content of each cell is destroyed.
 * Parameters:
 *    start   Starting address
 *    bytes   Length of area
 *            1 .. 255. Note, zero is NOT allowed!
 * TestMem returns the number of bytes remaining to test
 * when the first memory failure occurred.
 * If this value is nonzero, the first failing address is
 * found by subtracting the return value from 'start+bytes'.
 *
 */
{
#if defined(TRACE_HARNESS)
   printf ("TestMemData start 0x%x, bytes %d\n", start, bytes);
#endif
   return 0;
}


unsigned char TestMemSeq (
   data_address_t start,
   uint_least8_t  bytes)
/*
 * Test an area of RAM memory.
 * For each RAM cell in the given area, the logical complement of
 * the low byte of the cell address is written into the cell, in
 * one pass from the start to the end. Then, in a second pass,
 * each cell is read and the value is is verified.
 * The original content of each cell is destroyed.
 * Parameters:
 *    start   Starting address
 *    bytes   Length of area
 *            1 .. 255. Note, zero is NOT allowed!
 * TestMemSeq returns the number of bytes remaining to test
 * when the first memory failure occurred.
 * If this value is nonzero, the first failing address is
 * found by subtracting the return value from 'start+bytes'.
 *
 */
{
#if defined(TRACE_HARNESS)
   printf ("TestMemSeq start 0x%x, bytes %d\n", start, bytes);
#endif
   return 0;
}


/*    Telecommand and telemetry interface : ttc_ctrl.h    */


#include "ttc_ctrl.h"


static unsigned char tc_msb, tc_lsb;
/* Simulated TC interface registers. */

static uint16_t tc_word;
/* The simulated TC word, composed of tc_msb and tc_lsb. */


unsigned char Read_TC_MSB (void)
{
#if defined(TRACE_HARNESS)
   printf ("Read_TC_MSB\n");
#endif
   return tc_msb;
}


unsigned char Read_TC_LSB (void)
{
#if defined(TRACE_HARNESS)
   printf ("Read_TC_LSB\n");
#endif
   return tc_lsb;
}


static unsigned char tm_msb, tm_lsb;
/* Simulated TM interface registers. */


void Write_TM_LSB (unsigned char value)
{
#if defined(TRACE_HARNESS)
   printf ("Write_TM_LSB %d = 0x%x\n", value, value);
#endif
   tm_lsb = value;
}


void Write_TM_MSB (unsigned char value)
{
#if defined(TRACE_HARNESS)
   printf ("Write_TM_MSB %d = 0x%x\n", value, value);
#endif
   tm_msb = value;
}


/*    TC timer operations : isr_ctrl.h    */


static unsigned char tc_timer_overflow = 1;
/* Simulated overflow flag on the TC timer. */
/* 1 = overflow = good, sufficient interval between TCs. */
/* 0 = no overflow = bad. */


unsigned char TC_Timer_Overflow_Flag (void)
{
#if defined(TRACE_HARNESS)
   printf ("TC_Timer_Overflow_Flag\n");
#endif
   return tc_timer_overflow;
}


void Clear_TC_Timer_Overflow_Flag (void)
{
#if defined(TRACE_HARNESS)
   printf ("Clear_TC_Timer_Overflow_Flag\n");
#endif
   tc_timer_overflow = 0;
}


void Set_TC_Timer_Overflow_Flag (void)
{
#if defined(TRACE_HARNESS)
   printf ("Set_TC_Timer_Overflow_Flag\n");
#endif
   tc_timer_overflow = 1;
}


/*    A/D Converter operations : ad_ctrl.h    */


#include "ad_conv.h"

/*  A/D channels

Channel     Meaning
-------     -------
dec hex
  0  00     SU 1, plasma 1 +
  1  01     SU 1, plasma 1 -
  2  02     SU 1, piezo 1
  3  03     SU 1, piezo 2
  4  04     SU 1, plasma 2 +
  5  05     SU 1, temperature 0
  6  06     SU 1, temperature 1
  7  07
  8  08     SU 2, plasma 1 +
  9  09     SU 2, plasma 1 -
 10  0A     SU 2, piezo 1
 11  0B     SU 2, piezo 2
 12  0C     SU 2, plasma 2 +
 13  0D     SU 2, temperature 0
 14  0E     SU 2, temperature 1
 15  0F
 16  10     SU 1 & SU 2, +5V  supply
 17  11     SU 3 & SU 4, +5V  supply
 18  12     SU 1,2,3,4 , +50V supply
 19  13     DPU          +5V  supply
 20  14     SU 1 & SU 2, -5V  supply
 21  15     SU 3 & SU 4, -5V  supply
 22  16     SU 1,2,3,4,  -50V supply
 23  17
 24  18     SU 3, plasma 1 +
 25  19     SU 3, plasma 1 -
 26  1A     SU 3, piezo 1
 27  1B     SU 3, piezo 2
 28  1C     SU 3, plasma 2 +
 29  1D     SU 3, temperature 0
 30  1E     SU 3, temperature 1
 31  1F
 32  20     SU 4, plasma 1 +
 33  21     SU 4, plasma 1 -
 34  22     SU 4, piezo 1
 35  23     SU 4, piezo 2
 36  24     SU 4, plasma 2 +
 37  25     SU 4, temperature 0
 38  26     SU 4, temperature 1
 39  27
 40  28

A/D channel-selector register:

Bit         Meaning
---         -------
  7         1 = do not use interleave calibration
  6         1 = bipolar, 0 = unipolar
  5         Channel index, 32
  4         Channel index, 16
  3         Channel index,  8
  2         Channel index,  4
  1         Channel index,  2
  0         Channel index,  1

*/


typedef struct {uint16_t min; uint16_t max; } ad_limit_t;
/* Limits on the value of a simulated A/D reading. */

static ad_limit_t ad_limit[AD_CHANNELS] = {
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff},
   {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff}, {0, 0xffff} };
/* Limits on the simulated A/D readings for all channels. */


void Set_AD_Unlimited (void)
/* Sets no A/D limits. */
{
   uint_least8_t c;

#if defined(TRACE_HARNESS)
   printf ("Set AD Unlimited\n");
#endif

   _Pragma("loopbound min 40 max 40")
   for (c = 0; c < AD_CHANNELS; c++)
   {
      ad_limit[c].min = 0;
      ad_limit[c].max = 0xffff;
   }
}


void Set_AD_Nominal (void)
/* Sets A/D limits to ensure nominal (in-range) readings. */
{
#if defined(TRACE_HARNESS)
   printf ("Set AD Nominal\n");
#endif

  /* SU +5V supply: */

  ad_limit[16].min = 0xba00;
  ad_limit[16].max = 0xe4ff;

  ad_limit[17].min = 0xba00;
  ad_limit[17].max = 0xe4ff;

  /* SU -5V supply: */

  ad_limit[20].min = 0x0d00;
  ad_limit[20].max = 0x22ff;

  ad_limit[21].min = 0x0d00;
  ad_limit[21].max = 0x22ff;

  /* SU +50V supply: */

  ad_limit[18].min = 0xa800;
  ad_limit[18].max = 0xe3ff;

  /* SU -50V supply: */

  ad_limit[22].min = 0x0e00;
  ad_limit[22].max = 0x2cff;

  /* DPU +5V supply: */

  ad_limit[19].min = 0xba00;
  ad_limit[19].max = 0xe4ff;

  /* SU 1 temperatures: */

  ad_limit[5].min = 0x0000;
  ad_limit[5].max = 0xfaff;
  ad_limit[6].min = 0x0000;
  ad_limit[6].max = 0xf4ff;

  /* SU 2 temperatures: */

  ad_limit[13].min = 0x0000;
  ad_limit[13].max = 0xfaff;
  ad_limit[14].min = 0x0000;
  ad_limit[14].max = 0xf4ff;

  /* SU 3 temperatures: */

  ad_limit[29].min = 0x0000;
  ad_limit[29].max = 0xfaff;
  ad_limit[30].min = 0x0000;
  ad_limit[30].max = 0xf4ff;

  /* SU 4 temperatures: */

  ad_limit[37].min = 0x0000;
  ad_limit[37].max = 0xfaff;
  ad_limit[38].min = 0x0000;
  ad_limit[38].max = 0xf4ff;
}


static int ad_conv_timer;
/* Simulate some delay in the conversion. This is a signed
 * number so that the delay can be made large by initializing
 * the timer to a large negative value.
 */

#define AD_NUM_CONV 6
/* The number of consecutive A/D conversions separately simulated. */

static int ad_conv_delay[AD_NUM_CONV];
/* The conversion finishes when ad_conv_timer >= ad_conv_delay,
 * with provision for up to AD_NUM_CONV different delays for
 * many consecutive conversions. A value of 0 or 1 means that
 * the conversion is immediately ready, assuming that ad_conv_time
 * starts counting from zero (and not from a negative value).
 */

static unsigned int ad_conv_num = 0;
/* Counts the consecutive conversions for ad_conv_delay[]. */


#define AD_NUM_RAND 319
/* The amount of random A/D data defined below. */

static unsigned char ad_random[AD_NUM_RAND] = {
   0x6a, 0xde, 0xba, 0x90, 0xf2, 0x18, 0x48, 0xf3,
   0x9e, 0x2b, 0x31, 0xdb, 0xe0, 0x7e, 0xc6, 0x18,
   0x43, 0xd0, 0xd7, 0x6e, 0xbc, 0xee, 0x93, 0x9a,
   0x06, 0xb2, 0x3d, 0x1f, 0xc1, 0x51, 0x66, 0x69,
   0xbf, 0x1c, 0x9c, 0xfc, 0x9b, 0xf7, 0xf2, 0xd0,
   0xf4, 0x26, 0x60, 0x69, 0xc4, 0xd9, 0xdb, 0xd4,
   0xe7, 0x2b, 0x8a, 0xea, 0x9f, 0xab, 0x40, 0x3e,
   0xc3, 0xd8, 0x21, 0x61, 0x3b, 0x0f, 0xc1, 0x49,
   0xd3, 0x09, 0x9a, 0x4d, 0x33, 0x52, 0x7b, 0x8e,
   0x7e, 0x7b, 0x6a, 0x88, 0x4f, 0x84, 0xa2, 0xb4,
   0x83, 0xd9, 0xba, 0x79, 0x7d, 0x8f, 0xdf, 0xb2,
   0x8c, 0x86, 0x77, 0x4f, 0x29, 0x86, 0xd4, 0x8b,
   0x11, 0x65, 0x55, 0x74, 0xf4, 0x76, 0x83, 0x88,
   0xd6, 0xa6, 0xa7, 0x33, 0x22, 0xa3, 0x2e, 0x88,
   0x06, 0x54, 0x90, 0x37, 0xd5, 0xdb, 0xce, 0x7c,
   0x0b, 0xd1, 0xe1, 0xc0, 0x7d, 0xa5, 0x0b, 0xc9,
   0xaf, 0xe3, 0x75, 0xc5, 0xf5, 0xaf, 0xaa, 0xe2,
   0x2a, 0xff, 0x6e, 0x84, 0x0e, 0x04, 0x10, 0xf0,
   0x78, 0xdc, 0x96, 0x3d, 0x22, 0x96, 0x64, 0x5b,
   0x7b, 0x9e, 0x83, 0x45, 0xba, 0xb8, 0xe1, 0x31,
   0xc7, 0x0a, 0xe0, 0x31, 0xce, 0x29, 0x3d, 0x01,
   0xb8, 0xfc, 0x79, 0x83, 0x3d, 0xd1, 0x40, 0xe1,
   0x46, 0xfa, 0xe7, 0xc5, 0xdc, 0xc4, 0x1c, 0x24,
   0x29, 0x5a, 0xef, 0xeb, 0x92, 0x57, 0xba, 0x06,
   0x13, 0x1d, 0x35, 0xef, 0xb0, 0x2d, 0x69, 0x20,
   0x92, 0xb1, 0x82, 0x00, 0x8e, 0x3b, 0x12, 0xb3,
   0x78, 0xd7, 0x18, 0xb3, 0x54, 0x0f, 0xd1, 0x8e,
   0x88, 0x5d, 0x4e, 0x2b, 0x30, 0x30, 0x2d, 0x85,
   0xaa, 0x21, 0x01, 0xe1, 0x2c, 0x35, 0xa1, 0xee,
   0xa2, 0x17, 0xed, 0x60, 0x1b, 0x98, 0xea, 0x12,
   0x85, 0x21, 0xde, 0x45, 0x26, 0xef, 0x12, 0x3c,
   0x02, 0x8c, 0xd7, 0x49, 0xbd, 0x02, 0xa7, 0x7d,
   0xe7, 0x1c, 0x15, 0xf9, 0xaa, 0x44, 0x15, 0xb1,
   0xaa, 0x76, 0x5e, 0xf2, 0xb4, 0xfb, 0x85, 0x77,
   0xb9, 0x32, 0xb4, 0xc9, 0x70, 0xe1, 0xdb, 0x44,
   0x9f, 0x5b, 0x87, 0xca, 0xaa, 0xcb, 0x43, 0x53,
   0x7e, 0x49, 0xec, 0x1a, 0x13, 0x1d, 0xe1, 0x1b,
   0x13, 0xc3, 0x34, 0x95, 0x5d, 0x5a, 0xc3, 0xd0,
   0x33, 0x05, 0x82, 0x4a, 0x2e, 0x6d, 0x39, 0xeb,
   0x9c, 0x65, 0x81, 0x7f, 0xa1, 0x62, 0x11};
/* Random A/D data. */


void Next (unsigned int *index)
/* Go on to the next random value. */
{
   (*index) ++;
   if ((*index) >= AD_NUM_RAND) *index = 0;
}


unsigned char Next_Rand (unsigned int *index)
/* Go on to the next random value and return it. */
{
   Next (index);
   return ad_random[*index];
}


void Set_AD_Delay (int delay)
/* Sets ad_conv_delay[all] = delay. */
{
   int i;
   _Pragma("loopbound min 6 max 6")
   for (i = 0; i < AD_NUM_CONV; i++)
      ad_conv_delay[i] = delay;
}


static unsigned int ad_delay_rand = 0;
/* A roving index to randomize the A/D delays. */


void Random_AD_Delay (void)
/* Sets random ad_conv_delay[]. */
{
   int i;
   _Pragma("loopbound min 6 max 6")
   for (i = 0; i < AD_NUM_CONV; i++)
      ad_conv_delay[i] = (Next_Rand (&ad_delay_rand) % (ADC_MAX_TRIES + 10));
}


static unsigned int ad_rand_index = 0;
/* A roving index to the random A/D data. */

static unsigned int max_adc_hits = 0;
/* Maximum number of particle "hits" that may occur during
 * A/D conversions (in the Monitoring task, we assume).
 */

static unsigned int ad_hit_rand_index = 0;
/* A roving index to the random A/D data, for randomizing
 * the occurrence of "hits" during A/D conversions.
 */

static unsigned int total_adc_hits = 0;
/* The number of simulated particle "hits" during A/D conversion. */

static unsigned int ad_random_failures = 0;
/* The number of A/D conversion failures (conversion never done)
 * to be simulated, at random times, in the next conversions
 * (calls of Start_Conversion).
 */

static unsigned int ad_fail_index = 0;
/* A roving index to the random A/D data, for randomizing
 * the occurrence of A/D failures (conversion never done).
 */

/* See also ad_converting, declarer earlier. */


void Update_ADC_Channel_Reg (unsigned char channel)
{
#if defined(TRACE_HARNESS)
   printf ("Update_ADC_Channel_Reg %x\n", channel);
#endif
}


static unsigned int start_conversion_count = 0;
/* Counts the number of calls of Start_Conversion. */

static uint16_t ad_value = 0;
/* Simulated ADC value, set at Start_Conversion and reported
 * byte by byte.
 */


void Start_Conversion (void)
{
   uint_least8_t channel;

   channel = ADC_channel_register & 0x3f;

#if defined(TRACE_HARNESS)
   printf ("Start_Conversion on channel %d\n", channel);
#endif

   if (ad_converting != 0)
	{
#if defined(TRACE_HARNESS)
      printf ("- previous conversion did not end.\n");
#endif
   }

   /* Pick an A/D reading for this channel: */

   ad_value = ((uint16_t)Next_Rand (&ad_rand_index)) << 8;
   ad_value |= (uint16_t)Next_Rand (&ad_rand_index);

   if (ad_value < ad_limit[channel].min)
      ad_value = ad_limit[channel].min;
   else if (ad_value > ad_limit[channel].max)
      ad_value = ad_limit[channel].max;

   start_conversion_count ++;

   ad_conv_timer = 0;
   /* Normal case, may be changed below. */

   ad_conv_num ++;
   if (ad_conv_num >= AD_NUM_CONV) ad_conv_num = 0;

   if (ad_random_failures > 0)
   {
      if (Next_Rand (&ad_fail_index) > 0x3f)
      {
         /* Pretend that this conversion will fail (not end). */

#if defined(TRACE_HARNESS)
         printf ("Start_Conversion starts a failing A/D conversion.\n");
#endif
         ad_conv_timer = -5000;

         ad_random_failures --;
      }
   }

   ad_converting = 1;
}


void Report_Start_Conversion_Count (int problem)
/* Reports and then clears the count of Start_Conversion calls.
 * The problem parameter associates this count with a given
 * analysis problem for this benchmark.
 */
{
#if defined(TRACE_HARNESS)
   printf ("Called Start_Conversion %d times in problem %d.\n",
      start_conversion_count, problem);
#endif

   start_conversion_count = 0;
}


static unsigned int end_of_adc_count = 0;
/* Counts the number of calls of End_Of_ADC. */


unsigned char End_Of_ADC (void)
/* Is the A/D conversion done, that is, ready for readout?
 * 0 (CONVERSION_ACTIVE) means yes, any other value means no.
 */
{
#if defined(TRACE_HARNESS)
   printf ("End_Of_ADC\n");
#endif

   if (ad_converting == 0)
   {
#if defined(TRACE_HARNESS)
      printf ("- conversion not going on.\n");
#endif
   }

   end_of_adc_count ++;

   ad_conv_timer ++;

   if (ad_conv_timer >= ad_conv_delay[ad_conv_num])
   {  /* Conversion done. */

      ad_converting = 0;

      return 0;
   }
   else
   {  /* Conversion still going on. */

      /* Perhaps get a preemptive "hit": */

      if (max_adc_hits > 0)
      {
         /* We have some hits in reserve. */

         if (Next_Rand (&ad_hit_rand_index) > 0x7f)
         {
            /* Hit me again, Sam. */

#if defined(TRACE_HARNESS)
            printf ("Hit during A/D\n");
#endif
            confirm_hit_result = 1;

            total_adc_hits ++;
            max_adc_hits --;

            ad_converting = 0;

            return 0;
            /* No point in going on. */
         }
      }

      /* Conversion not yet finished and no hit. */

      return 1;
   }
}


void Report_End_Of_ADC_Count (int problem)
/* Reports and then clears the count of End_Of_ADC calls.
 * The problem parameter associates this count with a given
 * analysis problem for this benchmark.
 */
{
#if defined(TRACE_HARNESS)
   printf ("Called End_Of_ADC %d times in problem %d.\n",
      end_of_adc_count, problem);
#endif

   end_of_adc_count = 0;
}


unsigned char Get_Result (void)
{
   unsigned char value;

   /* Return the current MSB of ad_value: */

   value = (ad_value >> 8) & 0xff;

#if defined(TRACE_HARNESS)
   printf ("Get_Result %d = 0x%x\n", value, value);
#endif

   /* Shift the LSB to the MSB, for next Get_Result: */

   ad_value <<= 8;

   return value;
}

void Set_DAC_Output (unsigned char level)
{
#if defined(TRACE_HARNESS)
   printf ("Set_DAC_Output %d\n", level);
#endif
}



/* The variable ADC_channel_register is defined in health.c. */


/*    Sensor Unit operations : su_ctrl.h, dpu_ctrl.h, isr_ctrl.h    */


#include "su_ctrl.h"


/* Variables simulating the event sensors: */

static unsigned char hit_enabled       = 0;
static unsigned char trigger_flag      = 1;
static unsigned char event_flag        = ACCEPT_EVENT;
static unsigned char trigger_source_0  = 0;
static unsigned char trigger_source_1  = 0;

static unsigned char msb_counter       = 134;
static unsigned char lsb1_counter      = 77;
static unsigned char lsb2_counter      = 88;
static unsigned char rise_time_counter = 102;

static unsigned char sim_self_test = 0;
/* Whether to simulate (successful) SU Self Test. */

static unsigned char self_test_pulse = 0;
/* The level of the SU Self Test pulse. */


void Sim_Self_Test_Trigger (void)
/* If conditions are such that a Self Test Hit Trigger should
 * be generated, simulate this occurrence by modifying the
 * SU_state for the SU being tested as the Hit Trigger ISR would do.
 * This supports the testing of the SU Self Test sequences in the
 * Monitoring task.
 *
 * The simulation does not actually invoke the Hit Trigger ISR
 * nor does it process the Self Test event. These DEBIE functions
 * are tested by calling Hit Trigger ISR directly, not from the
 * SU Self Test sequences.
 */
{
   if ((sim_self_test   == 1)
   &&  (hit_enabled     == 1)
   &&  (self_test_pulse >  0)
   &&  (self_test_SU_number != NO_SU)
   &&  (SU_state[self_test_SU_number - SU1] == self_test_trigger_e))
   {
#if defined(TRACE_HARNESS)
      printf ("SU Self Test trigger!\n");
#endif
      SU_state[self_test_SU_number - SU1] = self_test_e;
   }
}


static unsigned int event_rand_index = 0;
/* Roving index for ad_random, for randomizing the event data. */


void Random_Event (void)
/* Sets random data in the event sensors. */
{
   unsigned char val;

   val = Next_Rand (&event_rand_index);

   trigger_flag     = (val >> 7) & 1;
   event_flag       = (val >> 6) & 1;
   trigger_source_0 = (val >> 5) & 1;
   trigger_source_1 = (val >> 4) & 1;

   msb_counter       = Next_Rand (&event_rand_index);
   lsb1_counter      = Next_Rand (&event_rand_index);
   lsb2_counter      = Next_Rand (&event_rand_index);
   rise_time_counter = Next_Rand (&event_rand_index);
}


static void Set_Trigger_SU (sensor_index_t SU)
/* Sets the given SU in trigger_source_0/1. */
{
#if defined(TRACE_HARNESS)
   printf ("Set Trigger SU index %d\n", SU);
#endif

   trigger_source_0 =  SU       & 1;
   trigger_source_1 = (SU >> 1) & 1;
}


void Enable_Hit_Trigger  (void)
{
#if defined(TRACE_HARNESS)
   printf ("Enable_Hit_Trigger\n");
#endif
   hit_enabled = 1;
   Sim_Self_Test_Trigger ();
}

void Disable_Hit_Trigger (void)
{
#if defined(TRACE_HARNESS)
   printf ("Disable_Hit_Trigger\n");
#endif
   hit_enabled = 0;
   Sim_Self_Test_Trigger ();
}


unsigned char Hit_Trigger_Flag (void)
{
#if defined(TRACE_HARNESS)
   printf ("Hit_Trigger_Flag\n");
#endif
   return trigger_flag;
  /* 1 means hit trigger ITs are enabled
   * 0 means they are disabled.
   */
}


unsigned char Event_Flag (void)
{
#if defined(TRACE_HARNESS)
   printf ("Event_Flag \n");
#endif
   return event_flag;
}


unsigned char Get_MSB_Counter (void)
{
#if defined(TRACE_HARNESS)
   printf ("Get_MSB_Counter\n");
#endif
   return msb_counter;
}


unsigned char Get_LSB1_Counter  (void)
{
#if defined(TRACE_HARNESS)
   printf ("Get_LSB1_Counter\n");
#endif
   return lsb1_counter;
}


unsigned char Get_LSB2_Counter  (void)
{
#if defined(TRACE_HARNESS)
   printf ("Get_LSB2_Counter\n");
#endif
   return lsb2_counter;
}


unsigned char Rise_Time_Counter (void)
{
#if defined(TRACE_HARNESS)
   printf ("Rise_Time_Counter\n");
#endif
   return rise_time_counter;
}


unsigned char Trigger_Source_0 (void)
{
#if defined(TRACE_HARNESS)
   printf ("Trigger_Source_0\n");
#endif
   return trigger_source_0;
}


unsigned char Trigger_Source_1 (void)
{
#if defined(TRACE_HARNESS)
   printf ("Trigger_Source_1\n");
#endif
   return trigger_source_1;
}


void Set_SU_Self_Test_Ch (unsigned char value)
/* Set the SU Self Test Channel selectors. */
{
#if defined(TRACE_HARNESS)
   printf ("Set SU Self-Test Channel %x\n", value);
#endif
}


void Set_Test_Pulse_Level (unsigned char level)
/* Set the SU Self Test pulse level. */
{
#if defined(TRACE_HARNESS)
   printf ("Set SU Self-Test Pulse Level %d\n", level);
#endif
   self_test_pulse = level;
   Sim_Self_Test_Trigger ();
}


static unsigned int v_down_errors = 0;
/* The number of consecutive error results to
 * be returned from the next calls of V_Down.
 */


unsigned char V_Down (void)
{
   unsigned char result;

   if (v_down_errors > 0)
   {
      result = 0;  /* Bad. */
      v_down_errors --;
   }
   else
      result = 1;  /* Good. */

#if defined(TRACE_HARNESS)
   printf ("V_Down %d\n", result);
#endif

   return result;
}


void SignalPeakDetectorReset(
   unsigned char low_reset_value,
   unsigned char high_reset_value)
{
#if defined(TRACE_HARNESS)
   printf ("SignalPeakDetectorReset low %d, high %d\n",
      low_reset_value, high_reset_value);
#endif
}


/* The following operations are implemented in hw_if.c:
 * ReadDelayCounters
 * ReadRiseTimeCounter
 * ResetDelayCounters
 * ResetPeakDetector
 * SetTriggerLevel
 * SetTestPulseLevel
 * GetVoltageStatus
 * Switch_SU_On
 * Switch_SU_Off
 * EnableAnalogSwitch
 * DisableAnalogSwitch
 * SelectSelfTestChannel
 * SelectTriggerSwitchLevel
 * SelectStartSwitchLevel
 */


/*    Accessing unaligned tm_ushort_t data   */


unsigned short int Short_Value (tm_ushort_t *x)
{
   unsigned short val;
   wcc_memcpy (&val, x, sizeof (val));
   return val;
}


/*    "Calling" a "patch function"   */


void Call_Patch (fptr_t func)
{
#if defined(TRACE_HARNESS)
   printf ("Call_Patch 0x%x\n", (code_address_t)func);
#endif
}


/*    Checking the sizes of some types    */

#include "tm_data.h"

static volatile uint_least8_t  tsize_s;
static volatile uint_least16_t tsize_l;

void Check_Type_Size (void)
/*
 * Disassemble the code of this function to see the sizes of
 * the types char, short, ..., science_data_file_t.
 * This function is never called.
 *
 */
{
   tsize_s = sizeof (char );
   tsize_s = sizeof (short);
   tsize_s = sizeof (int  );
   tsize_s = sizeof (long );

   tsize_s = sizeof (tm_ushort_t     );
   tsize_s = sizeof (dpu_time_t      );
   tsize_s = sizeof (tm_dpu_time_t   );
   tsize_s = sizeof (SU_settings_t   );
   tsize_s = sizeof (telemetry_data_t);

   tsize_s = sizeof (event_record_t     );
   tsize_l = sizeof (science_data_file_t);
}


/*   Test scenarios    */

/*
These test cases are not intended as a full functional test.
They are intended to provide sufficient coverage for measurement-
based WCET analysis of the tested code. Built-in checks of test
execution (program state) are used only to check that the DEBIE
software has done the expected thing in each test case, but the
checks are definitely not exhaustive.

Each test cases contributes measurements towards one of the
"analysis problems" defined for this benchmark. The macro
FOR_PROBLEM(P) is defined in problems.h and marks the start of
a test case to be included in problem P. Here P is an integer
number that identifies the analysis problem as follows:

P    Problem
--   -------
10   1 : TC interrupt handler
21   2a: TM interrupt handler, most common case
22   2b: TM interrupt handler, send internal time register
23   2c: TM interrupt handler, end of TM
31   3a: Hit Trigger interrupt handler, no ADC errors
32   3b: Hit Trigger interrupt handler, at most one ADC error
33   3c: Hit Trigger interrupt handler, any number of ADC errors
41   4a: TC Execution task, general case
42   4b: TC Execution task, start TC buffering, MSB
43   4c: TC Execution task, start TC buffering, LSB
44   4d: TC Execution task, buffer TC word
51   5a: Acquisition task, science data not full
52   5b: Acquisition task, science data full
61   6a: Monitoring task, no hits or errors
62   6b: Monitoring task, one hit during A/D conversion
63   6c: Monitoring task, many hits during A/D conversion
64   6d: Monitoring task, some SU error detected
65   6e: Monitoring task, any number of hits and errors

The macro END_PROBLEM is also defined in problems.h and marks
the end of execution of a case belonging to the problem named
by the most recently executed FOR_PROBLEM. Note that more cases
for the same problem can be added later with another FOR_PROBLEM.
*/

#define Prob1  10

#define Prob2a 21
#define Prob2b 22
#define Prob2c 23

#define Prob3a 31
#define Prob3b 32
#define Prob3c 33

#define Prob4a 41
#define Prob4b 42
#define Prob4c 43
#define Prob4d 44

#define Prob5a 51
#define Prob5b 52

#define Prob6a 61
#define Prob6b 62
#define Prob6c 63
#define Prob6d 64
#define Prob6e 65

#include "problems.h"

#if defined(TRACE_HARNESS)
#define CASE(TXT) printf ("\nCASE: %s:\n\n", TXT)
#else
#define CASE(TXT)
#endif


static int checks = 0;
/* Counts the number of checks done in Check/Zero/Nonzero. */

static int check_errors = 0;
/* Counts the number of errors found in Check/Zero/Nonzero. */


static void Report_Checks (void)
/* Print number of checks and check-errors. */
{
   TARGET_MARK;
#if defined(TRACE_HARNESS)
   printf ("Total checks done %d, failed %d\n", checks, check_errors);
#endif
}


static void Fail_Check (char *message)
/* Reports a failed check. */
{
   check_errors ++;
#if defined(TRACE_HARNESS)
   printf ("%s: FAILED (#%d)\n", message, check_errors);
#endif
}


static void Check_Zero (int cond)
/* Checks that cond == 0. */
{
   checks ++;
   if (cond)
   {
      Fail_Check("Check_Zero");
   }
}


static void Check_Nonzero (int cond)
/* Checks that cond != 0. */
{
   checks ++;
   if (cond == 0)
   {
      Fail_Check ("Check_Nonzero");
   }
}


#define Check Check_Nonzero
/* Check that the (boolean) parameter is true (not zero). */


static void Check_No_Errors (void)
/* Checks that no errors are flagged in telemetry_data.error_status. */
{
   Check_Zero (telemetry_data.error_status);
}


static void TC_Interrupt (void)
/* Runs the TC Interrupt Service. */
{
   FOR_PROBLEM(Prob1);
   TC_InterruptService();
   END_PROBLEM;
}


static void Send_TC (unsigned char address, unsigned char code)
/* Invokes TC_InterruptService with a TC composed of the
 * given address and code, provided with valid (even) parity.
 */
{
   unsigned char par;
   /* The parity. */

   /* Encode the address and code in the TC interface registers: */

   tc_msb = address << 1;
   tc_lsb = code;

   /* Generate the even parity bit: */

   par = tc_msb ^ tc_lsb;
   par = (par & 0x0F) ^ (par >>4);
   par = (par & 0x03) ^ (par >>2);
   par = (par & 0x01) ^ (par >>1);

   tc_msb |= par;

   tc_word = (tc_msb << 8) | tc_lsb;

   /* Invoke the TC ISR: */

   tc_timer_overflow = 1;

   TC_Interrupt ();
}


static void Send_TC_Word (uint_least16_t word)
/* Invokes TC_InterruptService with the given TC word,
 * exactly as given.
 */
{
   /* Set the high and low TC bytes: */

   tc_msb = (word >> 8) & 0xff;
   tc_lsb =  word       & 0xff;

   tc_word = (tc_msb << 8) | tc_lsb;

   /* Invoke the TC ISR: */

   tc_timer_overflow = 1;

   TC_Interrupt ();
}

static void TC_ISR_Tests (void)
/* Test of TC_InterruptService. */
{

#if defined(TRACE_HARNESS)
   printf ("\nTC_ISR_Tests\n");
#endif

   CASE("TC rejected because timer overflow is not set");

      tc_timer_overflow = 0;
      TC_Interrupt ();

   CASE("TC rejected because TC_state is SC_TM_e");

      TC_state = SC_TM_e;
      Send_TC (0,0);

   CASE("TC rejected because TC_state is memory_dump_e");

      TC_state = memory_dump_e;
      Send_TC (0,0);

   CASE("TC in TC_state = memory_patch_e");

      TC_state = memory_patch_e;
      Send_TC (0,0);
      Check (mail_count[TCTM_MAILBOX] == 1);
      FlushMail (TCTM_MAILBOX);

   CASE("TC with parity error");

      TC_state = TC_handling_e;
      tc_timer_overflow = 1;
      tc_msb = 0; tc_lsb = 1; tc_word = 1;
      TC_Interrupt ();

      Check_Nonzero (telemetry_data.error_status & PARITY_ERROR);

   CASE("TC = ERROR_STATUS_CLEAR, ok");

      Send_TC (ERROR_STATUS_CLEAR, ERROR_STATUS_CLEAR);

      /* The parity-error flag is not yet reset, because */
      /* the TC was not yet executed:                    */

      Check_Nonzero (telemetry_data.error_status & PARITY_ERROR);

      Check (mail_count[TCTM_MAILBOX] == 1);
      FlushMail (TCTM_MAILBOX);

      /* Clear the error manually: */

      telemetry_data.error_status = 0;

      Check_No_Errors ();

   CASE("TC = SEND_STATUS_REGISTER, ok");

      Send_TC (SEND_STATUS_REGISTER, 8);

      Check_No_Errors ();
      Check (TC_state == register_TM_e);
      Check (mail_count[TCTM_MAILBOX] == 1);
      FlushMail (TCTM_MAILBOX);


   CASE("TC type ALL_INVALID");

      /* An invalid TC code: */

      Send_TC (4, 4);

      Check (telemetry_data.error_status == TC_ERROR);
      Check (mail_count[TCTM_MAILBOX] == 0);
      telemetry_data.error_status = 0;

   CASE("TC type ONLY_EQUAL, fail");

      Send_TC (ERROR_STATUS_CLEAR, ~ERROR_STATUS_CLEAR);

      Check (telemetry_data.error_status == TC_ERROR);
      Check (mail_count[TCTM_MAILBOX] == 0);
      telemetry_data.error_status = 0;

   CASE ("TC type ON_OFF_TC, fail");

      /* Neither ON_VALUE nor OFF_VALUE nor SELF_TEST. */

      Send_TC (SWITCH_SU_3, 0x3F);

      Check (telemetry_data.error_status == TC_ERROR);
      Check (mail_count[TCTM_MAILBOX] == 0);
      telemetry_data.error_status = 0;

   CASE ("TC type ONLY_EVEN, fail (odd)");

      Send_TC (SEND_STATUS_REGISTER, 5);

      Check (TC_state == TC_handling_e);
      Check (telemetry_data.error_status == TC_ERROR);
      Check (mail_count[TCTM_MAILBOX] == 0);
      telemetry_data.error_status = 0;

   CASE ("TC type ONLY_EVEN, fail (too large)");

      Send_TC (SEND_STATUS_REGISTER, LAST_EVEN + 2);

      Check (TC_state == TC_handling_e);
      Check (telemetry_data.error_status == TC_ERROR);
      Check (mail_count[TCTM_MAILBOX] == 0);
      telemetry_data.error_status = 0;


   /* Other tests of TC ISR:
   TC in TC_state = register_TM_e: See TM_Tests.
   */
}


static void Handle_TC (int problem)
/* Checks that the TC mailbox has a message, then invokes
 * HandleTelecommand to handle (usually, execute) the message.
 * The problem parameter shows in which analysis problem the
 * HandleTelecommand should be included.
 */
{
   Check (mail_count[TCTM_MAILBOX] == 1);

   FOR_PROBLEM(problem);
   HandleTelecommand ();
   END_PROBLEM;

   Check (mail_count[TCTM_MAILBOX] == 0);
}


static void Exec_TC (
   unsigned char address, unsigned char code,
   int problem)
/* Invokes TC_InterruptService with a TC composed of the
 * given address and code, provided with valid (even) parity,
 * then invokes HandleTelecommand to execute the TC.
 * The problem parameter shows in which analysis problem the
 * HandleTelecommand should be included.
 */
{
   Send_TC (address, code);
   Handle_TC (problem);
}


static void Send_Patch_Code (
   uint16_t       address,
   unsigned char *checksum)
/* Sends the multi-word TC to patch code memory at the given address,
 * with some arbitary contents. Returns the checksum of the patch,
 * for use in the final word of the TC, which is not sent here.
 */
{
   uint_least8_t i;
   unsigned char sum;

   /* Send the patch address: */

   Exec_TC (WRITE_CODE_MEMORY_MSB, (address >> 8) & 0xff, Prob4b);

   sum = (tc_word >> 8) ^ (tc_word & 0xff);

   Check_No_Errors ();
   Check (TC_state == write_memory_e);

   Exec_TC (WRITE_CODE_MEMORY_LSB,  address       & 0xff, Prob4c);

   sum ^= (tc_word >> 8) ^ (tc_word & 0xff);

   Check_No_Errors ();
   Check (TC_state == memory_patch_e);

   /* Send the patch contents, 16 words = 32 octets: */

   _Pragma("loopbound min 16 max 16")
   for (i = 0; i < 16; i++)
   {
      Send_TC_Word (((uint_least16_t) i) << 6);
      sum ^= (tc_word >> 8) ^ (tc_word & 0xff);
      Handle_TC (Prob4d);
   }

   /* The last word remains to be sent. */

   *checksum = sum;
}


static void Send_Patch_Data (
   uint16_t       address,
   unsigned char *checksum)
/* Sends the multi-word TC to patch data memory at the given address,
 * with some arbitary contents. Returns the checksum of the patch,
 * for use in the final word of the TC, which is not sent here.
 */
{
   uint_least8_t i;
   unsigned char sum;

   /* Send the patch address: */

   Exec_TC (WRITE_DATA_MEMORY_MSB, (address >> 8) & 0xff, Prob4b);

   sum = (tc_word >> 8) ^ (tc_word & 0xff);

   Check_No_Errors ();
   Check (TC_state == write_memory_e);

   Exec_TC (WRITE_DATA_MEMORY_LSB,  address       & 0xff, Prob4c);

   sum ^= (tc_word >> 8) ^ (tc_word & 0xff);

   Check_No_Errors ();
   Check (TC_state == memory_patch_e);

   /* Send the patch contents, 16 words = 32 octets: */

   _Pragma("loopbound min 16 max 16")
   for (i = 0; i < 16; i++)
   {
      Send_TC_Word (((uint_least16_t) i) << 6);
      sum ^= (tc_word >> 8) ^ (tc_word & 0xff);
      Handle_TC (Prob4d);
   }

   /* The last word remains to be sent. */

   *checksum = sum;
}


static void Read_Data_Memory (uint16_t address)
/* Sends the TC to read 32 octets of data memory, starting at
 * the given address, receives the corresponding TM block, and
 * handles the TM_READY message to the TC Execution task.
 */
{
   uint_least8_t i;

   Exec_TC (READ_DATA_MEMORY_MSB, (address >> 8) & 0xff, Prob4a);

   Check_No_Errors ();
   Check (TC_state == read_memory_e);

   Exec_TC (READ_DATA_MEMORY_LSB, address & 0xff, Prob4a);

   Check_No_Errors ();
   Check (TC_state == memory_dump_e);

   /* The 16 first words of data: */

   _Pragma("loopbound min 16 max 16")
   for (i = 0; i < 16; i++)
   {
      Check (telemetry_pointer < telemetry_end_pointer);

      /* The first TM IT, below, acknowledges the immediate TC
       * response and transmits the first data word.
       */

      FOR_PROBLEM(Prob2a);
      TM_InterruptService ();
      END_PROBLEM;

      Check (TC_state == memory_dump_e);
   }

   /* The last word, with the checksum: */

   Check (telemetry_pointer == telemetry_end_pointer);

   /* The TM IT below acknowledges the last data word and
    * transmits the very last word of the Read Data Memory
    * sequence, containing the data checksum.
    */

   FOR_PROBLEM(Prob2c);
   TM_InterruptService ();
   END_PROBLEM;

   /* The TM_READY message: */

   Handle_TC (Prob1);

   Check_No_Errors ();
   Check (TC_state == TC_handling_e);

   /* There is a design error in TM_InterruptService:
    * when the Read Data Memory sequence ends, with the
    * transmission of the checksum word, the TM IT is
    * left enabled. If the TC Execution task does not
    * react quickly to the TM_READY message, and disable
    * the TM IT before the transmission of the checksum
    * word is completed, a new TM IT can invoke
    * TM_InterruptService once again, sending the checksum
    * word a second time, and perhaps a third time etc.
    */
}


void Clear_Errors (void)
/* Executes the ERROR_STATUS_CLEAR TC. */
{
   Exec_TC (ERROR_STATUS_CLEAR, ERROR_STATUS_CLEAR, Prob4a);

   Check_No_Errors ();
   Check (TC_state == TC_handling_e);
}


static void TC_Task_Tests (void)
/* Test of TelecommandExecutionTask. */
{
   unsigned char chsum;

#if defined(TRACE_HARNESS)
   printf ("\nTC_Task_Tests\n");
#endif

   CASE("TC = ERROR_STATUS_CLEAR, ok");

      /* Flag an error manually: */
      telemetry_data.error_status = PARITY_ERROR;

      Send_TC (ERROR_STATUS_CLEAR, ERROR_STATUS_CLEAR);

      /* The parity-error flag is not yet reset, because */
      /* the TC was not yet executed:                    */

      Check_Nonzero (telemetry_data.error_status & PARITY_ERROR);

      Handle_TC (Prob4a);

      /* Now the parity-error flag is reset: */

      Check_No_Errors ();

   CASE("TC to set SU parameters");

      Exec_TC (SET_SU_1_PLASMA_1P_THRESHOLD,  23, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1P_THRESHOLD,  26, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1P_THRESHOLD,  32, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1P_THRESHOLD, 102, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1M_THRESHOLD, 205, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1M_THRESHOLD, 123, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1M_THRESHOLD,  99, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1M_THRESHOLD,   1, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PIEZO_THRESHOLD,  14, Prob4a);
      Exec_TC (SET_SU_2_PIEZO_THRESHOLD,  54, Prob4a);
      Exec_TC (SET_SU_3_PIEZO_THRESHOLD,  74, Prob4a);
      Exec_TC (SET_SU_4_PIEZO_THRESHOLD, 104, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1P_CLASS_LEVEL, 104, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1P_CLASS_LEVEL, 204, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1P_CLASS_LEVEL, 214, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1P_CLASS_LEVEL, 234, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1M_CLASS_LEVEL, 104, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1M_CLASS_LEVEL,  88, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1M_CLASS_LEVEL,  66, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1M_CLASS_LEVEL,  33, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_2P_CLASS_LEVEL,  61, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_2P_CLASS_LEVEL,  21, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_2P_CLASS_LEVEL,  81, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_2P_CLASS_LEVEL,  11, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PIEZO_1_CLASS_LEVEL,  14, Prob4a);
      Exec_TC (SET_SU_2_PIEZO_1_CLASS_LEVEL,  24, Prob4a);
      Exec_TC (SET_SU_3_PIEZO_1_CLASS_LEVEL,  33, Prob4a);
      Exec_TC (SET_SU_4_PIEZO_1_CLASS_LEVEL,  77, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PIEZO_2_CLASS_LEVEL,  14, Prob4a);
      Exec_TC (SET_SU_2_PIEZO_2_CLASS_LEVEL,  14, Prob4a);
      Exec_TC (SET_SU_3_PIEZO_2_CLASS_LEVEL,  14, Prob4a);
      Exec_TC (SET_SU_4_PIEZO_2_CLASS_LEVEL,  14, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1E_1I_MAX_TIME, 191, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1E_1I_MAX_TIME, 171, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1E_1I_MAX_TIME, 161, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1E_1I_MAX_TIME, 151, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1E_PZT_MIN_TIME,  11, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1E_PZT_MIN_TIME,  22, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1E_PZT_MIN_TIME,  33, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1E_PZT_MIN_TIME,  44, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1E_PZT_MAX_TIME, 111, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1E_PZT_MAX_TIME, 122, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1E_PZT_MAX_TIME, 133, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1E_PZT_MAX_TIME, 144, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1I_PZT_MIN_TIME,  11, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1I_PZT_MIN_TIME,  10, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1I_PZT_MIN_TIME,   9, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1I_PZT_MIN_TIME,   8, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_SU_1_PLASMA_1I_PZT_MAX_TIME, 211, Prob4a);
      Exec_TC (SET_SU_2_PLASMA_1I_PZT_MAX_TIME, 210, Prob4a);
      Exec_TC (SET_SU_3_PLASMA_1I_PZT_MAX_TIME, 209, Prob4a);
      Exec_TC (SET_SU_4_PLASMA_1I_PZT_MAX_TIME, 208, Prob4a);

      Check_No_Errors ();

   CASE("TC to set classification coefficients");

      Exec_TC (SET_COEFFICIENT_1, 1, Prob4a);
      Exec_TC (SET_COEFFICIENT_2, 2, Prob4a);
      Exec_TC (SET_COEFFICIENT_3, 3, Prob4a);
      Exec_TC (SET_COEFFICIENT_4, 4, Prob4a);
      Exec_TC (SET_COEFFICIENT_5, 5, Prob4a);

      Check_No_Errors ();

   CASE("TC to patch code memory, call patch");

      Send_Patch_Code (0x1100, &chsum);

      chsum ^= 0x5A;   /* Call patch function. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x5A00 | chsum);
      Handle_TC (Prob4a);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Check_No_Errors ();
      Check (TC_state == TC_handling_e);

   CASE("TC to patch code memory, no action");

      Send_Patch_Code (0x1300, &chsum);

      chsum ^= 0x00;   /* No action. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x0000 | chsum);
      Handle_TC (Prob4a);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Check_No_Errors ();
      Check (TC_state == TC_handling_e);

   CASE("TC to patch code memory, soft reset");

      Send_Patch_Code (0x1400, &chsum);

      chsum ^= 0x09;   /* Sof Reset. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x0900 | chsum);
      Handle_TC (Prob4a);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Check_No_Errors ();
      Check (TC_state == TC_handling_e);

   CASE("TC to patch code memory, warm reset");

      Send_Patch_Code (0x2100, &chsum);

      chsum ^= 0x37;   /* Soft Reset. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x3700 | chsum);
      Handle_TC (Prob4a);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Check_No_Errors ();
      Check (TC_state == TC_handling_e);

   CASE("TC to patch code memory, invalid action");

      Send_Patch_Code (0x2400, &chsum);

      chsum ^= 0x62;   /* Invalid. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x6200 | chsum);
      Handle_TC (Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch code memory, invalid address");

      Send_Patch_Code (0x0fff, &chsum);

      chsum ^= 0x00;   /* No action. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x0000 | chsum);
      Handle_TC (Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch code memory, checksum error");

      Send_Patch_Code (0x1200, &chsum);

      chsum ^= 0x5A;   /* Correct checksum. */
      chsum ^= 0xff;   /* Wrong   checksum. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x5A00 | chsum);
      Handle_TC (Prob4a);

      Check_Nonzero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);
      Check (TC_state == TC_handling_e);

      Clear_Errors ();

   CASE("TC to patch code, TC sequence error at first word");

      Exec_TC (WRITE_CODE_MEMORY_LSB, 0x32, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch code, TC sequence error at second word");

      Exec_TC (WRITE_CODE_MEMORY_MSB, 0x32, Prob4a);

      Check_No_Errors ();
      Check (TC_state == write_memory_e);

      Exec_TC (CLEAR_WATCHDOG_FAILURES, CLEAR_WATCHDOG_FAILURES, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch data memory");

      Send_Patch_Data (0x2200, &chsum);

      chsum ^= 0x11;   /* Irrelevant for data patch. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x1100 | chsum);
      Handle_TC (Prob4a);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Check_No_Errors ();
      Check (TC_state == TC_handling_e);

   CASE("TC to patch data memory, address error");

      Send_Patch_Data (0xfef0, &chsum);

      chsum ^= 0x11;   /* Irrelevant for data patch. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x1100 | chsum);
      Handle_TC (Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);
      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch data memory, TC sequence error at first word");

      Exec_TC (WRITE_DATA_MEMORY_LSB, 0x32, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch data memory, TC sequence error at second word");

      Exec_TC (WRITE_DATA_MEMORY_MSB, 0x32, Prob4a);

      Check_No_Errors ();
      Check (TC_state == write_memory_e);

      Exec_TC (CLEAR_WATCHDOG_FAILURES, CLEAR_WATCHDOG_FAILURES, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Clear_Errors ();

   CASE("TC to patch data memory, checksum error");

      Send_Patch_Data (0x2300, &chsum);

      chsum ^= 0x11;   /* Correct checksum. */
      chsum ^= 0xff;   /* Wrong   checksum. */

      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Send_TC_Word (0x1100 | chsum);
      Handle_TC (Prob4a);

      Check_Nonzero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);
      Check (TC_state == TC_handling_e);

      Clear_Errors ();

   CASE("TC to patch data memory, time-out on TC word reception");

      Send_Patch_Data (0x2200, &chsum);

      chsum ^= 0x11;   /* Irrelevant for data patch. */

      Check (TC_state == memory_patch_e);
      Check_Zero (telemetry_data.mode_status & MEMORY_WRITE_ERROR);

      Check (mail_count[TCTM_MAILBOX] == 0);

      /* Empty mailbox => WaitMail signals "time-out". */

      FOR_PROBLEM(Prob4a);
      HandleTelecommand ();
      END_PROBLEM;

      Check (TC_state == TC_handling_e);
      Check (telemetry_data.error_status == TC_ERROR);

      Clear_Errors ();

   CASE("TC timeout during TC handling, normal");

      Check (mail_count[TCTM_MAILBOX] == 0);

      /* Empty mailbox => WaitMail signals "time-out". */

      FOR_PROBLEM(Prob4a);
      HandleTelecommand ();
      END_PROBLEM;

      Check (TC_state == TC_handling_e);
      Check_No_Errors ();

   CASE("TC to read data memory");

      Read_Data_Memory (0x2041);

   CASE("TC to read data memory, address error");

      Exec_TC (READ_DATA_MEMORY_MSB, 0xfe, Prob4a);

      Check_No_Errors ();
      Check (TC_state == read_memory_e);

      Exec_TC (READ_DATA_MEMORY_LSB, 0xf0, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Clear_Errors ();

   CASE("TC to read data memory, TC sequence error at first word");

      Exec_TC (READ_DATA_MEMORY_LSB, 0xfe, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Clear_Errors ();

   CASE("TC to read data memory, TC sequence error at second word");

      Exec_TC (READ_DATA_MEMORY_MSB, 0xab, Prob4a);

      Check_No_Errors ();
      Check (TC_state == read_memory_e);

      Exec_TC (CLEAR_WATCHDOG_FAILURES, CLEAR_WATCHDOG_FAILURES, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Clear_Errors ();

   CASE("TC = CLEAR_WATCHDOG_FAILURES");

      telemetry_data.watchdog_failures = 3;

      Exec_TC (CLEAR_WATCHDOG_FAILURES, CLEAR_WATCHDOG_FAILURES, Prob4a);

      Check_No_Errors ();
      Check_Zero (telemetry_data.watchdog_failures);

   CASE("TC = CLEAR_CHECKSUM_FAILURES");

      telemetry_data.checksum_failures = 9;

      Exec_TC (CLEAR_CHECKSUM_FAILURES, CLEAR_CHECKSUM_FAILURES, Prob4a);

      Check_No_Errors ();
      Check_Zero (telemetry_data.checksum_failures);

   CASE("TC to set DEBIE time");

      Exec_TC (SET_TIME_BYTE_3, 0x44, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_2, 0x33, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_1, 0x22, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_0, 0x11, Prob4a);

      Check_No_Errors ();

      Check (internal_time == 0x44332211);

   CASE("TC to set DEBIE time, sequence error at byte 2");

      internal_time = 0x01122334;

      Exec_TC (SET_TIME_BYTE_3, 0x44, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_1, 0x33, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check (internal_time == 0x44000000);

      Clear_Errors ();

   CASE("TC to set DEBIE time, sequence error at byte 1");

      internal_time = 0x01122334;

      Exec_TC (SET_TIME_BYTE_3, 0x44, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_2, 0x33, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_0, 0x22, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check (internal_time == 0x44330000);

      Clear_Errors ();

   CASE("TC to set DEBIE time, sequence error at byte 0");

      internal_time = 0x01122334;

      Exec_TC (SET_TIME_BYTE_3, 0x44, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_2, 0x33, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_1, 0x22, Prob4a);

      Check_No_Errors ();

      Exec_TC (SET_TIME_BYTE_2, 0x11, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check (internal_time == 0x44332200);

      Clear_Errors ();

   CASE("TC = SOFT_RESET");

      Exec_TC (SOFT_RESET, SOFT_RESET, Prob4a);

      Check_No_Errors ();

   CASE("TC = STOP_ACQUISITION in STAND_BY, fail");

      Check (GetMode() == STAND_BY);

      Exec_TC (STOP_ACQUISITION, STOP_ACQUISITION, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);
      Check (GetMode() == STAND_BY);

      Clear_Errors ();

   /* Telecommands tested elsewhere:

   ERROR_STATUS_CLEAR        See TC_ISR_Tests, TM_Tests, others.
   SEND_STATUS_REGISTER      See TM_Tests.
   SEND_SCIENCE_DATA_FILE    See TM_Tests, Acquisition_Tests.
   START_ACQUISITION         See Acquisition_Tests.
   STOP_ACQUISITION          See Acquisition_Tests.
   SWITCH_SU_1/2/3/4         See Acquisition_Tests, SU_Self_Test_Tests.
   TM_READY message          See TM_Tests.
   */
}


void Monitor_Health (int problem)
/* Executes HandleHealthMonitoring for a particular analysis problem. */
{
   start_conversion_count = 0;
   end_of_adc_count       = 0;

   FOR_PROBLEM(problem);
   HandleHealthMonitoring();
   END_PROBLEM;

   Report_Start_Conversion_Count (problem);
   Report_End_Of_ADC_Count       (problem);
}


void Monitoring_Task_Tests (void)
/* Tests of the Health Monitoring task. */
{
   int sec;
   int tot_errors, max_errors = 20;

   /* Reset all cycles: */

   health_mon_round   = HEALTH_COUNT ;
   temp_meas_count    = TEMP_COUNT   ;
   voltage_meas_count = VOLTAGE_COUNT;
   checksum_count     = CHECK_COUNT  ;

   Set_AD_Nominal ();
   /* To avoid monitoring alarms. */

   CASE("Monitoring without errors or interrupting hits");

      /* A/D conversions ready on second poll: */
      Set_AD_Delay (2);

      Check (health_mon_round   == HEALTH_COUNT );
      Check (temp_meas_count    == TEMP_COUNT   );
      Check (voltage_meas_count == VOLTAGE_COUNT);
      Check (checksum_count     == CHECK_COUNT  );

      /* The first 9 seconds: */

      _Pragma("loopbound min 9 max 9")
      for (sec = 1; sec <= 9; sec ++)
      {
         Monitor_Health (Prob6a);

         Check (health_mon_round   == HEALTH_COUNT - sec);
         Check (temp_meas_count    == TEMP_COUNT   );
         Check (voltage_meas_count == VOLTAGE_COUNT);
         Check (checksum_count     == CHECK_COUNT  - sec);
      }

      /* The 10th second: */

      Monitor_Health (Prob6a);

      Check (health_mon_round   == HEALTH_COUNT      );
      Check (temp_meas_count    == TEMP_COUNT    - 1 );
      Check (voltage_meas_count == VOLTAGE_COUNT - 1 );
      Check (checksum_count     == CHECK_COUNT   - 10);

      /* The remaining 170 seconds of the full period: */

      _Pragma("loopbound min 170 max 170")
      for (sec = 11; sec <= 180; sec ++)
      {
         Monitor_Health (Prob6a);
      }

      Check (health_mon_round   == HEALTH_COUNT );
      Check (temp_meas_count    == TEMP_COUNT   );
      Check (voltage_meas_count == VOLTAGE_COUNT);
      Check (checksum_count     == CHECK_COUNT  );

      Check_No_Errors ();


   CASE("Monitoring without errors, at most one interrupting hit");

      total_adc_hits = 0;

      _Pragma("loopbound min 180 max 180")
      for (sec = 1; sec <= 180; sec ++)
      {
         max_adc_hits = 1;
         Monitor_Health (Prob6b);
      }

#if defined(TRACE_HARNESS)
      printf ("Total hits %d\n", total_adc_hits);
#endif

      Check_Nonzero (total_adc_hits);

      Check (health_mon_round   == HEALTH_COUNT );
      Check (temp_meas_count    == TEMP_COUNT   );
      Check (voltage_meas_count == VOLTAGE_COUNT);
      Check (checksum_count     == CHECK_COUNT  );

      Check_No_Errors ();

   CASE("Monitoring without errors, many interrupting hits");

      total_adc_hits = 0;

      _Pragma("loopbound min 170 max 170")
      for (sec = 1; sec <= 180; sec ++)
      {
         max_adc_hits = HIT_BUDGET_DEFAULT;
         Monitor_Health (Prob6c);
      }

#if defined(TRACE_HARNESS)
      printf ("Total hits %d\n", total_adc_hits);
#endif

      Check_Nonzero (total_adc_hits);

      Check (health_mon_round   == HEALTH_COUNT );
      Check (temp_meas_count    == TEMP_COUNT   );
      Check (voltage_meas_count == VOLTAGE_COUNT);
      Check (checksum_count     == CHECK_COUNT  );

      Check_No_Errors ();

   CASE("Monitoring with SU errors, no interrupting hits");

      Set_AD_Unlimited ();
      max_adc_hits  = 0;

      tot_errors = 0;

      _Pragma("loopbound min 106 max 106")
      do
      {
         if (tot_errors == 4) check_current_errors = 5;

         if (tot_errors == max_errors - 1) v_down_errors = 1;
         /* The V_DOWN error has a dramatic effect, so
          * we save it for last.
          */

         _Pragma("loopbound min 170 max 170")
         for (sec = 1; sec <= 180; sec ++)
         {
            Monitor_Health (Prob6d);
         }

         if (telemetry_data.error_status != 0)
         {
            tot_errors ++;

#if defined(TRACE_HARNESS)
            printf ("Monitoring (6d) error %d, error status %x\n",
               tot_errors, telemetry_data.error_status);
#endif
            Clear_Errors ();
         }
      }
      while (tot_errors < max_errors);

      Check_No_Errors ();

   CASE("Monitoring with any kind of error and interrupting hit");

      Set_AD_Unlimited ();

      tot_errors = 0;

      _Pragma("loopbound min 64 max 64")
      do
      {
         if (tot_errors == 4) check_current_errors = 5;

         if (tot_errors == max_errors - 1) v_down_errors = 1;
         /* The V_DOWN error has a dramatic effect, so
          * we save it for last.
          */

         _Pragma("loopbound min 170 max 170")
         for (sec = 1; sec <= 180; sec ++)
         {
            max_adc_hits = HIT_BUDGET_DEFAULT;
            if (sec > 2*tot_errors) ad_random_failures = 2;
            Monitor_Health (Prob6e);
         }

         if (telemetry_data.error_status != 0)
         {
            tot_errors ++;

#if defined(TRACE_HARNESS)
            printf ("Monitoring (6e) error %d, error status %x\n",
               tot_errors, telemetry_data.error_status);
#endif
            Clear_Errors ();
         }
      }
      while (tot_errors < max_errors);

      Check_No_Errors ();
}


void TM_Tests (void)
/* Test of TM_InterruptService and telemetry functionality. */
{
   int octets;
   /* Number of octets sent and acknowledge by TM interrupt. */

#if defined(TRACE_HARNESS)
   printf ("\nTM_ISR_Tests\n");
#endif

   CASE("One whole round of register TM");

      Send_TC (SEND_STATUS_REGISTER, 0);

      Check_No_Errors ();
      Check (TC_state == register_TM_e);

      Handle_TC (Prob4a);

      Check (TC_state == register_TM_e);

      _Pragma("loopbound min 62 max 62")
      for (octets = 0; octets < sizeof (telemetry_data_t); octets += 2)
      {

         if (telemetry_pointer != (unsigned char *) &telemetry_data.time)
         {
            FOR_PROBLEM(Prob2a);
            TM_InterruptService ();
            END_PROBLEM;
         }
         else
         {
            FOR_PROBLEM(Prob2b);
            TM_InterruptService ();
            END_PROBLEM;
         }

         Check (TC_state == register_TM_e);
      }

   CASE("Partial register TM, stop by TC");

      Send_TC (SEND_STATUS_REGISTER, 22);

      Check_No_Errors ();
      Check (TC_state == register_TM_e);

      Handle_TC (Prob4a);

      Check (TC_state == register_TM_e);

      _Pragma("loopbound min 20 max 20")
      for (octets = 0; octets < 40; octets += 2)
      {

         if (telemetry_pointer != (unsigned char *) &telemetry_data.time)
         {
            FOR_PROBLEM(Prob2a);
            TM_InterruptService ();
            END_PROBLEM;
         }
         else
         {
            FOR_PROBLEM(Prob2b);
            TM_InterruptService ();
            END_PROBLEM;
         }

         Check (TC_state == register_TM_e);
      }

      Send_TC (ERROR_STATUS_CLEAR, ERROR_STATUS_CLEAR);

      Check (TC_state == TC_handling_e);

      Handle_TC (Prob4a);

      Check_No_Errors ();

   CASE("Science Data TM");

      Send_TC (SEND_SCIENCE_DATA_FILE, SEND_SCIENCE_DATA_FILE);

      Check_No_Errors ();
      Check (TC_state == SC_TM_e);

      Handle_TC (Prob4a);

      Check (TC_state == SC_TM_e);

      /* Absorb TM until a TM_READY message is sent to the TC task: */

      octets = 0;
      _Pragma("loopbound min 22 max 22")
      while (mail_count[TCTM_MAILBOX] == 0)
      {
         if (telemetry_pointer < telemetry_end_pointer)
         {
            FOR_PROBLEM(Prob2a);
            TM_InterruptService ();
            END_PROBLEM;
         }
         else
         {
            FOR_PROBLEM(Prob2c);
            TM_InterruptService ();
            END_PROBLEM;
         }

         octets += 2;
         Check (TC_state == SC_TM_e);
      }

#if defined(TRACE_HARNESS)
      printf ("Science TM octets sent %d\n", octets);
#endif

      /* Handle the TM_READY message: */

      Handle_TC (Prob4a);

      Check (TC_state == TC_handling_e);

   /* TM tests elsewhere:
    * End of memory-dump TM : See Read_Data_Memory and TC_Task_Tests.
    * Science Data TM (also): See Acquisition_Tests.
    */
}


static void Trigger_Hit (int problem)
/* Invoke HandleHitTrigger.
 * The problem parameter defines the analysis problem for this test.
 */
{
   Check (mail_count[ACQUISITION_MAILBOX] == 0);

#if defined(TRACE_HARNESS)
   printf ("Hit!\n");
#endif

   FOR_PROBLEM(problem);

   HandleHitTrigger ();

   END_PROBLEM;

#if defined(TRACE_HARNESS)
   if (mail_count[ACQUISITION_MAILBOX] == 0)
      printf ("- hit rejected\n");
   else
      printf ("- hit accepted\n");
#endif
}


static void Trigger_SU_Hit (
   sensor_index_t SU,
   int            problem)
/* Invoke HandleHitTrigger with the given SU in trigger_source_0/1.
 * The problem parameter defines the analysis problem for this test.
 */
{
   Set_Trigger_SU (SU);
   Trigger_Hit (problem);
}


static void Acquire_Hit (
   int hit_problem,
   int acq_problem)
/* Invoke HandleHitTrigger followed by HandleAcquisition if the hit
 * was accepted in the ISR. The problem parameters define the analysis
 * problems for this test, separately for the Hit Trigger ISR and
 * for the Acquisition task.
 */
{
   Trigger_Hit (hit_problem);

   if (mail_count[ACQUISITION_MAILBOX] > 0)
   {
      FOR_PROBLEM(acq_problem);
      HandleAcquisition ();
      END_PROBLEM;
   }
}


static void Hit_ISR_Tests (void)
/* Test of HandleHitTrigger. */
{
   sensor_index_t su;

#if defined(TRACE_HARNESS)
   printf ("\nHit_ISR_Tests\n");
#endif

   /* Reset the historical record: */

   telemetry_data.hit_budget_exceedings = 0;

   /* Test: */

   CASE("Hit Trigger, budget exhausted");

      Check (telemetry_data.hit_budget_exceedings == 0);

      hit_budget_left = 0;

      /* Once: */

      Trigger_SU_Hit (0, Prob3a);

      Check (mail_count[ACQUISITION_MAILBOX] == 0);

      Check (telemetry_data.hit_budget_exceedings == 1);

      /* Once more: */

      Trigger_SU_Hit (1, Prob3a);

      Check (mail_count[ACQUISITION_MAILBOX] == 0);

      Check (telemetry_data.hit_budget_exceedings == 2);

   CASE("Hit Trigger, budget exhausted for the 255th and 256th time");

      telemetry_data.hit_budget_exceedings = 254;

      hit_budget_left = 0;

      /* 255th time: */

      Trigger_SU_Hit (3, Prob3a);

      Check (mail_count[ACQUISITION_MAILBOX] == 0);

      Check (telemetry_data.hit_budget_exceedings == 255);

      /* 256th time: */

      Trigger_SU_Hit (1, Prob3a);

      Check (mail_count[ACQUISITION_MAILBOX] == 0);

      Check (telemetry_data.hit_budget_exceedings == 255);

   CASE("Hit Trigger, budget left, no A/D errors");

      hit_budget_left = 15;

      Set_AD_Delay (2);
      ad_conv_num = 0;
      _Pragma("loopbound min 4 max 4")
      for (su = 0; su < NUM_SU; su ++)
      {
         Trigger_SU_Hit (su, Prob3a);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (mail_message[ACQUISITION_MAILBOX] == su + 1);

         FlushMail (ACQUISITION_MAILBOX);
      }

      Check (hit_budget_left == 15 - NUM_SU);

   CASE("Hit Trigger, budget left, no A/D errors, SU self test ok");

      _Pragma("loopbound min 4 max 4")
      for (su = 0; su < NUM_SU; su++)
      {
         /* Right self test pulse: */

         self_test_SU_number = su + 1;

         SU_state[su] = self_test_trigger_e;

         Trigger_SU_Hit (su, Prob3a);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (mail_message[ACQUISITION_MAILBOX] == self_test_SU_number);

         Check (SU_state[su] == self_test_e);

         FlushMail (ACQUISITION_MAILBOX);
      }

   CASE("Hit Trigger, budget left, no A/D errors, SU self test wrong");

      _Pragma("loopbound min 4 max 4")
      for (su = 0; su < NUM_SU; su++)
      {
         /* Wrong self test pulse: */

         self_test_SU_number = su + 1;

         SU_state[su] = self_test_e;

         Trigger_SU_Hit (su, Prob3a);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (
            mail_message[ACQUISITION_MAILBOX]
            == (self_test_SU_number | HIT_SELF_TEST_RESET));

         Check (SU_state[su] == self_test_e);

         FlushMail (ACQUISITION_MAILBOX);
      }

      /* Reset the SU states: */

      self_test_SU_number = NO_SU;

      _Pragma("loopbound min 4 max 4")
      for (su = 0; su < NUM_SU; su++) SU_state[su] = off_e;

   CASE("Hit Trigger, budget left, all A/D delays at limit but ok");

      hit_budget_left = 15;

      Set_AD_Delay (ADC_MAX_TRIES);

      ad_conv_num = 0;

      _Pragma("loopbound min 4 max 4")
      for (su = 0; su < NUM_SU; su ++)
      {
         Trigger_SU_Hit (su, Prob3b);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (
            mail_message[ACQUISITION_MAILBOX]
            == ((su + 1) ));  // | HIT_ADC_ERROR));

         FlushMail (ACQUISITION_MAILBOX);
      }

      Check (hit_budget_left == 15 - NUM_SU);

   CASE("Hit Trigger, budget left, one A/D failure, others at limit");

      hit_budget_left = 15;

      ad_conv_delay[0] = ADC_MAX_TRIES + 1;

      _Pragma("loopbound min 4 max 4")
      for (su = 0; su < NUM_SU; su ++)
      {
         ad_conv_num = su;
         /* Offset starting index to make a different channel fail. */

         Trigger_SU_Hit (su, Prob3b);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (
            mail_message[ACQUISITION_MAILBOX]
            == ((su + 1) | HIT_ADC_ERROR));

         FlushMail (ACQUISITION_MAILBOX);
      }

      Check (hit_budget_left == 15 - NUM_SU);

   CASE("Hit Trigger, budget left, any number of A/D failures");

      hit_budget_left = 80;

      su = NUM_SU - 1;

      _Pragma("loopbound min 80 max 80")
      while (hit_budget_left > 0)
      {
         Random_AD_Delay ();

         Trigger_SU_Hit (su, Prob3c);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (
            (mail_message[ACQUISITION_MAILBOX] & SU_NUMBER_MASK)
            == su + 1);

         FlushMail (ACQUISITION_MAILBOX);

         if (su > 0) su --;
         else        su = NUM_SU - 1;
      }

   /* More tests in SU_Self_Test_Tests. */
}


unsigned char switch_su_cmd[NUM_SU] = {
   SWITCH_SU_1,
   SWITCH_SU_2,
   SWITCH_SU_3,
   SWITCH_SU_4};
/* The commands to switch Sensor Units ON or OFF. */


void Report_Event_Histo (void)
/* Report the collected event counts per SU and class. */
{
   sensor_index_t sen;
   int class;

#if defined(TRACE_HARNESS)
   for (sen = 0; sen < NUM_SU; sen ++)
      for (class = 0; class <NUM_CLASSES; class ++)
      {
         printf ("Events from SU %d, class %d: %d\n",
            sen, class, science_data.event_counter[sen][class]);
      }
#endif
}


static void Acquisition_Tests (void)
/* Tests of AcquisitionTask. */
{
   sensor_index_t sen;
   unsigned int hits;
   int octets;

   /* These tests are constructed as a "nominal operation"
    * scenario, which incidentally tests the AcquisitionTask
    * and the Hit Trigger ISR, as well as other functions.
    * The scenario is:
    *
    * - TC to switch sensors ON
    *   - Run Monitoring task to drive the SU state transition.
    * - TC to START ACQUISITION
    * - Particle hits until the Science Data is full
    * - The same number of particle hits with the Science Data full
    * - Science Data TM
    *   - some particle hits during science TM
    * - TC to enter STANDBY mode
    * - TC to switch sensors OFF.
    *
    */

   CASE("Turn Sensor Units ON");

      /* Send the SWITCH ON commands: */

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         Check (SU_state[sen] == off_e);

         Exec_TC (switch_su_cmd[sen], ON_VALUE, Prob4a);

         Check_No_Errors ();
         Check (SU_state[sen] == start_switching_e);
      }

      /* Prevent all errors in Monitoring: */

      Set_AD_Nominal ();
      max_adc_hits         = 0;
      ad_random_failures   = 0;
      check_current_errors = 0;
      v_down_errors        = 0;

      /* Run Health Monitoring to drive the SUs ON: */

      Monitor_Health (Prob6a);

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
         Check (SU_state[sen] == switching_e);

      Monitor_Health (Prob6a);

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
         Check (SU_state[sen] == on_e);

   CASE("SWITCH_SU_ON when already ON, fail");

      Exec_TC (SWITCH_SU_2, ON_VALUE, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check (SU_state[1] == on_e);

      Clear_Errors ();

   CASE("Start Acquisition");

      Check ((telemetry_data.mode_status & MODE_BITS_MASK) == STAND_BY);

      Exec_TC (START_ACQUISITION, START_ACQUISITION, Prob4a);

      Check_No_Errors ();

      Check ((telemetry_data.mode_status & MODE_BITS_MASK) == ACQUISITION);

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
         Check (SU_state[sen] == acquisition_e);

   CASE("TC = SWITCH_SU in ACQUISITION, fail");

      Exec_TC (SWITCH_SU_1, ON_VALUE, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Clear_Errors ();

   CASE("TC = START_ACQUISITION in ACQUISITION, fail");

      Exec_TC (START_ACQUISITION, START_ACQUISITION, Prob4a);

      Check (telemetry_data.error_status == TC_ERROR);

      Check (GetMode() == ACQUISITION);

      Clear_Errors ();

   CASE("Hits with Science Data not full");

      Set_AD_Delay (2);

      hits = 0;

      _Pragma("loopbound min 2625 max 2625")
      while (free_slot_index < max_events)
      {
         hits ++;
         internal_time ++;
         hit_budget_left = 10;

         Random_Event ();

         Acquire_Hit (Prob3a, Prob5a);
      }

#if defined(TRACE_HARNESS)
      printf ("Science Data filled with %d events after %d hits.\n",
         max_events, hits);
#endif
      Report_Event_Histo ();

   CASE("Hits with Science Data full");

      _Pragma("loopbound min 2625 max 2625")
      while (hits > 0)
      {
         hits --;
         internal_time ++;
         hit_budget_left = 10;

         Random_Event ();

         Acquire_Hit (Prob3a, Prob5b);
      }

      Report_Event_Histo ();

   CASE("Science Data TM, full Science Data, some hits during TM");

      Send_TC (SEND_SCIENCE_DATA_FILE, SEND_SCIENCE_DATA_FILE);

      Check_No_Errors ();
      Check (TC_state == SC_TM_e);

      Handle_TC (Prob4a);

      Check (TC_state == SC_TM_e);

      /* Absorb TM until a TM_READY message is sent to the TC task,
       * and simulate some particle hits at the same time:
       */

      hits = 0;
      /* We will make MAX_QUEUE_LENGTH + 4 hits. */

      hit_budget_left = MAX_QUEUE_LENGTH + 2;
      /* Ensure that some hits are rejected for budget exhaustion. */

      Check_Zero (event_queue_length);

      octets = 0;
      _Pragma("loopbound min 17676 max 17676")
      while (mail_count[TCTM_MAILBOX] == 0)
      {
         if (telemetry_pointer < telemetry_end_pointer)
         {
            FOR_PROBLEM(Prob2a);
            TM_InterruptService ();
            END_PROBLEM;
         }
         else
         {
            FOR_PROBLEM(Prob2c);
            TM_InterruptService ();
            END_PROBLEM;
         }

         octets += 2;
         Check (TC_state == SC_TM_e);

         if (hits < MAX_QUEUE_LENGTH + 4)
         {
            /* Hit during Science Data TM: */

            internal_time ++;
            Random_Event ();
            event_flag = ACCEPT_EVENT;

            Acquire_Hit (Prob3a, Prob5b);

            hits ++;

            if (hits <= MAX_QUEUE_LENGTH)
               Check (event_queue_length == hits);
            else
               Check (event_queue_length == MAX_QUEUE_LENGTH);
         }
      }

#if defined(TRACE_HARNESS)
      printf ("Science TM octets sent %d\n", octets);
#endif

      Check_Zero (hit_budget_left);

      Check (hits == MAX_QUEUE_LENGTH + 4);

      /* Handle the TM_READY message: */

      Handle_TC (Prob4a);

      Check (TC_state == TC_handling_e);

      /* Check that the queued events have been recorded: */

      Check_Zero (event_queue_length);

      Check (free_slot_index == MAX_QUEUE_LENGTH);

   CASE("Switch to Self Test in Acquisition mode, fail");

      Check (GetMode() == ACQUISITION);

      Check_Zero (telemetry_data.error_status & TC_ERROR);

      Exec_TC (SWITCH_SU_2, SELF_TEST, Prob4a);

      Check_Nonzero (telemetry_data.error_status & TC_ERROR);

      Exec_TC (ERROR_STATUS_CLEAR, ERROR_STATUS_CLEAR, Prob4a);

      Check_Zero (telemetry_data.error_status & TC_ERROR);

   CASE("Stop acquisition");

      Exec_TC (STOP_ACQUISITION, STOP_ACQUISITION, Prob4a);

      Check_No_Errors ();

      Check ((telemetry_data.mode_status & MODE_BITS_MASK) == STAND_BY);

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
         Check (SU_state[sen] == on_e);

   CASE("Turn Sensor Units OFF");

      /* Send the SWITCH OFF commands: */

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         Exec_TC (switch_su_cmd[sen], OFF_VALUE, Prob4a);

         Check_No_Errors ();
         Check (SU_state[sen] == off_e);
      }
}

void SU_Self_Test_Tests (void)
/* Tests of the "SU Self Test" function. */
{
   sensor_index_t sen;

   sim_self_test = 1;

   CASE("Turn Sensor Units ON for Self Test");

      /* Send the SWITCH ON commands: */

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         Check (SU_state[sen] == off_e);

         Exec_TC (switch_su_cmd[sen], ON_VALUE, Prob4a);

         Check_No_Errors ();
         Check (SU_state[sen] == start_switching_e);
      }

      /* Prevent all errors in Monitoring: */

      Set_AD_Nominal ();
      max_adc_hits         = 0;
      ad_random_failures   = 0;
      check_current_errors = 0;
      v_down_errors        = 0;

      /* Run Health Monitoring to drive the SUs ON: */

      Monitor_Health (Prob6a);

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
         Check (SU_state[sen] == switching_e);

      Monitor_Health (Prob6a);

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
         Check (SU_state[sen] == on_e);

   CASE("Switch SU2 to Self Test in Standby mode");

      Check (GetMode() == STAND_BY);
      Check_No_Errors ();

      Exec_TC (SWITCH_SU_2, SELF_TEST, Prob4a);

      Check_No_Errors ();

      Check (self_test_SU_number == 2);
      Check (SU_state[1] == self_test_mon_e);

   CASE ("Switch SU3 (also) to Self Test, fail");

      Exec_TC (SWITCH_SU_3, SELF_TEST, Prob4a);

      Check_Nonzero (telemetry_data.error_status & TC_ERROR);

      Exec_TC (ERROR_STATUS_CLEAR, ERROR_STATUS_CLEAR, Prob4a);

      Check_Zero (telemetry_data.error_status & TC_ERROR);

      Check (self_test_SU_number == 2);

      Check_No_Errors ();

   CASE("Run Self Test for SU2");

      /* Run Monitoring up to but not including round_7_e: */

      _Pragma("loopbound min 8 max 8")
      while (health_mon_round != round_7_e)
      {
         Monitor_Health (Prob6a);
      }

      Check (self_test_SU_number == 2);
      Check (SU_state[1] == self_test_mon_e);

      /* Run round_7_e of Monitoring to start Self Test: */

      Monitor_Health (Prob6a);

      Check (self_test_SU_number == 2);
      Check (SU_state[1] == self_test_e);

      /* Run round_6_e of Monitoring to execute Self Test: */

      Check (health_mon_round == round_6_e);

      Monitor_Health (Prob6a);

      Check (self_test_SU_number == NO_SU);
      Check (SU_state[1] == on_e);

      Check_No_Errors ();

   CASE("Run Self Test for SU2, fail");

      Check (GetMode() == STAND_BY);
      Check_No_Errors ();

      Exec_TC (SWITCH_SU_2, SELF_TEST, Prob4a);

      Check_No_Errors ();

      Check (self_test_SU_number == 2);
      Check (SU_state[1] == self_test_mon_e);

      sim_self_test = 0;
      /* Force self-test to fail. */

      /* Run Monitoring up to but not including round_7_e: */

      _Pragma("loopbound min 8 max 8")
      while (health_mon_round != round_7_e)
      {
         Monitor_Health (Prob6a);
      }

      Check (self_test_SU_number == 2);
      Check (SU_state[1] == self_test_mon_e);

      /* Run round_7_e of Monitoring to start Self Test: */

      Monitor_Health (Prob6a);

      Check (self_test_SU_number == 2);
      Check (SU_state[1] == self_test_e);

      /* Run round_6_e of Monitoring to execute Self Test: */

      Check (health_mon_round == round_6_e);

      Monitor_Health (Prob6a);

      Check (self_test_SU_number == NO_SU);
      Check (SU_state[1] == on_e);

      Check_Nonzero (telemetry_data.SU_status[1] & SELF_TEST_ERROR);
      Check (telemetry_data.error_status == 0x20);

      Clear_Errors ();

   CASE("Hit Trigger, SU Self Test, correct pulse");

      hit_budget_left = 15;

      Set_AD_Delay (2);
      ad_conv_num = 0;

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         self_test_SU_number = sen + SU1;

         SU_state[sen] = self_test_trigger_e;

         Trigger_SU_Hit (sen, Prob3a);

         Check (SU_state[sen] == self_test_e);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (mail_message[ACQUISITION_MAILBOX] == sen + SU1);

         FlushMail (ACQUISITION_MAILBOX);
      }

      Check (hit_budget_left == 15 - NUM_SU);

   CASE("Hit Trigger, SU Self Test, incorrect pulse");

      hit_budget_left = 15;

      Set_AD_Delay (2);
      ad_conv_num = 0;

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         self_test_SU_number = sen + SU1;

         SU_state[sen] = self_test_e;

         Trigger_SU_Hit (sen, Prob3a);

         Check (SU_state[sen] == self_test_e);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (mail_message[ACQUISITION_MAILBOX] ==
            ((sen + SU1) | HIT_SELF_TEST_RESET));

         FlushMail (ACQUISITION_MAILBOX);
      }

      Check (hit_budget_left == 15 - NUM_SU);

   CASE("Hit Trigger, SU Self Test, other pulse");

      hit_budget_left = 15;

      Set_AD_Delay (2);
      ad_conv_num = 0;

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         self_test_SU_number = sen + SU1;

         SU_state[sen] = on_e;

         Trigger_SU_Hit (sen, Prob3a);

         Check (SU_state[sen] == on_e);

         Check (mail_count[ACQUISITION_MAILBOX] == 1);

         Check (mail_message[ACQUISITION_MAILBOX] == sen + SU1);

         FlushMail (ACQUISITION_MAILBOX);
      }

      Check (hit_budget_left == 15 - NUM_SU);

      Check_No_Errors ();

   CASE("Turn Sensor Units OFF after Self Tests");

      /* Send the SWITCH OFF commands: */

      _Pragma("loopbound min 4 max 4")
      for (sen = 0; sen < NUM_SU; sen ++)
      {
         Exec_TC (switch_su_cmd[sen], OFF_VALUE, Prob4a);

         Check_No_Errors ();
         Check (SU_state[sen] == off_e);
      }
}



/*   StartSystem () : test scenario    */


static unsigned int test_round = 0;
/* Counts test repetitions. */


void StartSystem(unsigned char task_number)
/*   ORIGINALLY:                                                             */
/* Purpose        : Starts the system.                                       */
/* Interface      : input  - none                                            */
/*                  output - none                                            */
/* Preconditions  : none                                                     */
/* Postconditions : First task is called and system started.                 */
/* Algorithm      : See below, self explanatory.                             */
/*   IN HARNESS:                                                             */
/* Executes the test scenario.                                               */
{
#if defined(TRACE_HARNESS)
   printf ("StartSystem %d\n", task_number);
#endif

   /* Initialize the global data of the tasks: */

   InitHealthMonitoring ();

   /* Testing the ISRs and tasks: */

   _Pragma("loopbound min 1 max 1")
   do
   {
      test_round ++;

#if defined(TRACE_HARNESS)
      printf ("Test round %d\n", test_round);
#endif

      TARGET_START_TEST;

      TC_ISR_Tests ();

         Report_Checks ();

      TC_Task_Tests ();

         Report_Checks ();

      Monitoring_Task_Tests ();

         Report_Checks ();

      TM_Tests ();

         Report_Checks ();

      Hit_ISR_Tests ();

         Report_Checks ();

      Acquisition_Tests ();

         Report_Checks ();

      SU_Self_Test_Tests ();

         Report_Checks ();

      /* Clear for next round: */

      checks = 0;
   }
   while (TARGET_REPEAT_TEST);

#if defined(TRACE_HARNESS)
   printf ("Total mailbox overflows %d\n", mail_overflows);
   exit (0);
#endif

}
