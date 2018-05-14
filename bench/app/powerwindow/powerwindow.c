/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow

 Author: CoSys-Lab, University of Antwerp

 Function: powerwindow implement the powerwindow that can be seen in cars nowadays. 
	The window can be controlled by either driver or passenger. When an object is
	detected between the window frame and the glass during the raising of the glass,
	the glass will lower down for some distance. This benchmark contains 4 tasks which includes the
	driver side powerwindow, front passenger side powerwindow, back-left passenger side powerwindow,
	back-right passenger side powerwindow. These 4 tasks can be easily adjusted to execute in
	sequential order parallel on single or muti core.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow/powerwindow.c

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

*/

#include "powerwindow_HeaderFiles/powerwindow.h" 
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_Front.h"
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_BackL.h"
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_PSG_BackR.h"
#include "powerwindow_HeaderFiles/powerwindow_PW_Control_DRV.h"
#include "powerwindow_HeaderFiles/powerwindow_debounce.h"
#include "powerwindow_HeaderFiles/powerwindow_controlexclusion.h"       /* Control Model's header file */
#include "powerwindow_HeaderFiles/powerwindow_model_reference_types.h"
#include "powerwindow_HeaderFiles/powerwindow_powerwindow_control.h" 	/* PW passenger control Model's header file */
#include "powerwindow_HeaderFiles/powerwindow_rtwtypes.h"
#include "powerwindow_HeaderFiles/powerwindow_model_reference_types.h"
//#include <stdio.h>
/*
  Forward declaration of functions
*/


void powerwindow_Booleaninputarray_initialize(powerwindow_boolean_T*, powerwindow_boolean_T*);
void powerwindow_Uint8inputarray_initialize(powerwindow_uint8_T*, powerwindow_uint8_T*);
void powerwindow_init();
void powerwindow_main();
int powerwindow_return();
int main(void);




//DRV
void powerwindow_init_DRV(int);
void powerwindow_input_initialize_DRV(void);
void powerwindow_initialize_DRV(void);
void powerwindow_return_DRV(void);
void powerwindow_DRV_main(void);

// PSG_Front
void powerwindow_init_PSG_Front(int);
void powerwindow_input_initialize_PSG_Front(void);
void powerwindow_initialize_PSG_Front(void);
void powerwindow_return_PSG_Front(void);
void powerwindow_PSG_Front_main(void);

// PSG_BackL
void powerwindow_init_PSG_BackL(int);
void powerwindow_input_initialize_PSG_BackL(void);
void powerwindow_initialize_PSG_BackL(void);
void powerwindow_return_PSG_BackL(void);
void powerwindow_PSG_BackL_main(void);

// PSG_BackR
void powerwindow_init_PSG_BackR(int);
void powerwindow_input_initialize_PSG_BackR(void);
void powerwindow_initialize_PSG_BackR(void);
void powerwindow_return_PSG_BackR(void);
void powerwindow_PSG_BackR_main(void);



/*
  Declaration of global variables
*/

/* External inputs (root inport signals with auto storage) */

extern powerwindow_ExternalInputs_powerwindow_PW_C powerwindow_PW_Control_DRV_U;
extern powerwindow_ExternalOutputs_powerwindow_PW_ powerwindow_PW_Control_DRV_Y;
extern powerwindow_ExternalInputs_PW_Control_PSG_Front powerwindow_PW_Control_PSG_Front_U;
extern powerwindow_ExternalInputs_PW_Control_PSG_BackL powerwindow_PW_Control_PSG_BackL_U;
extern powerwindow_ExternalInputs_PW_Control_PSG_BackR powerwindow_PW_Control_PSG_BackR_U;



