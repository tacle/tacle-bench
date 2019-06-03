/*

  This program is part of the TACLeBench benchmark suite.
  Version V 1.x

  Name: powerwindow_controlexclusion.h

  Author: CoSys-Lab, University of Antwerp

  Function: headerfile

  Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_HeaderFiles/powerwindow_controlexclusion.h

  Changes: a brief summary of major functional changes and formatting)

  License: GNU General Public License

*/

#ifndef powerwindow_RTW_HEADER_controlexclusion_h_
#define powerwindow_RTW_HEADER_controlexclusion_h_
#ifndef powerwindow_controlexclusion_COMMON_INCLUDES_
#define powerwindow_controlexclusion_COMMON_INCLUDES_
#include "../powerwindow_HeaderFiles/powerwindow_rtwtypes.h"

#include "../powerwindow_HeaderFiles/powerwindow_rtw_solver.h"
#endif                                 /* powerwindow_controlexclusion_COMMON_INCLUDES_ */

#include "../powerwindow_HeaderFiles/powerwindow_controlexclusion_types.h"

/* Shared type includes */
#include "../powerwindow_HeaderFiles/powerwindow_model_reference_types.h"

/* user code (top of header file) */

/* Model reference registration function */

extern void powerwindow_controlexclusion_initialize( void );
extern void powerwindow_controlexclusion_terminate( void );
extern void powerwindow_controlexclusion_main( const powerwindow_boolean_T
    *rtu_Up_DRV, const powerwindow_boolean_T
    *rtu_Down_DRV, const powerwindow_boolean_T *rtu_Up_PSG,
    const powerwindow_boolean_T *rtu_Down_PSG,
    powerwindow_boolean_T *rty_Up, powerwindow_boolean_T *rty_Down );

/*-
   The generated code includes comments that allow you to trace directly
   back to the appropriate location in the model.  The basic format
   is <system>/block_name, where system is the system number (uniquely
   assigned by Simulink) and block_name is the name of the block.

   Use the MATLAB hilite_system command to trace the generated code back
   to the model.  For example,

   hilite_system('<S3>')    - opens system 3
   hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3

   Here is the system hierarchy for this model

   '<Root>' : 'ControlExclusion'
   '<S1>'   : 'ControlExclusion/Control_Clock_TicToc'
   '<S2>'   : 'ControlExclusion/Control_Clock_TicToc/ControlEx_PSG'
   '<S3>'   : 'ControlExclusion/Control_Clock_TicToc/Tic'
   '<S4>'   : 'powerwindow_controlexclusion_main/Control_Clock_TicToc/Toc'
   '<S5>'   : 'ControlExclusion/Control_Clock_TicToc/Tic/Tic_T'
   '<S6>'   : 'ControlExclusion/Control_Clock_TicToc/Toc/Toc_T'
*/
#endif                                 /* RTW_HEADER_ControlExclusion_h_ */

/*
   File trailer for generated code.

   [ EOF ]
*/
