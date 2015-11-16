/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_control

 Author: unknown

 Function: powerwindow_control is one functionality of the power window benchmark.
 	 It composes two control modules: logic and sensing. The first one is used to
 	 control the height of the power window. The second one is for pinch sensing.
 	 The two control components can run in parallel.

 Source: MRTC
 http://www.mrtc.mdh.se/projects/wcet/wcet_bench/PowerWindow/Debounce/Debounce.c

 Changes: a brief summary of major functional changes and formatting)

 License: general open-source

*/
#include <stdio.h>

#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control_private.h"
#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control.h"
#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control_private.h"

/* Named constants for Chart: '<S2>/stateflow control model' */
#define powerwindow_powerwindow_control_IN_AutoDown             ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_control_IN_AutoUp               ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_control_IN_Down                 ((powerwindow_uint8_T)2U)
#define powerwindow_powerwindow_control_IN_Down_d               ((powerwindow_uint8_T)1U)
#define powerwindow_control_IN_EndReached           ((powerwindow_uint8_T)1U)
#define powerwindow_control_IN_InitDown             ((powerwindow_uint8_T)3U)
#define powerwindow_control_IN_InitUp               ((powerwindow_uint8_T)2U)
#define powerwindow_control_IN_NO_ACTIVE_CHILD      ((powerwindow_uint8_T)0U)
#define powerwindow_control_IN_Neutral              ((powerwindow_uint8_T)2U)
#define powerwindow_control_IN_Pinch                ((powerwindow_uint8_T)2U)
#define powerwindow_control_IN_SenseEnd             ((powerwindow_uint8_T)1U)
#define powerwindow_control_IN_SensePinch           ((powerwindow_uint8_T)2U)
#define powerwindow_control_IN_Start                ((powerwindow_uint8_T)3U)
#define powerwindow_control_IN_System               ((powerwindow_uint8_T)3U)
#define powerwindow_control_IN_Up                   ((powerwindow_uint8_T)3U)


/*
  Forward declaration of functions
*/

void powerwindow_powerwindow_control_initialize(void);
void powerwindow_powerwindow_control_terminate(void);
void powerwindow_powerwindow_control_main(void);
static void powerwindow_powerwindow_control_broadcast_ticks(void); /* Forward declaration for local functions */

/*
  Declaration of global variables
*/

/* Block signals (auto storage) */
powerwindow_BlockIO_powerwindow_control powerwindow_powerwindow_control_B;

/* Block states (auto storage) */
/* Previous zero-crossings (trigger) states */
powerwindow_PrevZCSigStates_powerwindow_control powerwindow_powerwindow_control_PrevZCSigState;

/* External inputs (root inport signals with auto storage) */
powerwindow_ExternalInputs_powerwindow_control powerwindow_powerwindow_control_U;

/* External outputs (root outports fed by signals with auto storage) */
powerwindow_D_Work_powerwindow_control powerwindow_powerwindow_control_DWork;

powerwindow_ExternalOutputs_powerwindow_control powerwindow_powerwindow_control_Y;

/* Real-time model */
powerwindow_RT_MODEL_powerwindow_control powerwindow_powerwindow_control_M_;
powerwindow_RT_MODEL_powerwindow_control *const powerwindow_powerwindow_control_M = &powerwindow_powerwindow_control_M_;

/*
 Initialization- and return-value-related functions
 */

