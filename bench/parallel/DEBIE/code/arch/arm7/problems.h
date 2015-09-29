/*
problems.h for arm7/gcc-if07

Part of the DEBIE-1 benchmark.

$Id: problems.h,v 1.1 2008/04/09 11:26:38 niklas Exp $
*/


#ifndef PROBLEMS_H
#define PROBLEMS_H

#define FOR_PROBLEM(P) 
/* A marker to indicate that the program is about to execute   */
/* a test case that is to be included in the analysis problem  */
/* identified by P.                                            */

#define END_PROBLEM 
/* A marker to indicate the end of a test case that is to be   */
/* included in the analysis problem identified by the last     */
/* executed FOR_PROBLEM.                                       */

#endif
