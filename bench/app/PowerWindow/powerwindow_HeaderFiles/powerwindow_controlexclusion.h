/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_controlexclusion.h

 Author: University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_controlexclusion.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */

#ifndef powerwindow_RTW_HEADER_controlexclusion_h_
#define powerwindow_RTW_HEADER_controlexclusion_h_
#include <stddef.h>
#include <string.h>
#ifndef powerwindow_controlexclusion_COMMON_INCLUDES_
#define powerwindow_controlexclusion_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#endif                                 /* powerwindow_controlexclusion_COMMON_INCLUDES_ */

#include "../powerwindow_HeaderFiles/powerwindow_controlexclusion_types.h"

/* Macros for accessing real-time model data structure */
#ifndef powerwindow_rtmGetErrorStatus
#define powerwindow_rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef powerwindow_rtmSetErrorStatus
# define powerwindow_rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* External inputs (root inport signals with auto storage) */
typedef struct {
    powerwindow_boolean_T Up_DRV;                    /* '<Root>/Up_DRV' */
    powerwindow_boolean_T Down_DRV;                  /* '<Root>/Down_DRV' */
    powerwindow_boolean_T Up_PSG;                    /* '<Root>/Up_PSG' */
    powerwindow_boolean_T Down_PSG;                  /* '<Root>/Down_PSG' */
} powerwindow_ExternalInputs_controlexclusion_T;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
    powerwindow_boolean_T Up;                        /* '<Root>/Up' */
    powerwindow_boolean_T Down;                      /* '<Root>/Down' */
} powerwindow_ExternalOutputs_controlexclusion_T;

/* Real-time Model Data Structure */
struct powerwindow_tag_RTM_controlexclusion_T {
    const powerwindow_char_T * volatile errorStatus;
};

/* External inputs (root inport signals with auto storage) */
extern powerwindow_ExternalInputs_controlexclusion_T powerwindow_controlexclusion_U;

/* External outputs (root outports fed by signals with auto storage) */
extern powerwindow_ExternalOutputs_controlexclusion_T powerwindow_controlexclusion_Y;

/* Model entry point functions */
extern void powerwindow_controlexclusion_initialize(void);
extern void powerwindow_controlexclusion_terminate(void);
extern void powerwindow_controlexclusion_main(void);

/* Real-time Model object */
extern powerwindow_RT_MODEL_controlexclusion_T *const powerwindow_controlexclusion_M;

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Note that this particular code originates from a subsystem build,
 * and has its own system numbers different from the parent model.
 * Refer to the system hierarchy for this subsystem below, and use the
 * MATLAB hilite_system command to trace the generated code back
 * to the parent model.  For example,
 *
 * hilite_system('ControlExclusion/controlexclusion')    - opens subsystem ControlExclusion/controlexclusion
 * hilite_system('ControlExclusion/controlexclusion/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'ControlExclusion'
 * '<S1>'   : 'ControlExclusion/controlexclusion'
 */
#endif                                 /* powerwindow_RTW_HEADER_controlexclusion_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
