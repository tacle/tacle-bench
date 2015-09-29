/*
problems.h for mpc5554/gcc.
Part of the DEBIE-1 benchmark.
*/


#ifndef PROBLEMS_H
#define PROBLEMS_H

#define FOR_PROBLEM(P) {}
/* A marker to indicate that the program is about to execute   */
/* a test case that is to be included in the analysis problem  */
/* identified by P.                                            */

#define END_PROBLEM {}
/* A marker to indicate the end of a test case that is to be   */
/* included in the analysis problem identified by the last     */
/* executed FOR_PROBLEM.                                       */

#endif
