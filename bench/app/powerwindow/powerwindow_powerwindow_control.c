/*

 This program is part of the TACLeBench benchmark suite.
 Version V 1.x

 Name: powerwindow_powerwindow_control.c

 Author: CoSys-Lab, University of Antwerp

 Function: powerwindow_control is the main functionality of the power window benchmark.
 	 It contains 3 states: System, EndReached and Pinch, which are used to controll the
 	 position of the glass, if the window is fully closed and sensing pinch force to realize
 	 the powerwindow function.

 Source: https://github.com/tacle/tacle-bench/blob/master/bench/app/PowerWindow//powerwindow_powerwindow_control.c

 Changes: a brief summary of major functional changes and formatting)

 License: GNU General Public License

*/

#include "powerwindow_HeaderFiles/powerwindow_powerwindow_control.h"
#include "powerwindow_HeaderFiles/powerwindow_powerwindow_control_private.h"

/* Named constants for Chart: '<S2>/stateflow control model' */
#define powerwindow_powerwindow__IN_NO_ACTIVE_CHILD ((powerwindow_uint8_T)0U)
#define powerwindow_powerwindow_contr_IN_EndReached ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_contr_IN_SensePinch ((powerwindow_uint8_T)2U)
#define powerwindow_powerwindow_control_IN_AutoDown ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_control_IN_AutoUp  ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_control_IN_Down    ((powerwindow_uint8_T)2U)
#define powerwindow_powerwindow_control_IN_Down_d  ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_control_IN_InitDown ((powerwindow_uint8_T)3U)
#define powerwindow_powerwindow_control_IN_InitUp  ((powerwindow_uint8_T)2U)
#define powerwindow_powerwindow_control_IN_Neutral ((powerwindow_uint8_T)2U)
#define powerwindow_powerwindow_control_IN_Pinch   ((powerwindow_uint8_T)2U)
#define powerwindow_powerwindow_control_IN_SenseEnd ((powerwindow_uint8_T)1U)
#define powerwindow_powerwindow_control_IN_Start   ((powerwindow_uint8_T)3U)
#define powerwindow_powerwindow_control_IN_System  ((powerwindow_uint8_T)3U)
#define powerwindow_powerwindow_control_IN_Up      ((powerwindow_uint8_T)3U)

/* Forward declaration for local functions */
void powerwindow_powerwindow_control_Start(powerwindow_rtDW_PowerWindow_control *localDW);

void powerwindow_powerwindow_control_Init(powerwindow_boolean_T *rty_window_up, powerwindow_boolean_T
  *rty_window_down, powerwindow_boolean_T *rty_overcurrent, powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T *
  rty_wake, powerwindow_rtDW_PowerWindow_control *localDW);

void powerwindow_powerwindow_control_Start(powerwindow_rtDW_PowerWindow_control *localDW);

void powerwindow_powerwindow_control_initialize(const powerwindow_char_T **rt_errorStatus,
  powerwindow_RT_MODEL_PowerWindow_control *const PowerWindow_control_M,
  powerwindow_rtB_PowerWindow_control *localB, powerwindow_rtDW_PowerWindow_control *localDW,
  powerwindow_rtZCE_PowerWindow_control *localZCE);

void powerwindow_powerwindow_control_main(const powerwindow_boolean_T *rtu_up, const powerwindow_boolean_T *rtu_down,
  const powerwindow_boolean_T *rtu_endofdetectionrange, const powerwindow_uint8_T *rtu_currentsense,
  powerwindow_boolean_T *rty_window_up, powerwindow_boolean_T *rty_window_down, powerwindow_boolean_T
  *rty_overcurrent, powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T *rty_wake,
  powerwindow_rtB_PowerWindow_control *localB, powerwindow_rtDW_PowerWindow_control *localDW,
  powerwindow_rtZCE_PowerWindow_control *localZCE);

