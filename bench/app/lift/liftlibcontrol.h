#ifndef LIFTLIBCONTROL_H
#define LIFTLIBCONTROL_H

enum lift_Direction {
  lift_GO_LOAD = 8,
  lift_GO_TOP = 6,
  lift_GO_BOTTOM = 7,
  lift_GO_UP = 4,
  lift_GO_DOWN = 5
};

enum lift_Sensor {
  lift_SENS_IMPULS = 0,
  lift_SENS_TOP = 1,
  lift_SENS_BOTTOM = 2,
  lift_SENS_LOAD = 3
};

enum lift_Motor {
  lift_MOTOR_ON = 0,
  lift_MOTOR_UP = 1
};

enum lift_Command {
  lift_CMD_NONE = 0,
  lift_CMD_TOP = 1,
  lift_CMD_BOTTOM = 2,
  lift_CMD_UP = 3,
  lift_CMD_DOWN = 4
};

/* Global variables */
extern int lift_levelPos[ 16 ];
extern int lift_one_level;
extern int lift_cntValid;
extern int lift_cnt;
extern int lift_level;
extern int lift_loadLevel;
extern int lift_loadPending;
extern int lift_loadSensor;
extern int lift_cmd;
extern int lift_timMotor;
extern int lift_timImp;
extern int lift_directionUp;
extern int lift_lastImp;
extern int lift_dbgCnt;
extern int lift_endCnt;

/* Checksum */
extern int lift_checksum;

/* prototypes */
void lift_ctrl_init();
void lift_ctrl_loop();

/* internal prototypes */
int lift_check_run();
void lift_wait_for_motor_start();
void lift_do_cmd();
void lift_do_impulse( int val, int motor, int reset );
void lift_check_cmd();
void lift_check_level();

#endif
