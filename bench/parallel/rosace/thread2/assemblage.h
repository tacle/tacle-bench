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

  Name: assemblage.h

  Author: Unknown

  Function: 

  Source: https://svn.onera.fr/schedmcore/branches/ROSACE_CaseStudy/        
          c_posix_implementation/

  Original name: assemblage.h

  Changes: No major functional changes.

  License: See the terms above.

*/

#ifndef _assemblage_H
#define _assemblage_H
#include "nonencoded_task_params.h"
#include "multirate_precedence.h"
#include "com_patterns.h"
#include "types.h"

void get_task_set( int *task_number, struct nonencoded_task_params **task_set );

void get_precedence_set( int *prec_number,
                         struct multirate_precedence **presc );


#define H_C0    0
#define DELTA_E_C0  1
#define VZ_CONTROL  2
#define ENGINE    3
#define H_FILTER  4
#define AIRCRAFT_DYN  5
#define Q_FILTER  6
#define VZ_FILTER 7
#define AZ_FILTER 8
#define DELTA_TH_C0 9
#define ALTI_HOLD 10
#define VA_C0   11
#define VA_CONTROL  12
#define ELEVATOR  13
#define VA_FILTER 14





#endif