void powerwindow_powerwindow_con_broadcast_ticks(powerwindow_boolean_T *rty_window_up, powerwindow_boolean_T *
  rty_window_down, powerwindow_boolean_T *rty_overcurrent, powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T
  *rty_wake, powerwindow_rtB_PowerWindow_control *localB, powerwindow_rtDW_PowerWindow_control *localDW);

/* Function for Chart: '<S2>/stateflow control model' */
void powerwindow_powerwindow_con_broadcast_ticks(powerwindow_boolean_T *rty_window_up, powerwindow_boolean_T *
  rty_window_down, powerwindow_boolean_T *rty_overcurrent, powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T
  *rty_wake, powerwindow_rtB_PowerWindow_control *localB, powerwindow_rtDW_PowerWindow_control *localDW)
{
  /* Event: '<S3>:30' */
  /* During: PW_PSG/PWExternalClock/stateflow control model */
  if (localDW->is_active_c2_PowerWindow_contro == 0U) {
    /* Entry: PW_PSG/PWExternalClock/stateflow control model */
    localDW->is_active_c2_PowerWindow_contro = 1U;

    /* Entry Internal: PW_PSG/PWExternalClock/stateflow control model */
    /* Transition: '<S3>:102' */
    localDW->is_c2_PowerWindow_control = powerwindow_powerwindow_control_IN_System;
 
    /* Entry Internal 'System': '<S3>:94' */
    localDW->is_active_Logic = 1U;

    /* Entry Internal 'Logic': '<S3>:95' */
    /* Transition: '<S3>:82' */
    localDW->is_Logic = powerwindow_powerwindow_control_IN_Neutral;

    /* Chart: '<S2>/stateflow control model' incorporates:
     *  TriggerPort: '<S3>/ticks'
     */
    /* Entry 'Neutral': '<S3>:16' */
    *rty_window_up = false;
    *rty_window_down = false;
    *rty_wake = false;
    localDW->is_active_Sensing = 1U;

    /* Entry Internal 'Sensing': '<S3>:96' */
    /* Transition: '<S3>:153' */
    localDW->is_Sensing = powerwindow_powerwindow_control_IN_Start;
    localDW->temporalCounter_i2 = 0U;

    /* Chart: '<S2>/stateflow control model' incorporates:
     *  TriggerPort: '<S3>/ticks'
     */
    /* Entry 'Start': '<S3>:170' */
    *rty_overcurrent = false;
    *rty_pinch = false;
  } else {
    switch (localDW->is_c2_PowerWindow_control) {
     case powerwindow_powerwindow_contr_IN_EndReached:
      /* During 'EndReached': '<S3>:97' */
      if (localDW->temporalCounter_i1 >= 10) {
        /* Transition: '<S3>:101' */
        localDW->is_c2_PowerWindow_control = powerwindow_powerwindow_control_IN_System;

        /* Entry Internal 'System': '<S3>:94' */
        localDW->is_active_Logic = 1U;

        /* Entry Internal 'Logic': '<S3>:95' */
        /* Transition: '<S3>:82' */
        localDW->is_Logic = powerwindow_powerwindow_control_IN_Neutral;

        /* Chart: '<S2>/stateflow control model' incorporates:
         *  TriggerPort: '<S3>/ticks'
         */
        /* Entry 'Neutral': '<S3>:16' */
        *rty_window_up = false;
        *rty_window_down = false;
        *rty_wake = false;
        localDW->is_active_Sensing = 1U;

        /* Entry Internal 'Sensing': '<S3>:96' */
        /* Transition: '<S3>:153' */
        localDW->is_Sensing = powerwindow_powerwindow_control_IN_Start;
        localDW->temporalCounter_i2 = 0U;

        /* Chart: '<S2>/stateflow control model' incorporates:
         *  TriggerPort: '<S3>/ticks'
         */
        /* Entry 'Start': '<S3>:170' */
        *rty_overcurrent = false;
        *rty_pinch = false;
      }
      break;

     case powerwindow_powerwindow_control_IN_Pinch:
      /* During 'Pinch': '<S3>:152' */
      if (localDW->temporalCounter_i1 >= 40) {
        /* Transition: '<S3>:157' */
        localDW->is_c2_PowerWindow_control = powerwindow_powerwindow_control_IN_System;

        /* Entry Internal 'System': '<S3>:94' */
        localDW->is_active_Logic = 1U;

        /* Entry Internal 'Logic': '<S3>:95' */
        /* Transition: '<S3>:82' */
        localDW->is_Logic = powerwindow_powerwindow_control_IN_Neutral;

        /* Chart: '<S2>/stateflow control model' incorporates:
         *  TriggerPort: '<S3>/ticks'
         */
        /* Entry 'Neutral': '<S3>:16' */
        *rty_window_up = false;
        *rty_window_down = false;
        *rty_wake = false;
        localDW->is_active_Sensing = 1U;

        /* Entry Internal 'Sensing': '<S3>:96' */
        /* Transition: '<S3>:153' */
        localDW->is_Sensing = powerwindow_powerwindow_control_IN_Start;
        localDW->temporalCounter_i2 = 0U;

        /* Chart: '<S2>/stateflow control model' incorporates:
         *  TriggerPort: '<S3>/ticks'
         */
        /* Entry 'Start': '<S3>:170' */
        *rty_overcurrent = false;
        *rty_pinch = false;
      }
      break;

     default:
      /* Chart: '<S2>/stateflow control model' incorporates:
       *  TriggerPort: '<S3>/ticks'
       */
      /* During 'System': '<S3>:94' */
      if (*rty_pinch == 1) {
        /* Transition: '<S3>:155' */
        /* Exit Internal 'System': '<S3>:94' */
        /* Exit Internal 'Sensing': '<S3>:96' */
        localDW->is_Sensing = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
        localDW->is_active_Sensing = 0U;

        /* Exit Internal 'Logic': '<S3>:95' */
        /* Exit Internal 'Down': '<S3>:18' */
        localDW->is_Down = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
        localDW->is_Logic = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;

        /* Exit Internal 'Up': '<S3>:17' */
        localDW->is_Up = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
        localDW->is_active_Logic = 0U;
        localDW->is_c2_PowerWindow_control = powerwindow_powerwindow_control_IN_Pinch;
        localDW->temporalCounter_i1 = 0U;

        /* Entry 'Pinch': '<S3>:152' */
        *rty_window_up = false;
        *rty_window_down = true;
      } else if (*rty_overcurrent == 1) {
        /* Transition: '<S3>:100' */
        /* Exit Internal 'System': '<S3>:94' */
        /* Exit Internal 'Sensing': '<S3>:96' */
        localDW->is_Sensing = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
        localDW->is_active_Sensing = 0U;

        /* Exit Internal 'Logic': '<S3>:95' */
        /* Exit Internal 'Down': '<S3>:18' */
        localDW->is_Down = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
        localDW->is_Logic = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;

        /* Exit Internal 'Up': '<S3>:17' */
        localDW->is_Up = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
        localDW->is_active_Logic = 0U;
        localDW->is_c2_PowerWindow_control = powerwindow_powerwindow_contr_IN_EndReached;
        localDW->temporalCounter_i1 = 0U;

        /* Entry 'EndReached': '<S3>:97' */
        *rty_window_up = false;
        *rty_window_down = false;
      } else {
        /* During 'Logic': '<S3>:95' */
        switch (localDW->is_Logic) {
         case powerwindow_powerwindow_control_IN_Down_d:
          /* During 'Down': '<S3>:18' */
          if (localB->map[1]) {
            /* Transition: '<S3>:169' */
            /* Exit Internal 'Down': '<S3>:18' */
            localDW->is_Down = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
            localDW->is_Logic = powerwindow_powerwindow_control_IN_Up;

            /* Entry 'Up': '<S3>:17' */
            *rty_window_up = true;
            *rty_window_down = false;
            *rty_wake = true;
            localDW->is_Up = powerwindow_powerwindow_control_IN_Up;
          } else {
            switch (localDW->is_Down) {
             case powerwindow_powerwindow_control_IN_AutoDown:
              /* During 'AutoDown': '<S3>:111' */
              break;

             case powerwindow_powerwindow_control_IN_Down:
              /* During 'Down': '<S3>:110' */
              if (localB->map[0]) {
                /* Transition: '<S3>:26' */
                localDW->is_Down = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
                localDW->is_Logic = powerwindow_powerwindow_control_IN_Neutral;

                /* Entry 'Neutral': '<S3>:16' */
                *rty_window_up = false;
                *rty_window_down = false;
                *rty_wake = false;
              }
              break;

             default:
              /* During 'InitDown': '<S3>:109' */
              if (localDW->temporalCounter_i1 >= 20) {
                /* Transition: '<S3>:119' */
                if (localB->map[0]) {
                  /* Transition: '<S3>:120' */
                  localDW->is_Down = powerwindow_powerwindow_control_IN_AutoDown;
                } else {
                  if (localB->map[2]) {
                    /* Transition: '<S3>:121' */
                    localDW->is_Down = powerwindow_powerwindow_control_IN_Down;
                  }
                }
              }
              break;
            }
          }
          break;

         case powerwindow_powerwindow_control_IN_Neutral:
          /* During 'Neutral': '<S3>:16' */
          if (localB->map[1]) {
            /* Transition: '<S3>:24' */
            localDW->is_Logic = powerwindow_powerwindow_control_IN_Up;

            /* Entry 'Up': '<S3>:17' */
            *rty_window_up = true;
            *rty_window_down = false;
            *rty_wake = true;
            localDW->is_Up = powerwindow_powerwindow_control_IN_InitUp;
            localDW->temporalCounter_i1 = 0U;
          } else {
            if (localB->map[2]) {
              /* Transition: '<S3>:25' */
              localDW->is_Logic = powerwindow_powerwindow_control_IN_Down_d;

              /* Entry 'Down': '<S3>:18' */
              *rty_window_up = false;
              *rty_window_down = true;
              *rty_wake = true;
              localDW->is_Down = powerwindow_powerwindow_control_IN_InitDown;
              localDW->temporalCounter_i1 = 0U;
            }
          }
          break;

         default:
          /* During 'Up': '<S3>:17' */
          if (localB->map[2]) {
            /* Transition: '<S3>:166' */
            /* Exit Internal 'Up': '<S3>:17' */
            localDW->is_Up = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
            localDW->is_Logic = powerwindow_powerwindow_control_IN_Down_d;

            /* Entry 'Down': '<S3>:18' */
            *rty_window_up = false;
            *rty_window_down = true;
            *rty_wake = true;
            localDW->is_Down = powerwindow_powerwindow_control_IN_Down;
          } else {
            switch (localDW->is_Up) {
             case powerwindow_powerwindow_control_IN_AutoUp:
              /* During 'AutoUp': '<S3>:108' */
              break;

             case powerwindow_powerwindow_control_IN_InitUp:
              /* During 'InitUp': '<S3>:106' */
              if (localDW->temporalCounter_i1 >= 20) {
                /* Transition: '<S3>:115' */
                if (localB->map[0]) {
                  /* Transition: '<S3>:118' */
                  localDW->is_Up = powerwindow_powerwindow_control_IN_AutoUp;
                } else {
                  if (localB->map[1]) {
                    /* Transition: '<S3>:117' */
                    localDW->is_Up = powerwindow_powerwindow_control_IN_Up;
                  }
                }
              }
              break;

             default:
              /* During 'Up': '<S3>:107' */
              if (localB->map[0]) {
                /* Transition: '<S3>:23' */
                localDW->is_Up = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
                localDW->is_Logic = powerwindow_powerwindow_control_IN_Neutral;

                /* Entry 'Neutral': '<S3>:16' */
                *rty_window_up = false;
                *rty_window_down = false;
                *rty_wake = false;
              }
              break;
            }
          }
          break;
        }

        /* During 'Sensing': '<S3>:96' */
        switch (localDW->is_Sensing) {
         case powerwindow_powerwindow_control_IN_SenseEnd:
          /* During 'SenseEnd': '<S3>:147' */
          if ((localB->LogicalOperator == 0) && (*rty_window_up == 1)) {
            /* Transition: '<S3>:173' */
            localDW->is_Sensing = powerwindow_powerwindow_control_IN_Start;
            localDW->temporalCounter_i2 = 0U;

            /* Entry 'Start': '<S3>:170' */
            *rty_overcurrent = false;
            *rty_pinch = false;
          } else {
            *rty_overcurrent = (localB->RateTransition1 > 184);
          }
          break;

         case powerwindow_powerwindow_contr_IN_SensePinch:
          /* During 'SensePinch': '<S3>:148' */
          if ((localB->LogicalOperator == 1) || (*rty_window_down == 1)) {
            /* Transition: '<S3>:150' */
            localDW->is_Sensing = powerwindow_powerwindow_control_IN_SenseEnd;
          } else {
            *rty_pinch = (localB->RateTransition1 > 92);
          }
          break;

         default:
          /* During 'Start': '<S3>:170' */
          if (localDW->temporalCounter_i2 >= 6) {
            /* Transition: '<S3>:171' */
            localDW->is_Sensing = powerwindow_powerwindow_contr_IN_SensePinch;
          }
          break;
        }
      }
      break;
    }
  }
}

