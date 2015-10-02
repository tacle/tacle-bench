#include <stdio.h>
#include <stdint.h>
#include "lift_io.h"
#include "lift_control.h"

/* Global variables */
int levelPos [LEVEL_POS_LENGTH];
int one_level;

/**
 * Is the counter valid for level positioning?
 */
int cntValid;

/**
 * Position absolut or relativ.
 */
int cnt;

/**
 * Last stoped level (1..13) if position is absolute else 0.
 */
int level;

/**
 * load position in level, 0 means we don't know
 */
int loadLevel;

/**
 * we're going TOP or BOTTOM, but stop at load position.
 */
int loadPending;

/**
 * we're waiting for the load sensor to go.
 */
int loadSensor;

/**
 * cmd keeps the value of the command until the command is finished.
 * It is only updated by the switches if it's current value is CMD_NONE.
 */
int cmd;

int timMotor;

int timImp;

/**
 * Remember last direction for impuls count after motor off;
 */
int directionUp;

/**
 * Last value of impuls sensor.
 */
int lastImp;

int dbgCnt;

/**
 * stop value for the counter.
 */
int endCnt;


void ctrl_init()
{
  int i;

  io_init();
  cntValid = FALSE;
  cnt = 0;
  cmd = CMD_NONE;
  timMotor = 0;
  timImp = 0;
  directionUp = TRUE;
  lastImp = FALSE;
  loadLevel = 0;
  loadPending = FALSE;
  loadSensor = FALSE;
  i = 0;
  levelPos[i++] = 0;
  levelPos[i++]=58;
  levelPos[i++]=115;
  levelPos[i++]=173;
  levelPos[i++]=230;
  levelPos[i++]=288;
  levelPos[i++]=346;
  levelPos[i++]=403;
  levelPos[i++]=461;
  levelPos[i++]=518;
  levelPos[i++]=576;
  levelPos[i++]=634;
  levelPos[i++]=691;
  levelPos[i++]=749;
  levelPos[i++]=806;
  levelPos[i++]=864;
  one_level = levelPos[1];
}

void ctrl_loop()
{
	if (cmd==CMD_NONE) {
	  check_cmd();
	} else {
	  do_impulse(ctrl_io_in[SENS_IMPULS], ctrl_io_out[MOTOR_ON], ctrl_io_in[SENS_BOTTOM]);
	  do_cmd();
	}
	check_level();
	ctrl_io_led[13] = (dbgCnt&0x80) != 0;
	++dbgCnt;
	if ((dbgCnt&0x3f) == 0) {
          /*			dbg(io); */
	}
}

void check_level()
{
  int i;
  int middle = one_level>>2;
  if (cntValid) {
    _Pragma("loopbound min 14 max 14")
    for (level=1; level < LEVEL_POS_LENGTH; ++level) {
      if (cnt < levelPos[level]-middle) {
	break;
      }
    }
  } else {
    level = 0;
  }
  _Pragma("loopbound min 14 max 14")
  for (i=0; i<LED_PINS; ++i) {
    ctrl_io_led[i] = (i == level-1);
  }
}

void check_cmd()
{
  if (loadPending) {
    if (ctrl_io_in[SENS_BOTTOM]) {
      cmd = CMD_TOP;
    }
  } else if (ctrl_io_in[GO_UP]) {
    if (!ctrl_io_in[SENS_TOP] && level!=LEVEL_POS_LENGTH) {
      cmd = CMD_UP;
    }
  } else if (ctrl_io_in[GO_DOWN]) {
    if (!ctrl_io_in[SENS_BOTTOM] && level!=1) {
      cmd = CMD_DOWN;
    }
  } else if (ctrl_io_in[GO_LOAD]) {
    if (loadLevel!=0 && level<loadLevel) {
      cmd = CMD_TOP;
    } else {
      cmd = CMD_BOTTOM;
    }
    loadPending = TRUE;
    loadSensor = FALSE;
  } else if (ctrl_io_in[GO_TOP]) {
    if (!ctrl_io_in[SENS_TOP]) {
      cmd = CMD_TOP;
    }
  } else if (ctrl_io_in[GO_BOTTOM]) {
    if (!ctrl_io_in[SENS_BOTTOM]) {
      cmd = CMD_BOTTOM;
    }
  }
  if (cmd!=CMD_NONE) {
    timMotor = MOTOR_WAIT_CNT;
  }
}

void do_impulse(BOOLEAN val, BOOLEAN motor, BOOLEAN reset)
{
  if (val && !lastImp) {
    if (motor || timImp>0) {
      if (directionUp) {
	++cnt;
      } else {
	--cnt;
      }
    }
  }
  if (reset) {
    cnt = 0;
    cntValid = TRUE;
  }
  lastImp = val;
  if (timImp>0) {
    --timImp;
    if (timImp==0 && cmd!=CMD_NONE) {
      cmd = CMD_NONE;
    }
  }
}

void do_cmd()
{
  if (timMotor > 0) {
    wait_for_motor_start();
  } else {
    BOOLEAN run = check_run();
    if (ctrl_io_out[MOTOR_ON] && !run) {
      /* motor stopped: */
      cmd = CMD_WAIT;
      timImp = IMP_CONT_CNT;
    }
    ctrl_io_out[MOTOR_ON] = run;
  }
}

void wait_for_motor_start()
{
  --timMotor;
  directionUp = (cmd==CMD_UP || cmd==CMD_TOP);
  ctrl_io_out[MOTOR_UP] =  directionUp;
  if (!cntValid) {
    cnt = 0;		/* use relative counter */
    if (cmd==CMD_UP) {
      endCnt = one_level;
    } else {
      endCnt = -one_level;
    }
  } else {
    endCnt = cnt;
    int newLevel = -99;
    if (cmd==CMD_UP) {
      newLevel = level+1;
    } else if (cmd==CMD_DOWN){
      newLevel = level-1;
    }
    --newLevel;	/* level is one based */
    if (newLevel>=0 && newLevel<LEVEL_POS_LENGTH) {
      endCnt = levelPos[newLevel];
    }
  }
}

BOOLEAN check_run()
{
  if (cmd == CMD_UP) {
    if (cnt < endCnt - OFFSET && !ctrl_io_in[SENS_TOP])
      return TRUE;
  } else if (cmd == CMD_DOWN) {
    if (cnt > endCnt + OFFSET && !ctrl_io_in[SENS_BOTTOM])
      return TRUE;
  } else if (cmd == CMD_TOP) {
    if (loadPending && ctrl_io_in[SENS_LOAD]) {
      /* we are at load position */
      loadLevel = level;
      loadPending = FALSE;
      return FALSE;
    }
    if (!ctrl_io_in[SENS_TOP])
      return TRUE;
    /* safe fallback if load sensor does not work */
    loadPending = FALSE;
  } else if (cmd == CMD_BOTTOM) {
    if (loadPending) {
      if (loadSensor) {
	if (!ctrl_io_in[SENS_LOAD]) {
	  loadSensor = FALSE;
	  /* we are at load position */
	  loadPending = FALSE;
	  loadLevel = level;
	  return FALSE;
	}
      }
      loadSensor = ctrl_io_in[SENS_LOAD];
    }
    if (!ctrl_io_in[SENS_BOTTOM])
      return TRUE;
  }
  return FALSE;
}
