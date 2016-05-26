/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_controlexclusion

 Author: CoSys-Lab, University of Antwerp

 Function: powerwindow_controlexclusion is one functionality of the power window benchmark.
 	 It takes the input signal from the driver and the passenger to determine the final control signal.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow_controlexclusion.c

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

*/



#include "powerwindow_HeaderFiles/powerwindow_controlexclusion.h"
#include "powerwindow_HeaderFiles/powerwindow_controlexclusion_private.h"

/*
  Forward declaration of functions
*/

void powerwindow_controlexclusion_initialize(void);
void powerwindow_controlexclusion_terminate(void);
void powerwindow_controlexclusion_main(const powerwindow_boolean_T *rtu_Up_DRV, const powerwindow_boolean_T *rtu_Down_DRV,
                      const powerwindow_boolean_T *rtu_Up_PSG, const powerwindow_boolean_T *rtu_Down_PSG,
                      powerwindow_boolean_T *rty_Up, powerwindow_boolean_T *rty_Down);

/* Model initialize function */
void powerwindow_controlexclusion_initialize(void)
{
  /* (no initialization code required) */
}

/* Model terminate function */
void powerwindow_controlexclusion_terminate(void)
{
    /* (no terminate code required) */
}


/*
 Algorithm core functions
 */

/* Output and update for referenced model: 'ControlExclusion' */
void powerwindow_controlexclusion_main(const powerwindow_boolean_T *rtu_Up_DRV, const powerwindow_boolean_T *rtu_Down_DRV,
                      const powerwindow_boolean_T *rtu_Up_PSG, const powerwindow_boolean_T *rtu_Down_PSG,
                      powerwindow_boolean_T *rty_Up, powerwindow_boolean_T *rty_Down)
{
  /* Logic: '<S2>/Logical Operator11' incorporates:
   *  Logic: '<S2>/Logical Operator2'
   *  Logic: '<S2>/Logical Operator3'
   *  Logic: '<S2>/Logical Operator5'
   *  Logic: '<S2>/Logical Operator6'
   *  Logic: '<S2>/Logical Operator7'
   */
  *rty_Up = !(((!*rtu_Up_DRV) && (*rtu_Down_DRV)) || ((*rtu_Down_DRV) &&
    (!*rtu_Up_PSG) && (*rtu_Down_PSG)));

  /* Logic: '<S2>/Logical Operator12' incorporates:
   *  Logic: '<S2>/Logical Operator1'
   *  Logic: '<S2>/Logical Operator10'
   *  Logic: '<S2>/Logical Operator4'
   *  Logic: '<S2>/Logical Operator8'
   *  Logic: '<S2>/Logical Operator9'
   */
  *rty_Down = !(((*rtu_Up_DRV) && (!*rtu_Down_DRV)) || ((*rtu_Up_DRV) &&
    (*rtu_Up_PSG) && (!*rtu_Down_PSG)));
}



/*
 * File trailer for generated code.
 *
 * [EOF]
 */
