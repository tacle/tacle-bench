/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_PW_Control_PSG_BackL.c

 Author: CoSys-Lab, University of Antwerp

 Function: PW_Control_DRV realizes the functionality of back-left passenger side powerwindow. It connects the 3 smaller modules together.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow//powerwindow_powerwindow_control.c

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

*/


#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_BackL.h"

#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_BackL_private.h"

/*
 Forward declaration of functions
*/

void powerwindow_PW_Control_PSG_BackL_initialize(void);
void powerwindow_PW_Control_PSG_BackL_terminate(void);
void powerwindow_PW_Control_PSG_BackL_main(void);


/* Block states (auto storage) */
powerwindow_D_Work_PW_Control_PSG_BackL powerwindow_PW_Control_PSG_BackL_DWork;

/* External inputs (root inport signals with auto storage) */
powerwindow_ExternalInputs_PW_Control_PSG_BackL powerwindow_PW_Control_PSG_BackL_U;

/* External outputs (root outports fed by signals with auto storage) */
powerwindow_ExternalOutputs_PW_Control_PSG_BackL powerwindow_PW_Control_PSG_BackL_Y;

/* Real-time model */
powerwindow_RT_MODEL_PW_Control_PSG_BackL powerwindow_PW_Control_PSG_BackL_M_;
powerwindow_RT_MODEL_PW_Control_PSG_BackL *const powerwindow_PW_Control_PSG_BackL_M = &powerwindow_PW_Control_PSG_BackL_M_;

/* Model step function */
void powerwindow_PW_Control_PSG_BackL_main(void)
{
  /* local block i/o variables */
  powerwindow_boolean_T rtb_debounce_Up;
  powerwindow_boolean_T rtb_debounce_Down;
  powerwindow_boolean_T powerwindow_rtb_ControlEx_PSG_BackL_o1;
  powerwindow_boolean_T powerwindow_rtb_ControlEx_PSG_BackL_o2;

  /* ModelReference: '<S2>/debounce_Up' */
  powerwindow_debounce_main(&powerwindow_PW_Control_PSG_BackL_U.Up_PSG_BackL, &rtb_debounce_Up,
           &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtb),
           &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtdw),
           &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtzce));

  /* ModelReference: '<S2>/debounce_Down' */
  powerwindow_debounce_main(&powerwindow_PW_Control_PSG_BackL_U.Down_PSG_BackL, &rtb_debounce_Down,
           &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtb),
           &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtdw),
           &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtzce));

  /* ModelReference: '<S1>/ControlEx_PSG_BackL' */
  powerwindow_controlexclusion_main(&powerwindow_PW_Control_PSG_BackL_U.Up_DRV, &powerwindow_PW_Control_PSG_BackL_U.Down_DRV,
                   &rtb_debounce_Up, &rtb_debounce_Down,
                   &powerwindow_rtb_ControlEx_PSG_BackL_o1, &powerwindow_rtb_ControlEx_PSG_BackL_o2);

  /* ModelReference: '<S1>/PW_PSG_BackL' */
  powerwindow_powerwindow_control_main(&powerwindow_rtb_ControlEx_PSG_BackL_o1, &powerwindow_rtb_ControlEx_PSG_BackL_o2,
                      &powerwindow_PW_Control_PSG_BackL_U.endofdetectionrange,
                      &powerwindow_PW_Control_PSG_BackL_U.currentsense,
                      &powerwindow_PW_Control_PSG_BackL_Y.window_up, &powerwindow_PW_Control_PSG_BackL_Y.window_down,
                      &powerwindow_PW_Control_PSG_BackL_Y.overcurrent, &powerwindow_PW_Control_PSG_BackL_Y.pinch,
                      &powerwindow_PW_Control_PSG_BackL_Y.wake,
                      &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtb),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtdw),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtzce));
}

/* Model initialize function */
void powerwindow_PW_Control_PSG_BackL_initialize(void)
{
  /* Registration code */

  /* initialize error status */
  powerwindow_PW_PSG_BackL_rtmSetErrorStatus(powerwindow_PW_Control_PSG_BackL_M, (NULL));

  /* states (dwork) */
  (void) memset((void *)&powerwindow_PW_Control_PSG_BackL_DWork, 0,
                sizeof(powerwindow_D_Work_PW_Control_PSG_BackL));

  /* external inputs */
  (void) memset((void *)&powerwindow_PW_Control_PSG_BackL_U, 0,
                sizeof(powerwindow_ExternalInputs_PW_Control_PSG_BackL));

  /* external outputs */
  (void) memset((void *)&powerwindow_PW_Control_PSG_BackL_Y, 0,
                sizeof(powerwindow_ExternalOutputs_PW_Control_PSG_BackL));

  /* Model Initialize fcn for ModelReference Block: '<S1>/ControlEx_PSG_BackL' */
  powerwindow_controlexclusion_initialize();

  /* Model Initialize fcn for ModelReference Block: '<S2>/debounce_Down' */
  powerwindow_debounce_initialize(powerwindow_PW_PSG_BackL_rtmGetErrorStatusPointer(powerwindow_PW_Control_PSG_BackL_M),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtm),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtb),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtdw),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/debounce_Up' */
  powerwindow_debounce_initialize(powerwindow_PW_PSG_BackL_rtmGetErrorStatusPointer(powerwindow_PW_Control_PSG_BackL_M),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtm),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtb),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtdw),
                      &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S1>/PW_PSG_BackL' */
  powerwindow_powerwindow_control_initialize(powerwindow_PW_PSG_BackL_rtmGetErrorStatusPointer(powerwindow_PW_Control_PSG_BackL_M),
    &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtm),
    &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtb),
    &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtdw),
    &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtzce));

  /* Start for ModelReference: '<S2>/debounce_Up' */
  powerwindow_debounce_Start(&(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/debounce_Down' */
  powerwindow_debounce_Start(&(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtdw));

  /* Start for ModelReference: '<S1>/PW_PSG_BackL' */
  powerwindow_powerwindow_control_Start(&(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtdw));

  /* InitializeConditions for ModelReference: '<S2>/debounce_Up' */
  powerwindow_debounce_Init(&(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtb),
                &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Up_DWORK1.rtdw));

  /* InitializeConditions for ModelReference: '<S2>/debounce_Down' */
  powerwindow_debounce_Init(&(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtb),
                &(powerwindow_PW_Control_PSG_BackL_DWork.Debounce_Down_DWORK1.rtdw));

  /* InitializeConditions for ModelReference: '<S1>/PW_PSG_BackL' */
  powerwindow_powerwindow_control_Init(&powerwindow_PW_Control_PSG_BackL_Y.window_up,
    &powerwindow_PW_Control_PSG_BackL_Y.window_down, &powerwindow_PW_Control_PSG_BackL_Y.overcurrent,
    &powerwindow_PW_Control_PSG_BackL_Y.pinch, &powerwindow_PW_Control_PSG_BackL_Y.wake,
    &(powerwindow_PW_Control_PSG_BackL_DWork.PW_PSG_BackL_DWORK1.rtdw));
}

/* Model terminate function */
void powerwindow_PW_Control_PSG_BackL_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
