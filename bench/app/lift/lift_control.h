#ifndef LIFT_CONTROL_H_UKQR5ABB
#define LIFT_CONTROL_H_UKQR5ABB

#define FALSE 0
#define TRUE  1
#define BOOLEAN int

#define LEVEL_POS_LENGTH 14

#define GO_LOAD 8
#define GO_TOP 6
#define GO_BOTTOM 7
#define GO_UP 4
#define GO_DOWN 5

#define SENS_IMPULS 0
#define SENS_TOP 1
#define SENS_BOTTOM 2
#define SENS_LOAD 3

#define MOTOR_ON 0
#define MOTOR_UP 1

#define CMD_NONE 0
#define CMD_TOP 1
#define CMD_BOTTOM 2
#define CMD_UP 3
#define CMD_DOWN 4

/**
 * Wait for motor stop and continue impuls counting.
 * Go to CMD_NONE after the wait time (IMP_CONT).
 * All commands should end here.
 */
#define CMD_WAIT 99

/* controller period (ms) */
#define PERIOD 10

/**
 * Wait time till motor will start after dircetion.
 */
#define MOTOR_WAIT_MS 500
#define MOTOR_WAIT_CNT 50

/**
 * Additional time where pulse are still counted after motor stop.
 * This time also delayes new commands for a clean direction switch.
 */
#define IMP_CONT_MS 500
#define IMP_CONT_CNT 50

/**
 * compensate running motor after stop.
 */
#define OFFSET 1

/* Global, observable variables */

/* Global variables */
extern int levelPos [LEVEL_POS_LENGTH];
extern int one_level;
extern int cntValid;
extern int cnt;
extern int level;
extern int loadLevel;
extern int loadPending;
extern int loadSensor;
extern int cmd;
extern int timMotor;
extern int timImp;
extern int directionUp;
extern int lastImp;
extern int dbgCnt;
extern int endCnt;

/* prototypes */
void ctrl_init();
void ctrl_loop();

/* internal prototypes */
BOOLEAN check_run();
void wait_for_motor_start();
void do_cmd();
void do_impulse(BOOLEAN val, BOOLEAN motor, BOOLEAN reset);
void check_cmd();
void check_level();

#endif /* end of include guard: LIFT_CONTROL_H_UKQR5ABB */
