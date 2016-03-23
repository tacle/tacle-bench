/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_debounce_private.h

 Author: unknown

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_debounce_private.h

 Changes: a brief summary of major functional changes and formatting)

 License: unknown

 */
#ifndef powerwindow_RTW_HEADER_debounce_private_h_
#define powerwindow_RTW_HEADER_debounce_private_h_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* Macros for accessing real-time model data structure */
#ifndef powerwindow_rtmGetErrorStatus
# define powerwindow_rtmGetErrorStatus(rtm)        (*((rtm)->errorStatus))
#endif

#ifndef powerwindow_rtmSetErrorStatus
# define powerwindow_rtmSetErrorStatus(rtm, val)   (*((rtm)->errorStatus) = (val))
#endif

#ifndef powerwindow_rtmGetErrorStatusPointer
# define powerwindow_rtmGetErrorStatusPointer(rtm) (rtm)->errorStatus
#endif

#ifndef powerwindow_rtmSetErrorStatusPointer
# define powerwindow_rtmSetErrorStatusPointer(rtm, val) ((rtm)->errorStatus = (val))
#endif
#endif                                 /* RTW_HEADER_debounce_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
