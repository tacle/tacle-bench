/* 
   downlink.h
   Copyright (C) 2003 Antoine Drouin

 This file is part of paparazzi.

 paparazzi is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2, or (at your option)
 any later version.

 paparazzi is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with paparazzi; see the file COPYING.  If not, write to
 the Free Software Foundation, 59 Temple Place - Suite 330,
 Boston, MA 02111-1307, USA.  
*/ 

/*
  decodes downlink protocol
*/

#ifndef DOWNLINK_H
#define DOWNLINK_H

#include <glib.h>
#include "protocol.h"
#include "transport.h"
#include "geometry.h"

#define DL_STX            0x05
#define DL_ETX            0x06
#define DL_HEAD_LEN       1
#define DL_PAYLOAD_OFFSET 1
#define DL_TAIL_LEN       2


struct Transport* downlink_new(	struct PprzProtocol* protocol, 
				void(*err_callback)(gpointer callback_data, GError *error), 
				void(*msg_callback)(gpointer callback_data, struct TransportMsg* msg),
				gpointer callback_data);



#endif
