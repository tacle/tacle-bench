/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_debounce.c

 Author: CoSys-Lab, University of Antwerp

 Function: powerwindow_debounce_main is used to powerwindow_debounce_main the push-down button of the power window.
 	 In order to input a manual switch signal into a digital circuit,
 	 debouncing is necessary so that a single press does not appear like multiple presses.
 	 Without debouncing, pressing the button once may cause unpredictable results.
 	 powerwindow_debounce_main.c defines all the functions that will be used in debounce_PSG_Front which is a part of the Power window.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_debounce.c

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

 */


#include "powerwindow_HeaderFiles/powerwindow_debounce.h"
#include "powerwindow_HeaderFiles/powerwindow_debounce_private.h"

/* Named constants for Chart: '<Root>/Chart' */
#define debounce_IN_debounce           ((powerwindow_uint8_T)1U)
#define debounce_IN_NO_ACTIVE_CHILD    ((powerwindow_uint8_T)0U)
#define debounce_IN_Off                ((powerwindow_uint8_T)2U)
#define debounce_IN_Off_h              ((powerwindow_uint8_T)1U)
#define debounce_IN_On                 ((powerwindow_uint8_T)3U)
#define debounce_IN_On_b               ((powerwindow_uint8_T)2U)

/*
 Forward declaration of functions
*/

void powerwindow_debounce_Init(powerwindow_rtB_debounce_T *, powerwindow_rtDW_debounce_T *);
void powerwindow_debounce_Start(powerwindow_rtDW_debounce_T *);
void powerwindow_debounce_initialize(const powerwindow_char_T **, powerwindow_RT_MODEL_debounce_T * const,
                         powerwindow_rtB_debounce_T *, powerwindow_rtDW_debounce_T *, powerwindow_rtZCE_debounce_T *);
void powerwindow_debounce_main(const powerwindow_boolean_T *, powerwindow_boolean_T *, powerwindow_rtB_debounce_T *,
              powerwindow_rtDW_debounce_T *, powerwindow_rtZCE_debounce_T *);

/*
 Initialization- and return-value-related functions
 */

/* Initial conditions for referenced model: 'powerwindow_debounce_main' */
void powerwindow_debounce_Init(powerwindow_rtB_debounce_T *localB, powerwindow_rtDW_debounce_T *localDW)
{
    /* InitializeConditions for Chart: '<Root>/Chart' */
    localDW->is_debounce = debounce_IN_NO_ACTIVE_CHILD;
    localDW->temporalCounter_i1 = 0U;
    localDW->is_active_c3_debounce = 0U;
    localDW->is_c3_debounce = debounce_IN_NO_ACTIVE_CHILD;
    localB->Q = false;
}

/* Start for referenced model: 'powerwindow_debounce_main' */
void powerwindow_debounce_Start(powerwindow_rtDW_debounce_T *localDW)
{
    /* Start for DiscretePulseGenerator: '<Root>/period of 10ms' */
    localDW->clockTickCounter = 0L;
}

/* Model initialize function */
void powerwindow_debounce_initialize(const powerwindow_char_T **rt_errorStatus, powerwindow_RT_MODEL_debounce_T *
                         const debounce_M, powerwindow_rtB_debounce_T *localB, powerwindow_rtDW_debounce_T *localDW,
                         powerwindow_rtZCE_debounce_T *localZCE)
{
    /* Registration code */

    /* initialize error status */
	powerwindow_rtmSetErrorStatusPointer(debounce_M, rt_errorStatus);

    /* block I/O */
    (void) memset(((void *) localB), 0,
                  sizeof(powerwindow_rtB_debounce_T));

    /* states (dwork) */
    (void) memset((void *)localDW, 0,
                  sizeof(powerwindow_rtDW_debounce_T));
    localZCE->Chart_Trig_ZCE = powerwindow_POS_ZCSIG;
}

/*
 Algorithm core functions
 */

