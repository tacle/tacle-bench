/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_debounce.h

 Author: University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_debounce.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */

#ifndef powerwindow_RTW_HEADER_debounce_h_
#define powerwindow_RTW_HEADER_debounce_h_
#include <string.h>
#ifndef powerwindow_debounce_COMMON_INCLUDES_
#define powerwindow_debounce_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#endif                                 /* debounce_COMMON_INCLUDES_ */

#include "../powerwindow_HeaderFiles/powerwindow_debounce_types.h"

/* Shared type includes */
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* Block signals for model 'powerwindow_debounce_main' */
typedef struct {
    powerwindow_boolean_T Q;                         /* '<Root>/Chart' */
} powerwindow_rtB_debounce_T;

/* Block states (auto storage) for model 'powerwindow_debounce_main' */
typedef struct {
    powerwindow_int32_T clockTickCounter;            /* '<Root>/period of 10ms' */
    powerwindow_uint8_T is_active_c3_debounce;       /* '<Root>/Chart' */
    powerwindow_uint8_T is_c3_debounce;              /* '<Root>/Chart' */
    powerwindow_uint8_T is_debounce;                 /* '<Root>/Chart' */
    powerwindow_uint8_T temporalCounter_i1;          /* '<Root>/Chart' */
} powerwindow_rtDW_debounce_T;

/* Zero-crossing (trigger) state for model 'powerwindow_debounce_main' */
typedef struct {
    ZCSigState Chart_Trig_ZCE;           /* '<Root>/Chart' */
} powerwindow_rtZCE_debounce_T;

/* Real-time Model Data Structure */
struct powerwindow_tag_RTM_debounce_T {
    const powerwindow_char_T **errorStatus;
};

typedef struct {
    powerwindow_rtB_debounce_T rtb;
    powerwindow_rtDW_debounce_T rtdw;
    powerwindow_RT_MODEL_debounce_T rtm;
    powerwindow_rtZCE_debounce_T rtzce;
} powerwindow_rtMdlrefDWork_debounce_T;

/* Model reference registration function */
extern void powerwindow_debounce_initialize(const powerwindow_char_T **rt_errorStatus,
                                powerwindow_RT_MODEL_debounce_T *const debounce_M, powerwindow_rtB_debounce_T *localB, powerwindow_rtDW_debounce_T
                                *localDW, powerwindow_rtZCE_debounce_T *localZCE);
extern void powerwindow_debounce_Init(powerwindow_rtB_debounce_T *localB, powerwindow_rtDW_debounce_T *localDW);
extern void powerwindow_debounce_Start(powerwindow_rtDW_debounce_T *localDW);
extern void powerwindow_debounce_main(const powerwindow_boolean_T *rtu_Switch, powerwindow_boolean_T
                     *rty_debounced_Switch, powerwindow_rtB_debounce_T *localB,
                     powerwindow_rtDW_debounce_T *localDW, powerwindow_rtZCE_debounce_T *localZCE);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : 'powerwindow_debounce_main'
 * '<S1>'   : 'powerwindow_debounce_main/Chart'
 */
#endif                                 /* RTW_HEADER_debounce_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
