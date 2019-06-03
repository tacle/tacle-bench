/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: powerwindow_PW_Control_DRV.h

  Author: CoSys-Lab, University of Antwerp

  Function: headerfile

  Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_PW_Control_DRV.h

  Changes: a brief summary of major functional changes and formatting)

  License: GNU General Public License

*/

#ifndef powerwindow_RTW_HEADER_powerwindow_PW_Control_DRV_h_
#define powerwindow_RTW_HEADER_powerwindow_PW_Control_DRV_h_
#ifndef powerwindow_PW_Control_DRV_COMMON_INCLUDES_
# define powerwindow_PW_Control_DRV_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#endif                                 /* powerwindow_PW_Control_DRV_COMMON_INCLUDES_ */

#include "../powerwindow_HeaderFiles/powerwindow_PW_Control_DRV_types.h"

/* Shared type includes */
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* Child system includes */
#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control.h"
#include "../powerwindow_HeaderFiles/powerwindow_debounce.h"

/* Macros for accessing real-time model data structure */
#ifndef powerwindow_PW_DRV_rtmGetErrorStatus
# define powerwindow_PW_DRV_rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef powerwindow_PW_DRV_rtmSetErrorStatus
# define powerwindow_PW_DRV_rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

#ifndef powerwindow_PW_DRV_rtmGetErrorStatusPointer
# define powerwindow_PW_DRV_rtmGetErrorStatusPointer(rtm) ((const powerwindow_char_T **)(&((rtm)->errorStatus)))
#endif

/* Block states (auto storage) for system '<Root>' */
typedef struct {
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Up_DRV_DWORK1;/* '<S2>/Debounce_Up_DRV' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Down_DRV_DWORK1;/* '<S2>/Debounce_Down_DRV' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Up_PSG_BackL_DWORK1;/* '<S2>/Debounce_Up_PSG_BackL' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Down_PSG_BackL_DWORK1;/* '<S2>/Debounce_Down_PSG_BackL' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Up_PSG_Front_DWORK1;/* '<S2>/Debounce_Up_PSG_Front' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Down_PSG_Front_DWORK1;/* '<S2>/Debounce_Down_PSG_Front' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Up_PSG_BackR_DWORK1;/* '<S2>/Debounce_Up_PSG_BackR' */
  powerwindow_rtMdlrefDWork_debounce_T
  Debounce_Down_PSG_BackR_DWORK1;/* '<S2>/Debounce_Down_PSG_BackR' */
  powerwindow_rtMdlrefDWork_PowerWindow_contr PW_DRV_DWORK1;/* '<S1>/PW_DRV' */
} powerwindow_D_Work_powerwindow_PW_Control_D;

/* External inputs (root inport signals with auto storage) */
typedef struct {
  powerwindow_boolean_T In1;                       /* '<Root>/In1' */
  powerwindow_boolean_T In2;                       /* '<Root>/In2' */
  powerwindow_uint8_T   In3;                         /* '<Root>/In3' */
  powerwindow_boolean_T In4;                       /* '<Root>/In4' */
  powerwindow_boolean_T In5;                       /* '<Root>/In5' */
  powerwindow_boolean_T In6;                       /* '<Root>/In6' */
  powerwindow_boolean_T In7;                       /* '<Root>/In7' */
  powerwindow_boolean_T In8;                       /* '<Root>/In8' */
  powerwindow_boolean_T In9;                       /* '<Root>/In9' */
  powerwindow_boolean_T In10;                      /* '<Root>/In10' */
} powerwindow_ExternalInputs_powerwindow_PW_C;

/* External outputs (root outports fed by signals with auto storage) */
typedef struct {
  powerwindow_boolean_T Out1;                      /* '<Root>/Out1' */
  powerwindow_boolean_T Out2;                      /* '<Root>/Out2' */
  powerwindow_boolean_T Out3;                      /* '<Root>/Out3' */
  powerwindow_boolean_T Out4;                      /* '<Root>/Out4' */
  powerwindow_boolean_T Out5;                      /* '<Root>/Out5' */
  powerwindow_boolean_T Out6;                      /* '<Root>/Out6' */
  powerwindow_boolean_T Out7;                      /* '<Root>/Out7' */
  powerwindow_boolean_T Out8;                      /* '<Root>/Out8' */
  powerwindow_boolean_T Out9;                      /* '<Root>/Out9' */
  powerwindow_boolean_T Out10;                     /* '<Root>/Out10' */
  powerwindow_boolean_T Out11;                     /* '<Root>/Out11' */
} powerwindow_ExternalOutputs_powerwindow_PW_;

struct powerwindow_tag_RTM_PW_Control_DRV {
  const powerwindow_char_T *volatile errorStatus;
};

/* Block states (auto storage) */
extern powerwindow_D_Work_powerwindow_PW_Control_D
powerwindow_PW_Control_DR_DWork;

/* External inputs (root inport signals with auto storage) */
extern powerwindow_ExternalInputs_powerwindow_PW_C powerwindow_PW_Control_DRV_U;

/* External outputs (root outports fed by signals with auto storage) */
extern powerwindow_ExternalOutputs_powerwindow_PW_ powerwindow_PW_Control_DRV_Y;

/* Model entry point functions */
extern void powerwindow_PW_Control_DRV_initialize( void );
extern void powerwindow_PW_Control_DRV_main( void );
extern void powerwindow_PW_Control_DRV_terminate( void );

/* Real-time Model object */
extern powerwindow_RT_MODEL_PW_Control_DRV *const powerwindow_PW_Control_DRV_M;

/*-
   The generated code includes comments that allow you to trace directly
   back to the appropriate location in the model.  The basic format
   is <system>/block_name, where system is the system number (uniquely
   assigned by Simulink) and block_name is the name of the block.

   Note that this particular code originates from a subsystem build,
   and has its own system numbers different from the parent model.
   Refer to the system hierarchy for this subsystem below, and use the
   MATLAB hilite_system command to trace the generated code back
   to the parent model.  For example,

   hilite_system('PowerWindow/powerwindow_PW_Control_DRV')    - opens subsystem PowerWindow/powerwindow_PW_Control_DRV
   hilite_system('PowerWindow/powerwindow_PW_Control_DRV/Kp') - opens and selects block Kp

   Here is the system hierarchy for this model

   '<Root>' : 'PowerWindow'
   '<S1>'   : 'PowerWindow/powerwindow_PW_Control_DRV'
   '<S2>'   : 'PowerWindow/powerwindow_PW_Control_DRV/Debounce_DRV'
*/
#endif                                 /* RTW_HEADER_powerwindow_PW_Control_DRV_h_ */

/*
   File trailer for generated code.

   [ EOF ]
*/
