#include "liftlibio.h"
#include "liftlibcontrol.h"

/* Global variables */
int lift_levelPos[ 16 ];
int lift_one_level;

/**
   Is the counter valid for level positioning?
*/
int lift_cntValid;

/**
   Position absolute or relative<.
*/
int lift_cnt;

/**
   Last stoped level (1..13) if position is absolute else 0.
*/
int lift_level;

/**
   load position in level, 0 means we don't know
*/
int lift_loadLevel;

/**
   we're going TOP or BOTTOM, but stop at load position.
*/
int lift_loadPending;

/**
   we're waiting for the load sensor to go.
*/
int lift_loadSensor;

/**
   cmd keeps the value of the command until the command is finished.
   It is only updated by the switches if it's current value is CMD_NONE.
*/
int lift_cmd;

int lift_timMotor;

int lift_timImp;

/**
   Remember last direction for impuls count after motor off;
*/
int lift_directionUp;

/**
   Last value of impuls sensor.
*/
int lift_lastImp;

int lift_dbgCnt;

/**
   stop value for the counter.
*/
int lift_endCnt;


void lift_ctrl_init()
{
  int i;
  lift_checksum = 0;

  lift_io_init();
  lift_cntValid = 0;
  lift_cnt = 0;
  lift_cmd = lift_CMD_NONE;
  lift_timMotor = 0;
  lift_timImp = 0;
  lift_directionUp = 1;
  lift_lastImp = 0;
  lift_loadLevel = 0;
  lift_loadPending = 0;
  lift_loadSensor = 0;
  i = 0;
  lift_levelPos[ i++ ] = 0;
  lift_levelPos[ i++ ] = 58;
  lift_levelPos[ i++ ] = 115;
  lift_levelPos[ i++ ] = 173;
  lift_levelPos[ i++ ] = 230;
  lift_levelPos[ i++ ] = 288;
  lift_levelPos[ i++ ] = 346;
  lift_levelPos[ i++ ] = 403;
  lift_levelPos[ i++ ] = 461;
  lift_levelPos[ i++ ] = 518;
  lift_levelPos[ i++ ] = 576;
  lift_levelPos[ i++ ] = 634;
  lift_levelPos[ i++ ] = 691;
  lift_levelPos[ i++ ] = 749;
  lift_levelPos[ i++ ] = 806;
  lift_levelPos[ i++ ] = 864;
  lift_one_level = lift_levelPos[ 1 ];
}

void lift_ctrl_loop()
{
  if ( lift_cmd == lift_CMD_NONE )
    lift_check_cmd();
  else {
    lift_do_impulse( lift_ctrl_io_in[ lift_SENS_IMPULS ],
                     lift_ctrl_io_out[ lift_MOTOR_ON ],
                     lift_ctrl_io_in[ lift_SENS_BOTTOM ] );
    lift_do_cmd();
  }
  lift_check_level();
  lift_ctrl_io_led[ 13 ] = ( lift_dbgCnt & 0x80 ) != 0;
  ++lift_dbgCnt;
}


void lift_check_level()
{
  int i;
  int middle = lift_one_level >> 2;
  if ( lift_cntValid ) {
    _Pragma( "loopbound min 14 max 14" )
    for ( lift_level = 1; lift_level < 14; ++lift_level ) {
      if ( lift_cnt < lift_levelPos[ lift_level ] - middle )
        break;
    }
  } else
    lift_level = 0;
  _Pragma( "loopbound min 14 max 14" )
  for ( i = 0; i < 14; ++i )
    lift_ctrl_io_led[ i ] = ( i == lift_level - 1 );
}


void lift_check_cmd()
{
  if ( lift_loadPending ) {
    if ( lift_ctrl_io_in[ lift_SENS_BOTTOM ] )
      lift_cmd = lift_CMD_TOP;
  } else
    if ( lift_ctrl_io_in[ lift_GO_UP ] ) {
      if ( !lift_ctrl_io_in[ lift_SENS_TOP ] && lift_level != 14 )
        lift_cmd = lift_CMD_UP;
    } else
      if ( lift_ctrl_io_in[ lift_GO_DOWN ] ) {
        if ( !lift_ctrl_io_in[ lift_SENS_BOTTOM ] && lift_level != 1 )
          lift_cmd = lift_CMD_DOWN;
      } else
        if ( lift_ctrl_io_in[ lift_GO_LOAD ] ) {
          if ( lift_loadLevel != 0 && lift_level < lift_loadLevel )
            lift_cmd = lift_CMD_TOP;
          else
            lift_cmd = lift_CMD_BOTTOM;
          lift_loadPending = 1;
          lift_loadSensor = 0;
        } else
          if ( lift_ctrl_io_in[ lift_GO_TOP ] ) {
            if ( !lift_ctrl_io_in[ lift_SENS_TOP ] )
              lift_cmd = lift_CMD_TOP;
          } else
            if ( lift_ctrl_io_in[ lift_GO_BOTTOM ] ) {
              if ( !lift_ctrl_io_in[ lift_SENS_BOTTOM ] )
                lift_cmd = lift_CMD_BOTTOM;
            }
  if ( lift_cmd != lift_CMD_NONE )
    lift_timMotor = 50;
}


