/* This file has been generated from conf/radios/mc3030.xml */
/* Please DO NOT EDIT */

#ifndef RADIO_H
#define RADIO_H

#define RADIO_NAME "mc3030"

#define RADIO_CTL_NB 9

#define RADIO_CTL_D 0
#define RADIO_THROTTLE RADIO_CTL_D
#define RADIO_CTL_C 1
#define RADIO_ROLL RADIO_CTL_C
#define RADIO_CTL_B 2
#define RADIO_PITCH RADIO_CTL_B
#define RADIO_CTL_A 3
#define RADIO_YAW RADIO_CTL_A
#define RADIO_CTL_G 4
#define RADIO_MODE RADIO_CTL_G
#define RADIO_CTL_E 5
#define RADIO_GAIN1 RADIO_CTL_E
#define RADIO_CTL_F 6
#define RADIO_GAIN2 RADIO_CTL_F
#define RADIO_CTL_H 7
#define RADIO_LLS RADIO_CTL_H
#define RADIO_CTL_I 8
#define RADIO_CALIB RADIO_CTL_I

#define PPM_MIN_PULSE_WIDTH 850ul*CLOCK
#define PPM_MAX_PULSE_WIDTH 2100ul*CLOCK
#define PPM_SYNC_PULSE (uint8_t)(((uint32_t)(5000ul*CLOCK))/1024ul)

#define LastRadioFromPpm() {\
  static uint8_t avg_cpt = 0; /* Counter for averaging */\
   int16_t tmp_radio;\
  tmp_radio = ppm_pulses[ RADIO_THROTTLE ] - (CLOCK*1000);\
  last_radio[ RADIO_THROTTLE ] = tmp_radio * (MAX_PPRZ / 1 / (float)(CLOCK*(2200-1000)));\
  if (last_radio[ RADIO_THROTTLE ] > MAX_PPRZ) last_radio[ RADIO_THROTTLE ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_THROTTLE ] < 0) last_radio[ RADIO_THROTTLE ] = 0; \
\
  tmp_radio = ppm_pulses[ RADIO_ROLL ] - (CLOCK*1600);\
  last_radio[ RADIO_ROLL ] = tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/1/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/1/(float)(CLOCK*(1000-1600))));\
  if (last_radio[ RADIO_ROLL ] > MAX_PPRZ) last_radio[ RADIO_ROLL ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_ROLL ] < MIN_PPRZ) last_radio[ RADIO_ROLL ] = MIN_PPRZ; \
\
  tmp_radio = ppm_pulses[ RADIO_PITCH ] - (CLOCK*1600);\
  last_radio[ RADIO_PITCH ] = tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/1/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/1/(float)(CLOCK*(1000-1600))));\
  if (last_radio[ RADIO_PITCH ] > MAX_PPRZ) last_radio[ RADIO_PITCH ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_PITCH ] < MIN_PPRZ) last_radio[ RADIO_PITCH ] = MIN_PPRZ; \
\
  tmp_radio = ppm_pulses[ RADIO_YAW ] - (CLOCK*1600);\
  last_radio[ RADIO_YAW ] = tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/1/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/1/(float)(CLOCK*(1000-1600))));\
  if (last_radio[ RADIO_YAW ] > MAX_PPRZ) last_radio[ RADIO_YAW ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_YAW ] < MIN_PPRZ) last_radio[ RADIO_YAW ] = MIN_PPRZ; \
\
  tmp_radio = ppm_pulses[ RADIO_MODE ] - (CLOCK*1600);\
  avg_last_radio[ RADIO_MODE ] += tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(1000-1600))));\
  tmp_radio = ppm_pulses[ RADIO_GAIN1 ] - (CLOCK*1600);\
  avg_last_radio[ RADIO_GAIN1 ] += tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(1000-1600))));\
  tmp_radio = ppm_pulses[ RADIO_GAIN2 ] - (CLOCK*1600);\
  avg_last_radio[ RADIO_GAIN2 ] += tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(1000-1600))));\
  tmp_radio = ppm_pulses[ RADIO_LLS ] - (CLOCK*1600);\
  avg_last_radio[ RADIO_LLS ] += tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(1000-1600))));\
  tmp_radio = ppm_pulses[ RADIO_CALIB ] - (CLOCK*1600);\
  avg_last_radio[ RADIO_CALIB ] += tmp_radio * (tmp_radio >=0 ? (MAX_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(2200-1600))) : (MIN_PPRZ/AVERAGING_PERIOD/(float)(CLOCK*(1000-1600))));\
avg_cpt++;\
  if (avg_cpt == AVERAGING_PERIOD) {\
    avg_cpt = 0;\
    last_radio[ RADIO_MODE ] = avg_last_radio[ RADIO_MODE ];\
    avg_last_radio[ RADIO_MODE ] = 0;\
  if (last_radio[ RADIO_MODE ] > MAX_PPRZ) last_radio[ RADIO_MODE ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_MODE ] < MIN_PPRZ) last_radio[ RADIO_MODE ] = MIN_PPRZ; \
\
    last_radio[ RADIO_GAIN1 ] = avg_last_radio[ RADIO_GAIN1 ];\
    avg_last_radio[ RADIO_GAIN1 ] = 0;\
  if (last_radio[ RADIO_GAIN1 ] > MAX_PPRZ) last_radio[ RADIO_GAIN1 ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_GAIN1 ] < MIN_PPRZ) last_radio[ RADIO_GAIN1 ] = MIN_PPRZ; \
\
    last_radio[ RADIO_GAIN2 ] = avg_last_radio[ RADIO_GAIN2 ];\
    avg_last_radio[ RADIO_GAIN2 ] = 0;\
  if (last_radio[ RADIO_GAIN2 ] > MAX_PPRZ) last_radio[ RADIO_GAIN2 ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_GAIN2 ] < MIN_PPRZ) last_radio[ RADIO_GAIN2 ] = MIN_PPRZ; \
\
    last_radio[ RADIO_LLS ] = avg_last_radio[ RADIO_LLS ];\
    avg_last_radio[ RADIO_LLS ] = 0;\
  if (last_radio[ RADIO_LLS ] > MAX_PPRZ) last_radio[ RADIO_LLS ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_LLS ] < MIN_PPRZ) last_radio[ RADIO_LLS ] = MIN_PPRZ; \
\
    last_radio[ RADIO_CALIB ] = avg_last_radio[ RADIO_CALIB ];\
    avg_last_radio[ RADIO_CALIB ] = 0;\
  if (last_radio[ RADIO_CALIB ] > MAX_PPRZ) last_radio[ RADIO_CALIB ] = MAX_PPRZ;\
 else if (last_radio[ RADIO_CALIB ] < MIN_PPRZ) last_radio[ RADIO_CALIB ] = MIN_PPRZ; \
\
    last_radio_contains_avg_channels = TRUE;\
 }\
}

#endif // RADIO_H
