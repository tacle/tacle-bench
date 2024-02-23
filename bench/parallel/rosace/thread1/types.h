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

  Name: types.h

  Author: Unknown

  Function: 

  Source: https://svn.onera.fr/schedmcore/branches/ROSACE_CaseStudy/        
          c_posix_implementation/

  Original name: types.h

  Changes: No major functional changes.

  License: See the terms above.

*/

#ifndef __DEF_ROSACE_TYPES_H
#define __DEF_ROSACE_TYPES_H

#ifdef USE_FLOAT
#define REAL_TYPE float
#else
#define REAL_TYPE double
#endif

#if __STDC_VERSION__ >= 199901L
#include <stdint.h>
#else
typedef unsigned long long uint64_t;
#endif


/* we need forward declaration only in order
 * to avoid redefinition in assemblage_vX generated headers
 * Real "#include "assemblage.h" is only done in assemblage_includes.c
 */
struct aircraft_dynamics_outs_t {
    REAL_TYPE Va;
    REAL_TYPE Vz;
    REAL_TYPE q;
    REAL_TYPE az;
    REAL_TYPE h;
};

typedef enum SAMPLE_RANK {
    SPL_T, SPL_VA,SPL_AZ,SPL_Q,SPL_VZ,SPL_H,
    SPL_DELTA_TH_C, SPL_DELTA_E_C,
    SPL_SIZE
} SampleRank_t;

#endif
