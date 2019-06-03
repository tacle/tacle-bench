/*
   traces.c
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

#include "traces.h"

#ifdef TRACE_ON
unsigned long TraceLevel = TRACE_ERROR | TRACE_TRANSPORT
                           ; // | TRACE_MODEM_READ  ;// |  TRACE_DATA_READ | TRACE_DOWNLINK; //| TRACE_MODEM_READ;// ;//| TRACE_MSG// | TRACE_DOWNLINK;

// unsigned long TraceLevel = TRACE_DOWNLINK | TRACE_MSG | TRACE_ERROR ;

const char *print_hex( const unsigned char *buf, unsigned int len )
{
  const char d2h[  ] = "0123456789ABCDEF";
  int i = 0;
#define HEX_BUF_LEN 1024
#define FMT_LEN 3
#define FMT "%c%c "
  static unsigned char priv_buf[ HEX_BUF_LEN ];
  /*   printf("[ %d ] ", len); */
  for ( i = 0; i < len && i < HEX_BUF_LEN / FMT_LEN; i++ ) {
    //    printf(FMT, d2h[ (int)buf[ i ]/16 ], d2h[ (int)buf[ i ]%16 ]);
    sprintf( priv_buf + FMT_LEN * i, FMT, d2h[ ( int )buf[ i ] / 16 ],
             d2h[ ( int )buf[ i ] % 16 ] );
  }
  if ( len == 0 )
    priv_buf[ 0 ] = '\0';
  return priv_buf;
}

#endif