/* Initial conditions for referenced model: 'powerwindow_powerwindow_control' */
void powerwindow_powerwindow_control_Init(powerwindow_boolean_T *rty_window_up, powerwindow_boolean_T
  *rty_window_down, powerwindow_boolean_T *rty_overcurrent, powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T *
  rty_wake, powerwindow_rtDW_PowerWindow_control *localDW)
{
  /* InitializeConditions for Chart: '<S2>/stateflow control model' */
  localDW->is_active_Logic = 0U;
  localDW->is_Logic = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
  localDW->is_Down = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
  localDW->is_Up = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
  localDW->temporalCounter_i1 = 0U;
  localDW->is_active_Sensing = 0U;
  localDW->is_Sensing = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
  localDW->temporalCounter_i2 = 0U;
  localDW->is_active_c2_PowerWindow_contro = 0U;
  localDW->is_c2_PowerWindow_control = powerwindow_powerwindow__IN_NO_ACTIVE_CHILD;
  *rty_window_up = false;
  *rty_window_down = false;
  *rty_overcurrent = false;
  *rty_pinch = false;
  *rty_wake = false;
}

/* Start for referenced model: 'powerwindow_powerwindow_control' */
void powerwindow_powerwindow_control_Start(powerwindow_rtDW_PowerWindow_control *localDW)
{
  /* Start for DiscretePulseGenerator: '<S2>/period of 50ms' */
  localDW->clockTickCounter = 0L;
}

