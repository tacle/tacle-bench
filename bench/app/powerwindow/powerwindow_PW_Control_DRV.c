/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_PW_Control_DRV.c

 Author: CoSys-Lab, University of Antwerp

 Function: PW_Control_DRV realizes the functionality of driver side powerwindow. It connects the 3 smaller modules together.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow//powerwindow_powerwindow_control.c

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

*/
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_DRV.h"
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_DRV_private.h"



/*
 Forward declaration of functions
*/

void powerwindow_PW_Control_DRV_initialize(void);
void powerwindow_PW_Control_DRV_terminate(void);
void powerwindow_PW_Control_DRV_main(void);


/* Block states (auto storage) */
powerwindow_D_Work_powerwindow_PW_Control_D powerwindow_PW_Control_DR_DWork;

/* External inputs (root inport signals with auto storage) */
powerwindow_ExternalInputs_powerwindow_PW_C powerwindow_PW_Control_DRV_U;

/* External outputs (root outports fed by signals with auto storage) */
powerwindow_ExternalOutputs_powerwindow_PW_ powerwindow_PW_Control_DRV_Y;

/* Real-time model */
powerwindow_RT_MODEL_PW_Control_DRV powerwindow_PW_Control_DRV_M_;
powerwindow_RT_MODEL_PW_Control_DRV *const powerwindow_PW_Control_DRV_M = &powerwindow_PW_Control_DRV_M_;



/* Model step function */
void powerwindow_PW_Control_DRV_main(void)
{
  /* local block i/o variables */
  powerwindow_boolean_T rtb_Debounce_Up_DRV;
  powerwindow_boolean_T rtb_Debounce_Down_DRV;

  /* ModelReference: '<S2>/Debounce_Up_DRV' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In2, &rtb_Debounce_Up_DRV,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Down_DRV' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In4, &rtb_Debounce_Down_DRV,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Up_PSG_BackL' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In9,
           &powerwindow_PW_Control_DRV_Y.Out10,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Down_PSG_BackL' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In10,
           &powerwindow_PW_Control_DRV_Y.Out11,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Up_PSG_Front' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In5, &powerwindow_PW_Control_DRV_Y.Out6,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Down_PSG_Front' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In6, &powerwindow_PW_Control_DRV_Y.Out7,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Up_PSG_BackR' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In7, &powerwindow_PW_Control_DRV_Y.Out8,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtzce));

  /* ModelReference: '<S2>/Debounce_Down_PSG_BackR' */
  powerwindow_debounce_main(&powerwindow_PW_Control_DRV_U.In8, &powerwindow_PW_Control_DRV_Y.Out9,
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtb),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtdw),
           &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtzce));

  /* ModelReference: '<S1>/PW_DRV' */
  powerwindow_powerwindow_control_main(&rtb_Debounce_Up_DRV, &rtb_Debounce_Down_DRV,
                      &powerwindow_PW_Control_DRV_U.In1,
                      &powerwindow_PW_Control_DRV_U.In3,
                      &powerwindow_PW_Control_DRV_Y.Out1,
                      &powerwindow_PW_Control_DRV_Y.Out2,
                      &powerwindow_PW_Control_DRV_Y.Out3,
                      &powerwindow_PW_Control_DRV_Y.Out4,
                      &powerwindow_PW_Control_DRV_Y.Out5,
                      &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtb),
					  &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtdw),
					  &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtzce));
}


/* Model initialize function */
void powerwindow_PW_Control_DRV_initialize(void)
{
  /* Registration code */

  /* states (dwork) */
  (void) memset((void *)&powerwindow_PW_Control_DR_DWork, 0,
                sizeof(powerwindow_D_Work_powerwindow_PW_Control_D));

  /* external inputs */
  (void) memset((void *)&powerwindow_PW_Control_DRV_U, 0,
                sizeof(powerwindow_ExternalInputs_powerwindow_PW_C));

  /* external outputs */
  (void) memset((void *)&powerwindow_PW_Control_DRV_Y, 0,
                sizeof(powerwindow_ExternalOutputs_powerwindow_PW_));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Down_DRV' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtm),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Down_PSG_BackL' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Down_PSG_BackR' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Down_PSG_Front' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Up_DRV' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Up_PSG_BackL' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Up_PSG_BackR' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S2>/Debounce_Up_PSG_Front' */
  powerwindow_debounce_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtzce));

  /* Model Initialize fcn for ModelReference Block: '<S1>/PW_DRV' */
  powerwindow_powerwindow_control_initialize(powerwindow_PW_DRV_rtmGetErrorStatusPointer(powerwindow_PW_Control_DRV_M),
     &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtm),
	 &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtdw),
     &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtzce));

  /* Start for ModelReference: '<S2>/Debounce_Up_DRV' */
  powerwindow_debounce_Start(&(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Down_DRV' */
  powerwindow_debounce_Start(&(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Up_PSG_BackL' */
  powerwindow_debounce_Start
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Down_PSG_BackL' */
  powerwindow_debounce_Start
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Up_PSG_Front' */
  powerwindow_debounce_Start
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Down_PSG_Front' */
  powerwindow_debounce_Start
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Up_PSG_BackR' */
  powerwindow_debounce_Start
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtdw));

  /* Start for ModelReference: '<S2>/Debounce_Down_PSG_BackR' */
  powerwindow_debounce_Start
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtdw));

  /* Start for ModelReference: '<S1>/PW_DRV' */
  powerwindow_powerwindow_control_Start(&(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Up_DRV' */
  powerwindow_debounce_Init(&(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtb),
                &(powerwindow_PW_Control_DR_DWork.Debounce_Up_DRV_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Down_DRV' */
  powerwindow_debounce_Init(&(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtb),
                &(powerwindow_PW_Control_DR_DWork.Debounce_Down_DRV_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Up_PSG_BackL' */
  powerwindow_debounce_Init
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackL_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Down_PSG_BackL' */
  powerwindow_debounce_Init
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackL_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Up_PSG_Front' */
  powerwindow_debounce_Init
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_Front_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Down_PSG_Front' */
  powerwindow_debounce_Init
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_Front_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Up_PSG_BackR' */
  powerwindow_debounce_Init
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Up_PSG_BackR_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S2>/Debounce_Down_PSG_BackR' */
  powerwindow_debounce_Init
    (&(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtb),
     &(powerwindow_PW_Control_DR_DWork.Debounce_Down_PSG_BackR_DWORK1.rtdw));

  /* SystemInitialize for ModelReference: '<S1>/PW_DRV' */
  powerwindow_powerwindow_control_Init(&powerwindow_PW_Control_DRV_Y.Out1,
    &powerwindow_PW_Control_DRV_Y.Out2, &powerwindow_PW_Control_DRV_Y.Out3,
    &powerwindow_PW_Control_DRV_Y.Out4, &powerwindow_PW_Control_DRV_Y.Out5,
    &(powerwindow_PW_Control_DR_DWork.PW_DRV_DWORK1.rtdw));
}

/* Model terminate function */
void powerwindow_PW_Control_DRV_terminate(void)
{
  /* (no terminate code required) */
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
