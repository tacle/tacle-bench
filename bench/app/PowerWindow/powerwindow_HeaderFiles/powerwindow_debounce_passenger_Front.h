/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_debounce_passenger_Front.h

 Author: University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_debounce_passenger_Front.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */

#ifndef powerwindow_RTW_HEADER_debounce_passenger_Front_h_
#define powerwindow_RTW_HEADER_debounce_passenger_Front_h_
#include <stddef.h>
#include <string.h>
#ifndef powerwindow_debounce_passenger_Front_COMMON_INCLUDES_
#define powerwindow_debounce_passenger_Front_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#endif                                 /* debounce_passenger_Front_COMMON_INCLUDES_ */

#include "../powerwindow_HeaderFiles/powerwindow_debounce_passenger_Front_types.h"

/* Shared type includes */
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* Child system includes */
#include "../powerwindow_HeaderFiles/powerwindow_debounce.h"

/* Macros for accessing real-time model data structure */
#ifndef powerwindow_rtmGetErrorStatus
# define powerwindow_rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef powerwindow_rtmSetErrorStatus
# define powerwindow_rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef powerwindow_rtmGetErrorStatusPointer
# define powerwindow_rtmGetErrorStatusPointer(rtm) ((const powerwindow_char_T **)(&((rtm)->errorStatus)))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
    powerwindow_rtMdlrefDWork_debounce_T debounce_Up_DWORK1;/* '<S1>/debounce_Up' */
    powerwindow_rtMdlrefDWork_debounce_T debounce_Down_DWORK1;/* '<S1>/debounce_Down' */
} powerwindow_D_Work_debounce_passenger_Front;

/* External inputs (root inport signals with auto storage) */
typedef struct {
    powerwindow_boolean_T Up;                        /* '<Root>/Up' */
    powerwindow_boolean_T Down;                      /* '<Root>/Down' */
} powerwindow_ExternalInputs_debounce_passenger_Fro;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
    powerwindow_boolean_T debounced_Up;              /* '<Root>/debounced_Up' */
    powerwindow_boolean_T debounced_Down;            /* '<Root>/debounced_Down' */
} powerwindow_ExternalOutputs_debounce_passenger_Fr;

/* Real-time Model Data Structure */
struct powerwindow_tag_RTM_debounce_passenger_Front {
    const powerwindow_char_T * volatile errorStatus;
};

/* Block states (auto storage) */
extern powerwindow_D_Work_debounce_passenger_Front powerwindow_debounce_passenger_Front_DWork;

/* External inputs (root inport signals with auto storage) */
extern powerwindow_ExternalInputs_debounce_passenger_Fro powerwindow_debounce_passenger_Front_U;

/* External outputs (root outports fed by signals with auto storage) */
extern powerwindow_ExternalOutputs_debounce_passenger_Fr powerwindow_debounce_passenger_Front_Y;

/* Model entry point functions */
extern void powerwindow_debounce_passenger_Front_initialize(void);
extern void powerwindow_debounce_passenger_Front_main(void);
extern void powerwindow_debounce_passenger_Front_terminate(void);

/* Real-time Model object */
extern powerwindow_RT_MODEL_debounce_passenger_Front *const powerwindow_debounce_passenger_Front_M;

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
 * hilite_system('PowerWindow/debounce_passenger_Front')    - opens subsystem PowerWindow/debounce_passenger_Front
 * hilite_system('PowerWindow/debounce_passenger_Front/Kp') - opens and selects block Kp
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'PowerWindow'
 * '<S1>'   : 'PowerWindow/debounce_passenger_Front'
 */
#endif                                 /* RTW_HEADER_debounce_passenger_Front_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
