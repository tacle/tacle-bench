/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_powerwindow_control.h

 Author: unknown

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_powerwindow_control.h

 Changes: a brief summary of major functional changes and formatting)

 License: unknown

 */

#ifndef powerwindow_RTW_HEADER_powerwindow_control_h_
#define powerwindow_RTW_HEADER_powerwindow_control_h_
#include <stddef.h>
#include <string.h>
#ifndef powerwindow_control_COMMON_INCLUDES_
# define powerwindow_control_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#include "../powerwindow_HeaderFiles/powerwindow_rtw_continuous.h"
#include "../powerwindow_HeaderFiles/powerwindow_rtw_solver.h"
#endif                                 /* powerwindow_control_COMMON_INCLUDES_ */

#include "powerwindow_powerwindow_control_types.h"

/* Macros for accessing real-time model data structure */
#ifndef powerwindow_rtmGetErrorStatus
# define powerwindow_rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef powerwindow_rtmSetErrorStatus
# define powerwindow_rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

/* user code (top of header file) */

/* Block signals (auto storage) */
typedef struct {
    powerwindow_boolean_T map[3];                    /* '<S2>/map' */
    powerwindow_boolean_T LogicalOperator;           /* '<S2>/Logical Operator' */
} powerwindow_BlockIO_powerwindow_control;

/* Block states (auto storage) for system '<Root>' */
typedef struct {
    powerwindow_int32_T clockTickCounter;            /* '<S2>/period of 50ms' */
    powerwindow_uint8_T is_active_c2_powerwindow_control;         /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_c2_powerwindow_control;                /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_Up;                       /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_Down;                     /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_Logic;                    /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_active_Logic;             /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_Sensing;                  /* '<S2>/stateflow control model' */
    powerwindow_uint8_T is_active_Sensing;           /* '<S2>/stateflow control model' */
    powerwindow_uint8_T temporalCounter_i1;          /* '<S2>/stateflow control model' */
    powerwindow_uint8_T temporalCounter_i2;          /* '<S2>/stateflow control model' */
} powerwindow_D_Work_powerwindow_control;

/* Zero-crossing (trigger) state */
typedef struct {
    ZCSigState Toc_Trig_ZCE[2];          /* '<S1>/Toc' */
    ZCSigState Tic_Trig_ZCE[4];          /* '<S1>/Tic' */
    ZCSigState stateflowcontrolmodel_Trig_ZCE;/* '<S2>/stateflow control model' */
} powerwindow_PrevZCSigStates_powerwindow_control;

/* Constant parameters (auto storage) */
typedef struct {
    /* Computed Parameter: map_table
     * Referenced by: '<S2>/map'
     */
    powerwindow_boolean_T map_table[12];
} powerwindow_ConstParam_powerwindow_control;

/* External inputs (root inport signals with auto storage) */
typedef struct {
    powerwindow_uint8_T up;                          /* '<Root>/up' */
    powerwindow_uint8_T down;                        /* '<Root>/down' */
    powerwindow_uint8_T endofdetectionrange;         /* '<Root>/end of detection range' */
    powerwindow_uint8_T currentsense;                /* '<Root>/current sense' */
} powerwindow_ExternalInputs_powerwindow_control;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
    powerwindow_boolean_T window_up;                 /* '<Root>/window_up' */
    powerwindow_boolean_T window_down;               /* '<Root>/window_down' */
    powerwindow_boolean_T overcurrent;               /* '<Root>/overcurrent' */
    powerwindow_boolean_T pinch;                     /* '<Root>/pinch' */
    powerwindow_boolean_T wake;                      /* '<Root>/wake' */
} powerwindow_ExternalOutputs_powerwindow_control;

/* Real-time Model Data Structure */
struct powerwindow_tag_RTM_powerwindow_control {
    const powerwindow_char_T * volatile errorStatus;
};

/* Block signals (auto storage) */
extern powerwindow_BlockIO_powerwindow_control powerwindow_powerwindow_control_B;

/* Block states (auto storage) */
extern powerwindow_D_Work_powerwindow_control powerwindow_powerwindow_control_DWork;

/* External inputs (root inport signals with auto storage) */
extern powerwindow_ExternalInputs_powerwindow_control powerwindow_powerwindow_control_U;

/* External outputs (root outports fed by signals with auto storage) */
extern powerwindow_ExternalOutputs_powerwindow_control powerwindow_powerwindow_control_Y;

/* Constant parameters (auto storage) */
extern const powerwindow_ConstParam_powerwindow_control powerwindow_powerwindow_control_ConstP;

/* Model entry point functions */
extern void powerwindow_powerwindow_control_initialize(void);
extern void powerwindow_powerwindow_control_main(void);
extern void powerwindow_powerwindow_control_terminate(void);

/* Real-time Model object */
extern powerwindow_RT_MODEL_powerwindow_control *const powerwindow_powerwindow_control_M;

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
 * hilite_system('PowerWindow_control/powerwindow_control')    - opens subsystem PowerWindow_control/powerwindow_control
 * hilite_system('PowerWindow_control/powerwindow_control/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'PowerWindow_control'
 * '<S1>'   : 'PowerWindow_control/powerwindow_control'
 * '<S2>'   : 'PowerWindow_control/powerwindow_control/powerwindow_control'
 * '<S3>'   : 'PowerWindow_control/powerwindow_control/Tic'
 * '<S4>'   : 'PowerWindow_control/powerwindow_control/Toc'
 * '<S5>'   : 'PowerWindow_control/powerwindow_control/powerwindow_control/stateflow control model'
 * '<S6>'   : 'PowerWindow_control/powerwindow_control/Tic/Tic_T'
 * '<S7>'   : 'PowerWindow_control/powerwindow_control/Toc/Toc_T'
 */
#endif                                 /* powerwindow_RTW_HEADER_powerwindow_control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
