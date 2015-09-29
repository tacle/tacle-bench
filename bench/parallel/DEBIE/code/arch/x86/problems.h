/*
problems.h for intel/linux

Part of the DEBIE-1 benchmark.

$Id: problems.h,v 1.1 2008/03/28 08:22:17 niklas Exp $
*/


#ifndef PROBLEMS_H
#define PROBLEMS_H

#include "rpt_ipoint.h"

#define FOR_PROBLEM(P) { RPT_Ipoint(3); RPT_Ipoint(P); }
/* A marker to indicate that the program is about to execute   */
/* a test case that is to be included in the analysis problem  */
/* identified by P.                                            */
/* For RapiTime, ipoint 3 shall be defined as an "escape", and */
/* the associated info (P) shall be used to "demux" the trace. */

#define END_PROBLEM { RPT_Ipoint(3); RPT_Ipoint(0); }
/* A marked to indicate the end of a test case that is to be   */
/* included in the analysis problem identified by the last     */
/* executed FOR_PROBLEM.                                       */

#endif
