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

  Name: io.h

  Author: Unknown

  Function: Output function

  Source: https://svn.onera.fr/schedmcore/branches/ROSACE_CaseStudy/        
          c_posix_implementation/

  Original name: io.h

  Changes: No major functional changes.

  License: See the terms above.

*/

#ifndef __DEF_ROSACE_IO_H
#define __DEF_ROSACE_IO_H

#include "types.h"

typedef struct {
  struct aircraft_dynamics_outs_t sig_outputs;
  uint64_t t_simu;
  REAL_TYPE sig_delta_th_c;
  REAL_TYPE sig_delta_e_c;
} output_t;

void ROSACE_write_outputs( output_t *v );
void ROSACE_update_altitude_command( REAL_TYPE h_cons );

#endif
