/* This file has been generated from conf/flight_plans/braunschweig.xml */
/* Please DO NOT EDIT */

#ifndef INFLIGHT_CALIB_H
#define INFLIGHT_CALIB_H 

void inflight_calib(bool_t mode_changed) {
  if (pprz_mode == PPRZ_MODE_AUTO1) {
    if (inflight_calib_mode == IF_CALIB_MODE_UP) {
      static int16_t ir_pitch_neutral_init;
      if (mode_changed) {
        ir_pitch_neutral_init = ir_pitch_neutral;
        slider1_init = from_fbw.channels[RADIO_GAIN1];
      }
      ir_pitch_neutral = ParamValInt16(ir_pitch_neutral_init, 60.000000, from_fbw.channels[RADIO_GAIN1], slider1_init);
      slider_1_val = (float)ir_pitch_neutral;
    }
    if (inflight_calib_mode == IF_CALIB_MODE_UP) {
      static int16_t ir_roll_neutral_init;
      if (mode_changed) {
        ir_roll_neutral_init = ir_roll_neutral;
        slider2_init = from_fbw.channels[RADIO_GAIN2];
      }
      ir_roll_neutral = ParamValInt16(ir_roll_neutral_init, -60.000000, from_fbw.channels[RADIO_GAIN2], slider2_init);
      slider_2_val = (float)ir_roll_neutral;
    }
  }
  if (pprz_mode == PPRZ_MODE_AUTO2) {
    if (inflight_calib_mode == IF_CALIB_MODE_UP) {
      static float course_pgain_init;
      if (mode_changed) {
        course_pgain_init = course_pgain;
        slider1_init = from_fbw.channels[RADIO_GAIN1];
      }
      course_pgain = ParamValFloat(course_pgain_init, 0.100000, from_fbw.channels[RADIO_GAIN1], slider1_init);
      slider_1_val = (float)course_pgain;
    }
    if (inflight_calib_mode == IF_CALIB_MODE_UP) {
      static float max_roll_init;
      if (mode_changed) {
        max_roll_init = max_roll;
        slider2_init = from_fbw.channels[RADIO_GAIN2];
      }
      max_roll = ParamValFloat(max_roll_init, -0.200000, from_fbw.channels[RADIO_GAIN2], slider2_init);
      slider_2_val = (float)max_roll;
    }
  }
}

#endif // INFLIGHT_CALIB_H
