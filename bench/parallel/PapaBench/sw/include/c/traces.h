/*
   traces.h
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

#ifndef TRACES_H
#define TRACES_H

#define TRACE_ON
#define TIME_STAMP_TRACE

#ifdef TRACE_ON
#include <stdio.h>

#define TRACE_ERROR                   0x00000001L
#define TRACE_DATA_READ               0x00000002L
#define TRACE_MSG_RAW                 0x00000004L
#define TRACE_MSG                     0x00000008L
#define TRACE_CHECKSUM                0x00000010L
#define TRACE_DOWNLINK                0x00000020L
#define TRACE_MODEM                   0x00000040L
#define TRACE_MODEM_VERB              0x00000080L
#define TRACE_MODEM_READ              0x00000100L
#define TRACE_PARSER                  0x00000200L
#define TRACE_TRANSPORT               0x00000400L
#define TRACE_TRANSPORT_VERB          0x00000800L

extern unsigned long TraceLevel;

#define TRACE(type,fmt,args...)                         \
{                                                       \
  if (TraceLevel&type) {                                \
    fprintf(stderr, "[ "#type" ] " fmt, ##args);          \
  }                                                     \
}
const char *print_hex( const unsigned char *buf, unsigned int len );

#else
#define TRACE(type,fmt,args...)
#endif /* TRACE_ON */

#endif /* TRACES_H */