powerwindow_boolean_T powerwindow_debounce_Driver_DRV_U_Up_Input_DRV[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_DRV_U_Down_Input_DRV[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_Front_U_Up_Input_Front[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_Front_U_Down_Input_Front[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_BackL_U_Up_Input_BackL[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_BackL_U_Down_Input_BackL[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_BackR_U_Up_Input_BackR[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_Driver_BackR_U_Down_Input_BackR[powerwindow_input_length];
powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_Input_DRV[powerwindow_input_length];
powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_Input_DRV[powerwindow_input_length];


extern powerwindow_boolean_T powerwindow_debounce_Driver_DRV_U_Up_Input_DRV_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_DRV_U_Down_Input_DRV_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_Front_U_Up_Input_Front_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_Front_U_Down_Input_Front_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_BackL_U_Up_Input_BackL_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_BackL_U_Down_Input_BackL_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_BackR_U_Up_Input_BackR_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_debounce_Driver_BackR_U_Down_Input_BackR_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_DRV_Array[powerwindow_input_length];
extern powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_DRV_Array[powerwindow_input_length];

powerwindow_boolean_T powerwindow_controlexclusion_U_Up_DRV_Input_Front[powerwindow_input_length]; 	/* Here applied a push-down button, the signal is high when the button is not pressed. */
powerwindow_boolean_T powerwindow_controlexclusion_U_Down_DRV_Input_Front[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_passenger_Front_U_Up_Input_Front[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_passenger_Front_U_Down_Input_Front[powerwindow_input_length];
powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_Input_Front[powerwindow_input_length];
powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_Input_Front[powerwindow_input_length];

extern powerwindow_boolean_T powerwindow_debounce_passenger_Front_U_Up_Front_Array[powerwindow_input_length]; 	/* Here applied a push-down button, the signal is high when the button is not pressed. */
extern powerwindow_boolean_T powerwindow_debounce_passenger_Front_U_Down_Front_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_Front_Array[powerwindow_input_length];
extern powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_Front_Array[powerwindow_input_length];

powerwindow_boolean_T powerwindow_controlexclusion_U_Up_DRV_Input_BackL[powerwindow_input_length]; 	/* Here applied a push-down button, the signal is high when the button is not pressed. */
powerwindow_boolean_T powerwindow_controlexclusion_U_Down_DRV_Input_BackL[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_passenger_BackL_U_Up_Input_BackL[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_passenger_BackL_U_Down_Input_BackL[powerwindow_input_length];
powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_Input_BackL[powerwindow_input_length];
powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_Input_BackL[powerwindow_input_length];

extern powerwindow_boolean_T powerwindow_debounce_passenger_BackL_U_Up_BackL_Array[powerwindow_input_length]; 	/* Here applied a push-down button, the signal is high when the button is not pressed. */
extern powerwindow_boolean_T powerwindow_debounce_passenger_BackL_U_Down_BackL_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_BackL_Array[powerwindow_input_length];
extern powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_BackL_Array[powerwindow_input_length];

powerwindow_boolean_T powerwindow_controlexclusion_U_Up_DRV_Input_BackR[powerwindow_input_length]; 	/* Here applied a push-down button, the signal is high when the button is not pressed. */
powerwindow_boolean_T powerwindow_controlexclusion_U_Down_DRV_Input_BackR[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_passenger_BackR_U_Up_Input_BackR[powerwindow_input_length];
powerwindow_boolean_T powerwindow_debounce_passenger_BackR_U_Down_Input_BackR[powerwindow_input_length];
powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_Input_BackR[powerwindow_input_length];
powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_Input_BackR[powerwindow_input_length];

extern powerwindow_boolean_T powerwindow_debounce_passenger_BackR_U_Up_BackR_Array[powerwindow_input_length]; 	/* Here applied a push-down button, the signal is high when the button is not pressed. */
extern powerwindow_boolean_T powerwindow_debounce_passenger_BackR_U_Down_BackR_Array[powerwindow_input_length];
extern powerwindow_boolean_T powerwindow_powerwindow_control_U_endofdetectionrange_BackR_Array[powerwindow_input_length];
extern powerwindow_uint8_T powerwindow_powerwindow_control_U_currentsense_BackR_Array[powerwindow_input_length];

int powerwindow_main_inputcyclecounter;

/*
  Initialization- and return-value-related functions
*/
void powerwindow_init_DRV(int i)
{
	powerwindow_PW_Control_DRV_U.In1  = powerwindow_powerwindow_control_U_endofdetectionrange_Input_DRV[i];			/* The when the window reaches the end of the range, the endofdetectionrange changes to 0. */
	powerwindow_PW_Control_DRV_U.In3  = powerwindow_powerwindow_control_U_currentsense_Input_DRV[i];		/* When the currentsense is higher than 92 (based on experiments), one object is stuck between the window and the frame. Pinch is set to True.*/

	powerwindow_PW_Control_DRV_U.In2  = powerwindow_debounce_Driver_DRV_U_Up_Input_DRV[i];				/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
	powerwindow_PW_Control_DRV_U.In4  = powerwindow_debounce_Driver_DRV_U_Down_Input_DRV[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */

	powerwindow_PW_Control_DRV_U.In5  = powerwindow_debounce_Driver_Front_U_Up_Input_Front[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
	powerwindow_PW_Control_DRV_U.In6  = powerwindow_debounce_Driver_Front_U_Down_Input_Front[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */

	powerwindow_PW_Control_DRV_U.In9  = powerwindow_debounce_Driver_BackL_U_Up_Input_BackL[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
	powerwindow_PW_Control_DRV_U.In10 = powerwindow_debounce_Driver_BackL_U_Down_Input_BackL[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */

	powerwindow_PW_Control_DRV_U.In7  = powerwindow_debounce_Driver_BackR_U_Up_Input_BackR[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
	powerwindow_PW_Control_DRV_U.In8 = powerwindow_debounce_Driver_BackR_U_Down_Input_BackR[i];			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */

}

void powerwindow_init_PSG_Front(int i)
{


	powerwindow_PW_Control_PSG_Front_U.Up_DRV  = powerwindow_PW_Control_DRV_Y.Out6;			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
    powerwindow_PW_Control_PSG_Front_U.Down_DRV = powerwindow_PW_Control_DRV_Y.Out7;		/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lower the window. */

	powerwindow_PW_Control_PSG_Front_U.Up_PSG_Front	= powerwindow_debounce_passenger_Front_U_Up_Input_Front[i];
    powerwindow_PW_Control_PSG_Front_U.Down_PSG_Front = powerwindow_debounce_passenger_Front_U_Down_Input_Front[i];	/* '<Root>/Down'. Here applied a push-down button, the signal is high when the button is not pressed. Change to 0 to lower the window. */

    powerwindow_PW_Control_PSG_Front_U.endofdetectionrange = powerwindow_powerwindow_control_U_endofdetectionrange_Input_Front[i];		/* The when the window reaches the end of the range, the endofdetectionrange changes to 0. */

    powerwindow_PW_Control_PSG_Front_U.currentsense = powerwindow_powerwindow_control_U_currentsense_Input_Front[i];		/* When the currentsense is higher than 92 (based on experiments), one object is stuck between the window and the frame. Pinch is set to True.*/

}

void powerwindow_init_PSG_BackL(int i)
{


	powerwindow_PW_Control_PSG_BackL_U.Up_DRV  = powerwindow_PW_Control_DRV_Y.Out10;			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
    powerwindow_PW_Control_PSG_BackL_U.Down_DRV = powerwindow_PW_Control_DRV_Y.Out11;		/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lower the window. */

	powerwindow_PW_Control_PSG_BackL_U.Up_PSG_BackL	= powerwindow_debounce_passenger_BackL_U_Up_Input_BackL[i];
    powerwindow_PW_Control_PSG_BackL_U.Down_PSG_BackL = powerwindow_debounce_passenger_BackL_U_Down_Input_BackL[i];	/* '<Root>/Down'. Here applied a push-down button, the signal is high when the button is not pressed. Change to 0 to lower the window. */

    powerwindow_PW_Control_PSG_BackL_U.endofdetectionrange = powerwindow_powerwindow_control_U_endofdetectionrange_Input_BackL[i];		/* The when the window reaches the end of the range, the endofdetectionrange changes to 0. */

    powerwindow_PW_Control_PSG_BackL_U.currentsense = powerwindow_powerwindow_control_U_currentsense_Input_BackL[i];		/* When the currentsense is higher than 92 (based on experiments), one object is stuck between the window and the frame. Pinch is set to True.*/

}

void powerwindow_init_PSG_BackR(int i)
{


	powerwindow_PW_Control_PSG_BackR_U.Up_DRV  = powerwindow_PW_Control_DRV_Y.Out8;			/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lift the window. */
    powerwindow_PW_Control_PSG_BackR_U.Down_DRV = powerwindow_PW_Control_DRV_Y.Out9;		/* The debounced control signal from the driver. 1 when the button is not pressed, change to 0 to lower the window. */

	powerwindow_PW_Control_PSG_BackR_U.Up_PSG_BackR	= powerwindow_debounce_passenger_BackR_U_Up_Input_BackR[i];
    powerwindow_PW_Control_PSG_BackR_U.Down_PSG_BackR = powerwindow_debounce_passenger_BackR_U_Down_Input_BackR[i];	/* '<Root>/Down'. Here applied a push-down button, the signal is high when the button is not pressed. Change to 0 to lower the window. */

    powerwindow_PW_Control_PSG_BackR_U.endofdetectionrange = powerwindow_powerwindow_control_U_endofdetectionrange_Input_BackR[i];		/* The when the window reaches the end of the range, the endofdetectionrange changes to 0. */

    powerwindow_PW_Control_PSG_BackR_U.currentsense = powerwindow_powerwindow_control_U_currentsense_Input_BackR[i];		/* When the currentsense is higher than 92 (based on experiments), one object is stuck between the window and the frame. Pinch is set to True.*/

}

void powerwindow_input_initialize_DRV(void)
{

	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_DRV_U_Up_Input_DRV,powerwindow_debounce_Driver_DRV_U_Up_Input_DRV_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_DRV_U_Down_Input_DRV,powerwindow_debounce_Driver_DRV_U_Down_Input_DRV_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_Front_U_Up_Input_Front,powerwindow_debounce_Driver_Front_U_Up_Input_Front_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_Front_U_Down_Input_Front,powerwindow_debounce_Driver_Front_U_Down_Input_Front_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_BackL_U_Up_Input_BackL,powerwindow_debounce_Driver_BackL_U_Up_Input_BackL_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_BackL_U_Down_Input_BackL,powerwindow_debounce_Driver_BackL_U_Down_Input_BackL_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_BackL_U_Down_Input_BackL,powerwindow_debounce_Driver_BackR_U_Up_Input_BackR_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_Driver_BackR_U_Down_Input_BackR,powerwindow_debounce_Driver_BackR_U_Down_Input_BackR_Array);
	powerwindow_Booleaninputarray_initialize(powerwindow_powerwindow_control_U_endofdetectionrange_Input_DRV,powerwindow_powerwindow_control_U_endofdetectionrange_DRV_Array);
	powerwindow_Uint8inputarray_initialize(powerwindow_powerwindow_control_U_currentsense_DRV_Array,powerwindow_powerwindow_control_U_currentsense_DRV_Array);

}



void powerwindow_input_initialize_PSG_Front(void)
{

	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_passenger_Front_U_Up_Input_Front, powerwindow_debounce_passenger_Front_U_Up_Front_Array);

	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_passenger_Front_U_Down_Input_Front,powerwindow_debounce_passenger_Front_U_Down_Front_Array);

	powerwindow_Booleaninputarray_initialize(powerwindow_powerwindow_control_U_endofdetectionrange_Input_Front, powerwindow_powerwindow_control_U_endofdetectionrange_Front_Array);

	powerwindow_Uint8inputarray_initialize(powerwindow_powerwindow_control_U_currentsense_Input_Front,powerwindow_powerwindow_control_U_currentsense_Front_Array);

}


void powerwindow_input_initialize_PSG_BackL(void)
{

	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_passenger_BackL_U_Up_Input_BackL, powerwindow_debounce_passenger_BackL_U_Up_BackL_Array);

	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_passenger_BackL_U_Down_Input_BackL,powerwindow_debounce_passenger_BackL_U_Down_BackL_Array);

	powerwindow_Booleaninputarray_initialize(powerwindow_powerwindow_control_U_endofdetectionrange_Input_BackL, powerwindow_powerwindow_control_U_endofdetectionrange_BackL_Array);

	powerwindow_Uint8inputarray_initialize(powerwindow_powerwindow_control_U_currentsense_Input_BackL,powerwindow_powerwindow_control_U_currentsense_BackL_Array);

}

void powerwindow_input_initialize_PSG_BackR(void)
{
	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_passenger_BackR_U_Up_Input_BackR, powerwindow_debounce_passenger_BackR_U_Up_BackR_Array);

	powerwindow_Booleaninputarray_initialize(powerwindow_debounce_passenger_BackR_U_Down_Input_BackR,powerwindow_debounce_passenger_BackR_U_Down_BackR_Array);

	powerwindow_Booleaninputarray_initialize(powerwindow_powerwindow_control_U_endofdetectionrange_Input_BackR, powerwindow_powerwindow_control_U_endofdetectionrange_BackR_Array);

	powerwindow_Uint8inputarray_initialize(powerwindow_powerwindow_control_U_currentsense_Input_BackR,powerwindow_powerwindow_control_U_currentsense_BackR_Array);

}

void powerwindow_Booleaninputarray_initialize(powerwindow_boolean_T* arrayA, powerwindow_boolean_T* arrayB)
{

    register int i;
//    _Pragma( "loopbound min powerwindow_input_length max powerwindow_input_length" )
    for ( i = 0; i < powerwindow_input_length; i++ )
    	arrayA[i] = arrayB[i];
}

void powerwindow_Uint8inputarray_initialize(powerwindow_uint8_T* arrayA, powerwindow_uint8_T* arrayB)
{

    register int i;
//    _Pragma( "loopbound min powerwindow_input_length max powerwindow_input_length" )
    for ( i = 0; i < powerwindow_input_length; i++ )
    	arrayA[i] = arrayB[i];
}

void powerwindow_initialize_DRV(void)
{
    /* Initialize model */
	powerwindow_PW_Control_DRV_initialize();

}

void powerwindow_initialize_PSG_Front(void)
{
    /* Initialize model */
	powerwindow_PW_Control_PSG_Front_initialize();

}

void powerwindow_initialize_PSG_BackL(void)
{
    /* Initialize model */
	powerwindow_PW_Control_PSG_BackL_initialize();

}

void powerwindow_initialize_PSG_BackR(void)
{
    /* Initialize model */
	powerwindow_PW_Control_PSG_BackR_initialize();

}


void powerwindow_return_DRV(void)
{
    /* Terminate model */
	powerwindow_PW_Control_DRV_terminate();

}

void powerwindow_return_PSG_Front(void)
{
    /* Terminate model */
	powerwindow_PW_Control_PSG_Front_terminate();

}

void powerwindow_return_PSG_BackL(void)
{
    /* Terminate model */
	powerwindow_PW_Control_PSG_BackL_terminate();

}

void powerwindow_return_PSG_BackR(void)
{
    /* Terminate model */
	powerwindow_PW_Control_PSG_BackR_terminate();

}

/*
  Main functions
*/

/*
 * Associating powerwindow_main with a real-time clock or interrupt service routine
 * is what makes the generated code "real-time".  The function powerwindow_main is
 * always associated with the base rate of the model.  Subrates are managed
 * by the base rate from inside the generated code.  Enabling/disabling
 * interrupts and floating point context switches are target specific.  This
 * example code indicates where these should take place relative to executing
 * the generated code step function.  Overrun behavior should be tailored to
 * your application needs.  This example simply sets an error status in the
 * real-time model and returns from powerwindow_main.
 */


void powerwindow_DRV_main(void)
{

    static powerwindow_boolean_T OverrunFlag = 0;

    /* Disable interrupts here */

    /* Check for overrun */
    if (OverrunFlag) {
    	powerwindow_PW_DRV_rtmSetErrorStatus(powerwindow_PW_Control_DRV_M, "Overrun"); //////////

        return;
    }


    OverrunFlag = true;

    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */


    powerwindow_PW_Control_DRV_main();

    /* Get model outputs here */


    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */

}

/*
 * The example "main" function illustrates what is required by your
 * application code to initialize, execute, and terminate the generated code.
 * Attaching powerwindow_main to a real-time clock is target specific.  This example
 * illustates how you do this relative to initializing the model.
 */

void powerwindow_PSG_Front_main(void)
{

    static powerwindow_boolean_T OverrunFlag = 0;

    /* Disable interrupts here */

    /* Check for overrun */
    if (OverrunFlag) {
    	powerwindow_PW_PSG_Front_rtmSetErrorStatus(powerwindow_PW_Control_PSG_Front_M, "Overrun");

        return;
    }


    OverrunFlag = true;

    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */


    powerwindow_PW_Control_PSG_Front_main();

    /* Get model outputs here */


    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */

}

void powerwindow_PSG_BackL_main(void)
{

    static powerwindow_boolean_T OverrunFlag = 0;

    /* Disable interrupts here */

    /* Check for overrun */
    if (OverrunFlag) {
    	powerwindow_PW_PSG_BackL_rtmSetErrorStatus(powerwindow_PW_Control_PSG_BackL_M, "Overrun");

        return;
    }


    OverrunFlag = true;

    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */


    powerwindow_PW_Control_PSG_BackL_main();

    /* Get model outputs here */


    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */

}

void powerwindow_PSG_BackR_main(void)
{

    static powerwindow_boolean_T OverrunFlag = 0;

    /* Disable interrupts here */

    /* Check for overrun */
    if (OverrunFlag) {
    	powerwindow_PW_PSG_BackR_rtmSetErrorStatus(powerwindow_PW_Control_PSG_BackR_M, "Overrun");

        return;
    }


    OverrunFlag = true;

    /* Save FPU context here (if necessary) */
    /* Re-enable timer or interrupt here */
    /* Set model inputs here */


    powerwindow_PW_Control_PSG_BackR_main();

    /* Get model outputs here */


    /* Indicate task complete */
    OverrunFlag = false;

    /* Disable interrupts here */
    /* Restore FPU context here (if necessary) */
    /* Enable interrupts here */

}

void powerwindow_init(void)
{
	powerwindow_initialize_DRV();
	powerwindow_initialize_PSG_Front();
	powerwindow_initialize_PSG_BackL();
	powerwindow_initialize_PSG_BackR();
	powerwindow_main_inputcyclecounter=0;

}

void _Pragma( "entrypoint" ) powerwindow_main(void)
{
    /* Attach powerwindow_main to a timer or interrupt service routine with
     * period 0.005 seconds (the model's base sample time) here.  The
     * call syntax for powerwindow_main is
     *
     *  powerwindow_main();
     */
    //Task 1: Driver side window


    powerwindow_input_initialize_DRV();
    powerwindow_input_initialize_PSG_Front();
    powerwindow_input_initialize_PSG_BackL();
    powerwindow_input_initialize_PSG_BackR();

    while(powerwindow_main_inputcyclecounter<powerwindow_input_length)
    {

        powerwindow_init_DRV(powerwindow_main_inputcyclecounter);
        powerwindow_DRV_main();


    //Task 2: Front passenger side window


        powerwindow_init_PSG_Front(powerwindow_main_inputcyclecounter);
        powerwindow_PSG_Front_main();


    //Task 3: Back left passenger side window


        powerwindow_init_PSG_BackL(powerwindow_main_inputcyclecounter);
        powerwindow_PSG_BackL_main();


    //Task 4: Back right passenger side window


        powerwindow_init_PSG_BackR(powerwindow_main_inputcyclecounter);
        powerwindow_PSG_BackR_main();


        powerwindow_main_inputcyclecounter++;
    }


}

int powerwindow_return(void)
{
	powerwindow_return_DRV();
	powerwindow_return_PSG_Front();
	powerwindow_return_PSG_BackL();
	powerwindow_return_PSG_BackR();

	return 0;
}


int main(void){
 powerwindow_init();
 powerwindow_main();
 return powerwindow_return();
}


/*
 * File trailer for generated code.
 *
 * [EOF]
 */



