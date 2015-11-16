/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_control_data

 Author: unknown

 Function: powerwindow_control_data is used as the look up table for the control
 	 module in powerwindow_control

 Source: MRTC
 http://www.mrtc.mdh.se/projects/wcet/wcet_bench/PowerWindow/SourceFiles/debounce.c

 Changes: a brief summary of major functional changes and formatting)

 License: general open-source

 */


#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control.h"
#include "../powerwindow_HeaderFiles/powerwindow_powerwindow_control_private.h"

/* Constant parameters (auto storage) */
const powerwindow_ConstParam_powerwindow_control powerwindow_powerwindow_control_ConstP = {
    /* Computed Parameter: map_table
     * Referenced by: '<S1>/map'
     */
    { 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0 }
};

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
