/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_model_reference_types.h

 Author: unknown

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_model_reference_types.h

 Changes: a brief summary of major functional changes and formatting)

 License: unknown

 */

#ifndef powerwindow___MODEL_REFERENCE_TYPES_H__
#define powerwindow___MODEL_REFERENCE_TYPES_H__
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#ifndef powerwindow___MODEL_REFERENCE_TYPES__
#define powerwindow___MODEL_REFERENCE_TYPES__

/*===========================================================================*
 * Model reference type definitions                                          *
 *===========================================================================*/
/*
 * This structure is used by model reference to
 * communicate timing information through the hierarchy.
 */
typedef struct powerwindow__rtTimingBridge_tag powerwindow_rtTimingBridge;
struct powerwindow__rtTimingBridge_tag {
    powerwindow_uint32_T nTasks;
    powerwindow_uint32_T** clockTick;
    powerwindow_uint32_T** clockTickH;
    powerwindow_uint32_T* taskCounter;
    powerwindow_real_T** taskTime;
    powerwindow_boolean_T** rateTransition;
    powerwindow_boolean_T *firstInitCond;
};

#endif                                 /* __MODEL_REFERENCE_TYPES__ */
#endif                                 /* __MODEL_REFERENCE_TYPES_H__ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