/* Output and update for referenced model: 'powerwindow_powerwindow_control' */
void powerwindow_powerwindow_control_main(const powerwindow_boolean_T *rtu_up, const powerwindow_boolean_T *rtu_down,
  const powerwindow_boolean_T *rtu_endofdetectionrange, const powerwindow_uint8_T *rtu_currentsense,
  powerwindow_boolean_T *rty_window_up, powerwindow_boolean_T *rty_window_down, powerwindow_boolean_T
  *rty_overcurrent, powerwindow_boolean_T *rty_pinch, powerwindow_boolean_T *rty_wake,
  powerwindow_rtB_PowerWindow_control *localB, powerwindow_rtDW_PowerWindow_control *localDW,
  powerwindow_rtZCE_PowerWindow_control *localZCE)
{
  powerwindow_int16_T rowIdx;
  powerwindow_int16_T rtb_periodof50ms;

  /* DiscretePulseGenerator: '<S2>/period of 50ms' */
  rtb_periodof50ms = (localDW->clockTickCounter < 5L) &&
    (localDW->clockTickCounter >= 0L) ? 1 : 0;
  if (localDW->clockTickCounter >= 9L) {
    localDW->clockTickCounter = 0L;
  } else {
    localDW->clockTickCounter++;
  }

  /* End of DiscretePulseGenerator: '<S2>/period of 50ms' */

  /* Logic: '<S2>/Logical Operator' */
  localB->LogicalOperator = !*rtu_endofdetectionrange;

  /* RateTransition: '<S2>/Rate Transition1' */
  localB->RateTransition1 = *rtu_currentsense;

  /* CombinatorialLogic: '<S2>/map' */
  rowIdx = (powerwindow_int16_T)(((powerwindow_uint16_T)*rtu_up << 1) + *rtu_down);
  localB->map[0U] = rtCP_map_table[(powerwindow_uint16_T)rowIdx];
  localB->map[1U] = rtCP_map_table[rowIdx + 4U];
  localB->map[2U] = rtCP_map_table[rowIdx + 8U];

  /* Chart: '<S2>/stateflow control model' incorporates:
   *  TriggerPort: '<S3>/ticks'
   */
  /* DataTypeConversion: '<S2>/Data Type Conversion' */
  if (((rtb_periodof50ms != 0) != (localZCE->stateflowcontrolmodel_Trig_ZCE ==
        powerwindow_POS_ZCSIG)) && (localZCE->stateflowcontrolmodel_Trig_ZCE !=
                        powerwindow_UNINITIALIZED_ZCSIG)) {
    /* Gateway: PW_PSG/PWExternalClock/stateflow control model */
    if (localDW->temporalCounter_i1 < 63U) {
      localDW->temporalCounter_i1++;
    }

    if (localDW->temporalCounter_i2 < 7U) {
      localDW->temporalCounter_i2++;
    }

    powerwindow_powerwindow_con_broadcast_ticks(rty_window_up, rty_window_down,
      rty_overcurrent, rty_pinch, rty_wake, localB, localDW);
  }

  localZCE->stateflowcontrolmodel_Trig_ZCE = (powerwindow_uint8_T)(rtb_periodof50ms != 0 ?
    (powerwindow_int16_T)powerwindow_POS_ZCSIG : (powerwindow_int16_T)powerwindow_ZERO_ZCSIG);

  /* End of DataTypeConversion: '<S2>/Data Type Conversion' */
}

/* Model initialize function */
void powerwindow_powerwindow_control_initialize(const powerwindow_char_T **rt_errorStatus,
  powerwindow_RT_MODEL_PowerWindow_control *const PowerWindow_control_M,
  powerwindow_rtB_PowerWindow_control *localB, powerwindow_rtDW_PowerWindow_control *localDW,
  powerwindow_rtZCE_PowerWindow_control *localZCE)
{
  /* Registration code */

  /* initialize error status */
	powerwindow_powerwindow_control_rtmSetErrorStatusPointer(PowerWindow_control_M, rt_errorStatus);

  /* block I/O */
  (void) memset(((void *) localB), 0,
                sizeof(powerwindow_rtB_PowerWindow_control));

  /* states (dwork) */
  (void) memset((void *)localDW, 0,
                sizeof(powerwindow_rtDW_PowerWindow_control));
  localZCE->stateflowcontrolmodel_Trig_ZCE = powerwindow_UNINITIALIZED_ZCSIG;
}

/*
 * File trailer for generated code.
 *
 * [EOF]
 */
