/* Automatically generated from conf/messages.xml */
/* Please DO NOT EDIT */
#define DL_ID 0
#define DL_BOOT 1
#define DL_CALIB_START 2
#define DL_CALIB_CONTRAST 3
#define DL_TAKEOFF 4
#define DL_RAD_OF_IR 5
#define DL_ATTITUDE 6
#define DL_ADC 7
#define DL_GPS 8
#define DL_NAVIGATION_REF 9
#define DL_NAVIGATION 10
#define DL_PPRZ_MODE 11
#define DL_BAT 12
#define DL_DEBUG 13
#define DL_CLIMB_PID 14
#define DL_DOWNLINK_STATUS 15
#define DL_MODEM_STATUS 16
#define DL_SETTINGS 17
#define DL_DESIRED 18
#define DL_WIND 19
#define DL_IMU 20
#define DL_RAW_IMU 21
#define DL_KALMAN 22
#define DL_MSG_NB 23

#define DOWNLINK_SEND_ID(md5sum){ \
	if (MODEM_CHECK_FREE_SPACE(20)) {\
	  ModemStartMessage(DL_ID) \
	  {\
	    int i;\
	    for(i = 0; i < 16; i++) {\
	    MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(&md5sum[i])); \
	  }\
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_BOOT(version){ \
	if (MODEM_CHECK_FREE_SPACE(6)) {\
	  ModemStartMessage(DL_BOOT) \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(version)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_CALIB_START(){ \
	if (MODEM_CHECK_FREE_SPACE(4)) {\
	  ModemStartMessage(DL_CALIB_START) \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_CALIB_CONTRAST(adc){ \
	if (MODEM_CHECK_FREE_SPACE(6)) {\
	  ModemStartMessage(DL_CALIB_CONTRAST) \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(adc)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_TAKEOFF(cpu_time){ \
	if (MODEM_CHECK_FREE_SPACE(6)) {\
	  ModemStartMessage(DL_TAKEOFF) \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(cpu_time)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_RAD_OF_IR(ir, rad, rad_of_ir, ir_roll_ntrl, ir_pitch_ntrl){ \
	if (MODEM_CHECK_FREE_SPACE(20)) {\
	  ModemStartMessage(DL_RAD_OF_IR) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(ir)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rad)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rad_of_ir)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(ir_roll_ntrl)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(ir_pitch_ntrl)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_ATTITUDE(phi, psi, theta){ \
	if (MODEM_CHECK_FREE_SPACE(16)) {\
	  ModemStartMessage(DL_ATTITUDE) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(phi)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(psi)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(theta)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_ADC(roll, pitch){ \
	if (MODEM_CHECK_FREE_SPACE(8)) {\
	  ModemStartMessage(DL_ADC) \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(roll)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(pitch)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_GPS(mode, east, north, course, alt, speed, climb, tow){ \
	if (MODEM_CHECK_FREE_SPACE(33)) {\
	  ModemStartMessage(DL_GPS) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(mode)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(east)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(north)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(course)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(alt)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(speed)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(climb)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(tow)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_NAVIGATION_REF(utm_east, utm_north){ \
	if (MODEM_CHECK_FREE_SPACE(12)) {\
	  ModemStartMessage(DL_NAVIGATION_REF) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(utm_east)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(utm_north)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_NAVIGATION(cur_block, cur_stage, pos_x, pos_y, desired_course, dist2_wp, course_pgain, dist2_home){ \
	if (MODEM_CHECK_FREE_SPACE(30)) {\
	  ModemStartMessage(DL_NAVIGATION) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(cur_block)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(cur_stage)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(pos_x)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(pos_y)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(desired_course)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(dist2_wp)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(course_pgain)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(dist2_home)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_PPRZ_MODE(ap_mode, ap_altitude, if_calib_mode, mcu1_status, lls_calib){ \
	if (MODEM_CHECK_FREE_SPACE(9)) {\
	  ModemStartMessage(DL_PPRZ_MODE) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(ap_mode)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(ap_altitude)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(if_calib_mode)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(mcu1_status)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(lls_calib)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_BAT(voltage, flight_time, low_battery, block_time, stage_time){ \
	if (MODEM_CHECK_FREE_SPACE(12)) {\
	  ModemStartMessage(DL_BAT) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(voltage)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(flight_time)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(low_battery)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(block_time)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(stage_time)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_DEBUG(i2c_nb_err, i2c_mcu1_nb_err, modem_nb_err, gps_nb_err, ppm_rate){ \
	if (MODEM_CHECK_FREE_SPACE(9)) {\
	  ModemStartMessage(DL_DEBUG) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(i2c_nb_err)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(i2c_mcu1_nb_err)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(modem_nb_err)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(gps_nb_err)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(ppm_rate)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_CLIMB_PID(gaz, climb, sum_err, p_gain){ \
	if (MODEM_CHECK_FREE_SPACE(18)) {\
	  ModemStartMessage(DL_CLIMB_PID) \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(gaz)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(climb)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(sum_err)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(p_gain)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_DOWNLINK_STATUS(run_time, rx_bytes, rx_msgs, rx_err, rx_bytes_rate, rx_msgs_rate){ \
	if (MODEM_CHECK_FREE_SPACE(28)) {\
	  ModemStartMessage(DL_DOWNLINK_STATUS) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(run_time)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rx_bytes)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rx_msgs)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rx_err)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rx_bytes_rate)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(rx_msgs_rate)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_MODEM_STATUS(detected, valim, cd, nb_byte, nb_msg, nb_err){ \
	if (MODEM_CHECK_FREE_SPACE(22)) {\
	  ModemStartMessage(DL_MODEM_STATUS) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(detected)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(valim)); \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(cd)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(nb_byte)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(nb_msg)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(nb_err)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_SETTINGS(mode, slider_1_val, slider_2_val){ \
	if (MODEM_CHECK_FREE_SPACE(13)) {\
	  ModemStartMessage(DL_SETTINGS) \
	  MODEM_PUT_1_BYTE_BY_ADDR((uint8_t*)(mode)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(slider_1_val)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(slider_2_val)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_DESIRED(roll, pitch, desired_x, desired_y, desired_altitude){ \
	if (MODEM_CHECK_FREE_SPACE(24)) {\
	  ModemStartMessage(DL_DESIRED) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(roll)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(pitch)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(desired_x)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(desired_y)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(desired_altitude)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_WIND(dir, speed, mean_as, nb_sample, stddev){ \
	if (MODEM_CHECK_FREE_SPACE(22)) {\
	  ModemStartMessage(DL_WIND) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(dir)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(speed)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(mean_as)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(nb_sample)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(stddev)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_IMU(gyro_x, gyro_y, gyro_z, accel_x, accel_y, accel_z){ \
	if (MODEM_CHECK_FREE_SPACE(28)) {\
	  ModemStartMessage(DL_IMU) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(gyro_x)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(gyro_y)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(gyro_z)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(accel_x)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(accel_y)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(accel_z)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_RAW_IMU(raw_gx, raw_gy, raw_gz, raw_ax, raw_ay, raw_az){ \
	if (MODEM_CHECK_FREE_SPACE(16)) {\
	  ModemStartMessage(DL_RAW_IMU) \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(raw_gx)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(raw_gy)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(raw_gz)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(raw_ax)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(raw_ay)); \
	  MODEM_PUT_2_BYTE_BY_ADDR((uint8_t*)(raw_az)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define DOWNLINK_SEND_KALMAN(phi, phi_dot, phi_bias, theta, theta_dot, theta_bias){ \
	if (MODEM_CHECK_FREE_SPACE(28)) {\
	  ModemStartMessage(DL_KALMAN) \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(phi)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(phi_dot)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(phi_bias)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(theta)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(theta_dot)); \
	  MODEM_PUT_4_BYTE_BY_ADDR((uint8_t*)(theta_bias)); \
	  ModemEndMessage() \
	} \
	else \
	  modem_nb_ovrn++; \
}

#define MESSAGES_MD5SUM "\120\162\150\107\166\250\102\343\211\352\231\260\061\055\031\274"
// Load: intant(buffer) 16(37) 8(37) 13(37) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 9(58) 16(58) 8(58) 13(58) 24(70) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 9(58) 16(58) 8(58) 13(58) 24(70) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 12(61) 16(61) 8(61) 13(61) 24(73) 0(61) 16(61) 8(61) 13(61) 18(55) 9(64) 16(64) 8(64) 13(64) 24(70) 0(61) 16(61) 8(61) 13(61) 12(49) 0(49) 16(49) 8(49) 13(49) 24(61) 0(61) 16(61) 8(61) 13(61) 18(55) 9(64) 16(64) 8(64) 13(64) 24(70) 0(61) 16(61) 8(61) 13(61) 12(49) 0(49) 16(49) 8(49) 13(49) 24(61) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 9(58) 16(58) 8(58) 13(58) 24(70) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 9(58) 16(58) 8(58) 13(58) 24(70) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 12(61) 16(61) 8(61) 13(61) 24(73) 0(61) 16(61) 8(61) 13(61) 18(55) 9(64) 16(64) 8(64) 13(64) 24(70) 0(61) 16(61) 8(61) 13(61) 12(49) 0(49) 16(49) 8(49) 13(49) 24(61) 0(61) 16(61) 8(61) 13(61) 18(55) 9(64) 16(64) 8(64) 13(64) 24(70) 0(61) 16(61) 8(61) 13(61) 12(49) 0(49) 16(49) 8(49) 13(49) 24(61) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 9(58) 16(58) 8(58) 13(58) 24(70) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 12(49) 9(58) 16(58) 8(58) 13(58) 24(70) 0(61) 16(61) 8(61) 13(61) 18(55) 0(55) 16(55) 8(55) 13(55) 24(61) 0(61) 16(61) 8(61) 13(61) 0(37) 0(37)

/*#define PeriodicSend() {  // 10Hz // \
  static uint8_t i;\
  i++; if (i == 250) i = 0;\
  if (i % 5 == 0) PERIODIC_SEND_ATTITUDE();\
  if (i % 5 == 1) PERIODIC_SEND_ADC();\
  if (i % 5 == 2) PERIODIC_SEND_SETTINGS();\
  if (i % 10 == 3) PERIODIC_SEND_DESIRED();\
  if (i % 20 == 8) PERIODIC_SEND_BAT();\
  if (i % 20 == 18) PERIODIC_SEND_CLIMB_PID();\
  if (i % 50 == 9) PERIODIC_SEND_PPRZ_MODE();\
  if (i % 50 == 29) PERIODIC_SEND_DEBUG();\
  if (i % 100 == 49) PERIODIC_SEND_NAVIGATION_REF();\
}*/