/* Model initialize function */
void powerwindow_powerwindow_control_initialize(void)
{
    /* Registration code */

    /* initialize error status */
    powerwindow_rtmSetErrorStatus(powerwindow_powerwindow_control_M, (NULL));

    /* block I/O */
    (void) memset(((void *) &powerwindow_powerwindow_control_B), 0,
                  sizeof(powerwindow_BlockIO_powerwindow_control));

    /* states (dwork) */
    (void) memset((void *)&powerwindow_powerwindow_control_DWork, 0,
                  sizeof(powerwindow_D_Work_powerwindow_control));

    /* external inputs */
    (void) memset((void *)&powerwindow_powerwindow_control_U, 0,
                  sizeof(powerwindow_ExternalInputs_powerwindow_control));

    /* external outputs */
    (void) memset((void *)&powerwindow_powerwindow_control_Y, 0,
                  sizeof(powerwindow_ExternalOutputs_powerwindow_control));

    /* Start for DiscretePulseGenerator: '<S2>/period of 50ms' */
    powerwindow_powerwindow_control_DWork.clockTickCounter = 0L;
    powerwindow_powerwindow_control_PrevZCSigState.stateflowcontrolmodel_Trig_ZCE = powerwindow_UNINITIALIZED_ZCSIG;
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[0] = powerwindow_UNINITIALIZED_ZCSIG;
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[1] = powerwindow_UNINITIALIZED_ZCSIG;
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[2] = powerwindow_UNINITIALIZED_ZCSIG;
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[3] = powerwindow_UNINITIALIZED_ZCSIG;
    powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[0] = powerwindow_UNINITIALIZED_ZCSIG;
    powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[1] = powerwindow_UNINITIALIZED_ZCSIG;

    /* InitializeConditions for Chart: '<S2>/stateflow control model' */
    powerwindow_powerwindow_control_DWork.is_active_Logic = 0U;
    powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_NO_ACTIVE_CHILD;
    powerwindow_powerwindow_control_DWork.is_Down = powerwindow_control_IN_NO_ACTIVE_CHILD;
    powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_NO_ACTIVE_CHILD;
    powerwindow_powerwindow_control_DWork.temporalCounter_i1 = 0U;
    powerwindow_powerwindow_control_DWork.is_active_Sensing = 0U;
    powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_NO_ACTIVE_CHILD;
    powerwindow_powerwindow_control_DWork.temporalCounter_i2 = 0U;
    powerwindow_powerwindow_control_DWork.is_active_c2_powerwindow_control = 0U;
    powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control = powerwindow_control_IN_NO_ACTIVE_CHILD;

    /* InitializeConditions for Outport: '<Root>/window_up' incorporates:
     *  InitializeConditions for Chart: '<S2>/stateflow control model'
     */
    powerwindow_powerwindow_control_Y.window_up = false;

    /* InitializeConditions for Outport: '<Root>/window_down' incorporates:
     *  InitializeConditions for Chart: '<S2>/stateflow control model'
     */
    powerwindow_powerwindow_control_Y.window_down = false;

    /* InitializeConditions for Outport: '<Root>/overcurrent' incorporates:
     *  InitializeConditions for Chart: '<S2>/stateflow control model'
     */
    powerwindow_powerwindow_control_Y.overcurrent = false;

    /* InitializeConditions for Outport: '<Root>/pinch' incorporates:
     *  InitializeConditions for Chart: '<S2>/stateflow control model'
     */
    powerwindow_powerwindow_control_Y.pinch = false;

    /* InitializeConditions for Outport: '<Root>/wake' incorporates:
     *  InitializeConditions for Chart: '<S2>/stateflow control model'
     */
    powerwindow_powerwindow_control_Y.wake = false;
}

/* Model terminate function */
void powerwindow_powerwindow_control_terminate(void)
{
    /* (no terminate code required) */
}

/*
 Local broadcast ticks functions
 */

