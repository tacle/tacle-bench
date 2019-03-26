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

  Name: io.c

  Author: Unknown

  Function: Output function

  Source: https://svn.onera.fr/schedmcore/branches/ROSACE_CaseStudy/        
          c_posix_implementation/

  Original name: io.c

  Changes: delete the code within ROSACE_write_outputs because
           stdio.h is not available

  License: See the terms above.

*/


#include "types.h"
#include "io.h"

#define FMTREAL "%5.15f"
#define BASE_FREQUENCY 200.0

extern REAL_TYPE h_c;

void ROSACE_update_altitude_command( REAL_TYPE h_cons )
{
  h_c = h_cons;
}

void ROSACE_write_outputs( output_t *v )
{
  
}
