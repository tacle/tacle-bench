/*
  cpulib: Routines for CPU initialization and configuration.

  Target  : iSYSTEM LPC2138-M minitarget board (ARM7TDMI)
  Source  : iSYSTEM iF-DEV-LPC kit
  Changes : Tidorum Ltd (N. Holsti)

  $Id: cpulib.c,v 1.1 2008/04/08 09:44:55 niklas Exp $
*/

#include "cpulib.h"

void cpulib_init_cpu ( void )
{
  // The MAM is assumed to be initialized in crt0.s.

  // Turn off the LED:

  IO0DIR |= IO_LED; // Set pin direction to output.
  IO0SET |= IO_LED; // Set pin value to 1 = turn off LED.
}


void cpulib_go_fast ( unsigned int mode )
{
  // Turn on PLL from 12 MHz to 60 MHz cclk:

  PLLCFG = 0x24;
  PLLCON = 0x1;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
  while ( !( PLLSTAT & 0x400 ) ) {};
  PLLCON = 0x3;
  PLLFEED = 0xAA;
  PLLFEED = 0x55;
}


void cpulib_set_led ( int state )
{
  if ( state )
    IO0CLR = IO_LED; // LED on.
  else
    IO0SET = IO_LED; // LED off.
}


static volatile int flash_timer;
/* Loop counter for timing flash_led. */


void cpulib_flash_led ( void )
{
  cpulib_set_led ( 1 );

  flash_timer = 10000;
  while ( flash_timer > 0 ) flash_timer--;

  cpulib_set_led ( 0 );
}


void cpulib_blink_led ( void )
{
  cpulib_set_led ( 0 );

  flash_timer = 30000;
  while ( flash_timer > 0 ) flash_timer--;

  cpulib_set_led ( 1 );

  flash_timer = 10000;
  while ( flash_timer > 0 ) flash_timer--;

  cpulib_set_led ( 0 );
}

