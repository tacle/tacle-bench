/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: powerwindow_PW_Control_PSG_BackR.c

  Author: CoSys-Lab, University of Antwerp

  Function: PW_Control_DRV realizes the functionality of back-right passenger side powerwindow. It connects the 3 smaller modules together.

  Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow//powerwindow_PW_Control_PSG_BackR.c

  Changes: a brief summary of major functional changes and formatting)

  License: GNU General Public License

*/


#include "wcclib.h"
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_BackR.h"
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_BackR_private.h"

/*
  Forward declaration of functions
*/

void powerwindow_PW_Control_PSG_BackR_initialize( void );
void powerwindow_PW_Control_PSG_BackR_terminate( void );
void powerwindow_PW_Control_PSG_BackR_main( void );


/* Block states (auto storage) */
powerwindow_D_Work_PW_Control_PSG_BackR powerwindow_PW_Control_PSG_BackR_DWork;

/* External inputs (root inport signals with auto storage) */
powerwindow_ExternalInputs_PW_Control_PSG_BackR
powerwindow_PW_Control_PSG_BackR_U;

/* External outputs (root outports fed by signals with auto storage) */
powerwindow_ExternalOutputs_PW_Control_PSG_BackR
powerwindow_PW_Control_PSG_BackR_Y;

/* Real-time model */
powerwindow_RT_MODEL_PW_Control_PSG_BackR powerwindow_PW_Control_PSG_BackR_M_;
powerwindow_RT_MODEL_PW_Control_PSG_BackR *const
powerwindow_PW_Control_PSG_BackR_M = &powerwindow_PW_Control_PSG_BackR_M_;

/* Model step function */
void powerwindow_PW_Control_PSG_BackR_main( void )
{
  /* local block i/o variables */
  powerwindow_boolean_T rtb_debounce_Up;
  powerwindow_boolean_T rtb_debounce_Down;
  powerwindow_boolean_T powerwindow_rtb_ControlEx_PSG_BackR_o1;
  powerwindow_boolean_T powerwindow_rtb_ControlEx_PSG_BackR_o2;

  /* ModelReference: '<S2>/debounce_Up' */
  powerwindow_debounce_main( &powerwindow_PW_Control_PSG_BackR_U.Up_PSG_BackR,
                             &rtb_debounce_Up,
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtb ),
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtdw ),
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtzce ) );

  /* ModelReference: '<S2>/debounce_Down' */
  powerwindow_debounce_main( &powerwindow_PW_Control_PSG_BackR_U.Down_PSG_BackR,
                             &rtb_debounce_Down,
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtb ),
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtdw ),
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtzce ) );

  /* ModelReference: '<S1>/ControlEx_PSG_BackR' */
  powerwindow_controlexclusion_main( &powerwindow_PW_Control_PSG_BackR_U.Up_DRV,
                                     &powerwindow_PW_Control_PSG_BackR_U.Down_DRV,
                                     &rtb_debounce_Up, &rtb_debounce_Down,
                                     &powerwindow_rtb_ControlEx_PSG_BackR_o1,
                                     &powerwindow_rtb_ControlEx_PSG_BackR_o2 );

  /* ModelReference: '<S1>/PW_PSG_BackR' */
  powerwindow_powerwindow_control_main( &powerwindow_rtb_ControlEx_PSG_BackR_o1,
                                        &powerwindow_rtb_ControlEx_PSG_BackR_o2,
                                        &powerwindow_PW_Control_PSG_BackR_U.endofdetectionrange,
                                        &powerwindow_PW_Control_PSG_BackR_U.currentsense,
                                        &powerwindow_PW_Control_PSG_BackR_Y.window_up,
                                        &powerwindow_PW_Control_PSG_BackR_Y.window_down,
                                        &powerwindow_PW_Control_PSG_BackR_Y.overcurrent,
                                        &powerwindow_PW_Control_PSG_BackR_Y.pinch,
                                        &powerwindow_PW_Control_PSG_BackR_Y.wake,
                                        &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtb ),
                                        &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtdw ),
                                        &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtzce ) );
}