void lift_do_impulse( int val, int motor, int reset )
{
  if ( val && !lift_lastImp ) {
    if ( motor || lift_timImp > 0 ) {
      if ( lift_directionUp )
        ++lift_cnt;
      else
        --lift_cnt;
    }
  }
  if ( reset ) {
    lift_cnt = 0;
    lift_cntValid = 1;
  }
  lift_lastImp = val;
  if ( lift_timImp > 0 ) {
    --lift_timImp;
    if ( lift_timImp == 0 && lift_cmd != lift_CMD_NONE )
      lift_cmd = lift_CMD_NONE;
  }
}


void lift_do_cmd()
{
  int run = 0;
  if ( lift_timMotor > 0 )
    lift_wait_for_motor_start();
  else {
    run = lift_check_run();
    if ( lift_ctrl_io_out[ lift_MOTOR_ON ] && !run ) {
      /* motor stopped: */
      lift_cmd = 99;
      lift_timImp = 50;
    }
    lift_ctrl_io_out[ lift_MOTOR_ON ] = run;
  }
}


void lift_wait_for_motor_start()
{
  int newLevel = 0;
  --lift_timMotor;
  lift_directionUp = ( lift_cmd == lift_CMD_UP || lift_cmd == lift_CMD_TOP );
  lift_ctrl_io_out[ lift_MOTOR_UP ] =  lift_directionUp;
  if ( !lift_cntValid ) {
    lift_cnt = 0;   /* use relative counter */
    if ( lift_cmd == lift_CMD_UP )
      lift_endCnt = lift_one_level;
    else
      lift_endCnt = -lift_one_level;
  } else {
    lift_endCnt = lift_cnt;
    newLevel = -99;
    if ( lift_cmd == lift_CMD_UP )
      newLevel = lift_level + 1;
    else
      if ( lift_cmd == lift_CMD_DOWN )
        newLevel = lift_level - 1;
    --newLevel; /* lift_level is one based */
    if ( newLevel >= 0 && newLevel < 14 )
      lift_endCnt = lift_levelPos[ newLevel ];
  }
}


int lift_check_run()
{
  if ( lift_cmd == lift_CMD_UP ) {
    if ( lift_cnt < lift_endCnt - 1 && !lift_ctrl_io_in[ lift_SENS_TOP ] )
      return 1;
  } else
    if ( lift_cmd == lift_CMD_DOWN ) {
      if ( lift_cnt > lift_endCnt + 1 && !lift_ctrl_io_in[ lift_SENS_BOTTOM ] )
        return 1;
    } else
      if ( lift_cmd == lift_CMD_TOP ) {
        if ( lift_loadPending && lift_ctrl_io_in[ lift_SENS_LOAD ] ) {
          /* we are at lift_load position */
          lift_loadLevel = lift_level;
          lift_loadPending = 0;
          return 0;
        }
        if ( !lift_ctrl_io_in[ lift_SENS_TOP ] )
          return 1;
        /* safe fallback if lift_load sensor does not work */
        lift_loadPending = 0;
      } else
        if ( lift_cmd == lift_CMD_BOTTOM ) {
          if ( lift_loadPending ) {
            if ( lift_loadSensor ) {
              if ( !lift_ctrl_io_in[ lift_SENS_LOAD ] ) {
                lift_loadSensor = 0;
                /* we are at lift_load position */
                lift_loadPending = 0;
                lift_loadLevel = lift_level;
                return 0;
              }
            }
            lift_loadSensor = lift_ctrl_io_in[ lift_SENS_LOAD ];
          }
          if ( !lift_ctrl_io_in[ lift_SENS_BOTTOM ] )
            return 1;
        }
  return 0;
}