/* Function for Chart: '<S2>/stateflow control model' */
static void powerwindow_powerwindow_control_broadcast_ticks(void)
{
    /* Event: '<S5>:30' */
    /* During: powerwindow_control/powerwindow_control/stateflow control model */
    if (powerwindow_powerwindow_control_DWork.is_active_c2_powerwindow_control == 0U) {
        /* Entry: powerwindow_control/powerwindow_control/stateflow control model */
        powerwindow_powerwindow_control_DWork.is_active_c2_powerwindow_control = 1U;

        /* Entry Internal: powerwindow_control/powerwindow_control/stateflow control model */
        /* Transition: '<S5>:102' */
        powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control = powerwindow_control_IN_System;

        /* Entry Internal 'System': '<S5>:94' */
        powerwindow_powerwindow_control_DWork.is_active_Logic = 1U;

        /* Entry Internal 'Logic': '<S5>:95' */
        /* Transition: '<S5>:82' */
        powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Neutral;

        /* Entry 'Neutral': '<S5>:16' */
        powerwindow_powerwindow_control_Y.window_up = false;
        powerwindow_powerwindow_control_Y.window_down = false;
        powerwindow_powerwindow_control_Y.wake = false;
        powerwindow_powerwindow_control_DWork.is_active_Sensing = 1U;

        /* Entry Internal 'Sensing': '<S5>:96' */
        /* Transition: '<S5>:153' */
        powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_Start;
        powerwindow_powerwindow_control_DWork.temporalCounter_i2 = 0U;

        /* Entry 'Start': '<S5>:170' */
        powerwindow_powerwindow_control_Y.overcurrent = false;
        powerwindow_powerwindow_control_Y.pinch = false;
    } else {
        switch (powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control) {
        case powerwindow_control_IN_EndReached:
            /* During 'EndReached': '<S5>:97' */
            if (powerwindow_powerwindow_control_DWork.temporalCounter_i1 >= 10) {
                /* Transition: '<S5>:101' */
                powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control = powerwindow_control_IN_System;

                /* Entry Internal 'System': '<S5>:94' */
                powerwindow_powerwindow_control_DWork.is_active_Logic = 1U;

                /* Entry Internal 'Logic': '<S5>:95' */
                /* Transition: '<S5>:82' */
                powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Neutral;

                /* Entry 'Neutral': '<S5>:16' */
                powerwindow_powerwindow_control_Y.window_up = false;
                powerwindow_powerwindow_control_Y.window_down = false;
                powerwindow_powerwindow_control_Y.wake = false;
                powerwindow_powerwindow_control_DWork.is_active_Sensing = 1U;

                /* Entry Internal 'Sensing': '<S5>:96' */
                /* Transition: '<S5>:153' */
                powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_Start;
                powerwindow_powerwindow_control_DWork.temporalCounter_i2 = 0U;

                /* Entry 'Start': '<S5>:170' */
                powerwindow_powerwindow_control_Y.overcurrent = false;
                powerwindow_powerwindow_control_Y.pinch = false;
            }
            break;

        case powerwindow_control_IN_Pinch:
            /* During 'Pinch': '<S5>:152' */
            if (powerwindow_powerwindow_control_DWork.temporalCounter_i1 >= 40) {
                /* Transition: '<S5>:157' */
                powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control = powerwindow_control_IN_System;

                /* Entry Internal 'System': '<S5>:94' */
                powerwindow_powerwindow_control_DWork.is_active_Logic = 1U;

                /* Entry Internal 'Logic': '<S5>:95' */
                /* Transition: '<S5>:82' */
                powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Neutral;

                /* Entry 'Neutral': '<S5>:16' */
                powerwindow_powerwindow_control_Y.window_up = false;
                powerwindow_powerwindow_control_Y.window_down = false;
                powerwindow_powerwindow_control_Y.wake = false;
                powerwindow_powerwindow_control_DWork.is_active_Sensing = 1U;

                /* Entry Internal 'Sensing': '<S5>:96' */
                /* Transition: '<S5>:153' */
                powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_Start;
                powerwindow_powerwindow_control_DWork.temporalCounter_i2 = 0U;

                /* Entry 'Start': '<S5>:170' */
                powerwindow_powerwindow_control_Y.overcurrent = false;
                powerwindow_powerwindow_control_Y.pinch = false;
            }
            break;

        default:
            /* During 'System': '<S5>:94' */
            if (powerwindow_powerwindow_control_Y.pinch == 1) {
                /* Transition: '<S5>:155' */
                /* Exit Internal 'System': '<S5>:94' */
                /* Exit Internal 'Sensing': '<S5>:96' */
                powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_NO_ACTIVE_CHILD;
                powerwindow_powerwindow_control_DWork.is_active_Sensing = 0U;

                /* Exit Internal 'Logic': '<S5>:95' */
                /* Exit Internal 'Down': '<S5>:18' */
                powerwindow_powerwindow_control_DWork.is_Down = powerwindow_control_IN_NO_ACTIVE_CHILD;
                powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_NO_ACTIVE_CHILD;

                /* Exit Internal 'Up': '<S5>:17' */
                powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_NO_ACTIVE_CHILD;
                powerwindow_powerwindow_control_DWork.is_active_Logic = 0U;
                powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control = powerwindow_control_IN_Pinch;
                powerwindow_powerwindow_control_DWork.temporalCounter_i1 = 0U;

                /* Entry 'Pinch': '<S5>:152' */
                powerwindow_powerwindow_control_Y.window_up = false;
                powerwindow_powerwindow_control_Y.window_down = true;
            } else if (powerwindow_powerwindow_control_Y.overcurrent == 1) {
                /* Transition: '<S5>:100' */
                /* Exit Internal 'System': '<S5>:94' */
                /* Exit Internal 'Sensing': '<S5>:96' */
                powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_NO_ACTIVE_CHILD;
                powerwindow_powerwindow_control_DWork.is_active_Sensing = 0U;

                /* Exit Internal 'Logic': '<S5>:95' */
                /* Exit Internal 'Down': '<S5>:18' */
                powerwindow_powerwindow_control_DWork.is_Down = powerwindow_control_IN_NO_ACTIVE_CHILD;
                powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_NO_ACTIVE_CHILD;

                /* Exit Internal 'Up': '<S5>:17' */
                powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_NO_ACTIVE_CHILD;
                powerwindow_powerwindow_control_DWork.is_active_Logic = 0U;
                powerwindow_powerwindow_control_DWork.is_c2_powerwindow_control = powerwindow_control_IN_EndReached;
                powerwindow_powerwindow_control_DWork.temporalCounter_i1 = 0U;

                /* Entry 'EndReached': '<S5>:97' */
                powerwindow_powerwindow_control_Y.window_up = false;
                powerwindow_powerwindow_control_Y.window_down = false;
            } else {
                /* During 'Logic': '<S5>:95' */
                switch (powerwindow_powerwindow_control_DWork.is_Logic) {
                case powerwindow_powerwindow_control_IN_Down_d:
                    /* During 'Down': '<S5>:18' */
                    if (powerwindow_powerwindow_control_B.map[1]) {
                        /* Transition: '<S5>:169' */
                        /* Exit Internal 'Down': '<S5>:18' */
                        powerwindow_powerwindow_control_DWork.is_Down = powerwindow_control_IN_NO_ACTIVE_CHILD;
                        powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Up;

                        /* Entry 'Up': '<S5>:17' */
                        powerwindow_powerwindow_control_Y.window_up = true;
                        powerwindow_powerwindow_control_Y.window_down = false;
                        powerwindow_powerwindow_control_Y.wake = true;
                        powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_Up;
                    } else {
                        switch (powerwindow_powerwindow_control_DWork.is_Down) {
                        case powerwindow_powerwindow_control_IN_AutoDown:
                            /* During 'AutoDown': '<S5>:111' */
                            break;

                        case powerwindow_powerwindow_control_IN_Down:
                            /* During 'Down': '<S5>:110' */
                            if (powerwindow_powerwindow_control_B.map[0]) {
                                /* Transition: '<S5>:26' */
                                powerwindow_powerwindow_control_DWork.is_Down = powerwindow_control_IN_NO_ACTIVE_CHILD;
                                powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Neutral;

                                /* Entry 'Neutral': '<S5>:16' */
                                powerwindow_powerwindow_control_Y.window_up = false;
                                powerwindow_powerwindow_control_Y.window_down = false;
                                powerwindow_powerwindow_control_Y.wake = false;
                            }
                            break;

                        default:
                            /* During 'InitDown': '<S5>:109' */
                            if (powerwindow_powerwindow_control_DWork.temporalCounter_i1 >= 20) {
                                /* Transition: '<S5>:119' */
                                if (powerwindow_powerwindow_control_B.map[0]) {
                                    /* Transition: '<S5>:120' */
                                    powerwindow_powerwindow_control_DWork.is_Down = powerwindow_powerwindow_control_IN_AutoDown;
                                } else {
                                    if (powerwindow_powerwindow_control_B.map[2]) {
                                        /* Transition: '<S5>:121' */
                                        powerwindow_powerwindow_control_DWork.is_Down = powerwindow_powerwindow_control_IN_Down;
                                    }
                                }
                            }
                            break;
                        }
                    }
                    break;

                case powerwindow_control_IN_Neutral:
                    /* During 'Neutral': '<S5>:16' */
                    if (powerwindow_powerwindow_control_B.map[1]) {
                        /* Transition: '<S5>:24' */
                        powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Up;

                        /* Entry 'Up': '<S5>:17' */
                        powerwindow_powerwindow_control_Y.window_up = true;
                        powerwindow_powerwindow_control_Y.window_down = false;
                        powerwindow_powerwindow_control_Y.wake = true;
                        powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_InitUp;
                        powerwindow_powerwindow_control_DWork.temporalCounter_i1 = 0U;
                    } else {
                        if (powerwindow_powerwindow_control_B.map[2]) {
                            /* Transition: '<S5>:25' */
                            powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_powerwindow_control_IN_Down_d;

                            /* Entry 'Down': '<S5>:18' */
                            powerwindow_powerwindow_control_Y.window_up = false;
                            powerwindow_powerwindow_control_Y.window_down = true;
                            powerwindow_powerwindow_control_Y.wake = true;
                            powerwindow_powerwindow_control_DWork.is_Down = powerwindow_control_IN_InitDown;
                            powerwindow_powerwindow_control_DWork.temporalCounter_i1 = 0U;
                        }
                    }
                    break;

                default:
                    /* During 'Up': '<S5>:17' */
                    if (powerwindow_powerwindow_control_B.map[2]) {
                        /* Transition: '<S5>:166' */
                        /* Exit Internal 'Up': '<S5>:17' */
                        powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_NO_ACTIVE_CHILD;
                        powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_powerwindow_control_IN_Down_d;

                        /* Entry 'Down': '<S5>:18' */
                        powerwindow_powerwindow_control_Y.window_up = false;
                        powerwindow_powerwindow_control_Y.window_down = true;
                        powerwindow_powerwindow_control_Y.wake = true;
                        powerwindow_powerwindow_control_DWork.is_Down = powerwindow_powerwindow_control_IN_Down;
                    } else {
                        switch (powerwindow_powerwindow_control_DWork.is_Up) {
                        case powerwindow_powerwindow_control_IN_AutoUp:
                            /* During 'AutoUp': '<S5>:108' */
                            break;

                        case powerwindow_control_IN_InitUp:
                            /* During 'InitUp': '<S5>:106' */
                            if (powerwindow_powerwindow_control_DWork.temporalCounter_i1 >= 20) {
                                /* Transition: '<S5>:115' */
                                if (powerwindow_powerwindow_control_B.map[0]) {
                                    /* Transition: '<S5>:118' */
                                    powerwindow_powerwindow_control_DWork.is_Up = powerwindow_powerwindow_control_IN_AutoUp;
                                } else {
                                    if (powerwindow_powerwindow_control_B.map[1]) {
                                        /* Transition: '<S5>:117' */
                                        powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_Up;
                                    }
                                }
                            }
                            break;

                        default:
                            /* During 'Up': '<S5>:107' */
                            if (powerwindow_powerwindow_control_B.map[0]) {
                                /* Transition: '<S5>:23' */
                                powerwindow_powerwindow_control_DWork.is_Up = powerwindow_control_IN_NO_ACTIVE_CHILD;
                                powerwindow_powerwindow_control_DWork.is_Logic = powerwindow_control_IN_Neutral;

                                /* Entry 'Neutral': '<S5>:16' */
                                powerwindow_powerwindow_control_Y.window_up = false;
                                powerwindow_powerwindow_control_Y.window_down = false;
                                powerwindow_powerwindow_control_Y.wake = false;
                            }
                            break;
                        }
                    }
                    break;
                }

                /* During 'Sensing': '<S5>:96' */
                switch (powerwindow_powerwindow_control_DWork.is_Sensing) {
                case powerwindow_control_IN_SenseEnd:
                    /* During 'SenseEnd': '<S5>:147' */
                    if ((powerwindow_powerwindow_control_B.LogicalOperator == 0) && (powerwindow_powerwindow_control_Y.window_up == 1)) {
                        /* Transition: '<S5>:173' */
                        powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_Start;
                        powerwindow_powerwindow_control_DWork.temporalCounter_i2 = 0U;

                        /* Entry 'Start': '<S5>:170' */
                        powerwindow_powerwindow_control_Y.overcurrent = false;
                        powerwindow_powerwindow_control_Y.pinch = false;
                    } else {
                        /* Inport: '<Root>/current sense' */
                        powerwindow_powerwindow_control_Y.overcurrent = (powerwindow_powerwindow_control_U.currentsense > 184);
                    }
                    break;

                case powerwindow_control_IN_SensePinch:
                    /* During 'SensePinch': '<S5>:148' */
                    if ((powerwindow_powerwindow_control_B.LogicalOperator == 1) || (powerwindow_powerwindow_control_Y.window_down == 1)) {
                        /* Transition: '<S5>:150' */
                        powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_SenseEnd;
                    } else {
                        /* Inport: '<Root>/current sense' */
                        powerwindow_powerwindow_control_Y.pinch = (powerwindow_powerwindow_control_U.currentsense > 92);
                    }
                    break;

                default:
                    /* During 'Start': '<S5>:170' */
                    if (powerwindow_powerwindow_control_DWork.temporalCounter_i2 >= 6) {
                        /* Transition: '<S5>:171' */
                        powerwindow_powerwindow_control_DWork.is_Sensing = powerwindow_control_IN_SensePinch;
                    }
                    break;
                }
            }
            break;
        }
    }
}

