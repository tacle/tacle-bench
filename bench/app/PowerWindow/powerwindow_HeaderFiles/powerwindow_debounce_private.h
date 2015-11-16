/*
 * File: debounce_private.h
 *
 * Code generated for Simulink model 'debounce'.
 *
 * Model version                  : 1.34
 * Simulink Coder version         : 8.7 (R2014b) 08-Sep-2014
 * C/C++ source code generated on : Tue Jun 30 15:05:13 2015
 *
 * Target selection: ert.tlc
 * Embedded hardware selection: Atmel->AVR
 * Code generation objectives: Unspecified
 * Validation result: Not run
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
