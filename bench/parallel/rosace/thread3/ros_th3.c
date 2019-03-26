/* ----------------------------------------------------------------------------
   SchedMCore - A MultiCore Scheduling Framework
   Copyright (C) 2009-2011, ONERA, Toulouse, FRANCE - LIFL, Lille, FRANCE

   This file is part of Prelude

   Prelude is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation ; either version 2 of
   the License, or (at your option) any later version.

   Prelude is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY ; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this program ; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
   USA
  ---------------------------------------------------------------------------- */

/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.9

  Name: ros_th3

  Author: Dustin Green and others

  Function: executes the third thread of original threads.c 
  
  Source: https://svn.onera.fr/schedmcore/branches/ROSACE_CaseStudy/c_posix_implementation/

  Original name: threads, rosace

  Changes: union of rosace.c and threads.c 

  License: see above 

*/

#include "types.h"
#include "assemblage_includes.h"
#include "assemblage.h"
#include "common.h"


// Return variable
int ros_th3_check;


/*
  Forward declaration of functions
*/

void* ros_th3_thread3( void* arg );
int ros_th3_run_rosace( uint64_t nbstep );
int main( void );
void ros_th3_init( void );
void ros_th3_main( void );
int ros_th3_return( void );


/*
  Initialization function
*/

void ros_th3_init( void )
{
  ros_th3_check = -1;
  rosace_init();  
}


/*
  Return function
*/

int ros_th3_return( void )
{
  return ros_th3_check;  
}


/*
  Main functions
*/

void ros_th3_main( void )
{
  uint64_t tsimu = 300*200;
  ros_th3_check = ros_th3_run_rosace( tsimu );
}


int ros_th3_run_rosace( uint64_t nbstep ){
  max_step_simu = nbstep;
  // Set first command
  ROSACE_update_altitude_command( 11000.0 );
    
  // Exit
  ros_th3_thread3( NULL );
  return 0;
}


void* ros_th3_thread3( void* arg ) {
  uint64_t mystep_simu = step_simu;
  
  _Pragma( "loopbound min 60000 max 60000" )
  while( mystep_simu<max_step_simu ) {
    // --- 100 Hz ---
    if( mystep_simu%2 == 0 ) {
      CALL(Q_FILTER);
    }
    // --- End 100 Hz ---
    mystep_simu++;
  }
  return NULL;
}


int main( void )
{
  ros_th3_init();
  
  ros_th3_main();
  
  return ros_th3_return();  
}