/*
 Algorithm core functions
 */

/* Model step function */
void powerwindow_powerwindow_control_main(void)
{
    powerwindow_int16_T rtb_periodof50ms;

    /* CombinatorialLogic: '<S2>/map' incorporates:
     *  DataTypeConversion: '<S2>/control input'
     *  Inport: '<Root>/down'
     *  Inport: '<Root>/up'
     */
    rtb_periodof50ms = (powerwindow_int16_T)(((powerwindow_uint16_T)(powerwindow_powerwindow_control_U.up != 0) << 1) +
                                 (powerwindow_powerwindow_control_U.down != 0));
    powerwindow_powerwindow_control_B.map[0U] = powerwindow_powerwindow_control_ConstP.map_table[(powerwindow_uint16_T)rtb_periodof50ms];
    powerwindow_powerwindow_control_B.map[1U] = powerwindow_powerwindow_control_ConstP.map_table[rtb_periodof50ms + 4U];
    powerwindow_powerwindow_control_B.map[2U] = powerwindow_powerwindow_control_ConstP.map_table[rtb_periodof50ms + 8U];

    /* Logic: '<S2>/Logical Operator' incorporates:
     *  DataTypeConversion: '<S2>/Data Type Conversion1'
     *  Inport: '<Root>/end of detection range'
     */
    powerwindow_powerwindow_control_B.LogicalOperator = !(powerwindow_powerwindow_control_U.endofdetectionrange != 0);

    /* DiscretePulseGenerator: '<S2>/period of 50ms' */
    rtb_periodof50ms = (powerwindow_powerwindow_control_DWork.clockTickCounter < 5L) &&
                       (powerwindow_powerwindow_control_DWork.clockTickCounter >= 0L) ? 1 : 0;
    if (powerwindow_powerwindow_control_DWork.clockTickCounter >= 9L) {
        powerwindow_powerwindow_control_DWork.clockTickCounter = 0L;
    } else {
        powerwindow_powerwindow_control_DWork.clockTickCounter++;
    }

    /* End of DiscretePulseGenerator: '<S2>/period of 50ms' */

    /* Chart: '<S2>/stateflow control model' incorporates:
     *  TriggerPort: '<S5>/ticks'
     */
    /* DataTypeConversion: '<S2>/Data Type Conversion' */
    if (((rtb_periodof50ms != 0) !=
            (powerwindow_powerwindow_control_PrevZCSigState.stateflowcontrolmodel_Trig_ZCE == powerwindow_POS_ZCSIG)) &&
            (powerwindow_powerwindow_control_PrevZCSigState.stateflowcontrolmodel_Trig_ZCE !=
             powerwindow_UNINITIALIZED_ZCSIG)) {
        /* Gateway: powerwindow_control/powerwindow_control/stateflow control model */
        if (powerwindow_powerwindow_control_DWork.temporalCounter_i1 < 63U) {
            powerwindow_powerwindow_control_DWork.temporalCounter_i1++;
        }

        if (powerwindow_powerwindow_control_DWork.temporalCounter_i2 < 7U) {
            powerwindow_powerwindow_control_DWork.temporalCounter_i2++;
        }

        powerwindow_powerwindow_control_broadcast_ticks();
    }

    powerwindow_powerwindow_control_PrevZCSigState.stateflowcontrolmodel_Trig_ZCE = (powerwindow_uint8_T)
            (rtb_periodof50ms != 0 ? (powerwindow_int16_T)powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* End of DataTypeConversion: '<S2>/Data Type Conversion' */

    /* Outputs for Triggered SubSystem: '<S1>/Tic' incorporates:
     *  TriggerPort: '<S3>/Trigger'
     */
    /* Inport: '<Root>/up' incorporates:
     *  Inport: '<Root>/current sense'
     *  Inport: '<Root>/down'
     *  Inport: '<Root>/end of detection range'
     */
    /*  if ((((powerwindow_powerwindow_control_U.up > 0) != (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[0] == powerwindow_POS_ZCSIG))
           && (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[0] != powerwindow_UNINITIALIZED_ZCSIG)) ||
          (((powerwindow_powerwindow_control_U.down > 0) != (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[1] ==
             powerwindow_POS_ZCSIG)) && (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[1] !=
                             powerwindow_UNINITIALIZED_ZCSIG)) ||
          (((powerwindow_powerwindow_control_U.endofdetectionrange > 0) !=
            (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[2] == powerwindow_POS_ZCSIG)) &&
           (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[2] != powerwindow_UNINITIALIZED_ZCSIG)) ||
          (((powerwindow_powerwindow_control_U.currentsense > 0) != (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[3] ==
             powerwindow_POS_ZCSIG)) && (powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[3] !=
                             powerwindow_UNINITIALIZED_ZCSIG))) {
    */
    /* MATLAB Function: '<S3>/Tic_T' */
    /* MATLAB Function 'powerwindow_control/Tic/Tic_T': '<S6>:1' */
    /* '<S6>:1:4' */
    /*    Tic();
      }

     */




    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[0] = (powerwindow_uint8_T)(powerwindow_powerwindow_control_U.up > 0 ? (powerwindow_int16_T)
            powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* Inport: '<Root>/down' */
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[1] = (powerwindow_uint8_T)(powerwindow_powerwindow_control_U.down > 0 ? (powerwindow_int16_T)
            powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* Inport: '<Root>/end of detection range' */
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[2] = (powerwindow_uint8_T)(powerwindow_powerwindow_control_U.endofdetectionrange
            > 0 ? (powerwindow_int16_T)powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* Inport: '<Root>/current sense' */
    powerwindow_powerwindow_control_PrevZCSigState.Tic_Trig_ZCE[3] = (powerwindow_uint8_T)(powerwindow_powerwindow_control_U.currentsense > 0 ?
            (powerwindow_int16_T)powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* End of Outputs for SubSystem: '<S1>/Tic' */

    /* Outputs for Triggered SubSystem: '<S1>/Toc' incorporates:
     *  TriggerPort: '<S4>/Trigger'
     */
    /*  if ((((powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[0] == powerwindow_POS_ZCSIG) !=
            powerwindow_powerwindow_control_Y.window_up) && (powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[0] !=
            powerwindow_UNINITIALIZED_ZCSIG)) || (((powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[1] ==
             powerwindow_POS_ZCSIG) != powerwindow_powerwindow_control_Y.window_down) &&
           (powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[1] != powerwindow_UNINITIALIZED_ZCSIG))) {
    */
    /* MATLAB Function: '<S4>/Toc_T' */
    /* MATLAB Function 'powerwindow_control/Toc/Toc_T': '<S7>:1' */
    /* '<S7>:1:4' */
    /*   Toc();
      }
    */
    powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[0] = (powerwindow_uint8_T)(powerwindow_powerwindow_control_Y.window_up ?
            (powerwindow_int16_T)powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);
    powerwindow_powerwindow_control_PrevZCSigState.Toc_Trig_ZCE[1] = (powerwindow_uint8_T)(powerwindow_powerwindow_control_Y.window_down ?
            (powerwindow_int16_T)powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* End of Outputs for SubSystem: '<S1>/Toc' */
}


/*
 * File trailer for generated code.
 *
 * [EOF]
 */
