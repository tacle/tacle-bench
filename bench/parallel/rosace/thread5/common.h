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

  Name: common.h

  Author: Dustin Green

  Function: Common used functions and variables from original threads.c 

  Source: 

  Original name: 

  Changes: No major functional changes

  License: See the terms above.

*/

#ifndef COMMON_H
#define COMMON_H

#include "types.h"
#include "assemblage_includes.h"
#include "assemblage.h"


// I/O
output_t outs;
uint64_t step_simu;
uint64_t max_step_simu;


// Output variables
extern double aircraft_dynamics495_Va_Va_filter_100449_Va[ 2 ];
extern double aircraft_dynamics495_az_az_filter_100458_az[ 2 ];
extern double aircraft_dynamics495_Vz_Vz_filter_100452_Vz[ 2 ];
extern double aircraft_dynamics495_q_q_filter_100455_q[ 2 ];
extern double aircraft_dynamics495_h_h_filter_100446_h[ 2 ];
extern double Va_control_50474_delta_th_c_delta_th_c;
extern double Vz_control_50483_delta_e_c_delta_e_c;


// Task set
struct nonencoded_task_params *tasks;


// Common functions
void rosace_init( void );
void copy_output_vars( output_t *v, uint64_t step );


// This should be set to 1 to run in "real-time" in the sense
// that the simulation time is close to the real world time
#define RUN_WITH_REAL_TIME  0

#define CALL( val ) tasks[ (val) ].ne_t_body( NULL )

#endif
