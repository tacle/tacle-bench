/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_powerwindow_control.h

 Author: CoSys-Lab, University of Antwerp

 Function: headerfile

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_powerwindow_control.h

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */

#ifndef powerwindow_RTW_HEADER_PowerWindow_control_h_
#define powerwindow_RTW_HEADER_PowerWindow_control_h_
#ifndef powerwindow_powerWindow_control_COMMON_INCLUDES_
# define powerwindow_powerWindow_control_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"

#include "../powerwindow_HeaderFiles/powerwindow_rtw_solver.h"
#endif                                 /* PowerWindow_control_COMMON_INCLUDES_ */

#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control_types.h"
#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control_private.h"

/* Shared type includes */
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* user code (top of header file) */

/* Block signals for model 'PowerWindow_control' */
typedef struct {
	powerwindow_uint8_T RateTransition1; /* '<S2>/Rate Transition1' */
	powerwindow_boolean_T LogicalOperator; /* '<S2>/Logical Operator' */
	powerwindow_boolean_T map[3]; /* '<S2>/map' */
} powerwindow_rtB_PowerWindow_control;

/* Block states (auto storage) for model 'PowerWindow_control' */
typedef struct {
	powerwindow_int32_T clockTickCounter; /* '<S2>/period of 50ms' */
	powerwindow_uint8_T is_active_c2_PowerWindow_contro;/* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_c2_PowerWindow_control; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_Up; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_Down; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_Logic; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_active_Logic; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_Sensing; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T is_active_Sensing; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T temporalCounter_i1; /* '<S2>/stateflow control model' */
	powerwindow_uint8_T temporalCounter_i2; /* '<S2>/stateflow control model' */
} powerwindow_rtDW_PowerWindow_control;

/* Zero-crossing (trigger) state for model 'PowerWindow_control' */
typedef struct {
	ZCSigState stateflowcontrolmodel_Trig_ZCE;/* '<S2>/stateflow control model' */
} powerwindow_rtZCE_PowerWindow_control;

/* Real-time Model Data Structure */
struct powerwindow_tag_RTM_PowerWindow_control{
	const powerwindow_char_T **errorStatus;
};

typedef struct {
	powerwindow_rtB_PowerWindow_control rtb;
	powerwindow_rtDW_PowerWindow_control rtdw;
	powerwindow_RT_MODEL_PowerWindow_control rtm;
	powerwindow_rtZCE_PowerWindow_control rtzce;
} powerwindow_rtMdlrefDWork_PowerWindow_contr;



/* Model reference registration function */
extern void powerwindow_powerwindow_control_initialize(
		const powerwindow_char_T **rt_errorStatus,
		powerwindow_RT_MODEL_PowerWindow_control * const PowerWindow_control_M,
		powerwindow_rtB_PowerWindow_control *localB,
		powerwindow_rtDW_PowerWindow_control *localDW,
		powerwindow_rtZCE_PowerWindow_control *localZCE);
extern void powerwindow_powerwindow_control_Init(
		powerwindow_boolean_T *rty_window_up,
		powerwindow_boolean_T *rty_window_down,
		powerwindow_boolean_T *rty_overcurrent,
		powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T * rty_wake,
		powerwindow_rtDW_PowerWindow_control *localDW);
extern void powerwindow_powerwindow_control_Start(
		powerwindow_rtDW_PowerWindow_control *localDW);
extern void powerwindow_powerwindow_control_main(
		const powerwindow_boolean_T *rtu_up,
		const powerwindow_boolean_T *rtu_down,
		const powerwindow_boolean_T *rtu_endofdetectionrange,
		const powerwindow_uint8_T *rtu_currentsense,
		powerwindow_boolean_T *rty_window_up,
		powerwindow_boolean_T *rty_window_down,
		powerwindow_boolean_T *rty_overcurrent,
		powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T *rty_wake,
		powerwindow_rtB_PowerWindow_control *localB,
		powerwindow_rtDW_PowerWindow_control *localDW,
		powerwindow_rtZCE_PowerWindow_control *localZCE);

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
 * '<Root>' : 'PowerWindow_control'
 * '<S1>'   : 'PowerWindow_control/PW_PSG'
 * '<S2>'   : 'PowerWindow_control/PW_PSG/PW_PSG'
 * '<S3>'   : 'PowerWindow_control/PW_PSG/Tic'
 * '<S4>'   : 'PowerWindow_control/PW_PSG/Toc'
 * '<S5>'   : 'PowerWindow_control/PW_PSG/PW_PSG/stateflow control model'
 * '<S6>'   : 'PowerWindow_control/PW_PSG/Tic/Tic_T'
 * '<S7>'   : 'PowerWindow_control/PW_PSG/Toc/Toc_T'
 */
#endif                                 /* powerwindow_RTW_HEADER_PowerWindow_control_h_ */

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