/* Model initialize function */
void powerwindow_PW_Control_PSG_BackR_initialize( void )
{
  /* Registration code */

  /* initialize error status */
  powerwindow_PW_PSG_BackR_rtmSetErrorStatus( powerwindow_PW_Control_PSG_BackR_M,
      ( NULL ) );

  /* states (dwork) */
  ( void ) wcclib_memset( ( void * )&powerwindow_PW_Control_PSG_BackR_DWork, 0,
                   sizeof( powerwindow_D_Work_PW_Control_PSG_BackR ) );

  /* external inputs */
  ( void ) wcclib_memset( ( void * )&powerwindow_PW_Control_PSG_BackR_U, 0,
                   sizeof( powerwindow_ExternalInputs_PW_Control_PSG_BackR ) );

  /* external outputs */
  ( void ) wcclib_memset( ( void * )&powerwindow_PW_Control_PSG_BackR_Y, 0,
                   sizeof( powerwindow_ExternalOutputs_PW_Control_PSG_BackR ) );

  /* Model Initialize fcn for ModelReference Block: '<S1>/ControlEx_PSG_BackR' */
  powerwindow_controlexclusion_initialize();

  /* Model Initialize fcn for ModelReference Block: '<S2>/debounce_Down' */
  powerwindow_debounce_initialize(
    powerwindow_PW_PSG_BackR_rtmGetErrorStatusPointer(
      powerwindow_PW_Control_PSG_BackR_M ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtm ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtb ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtdw ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtzce ) );

  /* Model Initialize fcn for ModelReference Block: '<S2>/debounce_Up' */
  powerwindow_debounce_initialize(
    powerwindow_PW_PSG_BackR_rtmGetErrorStatusPointer(
      powerwindow_PW_Control_PSG_BackR_M ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtm ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtb ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtdw ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtzce ) );

  /* Model Initialize fcn for ModelReference Block: '<S1>/PW_PSG_BackR' */
  powerwindow_powerwindow_control_initialize(
    powerwindow_PW_PSG_BackR_rtmGetErrorStatusPointer(
      powerwindow_PW_Control_PSG_BackR_M ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtm ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtb ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtdw ),
    &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtzce ) );

  /* Start for ModelReference: '<S2>/debounce_Up' */
  powerwindow_debounce_Start( &
                              ( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtdw ) );

  /* Start for ModelReference: '<S2>/debounce_Down' */
  powerwindow_debounce_Start( &
                              ( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtdw ) );

  /* Start for ModelReference: '<S1>/PW_PSG_BackR' */
  powerwindow_powerwindow_control_Start( &
                                         ( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtdw ) );

  /* InitializeConditions for ModelReference: '<S2>/debounce_Up' */
  powerwindow_debounce_Init( &
                             ( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtb ),
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Up_DWORK1.rtdw ) );

  /* InitializeConditions for ModelReference: '<S2>/debounce_Down' */
  powerwindow_debounce_Init( &
                             ( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtb ),
                             &( powerwindow_PW_Control_PSG_BackR_DWork.Debounce_Down_DWORK1.rtdw ) );

  /* InitializeConditions for ModelReference: '<S1>/PW_PSG_BackR' */
  powerwindow_powerwindow_control_Init(
    &powerwindow_PW_Control_PSG_BackR_Y.window_up,
    &powerwindow_PW_Control_PSG_BackR_Y.window_down,
    &powerwindow_PW_Control_PSG_BackR_Y.overcurrent,
    &powerwindow_PW_Control_PSG_BackR_Y.pinch,
    &powerwindow_PW_Control_PSG_BackR_Y.wake,
    &( powerwindow_PW_Control_PSG_BackR_DWork.PW_PSG_BackR_DWORK1.rtdw ) );
}

/* Model terminate function */
void powerwindow_PW_Control_PSG_BackR_terminate( void )
{
  /* (no terminate code required) */
}

/*
   File trailer for generated code.

   [ EOF ]
*/
