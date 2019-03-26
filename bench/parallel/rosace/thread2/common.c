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

  Name: common.c

  Author: Dustin Green

  Function: Common used functions from original threads.c 

  Source: 

  Original name: 

  Changes: No major changes of functions from original threads.c

  License: See the terms above.

*/

#include "common.h"
#include "types.h"
#include "assemblage_includes.h"
#include "assemblage.h"


void rosace_init( void )
{

  // Initial values
  outs.sig_outputs.Va = 0;
  outs.sig_outputs.Vz = 0;
  outs.sig_outputs.q  = 0;
  outs.sig_outputs.az = 0;
  outs.sig_outputs.h  = 0;
  outs.t_simu         = 0;
  step_simu           = 0;

  // Get the task set (required for CALL() macro)
  int tmp;
  get_task_set( &tmp, &tasks );
}


void copy_output_vars( output_t *v, uint64_t step )
{
  v->sig_outputs.Va   = aircraft_dynamics495_Va_Va_filter_100449_Va[ step % 2 ];
  v->sig_outputs.Vz   = aircraft_dynamics495_Vz_Vz_filter_100452_Vz[ step % 2 ];
  v->sig_outputs.q    = aircraft_dynamics495_q_q_filter_100455_q[ step % 2 ];
  v->sig_outputs.az   = aircraft_dynamics495_az_az_filter_100458_az[ step % 2 ];
  v->sig_outputs.h    = aircraft_dynamics495_h_h_filter_100446_h[ step % 2 ];
  v->sig_delta_th_c = Va_control_50474_delta_th_c_delta_th_c;
  v->sig_delta_e_c  = Vz_control_50483_delta_e_c_delta_e_c;
}

