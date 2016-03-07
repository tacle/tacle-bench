/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: controlexclusion

 Author: unknown

 Function: controlexclusion is one functionality of the power window benchmark.
 	 It gives the driver higher priority than the passenger. Passenger can control the power window
 	 only when driver does not have to adjust the window height.

 Source: MRTC
 http://www.mrtc.mdh.se/projects/wcet/wcet_bench/PowerWindow/Debounce/Debounce.c

 Changes: a brief summary of major functional changes and formatting)

 License: unknown

*/

#include "../powerwindow_HeaderFiles/powerwindow_controlexclusion.h"
#include "../powerwindow_HeaderFiles/powerwindow_controlexclusion_private.h"

/*
  Forward declaration of functions
*/

void powerwindow_controlexclusion_initialize(void);
void powerwindow_controlexclusion_terminate(void);
void powerwindow_controlexclusion_main(void);

/*
  Declaration of global variables
*/

/* External inputs (root inport signals with auto storage) */
powerwindow_ExternalInputs_controlexclusion_T powerwindow_controlexclusion_U;

/* External outputs (root outports fed by signals with auto storage) */
powerwindow_ExternalOutputs_controlexclusion_T powerwindow_controlexclusion_Y;

/* Real-time model */
powerwindow_RT_MODEL_controlexclusion_T powerwindow_controlexclusion_M_;
powerwindow_RT_MODEL_controlexclusion_T *const powerwindow_controlexclusion_M = &powerwindow_controlexclusion_M_;

/*
  Initialization- and return-value-related functions
*/

/* Model initialize function */
void powerwindow_controlexclusion_initialize(void)
{

    /* Registration code */

    /* initialize error status */
	powerwindow_rtmSetErrorStatus(powerwindow_controlexclusion_M, (NULL));

    /* external inputs */
    (void) memset((void *)&powerwindow_controlexclusion_U, 0,
                  sizeof(powerwindow_ExternalInputs_controlexclusion_T));

    /* external outputs */
    (void) memset((void *)&powerwindow_controlexclusion_Y, 0,
                  sizeof(powerwindow_ExternalOutputs_controlexclusion_T));


}

/* Model terminate function */
void powerwindow_controlexclusion_terminate(void)
{
    /* (no terminate code required) */
}

/*
 Algorithm core functions
 */

/* Model step function */
void powerwindow_controlexclusion_main(void)
{

    /* Outport: '<Root>/Up' incorporates:
     *  Inport: '<Root>/Down_DRV'
     *  Inport: '<Root>/Down_PSG'
     *  Inport: '<Root>/Up_DRV'
     *  Inport: '<Root>/Up_PSG'
     *  Logic: '<S1>/Logical Operator11'
     *  Logic: '<S1>/Logical Operator2'
     *  Logic: '<S1>/Logical Operator3'
     *  Logic: '<S1>/Logical Operator5'
     *  Logic: '<S1>/Logical Operator6'
     *  Logic: '<S1>/Logical Operator7'
     */
    powerwindow_controlexclusion_Y.Up = !(((!powerwindow_controlexclusion_U.Up_DRV) && powerwindow_controlexclusion_U.Down_DRV)
                              || (powerwindow_controlexclusion_U.Down_DRV && (!powerwindow_controlexclusion_U.Up_PSG) &&
                                  powerwindow_controlexclusion_U.Down_PSG));

    /* Outport: '<Root>/Down' incorporates:
     *  Inport: '<Root>/Down_DRV'
     *  Inport: '<Root>/Down_PSG'
     *  Inport: '<Root>/Up_DRV'
     *  Inport: '<Root>/Up_PSG'
     *  Logic: '<S1>/Logical Operator1'
     *  Logic: '<S1>/Logical Operator10'
     *  Logic: '<S1>/Logical Operator12'
     *  Logic: '<S1>/Logical Operator4'
     *  Logic: '<S1>/Logical Operator8'
     *  Logic: '<S1>/Logical Operator9'
     */
    powerwindow_controlexclusion_Y.Down = !((powerwindow_controlexclusion_U.Up_DRV && (!powerwindow_controlexclusion_U.Down_DRV))
                                || (powerwindow_controlexclusion_U.Up_DRV && powerwindow_controlexclusion_U.Up_PSG &&
                                    (!powerwindow_controlexclusion_U.Down_PSG)));

}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
