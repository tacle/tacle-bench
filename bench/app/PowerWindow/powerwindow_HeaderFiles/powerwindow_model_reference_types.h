/*
 * File: model_reference_types.h
 *
 * Code generated for Simulink model 'Debounce'.
 *
 * Model version                  : 1.34
 * Simulink Coder version         : 8.7 (R2014b) 08-Sep-2014
 * C/C++ source code generated on : Tue Jun 30 15:05:13 2015
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
