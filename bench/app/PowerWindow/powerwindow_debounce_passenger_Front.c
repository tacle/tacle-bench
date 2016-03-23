/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: debounce_passenger_Front.c

 Author: unknown

 Function: debounce_passenger_Front calls powerwindow_debounce.c to debounce the push-down button
 	 of the power window for the front passenger.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/debounce_passenger_Front.c

 Changes: a brief summary of major functional changes and formatting)

 License: unknown

*/

#include "powerwindow_HeaderFiles/powerwindow.h"
#include "powerwindow_HeaderFiles/powerwindow_debounce.h"
#include "powerwindow_HeaderFiles/powerwindow_debounce_passenger_Front.h"
#include "powerwindow_HeaderFiles/powerwindow_debounce_passenger_Front_private.h"

/*
  Forward declaration of functions
*/

void powerwindow_debounce_passenger_Front_initialize(void);
void powerwindow_debounce_passenger_Front_terminate(void);
void powerwindow_debounce_passenger_Front_main(void);


/*
  Declaration of global variables
*/

/* Block states (auto storage) */
powerwindow_D_Work_debounce_passenger_Front powerwindow_debounce_passenger_Front_DWork;

/* External inputs (root inport signals with auto storage) */
powerwindow_ExternalInputs_debounce_passenger_Fro powerwindow_debounce_passenger_Front_U;

/* External outputs (root outports fed by signals with auto storage) */
powerwindow_ExternalOutputs_debounce_passenger_Fr powerwindow_debounce_passenger_Front_Y;

/* Real-time model */
powerwindow_RT_MODEL_debounce_passenger_Front powerwindow_debounce_passenger_Front_M_;
powerwindow_RT_MODEL_debounce_passenger_Front *const powerwindow_debounce_passenger_Front_M = &powerwindow_debounce_passenger_Front_M_;


/*
  Initialization- and return-value-related functions
*/

/* Model initialize function */
void powerwindow_debounce_passenger_Front_initialize(void)
{
    /* Registration code */

    /* initialize error status */
	powerwindow_rtmSetErrorStatus(powerwindow_debounce_passenger_Front_M, (NULL));

    /* states (dwork) */
    (void) memset((void *)&powerwindow_debounce_passenger_Front_DWork, 0,
                  sizeof(powerwindow_D_Work_debounce_passenger_Front));

    /* external inputs */
    (void) memset((void *)&powerwindow_debounce_passenger_Front_U, 0,
                  sizeof(powerwindow_ExternalInputs_debounce_passenger_Fro));

    /* external outputs */
    (void) memset((void *)&powerwindow_debounce_passenger_Front_Y, 0,
                  sizeof(powerwindow_ExternalOutputs_debounce_passenger_Fr));

    /* Model Initialize fcn for ModelReference Block: '<S1>/debounce_Down' */
    powerwindow_debounce_initialize(powerwindow_rtmGetErrorStatusPointer(powerwindow_debounce_passenger_Front_M),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtm),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtb),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtdw),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtzce));

    /* Model Initialize fcn for ModelReference Block: '<S1>/debounce_Up' */
    powerwindow_debounce_initialize(powerwindow_rtmGetErrorStatusPointer(powerwindow_debounce_passenger_Front_M),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtm),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtb),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtdw),
                        &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtzce));

    /* Start for ModelReference: '<S1>/debounce_Up' */
    powerwindow_debounce_Start(&(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtdw));

    /* Start for ModelReference: '<S1>/debounce_Down' */
    powerwindow_debounce_Start(&(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtdw));

    /* InitializeConditions for ModelReference: '<S1>/debounce_Up' */
    powerwindow_debounce_Init(&(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtb),
                  &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtdw));

    /* InitializeConditions for ModelReference: '<S1>/debounce_Down' */
    powerwindow_debounce_Init(&(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtb),
                  &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtdw));
}

/* Model terminate function */
void powerwindow_debounce_passenger_Front_terminate(void)
{
    /* (no terminate code required) */
}

/*
 Algorithm core functions
 */

/* Model step function */
void powerwindow_debounce_passenger_Front_main(void)
{

    /* ModelReference: '<S1>/debounce_Up' */
    powerwindow_debounce_main(&powerwindow_debounce_passenger_Front_U.Up, &powerwindow_debounce_passenger_Front_Y.debounced_Up,
             &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtb),
             &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtdw),
             &(powerwindow_debounce_passenger_Front_DWork.debounce_Up_DWORK1.rtzce));

    /* ModelReference: '<S1>/debounce_Down' */
    powerwindow_debounce_main(&powerwindow_debounce_passenger_Front_U.Down, &powerwindow_debounce_passenger_Front_Y.debounced_Down,
             &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtb),
             &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtdw),
             &(powerwindow_debounce_passenger_Front_DWork.debounce_Down_DWORK1.rtzce));

}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
