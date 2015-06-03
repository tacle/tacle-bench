/* This file has been generated from conf/airframes/twinstar3.xml */
/* Please DO NOT EDIT */

#ifndef AIRFRAME_H
#define AIRFRAME_H 

#define AIRFRAME_NAME "Twinstar trois"

#define CTL_BRD_V1_2_1 1

#define ADC_CHANNEL_IR1 1
#define ADC_CHANNEL_IR2 2

#define NB_SERVO 6

#define SERVO_MOTOR_LEFT 3
#define SERVO_MOTOR_LEFT_TRAVEL 0.833333333333
#define SERVOS_NEUTRALS_3 1000

#define SERVO_MOTOR_RIGHT 9
#define SERVO_MOTOR_RIGHT_TRAVEL 0.833333333333
#define SERVOS_NEUTRALS_9 1000

#define SERVO_AILERON_LEFT 0
#define SERVO_AILERON_LEFT_TRAVEL -0.55
#define SERVOS_NEUTRALS_0 1600

#define SERVO_AILERON_RIGHT 2
#define SERVO_AILERON_RIGHT_TRAVEL -0.520833333333
#define SERVOS_NEUTRALS_2 1650

#define SERVO_ELEVATOR 6
#define SERVO_ELEVATOR_TRAVEL 0.496666666667
#define SERVOS_NEUTRALS_6 1530

#define SERVO_RUDDER 7
#define SERVO_RUDDER_TRAVEL -0.483333333333
#define SERVOS_NEUTRALS_7 1450


#define SERVOS_MINS {1850,1000,2000,1000,1000,1000,1220,1850,1000,1000}
#define SERVOS_NEUTRALS {1600,1500,1650,1000,1500,1500,1530,1450,1500,1000}
#define SERVOS_MAXS {1190,2000,1375,2000,2000,2000,1816,1270,2000,2000}

#define SERVO_MIN_US 1000ul
#define SERVO_MAX_US 2000ul

#define ServoSet(values) { \
  uint16_t servo_value;\
  int16_t _var_roll;\
  servo_value = SERVO_NEUTRAL(SERVO_MOTOR_LEFT) + (int16_t)((2 * values[RADIO_GAIN1])*SERVO_MOTOR_LEFT_TRAVEL);\
  servo_widths[SERVO_MOTOR_LEFT] = ChopServo(servo_value);\
\
  servo_value = SERVO_NEUTRAL(SERVO_MOTOR_RIGHT) + (int16_t)((2 * values[RADIO_THROTTLE])*SERVO_MOTOR_RIGHT_TRAVEL);\
  servo_widths[SERVO_MOTOR_RIGHT] = ChopServo(servo_value);\
\
  servo_value = SERVO_NEUTRAL(SERVO_ELEVATOR) + (int16_t)((values[RADIO_PITCH])*SERVO_ELEVATOR_TRAVEL);\
  servo_widths[SERVO_ELEVATOR] = ChopServo(servo_value);\
\
  _var_roll = values[RADIO_ROLL];\
  servo_value = SERVO_NEUTRAL(SERVO_AILERON_LEFT) + (int16_t)(((_var_roll > 0 ? 1 : AILERON_DIFF) * _var_roll)*SERVO_AILERON_LEFT_TRAVEL);\
  servo_widths[SERVO_AILERON_LEFT] = ChopServo(servo_value);\
\
  servo_value = SERVO_NEUTRAL(SERVO_AILERON_RIGHT) + (int16_t)(((_var_roll > 0 ? AILERON_DIFF : 1) * _var_roll)*SERVO_AILERON_RIGHT_TRAVEL);\
  servo_widths[SERVO_AILERON_RIGHT] = ChopServo(servo_value);\
\
  servo_value = SERVO_NEUTRAL(SERVO_RUDDER) + (int16_t)((values[RADIO_YAW] + values[RADIO_ROLL]*COMBI_SWITCH)*SERVO_RUDDER_TRAVEL);\
  servo_widths[SERVO_RUDDER] = ChopServo(servo_value);\
\
}
#define AILERON_DIFF 0.66
#define COMBI_SWITCH 1.0

#define IR_ROLL_NEUTRAL_DEFAULT -915
#define IR_PITCH_NEUTRAL_DEFAULT 110
#define IR_DEFAULT_CONTRAST 200
#define IR_RAD_OF_IR_CONTRAST 0.75
#define IR_RollOfIrs(x1,x2) (-1*(x1)+ -1*(x2))
#define IR_PitchOfIrs(x1,x2) (-1*(x1)+ 1*(x2))
#define IR_RAD_OF_IR_MAX_VAL 0.0045
#define IR_RAD_OF_IR_MIN_VAL 0.00075

#define ROLL_PGAIN 10000.
#define PITCH_OF_ROLL 0.0
#define PITCH_PGAIN 15000.
#define MAX_ROLL 0.35
#define MAX_PITCH 0.35
#define MIN_PITCH -0.35

#define CLIMB_PITCH_PGAIN -0.1
#define CLIMB_PITCH_IGAIN 0.025
#define CLIMB_PGAIN -0.03
#define CLIMB_IGAIN 0.1
#define CLIMB_MAX 1.
#define CLIMB_LEVEL_GAZ 0.31
#define CLIMB_PITCH_OF_VZ_PGAIN 0.05
#define CLIMB_GAZ_OF_CLIMB 0.2

#define COURSE_PGAIN -0.2
#define ALTITUDE_PGAIN -0.025
#define NAV_PITCH 0.

#define VOLTAGE_ADC_A 0.0175
#define VOLTAGE_ADC_B 0.088
#define VoltageOfAdc(adc) (VOLTAGE_ADC_A * adc + VOLTAGE_ADC_B)
#define LOW_BATTERY 93

#define NOMINAL_AIRSPEED 10.
#define CARROT 5.

#define ROLL_RESPONSE_FACTOR 4.
#define YAW_RESPONSE_FACTOR 4.
#define WEIGHT 1.3


#endif // AIRFRAME_H
