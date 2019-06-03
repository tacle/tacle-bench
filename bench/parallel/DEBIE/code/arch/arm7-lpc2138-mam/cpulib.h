/*
  cpulib: Routines for CPU initialization and configuration.

  Target  : iSYSTEM LPC2138-M minitarget board (ARM7TDMI)
  Source  : iSYSTEM iF-DEV-LPC kit
  Changes : Tidorum Ltd (N. Holsti)

  $Id: cpulib.h,v 1.1 2008/04/08 09:44:55 niklas Exp $
*/


#ifndef CPULIB_H
#define CPULIB_H

#define REG(x) (*((volatile unsigned int *)(x)))
#define IO0DIR      REG(0xE0028008)  // I/O direction control
#define IO0CLR      REG(0xE002800C)  // output clear
#define IO0SET      REG(0xE0028004)  // output set
#define PLLCFG      REG(0xE01FC084)  // PLL Config Reg. 
#define PLLCON      REG(0xE01FC080)  // PLL Control Reg.
#define PLLFEED     REG(0xE01FC08C)  // PLL-Feed 0xAA, 0x55
#define PLLSTAT     REG(0xE01FC088)  // PLL-Status register
#define MAMCR       REG(0xE01FC000)  // MAM Control register
#define MAMTIM      REG(0xE01FC004)  // MAM Timing register


#define IO_LED    0x00000001
// The LED is connected to pin P0.0.


void cpulib_init_cpu ( void );
// Initializes the processor (in addition to crt0 actions).
// Defines the LED control pin (P0.0) as output and emits a "1"
// to turn off the LED.

void cpulib_go_fast ( unsigned int mode );
// Sets the PLL to run the processor at 60 MHz.
// Does not alter the MAM mode (because of the LPC2138 errata).

void  cpulib_set_led ( int state );
// Turns the LED on if state != 0, off if state = 0.

void cpulib_flash_led ( void );
// Turns the LED on for a brief time, then off.
// Useful only if the LED is initially off.

void cpulib_blink_led ( void );
// Turns the LED off for short time, then on
// for a shorter time, then off again.

#endif
