/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_powerwindow_control_private.h

 Author: CoSys-Lab, University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_powerwindow_control_private.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */
#ifndef powerwindow_RTW_HEADER_PowerWindow_control_private_h_
#define powerwindow_RTW_HEADER_PowerWindow_control_private_h_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* Macros for accessing real-time model data structure */
#ifndef powerwindow_powerwindow_control_rtmGetErrorStatus
# define powerwindow_powerwindow_control_rtmGetErrorStatus(rtm)        (*((rtm)->errorStatus))
#endif

#ifndef powerwindow_powerwindow_control_rtmSetErrorStatus
# define powerwindow_powerwindow_control_rtmSetErrorStatus(rtm, val)   (*((rtm)->errorStatus) = (val))
#endif

#ifndef powerwindow_powerwindow_control_rtmGetErrorStatusPointer
# define powerwindow_powerwindow_control_rtmGetErrorStatusPointer(rtm) (rtm)->errorStatus
#endif

#ifndef powerwindow_powerwindow_control_rtmSetErrorStatusPointer
# define powerwindow_powerwindow_control_rtmSetErrorStatusPointer(rtm, val) ((rtm)->errorStatus = (val))
#endif

extern const powerwindow_boolean_T powerwindow_rtCP_pooled_6bUUQf1tASYw[12];

#define rtCP_map_table                 powerwindow_rtCP_pooled_6bUUQf1tASYw  /* Computed Parameter: map_table
                                                                  * Referenced by: '<S2>/map'
                                                                  */
#endif                                 /* RTW_HEADER_PowerWindow_control_private_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
