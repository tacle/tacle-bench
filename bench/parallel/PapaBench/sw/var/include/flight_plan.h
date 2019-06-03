/* This file has been generated from conf/flight_plans/braunschweig.xml */
/* Please DO NOT EDIT */

#ifndef FLIGHT_PLAN_H
#define FLIGHT_PLAN_H

#define FLIGHT_PLAN_NAME "EMAV 2004, 8 shape"
#define NAV_UTM_EAST0 605530
#define NAV_UTM_NORTH0 5797350
#define QFU 270.0
#define WP_HOME 0
#define WAYPOINTS { \
 {0.0, 0.0, 200},\
 {0.0, 0.0, 200},\
 {115.0, -75.0, 200},\
 {156.7, -41.7, 200},\
 {115.0, 0.0, 200},\
 {0.0, -75.0, 200},\
 {-51.7, -36.7, 200},\
};
#define NB_WAYPOINT 7
#define GROUND_ALT 125.
#define SECURITY_ALT 150.
#define MAX_DIST_FROM_HOME 500.
#ifdef NAV_C

static inline void auto_nav( void )
{
  /*#ifdef WITH_SWITCH
    switch (nav_block) {
      Block(0) // init
      switch(nav_stage) {
        Label(while_1)
        Stage(0)
          if (! (!(estimator_flight_time))) Goto(endwhile_2) else NextStage();
          Stage(1)
            Goto(while_1)
          Label(endwhile_2)
        Stage(2)
          if ((estimator_flight_time>8)) NextStage() else {
            desired_course = RadOfDeg(QFU);
            auto_pitch = FALSE;
            nav_pitch = 0.150000;
            vertical_mode = VERTICAL_MODE_AUTO_GAZ;
            nav_desired_gaz = TRIM_UPPRZ(0.800000*MAX_PPRZ);
          }
          return;
        Stage(3)
          if ((estimator_z>SECURITY_ALT)) NextStage() else {
            desired_course = RadOfDeg(QFU);
            auto_pitch = FALSE;
            nav_pitch = 0.000000;
            vertical_mode = VERTICAL_MODE_AUTO_CLIMB;
            desired_climb = 8.000000;
          }
          return;
        Stage(4)
          NextBlock()
      }

      Block(1) // two
      if RcEvent1() { GotoBlock(2) }
      switch(nav_stage) {
        Label(while_3)
        Stage(0)
          if (! (TRUE)) Goto(endwhile_4) else NextStage();
          Stage(1)
            if (approaching(1)) NextStageFrom(1) else {
              fly_to(1);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 1 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(2)
            if (approaching(4)) NextStageFrom(4) else {
              fly_to(4);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 4 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(3)
            Goto(while_3)
          Label(endwhile_4)
        Stage(4)
          NextBlock()
      }

      Block(2) // height
      if RcEvent1() { GotoBlock(3) }
      switch(nav_stage) {
        Label(while_5)
        Stage(0)
          if (! (TRUE)) Goto(endwhile_6) else NextStage();
          Stage(1)
            if (approaching(6)) NextStageFrom(6) else {
              fly_to(6);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 6 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(2)
            if (approaching(1)) NextStageFrom(1) else {
              fly_to(1);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 1 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(3)
            if (approaching(2)) NextStageFrom(2) else {
              route_to(last_wp, 2);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 2 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(4)
            if (approaching(3)) NextStageFrom(3) else {
              fly_to(3);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 3 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(5)
            if (approaching(4)) NextStageFrom(4) else {
              fly_to(4);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 4 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(6)
            if (approaching(5)) NextStageFrom(5) else {
              route_to(last_wp, 5);
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 5 ].a;
              pre_climb = 0.;
            }
            return;
          Stage(7)
            Goto(while_5)
          Label(endwhile_6)
        Stage(8)
          NextBlock()
      }

      Block(3) // xyz
      if RcEvent1() { GotoBlock(4) }
      switch(nav_stage) {
        Stage(0)


         //NormCourse(x) { while (x < 0) x += 360; while (x >= 360) x -= 360;}


          //CircleXY(x,y,radius) { float alpha = atan2(estimator_y - y, estimator_x - x);float alpha_carrot = alpha + CARROT / (-radius * estimator_hspeed_mod);
    //fly_to_xy(x+cos(alpha_carrot)*fabs(radius),y+sin(alpha_carrot)*fabs(radius)); qdr = DegOfRad(M_PI/2 - alpha_carrot); NormCourse(qdr);}


          //Goto3D(50)
    {static float carrot_x, carrot_y; int16_t pitch; int16_t roll; if (pprz_mode == PPRZ_MODE_AUTO2) { int16_t yaw = from_fbw.channels[ RADIO_YAW ]; if (yaw >
    MIN_DX || yaw < -MIN_DX) { carrot_x += FLOAT_OF_PPRZ(yaw, 0, -20.);carrot_x = Min(carrot_x, MAX_DIST_CARROT); carrot_x = Max(carrot_x, -MAX_DIST_CARROT); }
    pitch = from_fbw.channels[ RADIO_PITCH ]; if (pitch > MIN_DX || pitch < -MIN_DX) { carrot_y += FLOAT_OF_PPRZ(pitch, 0, -20.); carrot_y = Min(carrot_y,
    MAX_DIST_CARROT); carrot_y = Max(carrot_y, -MAX_DIST_CARROT);} vertical_mode = VERTICAL_MODE_AUTO_ALT; roll =  from_fbw.channels[ RADIO_ROLL ]; if (roll >
    MIN_DX || roll < -MIN_DX) { desired_altitude += FLOAT_OF_PPRZ(roll, 0, -1.0); desired_altitude = Max(desired_altitude, MIN_HEIGHT_CARROT+GROUND_ALT);
    desired_altitude = Min(desired_altitude, MAX_HEIGHT_CARROT+GROUND_ALT); } } { float alpha = atan2(estimator_y - carrot_y, estimator_x - carrot_x);float alpha_carrot = alpha + CARROT / (-50 * estimator_hspeed_mod);
    fly_to_xy(carrot_x+cos(alpha_carrot)*fabs(50),carrot_y+sin(alpha_carrot)*fabs(50)); qdr = DegOfRad(M_PI/2 - alpha_carrot); { while (qdr < 0) qdr += 360;
    while (qdr >= 360) qdr -= 360;}} }


          return;
        Stage(1)
          NextBlock()
      }

      Block(4) // circle
      if RcEvent1() { GotoBlock(5) }
      switch(nav_stage) {
        Stage(0)
          auto_pitch = FALSE;
          nav_pitch = 0.000000;
          vertical_mode = VERTICAL_MODE_AUTO_ALT;
          desired_altitude = waypoints[ 0 ].a;
          pre_climb = 0.;
          Circle(0, 150);
          return;
        Stage(1)
          NextBlock()
      }

      Block(5) // hippo
      if RcEvent1() { GotoBlock(1) }
      switch(nav_stage) {
        Label(while_7)
        Stage(0)
          if (! (TRUE)) Goto(endwhile_8) else NextStage();
          Stage(1)
            auto_pitch = FALSE;
            nav_pitch = 0.000000;
            vertical_mode = VERTICAL_MODE_AUTO_ALT;
            desired_altitude = waypoints[ 1 ].a;
            pre_climb = 0.;
            Circle(1, 100);
            if (Qdr(0)) NextStage();
            return;
          Stage(2)
            auto_pitch = FALSE;
            nav_pitch = 0.000000;
            vertical_mode = VERTICAL_MODE_AUTO_ALT;
            desired_altitude = waypoints[ 4 ].a;
            pre_climb = 0.;
            Circle(4, 100);
            if (Qdr(180)) NextStage();
            return;
          Stage(3)
            Goto(while_7)
          Label(endwhile_8)
        Stage(4)
          NextBlock()
      }

    }
    #else*/
  if ( nav_block == 0 ) {
    // init

    if ( nav_stage == 0 ) {
      Label( while_1 )
      nav_stage = 0;
      if ( ! ( !( estimator_flight_time ) ) ) Goto( endwhile_2 )
        else NextStage();
    } else
      if ( nav_stage == 1 ) {
        Goto( while_1 )
      } else
        if ( nav_stage == 2 ) {
          Label( endwhile_2 )
          nav_stage = 2;
          if ( ( estimator_flight_time > 8 ) ) NextStage() else {
              desired_course = RadOfDeg( QFU );
              auto_pitch = FALSE;
              nav_pitch = 0.150000;
              vertical_mode = VERTICAL_MODE_AUTO_GAZ;
              nav_desired_gaz = TRIM_UPPRZ( 0.800000 * MAX_PPRZ );
            }
          return;
        } else
          if ( nav_stage == 3 ) {
            nav_stage = 3;
            if ( ( estimator_z > SECURITY_ALT ) ) NextStage() else {
                desired_course = RadOfDeg( QFU );
                auto_pitch = FALSE;
                nav_pitch = 0.000000;
                vertical_mode = VERTICAL_MODE_AUTO_CLIMB;
                desired_climb = 8.000000;
              }
            return;
          } else
            if ( nav_stage == 4 )
              NextBlock()
              else { }
  } else
    if ( nav_block == 1 ) { // two
      nav_block = 1;
      if RcEvent1() {
        GotoBlock( 2 )
      }
      if ( nav_stage == 0 ) {
        Label( while_3 )
        if ( ! ( TRUE ) ) Goto( endwhile_4 ) else NextStage();
      } else
        if ( nav_stage == 1 ) {
          nav_stage = 1;
          if ( approaching( 1 ) ) NextStageFrom( 1 ) else {
              fly_to( 1 );
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 1 ].a;
              pre_climb = 0.;
            }
          return;
        } else
          if ( nav_stage == 2 ) {
            nav_stage = 2;
            if ( approaching( 4 ) ) NextStageFrom( 4 ) else {
                fly_to( 4 );
                auto_pitch = FALSE;
                nav_pitch = 0.000000;
                vertical_mode = VERTICAL_MODE_AUTO_ALT;
                desired_altitude = waypoints[ 4 ].a;
                pre_climb = 0.;
              }
            return;
          } else
            if ( nav_stage == 3 ) {
              nav_stage = 3;
              Goto( while_3 )
            } else
              if ( nav_stage == 4 ) {
                Label( endwhile_4 )
                nav_stage = 4;
                NextBlock()
              } else { }
    } else
      if ( nav_block == 2 ) { // height
        nav_block = 2;
        if RcEvent1() {
          GotoBlock( 3 )
        }

        if ( nav_stage == 0 ) {
          Label( while_5 )
          nav_stage = 0;
          if ( ! ( TRUE ) ) Goto( endwhile_6 ) else NextStage();
        } else
          if ( nav_stage == 1 ) {
            nav_stage = 1;
            if ( approaching( 6 ) ) NextStageFrom( 6 ) else {
                fly_to( 6 );
                auto_pitch = FALSE;
                nav_pitch = 0.000000;
                vertical_mode = VERTICAL_MODE_AUTO_ALT;
                desired_altitude = waypoints[ 6 ].a;
                pre_climb = 0.;
              }
            return;
          } else
            if ( nav_stage == 2 ) {
              nav_stage = 2;
              if ( approaching( 1 ) ) NextStageFrom( 1 ) else {
                  fly_to( 1 );
                  auto_pitch = FALSE;
                  nav_pitch = 0.000000;
                  vertical_mode = VERTICAL_MODE_AUTO_ALT;
                  desired_altitude = waypoints[ 1 ].a;
                  pre_climb = 0.;
                }
              return;
            } else
              if ( nav_stage == 3 ) {
                nav_stage = 3;
                if ( approaching( 2 ) ) NextStageFrom( 2 ) else {
                    route_to( last_wp, 2 );
                    auto_pitch = FALSE;
                    nav_pitch = 0.000000;
                    vertical_mode = VERTICAL_MODE_AUTO_ALT;
                    desired_altitude = waypoints[ 2 ].a;
                    pre_climb = 0.;
                  }
                return;
              } else
                if ( nav_stage == 4 ) {
                  nav_stage = 4;
                  if ( approaching( 3 ) ) NextStageFrom( 3 ) else {
                      fly_to( 3 );
                      auto_pitch = FALSE;
                      nav_pitch = 0.000000;
                      vertical_mode = VERTICAL_MODE_AUTO_ALT;
                      desired_altitude = waypoints[ 3 ].a;
                      pre_climb = 0.;
                    }
                  return;
                } else
                  if ( nav_stage == 5 ) {
                    nav_stage = 5;
                    if ( approaching( 4 ) ) NextStageFrom( 4 ) else {
                        fly_to( 4 );
                        auto_pitch = FALSE;
                        nav_pitch = 0.000000;
                        vertical_mode = VERTICAL_MODE_AUTO_ALT;
                        desired_altitude = waypoints[ 4 ].a;
                        pre_climb = 0.;
                      }
                    return;
                  } else
                    if ( nav_stage == 6 ) {
                      nav_stage = 6;
                      if ( approaching( 5 ) ) NextStageFrom( 5 ) else {
                          route_to( last_wp, 5 );
                          auto_pitch = FALSE;
                          nav_pitch = 0.000000;
                          vertical_mode = VERTICAL_MODE_AUTO_ALT;
                          desired_altitude = waypoints[ 5 ].a;
                          pre_climb = 0.;
                        }
                      return;
                    } else
                      if ( nav_stage == 7 ) {
                        nav_stage = 7;
                        Goto( while_5 )
                      }

                      else
                        if ( nav_stage == 8 ) {
                          Label( endwhile_6 )
                          nav_stage = 8;
                          NextBlock();
                        } else { }
      } else
        if ( nav_block == 3 ) { // xyz
          nav_block = 3;
          if RcEvent1() {
            GotoBlock( 4 )
          }
          if ( nav_stage == 0 ) {
            nav_stage = 0;
            //Goto3D(50)
            {
              static float carrot_x, carrot_y;
              int16_t pitch;
              int16_t roll;
              if ( pprz_mode == PPRZ_MODE_AUTO2 ) {
                int16_t yaw = from_fbw.channels[ RADIO_YAW ];
                if ( yaw > MIN_DX || yaw < -MIN_DX ) {
                  carrot_x += FLOAT_OF_PPRZ( yaw, 0, -20. );
                  carrot_x = Min( carrot_x, MAX_DIST_CARROT );
                  carrot_x = Max( carrot_x, -MAX_DIST_CARROT );
                }
                pitch = from_fbw.channels[ RADIO_PITCH ];
                if ( pitch > MIN_DX || pitch < -MIN_DX ) {
                  carrot_y += FLOAT_OF_PPRZ( pitch, 0, -20. );
                  carrot_y = Min( carrot_y,
                                  MAX_DIST_CARROT );
                  carrot_y = Max( carrot_y, -MAX_DIST_CARROT );
                }
                vertical_mode = VERTICAL_MODE_AUTO_ALT;
                roll =  from_fbw.channels[ RADIO_ROLL ];
                if ( roll > MIN_DX || roll < -MIN_DX ) {
                  desired_altitude += FLOAT_OF_PPRZ( roll, 0, -1.0 );
                  desired_altitude = Max( desired_altitude, MIN_HEIGHT_CARROT + GROUND_ALT );
                  desired_altitude = Min( desired_altitude, MAX_HEIGHT_CARROT + GROUND_ALT );
                }
              }
              {
                float alpha = atan2( estimator_y - carrot_y, estimator_x - carrot_x );
                float alpha_carrot = alpha + CARROT / ( -50 * estimator_hspeed_mod );
                fly_to_xy( carrot_x + cos( alpha_carrot )*fabs( 50 ),
                           carrot_y + sin( alpha_carrot )*fabs( 50 ) );
                qdr = DegOfRad( M_PI / 2 - alpha_carrot );
                {
                  _Pragma( "loopbounds min 0 max 1" )
                  while ( qdr < 0 ) qdr += 360;
                  _Pragma( "loopbounds min 0 max 1" )
                  while ( qdr >= 360 ) qdr -= 360;
                }
              }
            }

            return;
          } else
            if ( nav_stage == 1 ) {
              nav_stage = 1;
              NextBlock()
            } else { }
        } else
          if ( nav_block == 4 ) {
            nav_block = 4;
            if RcEvent1() {
              GotoBlock( 5 )
            }
            if ( nav_stage == 0 ) {
              nav_stage = 0;
              auto_pitch = FALSE;
              nav_pitch = 0.000000;
              vertical_mode = VERTICAL_MODE_AUTO_ALT;
              desired_altitude = waypoints[ 0 ].a;
              pre_climb = 0.;
              Circle( 0, 150 );
              return;
            } else
              if ( nav_stage == 1 ) {
                nav_stage = 1;
                NextBlock()
              } else {}
          } else
            if ( nav_block == 5 ) {
              nav_block = 5;
              if RcEvent1() {
                GotoBlock( 1 )
              }
              if ( nav_stage == 0 ) {
                Label( while_7 )
                nav_stage = 0;
                if ( ! ( TRUE ) ) Goto( endwhile_8 ) else NextStage();
              }

              else
                if ( nav_stage == 1 ) {
                  nav_stage = 1;
                  auto_pitch = FALSE;
                  nav_pitch = 0.000000;
                  vertical_mode = VERTICAL_MODE_AUTO_ALT;
                  desired_altitude = waypoints[ 1 ].a;
                  pre_climb = 0.;
                  Circle( 1, 100 );
                  if ( Qdr( 0 ) ) NextStage();
                  return;
                } else
                  if ( nav_stage == 2 ) {
                    nav_stage = 2;
                    auto_pitch = FALSE;
                    nav_pitch = 0.000000;
                    vertical_mode = VERTICAL_MODE_AUTO_ALT;
                    desired_altitude = waypoints[ 4 ].a;
                    pre_climb = 0.;
                    Circle( 4, 100 );
                    if ( Qdr( 180 ) ) NextStage();
                    return;
                  } else
                    if ( nav_stage == 3 ) {
                      nav_stage = 3;
                      Goto( while_7 )
                    } else
                      if ( nav_stage == 4 ) {
                        Label( endwhile_8 )
                        nav_stage = 4;
                        NextBlock()
                      } else { }
            } else { }

//#endif
}
#endif // NAV_C

#endif // FLIGHT_PLAN_H