/* Output and update for referenced model: 'powerwindow_debounce_main' */
void powerwindow_debounce_main(const powerwindow_boolean_T *rtu_Switch, powerwindow_boolean_T *rty_debounced_Switch,
              powerwindow_rtB_debounce_T *localB, powerwindow_rtDW_debounce_T *localDW, powerwindow_rtZCE_debounce_T
              *localZCE)
{
    powerwindow_int16_T rtb_periodof10ms;

    /* DiscretePulseGenerator: '<Root>/period of 10ms' */
    rtb_periodof10ms = (localDW->clockTickCounter < 1L) &&
                       (localDW->clockTickCounter >= 0L) ? 1 : 0;
    if (localDW->clockTickCounter >= 1L) {
        localDW->clockTickCounter = 0L;
    } else {
        localDW->clockTickCounter++;
    }

    /* End of DiscretePulseGenerator: '<Root>/period of 10ms' */

    /* Chart: '<Root>/Chart' incorporates:
     *  TriggerPort: '<S1>/ticks'
     */
    /* DataTypeConversion: '<Root>/Data Type Conversion' */
    if ((rtb_periodof10ms != 0) && (localZCE->Chart_Trig_ZCE != powerwindow_POS_ZCSIG)) {
        /* Gateway: Chart */
        if (localDW->temporalCounter_i1 < 7U) {
            localDW->temporalCounter_i1++;
        }

        /* Event: '<S1>:13' */
        /* During: Chart */
        if (localDW->is_active_c3_debounce == 0U) {
            /* Entry: Chart */
            localDW->is_active_c3_debounce = 1U;

            /* Entry Internal: Chart */
            /* Transition: '<S1>:9' */
            localDW->is_c3_debounce = debounce_IN_Off;

            /* Entry 'Off': '<S1>:1' */
            localB->Q = true;
        } else {
            switch (localDW->is_c3_debounce) {
            case debounce_IN_debounce:
                /* During 'powerwindow_debounce_main': '<S1>:6' */
                if (localDW->is_debounce == debounce_IN_Off_h) {
                    /* During 'Off': '<S1>:8' */
                    if ((powerwindow_int16_T)*rtu_Switch < 1) {
                        /* Transition: '<S1>:12' */
                        localDW->is_debounce = debounce_IN_On_b;
                        localDW->temporalCounter_i1 = 0U;
                    } else {
                        if (localDW->temporalCounter_i1 >= 3) {
                            /* Transition: '<S1>:16' */
                            localDW->is_debounce = debounce_IN_NO_ACTIVE_CHILD;
                            localDW->is_c3_debounce = debounce_IN_Off;

                            /* Entry 'Off': '<S1>:1' */
                            localB->Q = true;
                        }
                    }
                } else {
                    /* During 'On': '<S1>:7' */
                    if ((powerwindow_int16_T)*rtu_Switch > 0) {
                        /* Transition: '<S1>:11' */
                        localDW->is_debounce = debounce_IN_Off_h;
                        localDW->temporalCounter_i1 = 0U;
                    } else {
                        if (localDW->temporalCounter_i1 >= 3) {
                            /* Transition: '<S1>:14' */
                            localDW->is_debounce = debounce_IN_NO_ACTIVE_CHILD;
                            localDW->is_c3_debounce = debounce_IN_On;

                            /* Entry 'On': '<S1>:5' */
                            localB->Q = false;
                        }
                    }
                }
                break;

            case debounce_IN_Off:
                /* During 'Off': '<S1>:1' */
                if ((powerwindow_int16_T)*rtu_Switch < 1) {
                    /* Transition: '<S1>:10' */
                    localDW->is_c3_debounce = debounce_IN_debounce;
                    localDW->is_debounce = debounce_IN_On_b;
                    localDW->temporalCounter_i1 = 0U;
                }
                break;

            default:
                /* During 'On': '<S1>:5' */
                if ((powerwindow_int16_T)*rtu_Switch > 0) {
                    /* Transition: '<S1>:15' */
                    localDW->is_c3_debounce = debounce_IN_debounce;
                    localDW->is_debounce = debounce_IN_Off_h;
                    localDW->temporalCounter_i1 = 0U;
                }
                break;
            }
        }
    }

    localZCE->Chart_Trig_ZCE = (powerwindow_uint8_T)(rtb_periodof10ms != 0 ? (powerwindow_int16_T)
                                         powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

    /* End of DataTypeConversion: '<Root>/Data Type Conversion' */

    /* DataTypeConversion: '<Root>/Data Type Conversion2' */
    *rty_debounced_Switch = localB->Q;
}


/*
 * File trailer for generated code.
 *
 * [EOF]
 */
