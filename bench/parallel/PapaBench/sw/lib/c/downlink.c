/*
   downlink.c
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


#include <string.h>

#include "downlink.h"

#define GDOUBLE_FROM_DIF_TV(a,b) ((gdouble)(a.tv_sec-b.tv_sec) + 1e-6 * (gdouble)(a.tv_usec-b.tv_usec))


#define READ_GFLOAT_FROM_BUF(dest, buf) dest = *((gfloat*)(buf)); buf += 4;
#define READ_GUINT32_FROM_BUF(dest, buf) dest = GUINT32_FROM_LE(*((guint32*)(buf))); buf += 4;
#define READ_GUINT16_FROM_BUF(dest, buf) dest = GUINT16_FROM_LE(*((guint16*)(buf))); buf += 2;
#define READ_GINT32_FROM_BUF(dest, buf) dest = GINT32_FROM_LE(*((gint32*)(buf))); buf += 4;
#define READ_GINT16_FROM_BUF(dest, buf) dest = GINT16_FROM_LE(*((gint16*)(buf))); buf += 2;
#define READ_GUINT8_FROM_BUF(dest, buf)  dest = (guint8)(*(buf++));

struct Transport *downlink_new( struct PprzProtocol *protocol,
                                void( *err_callback )( gpointer callback_data, GError *error ),
                                void( *msg_callback )( gpointer callback_data, struct TransportMsg *msg ),
                                gpointer callback_data )
{
  struct Transport *transport;
  guint *fixed_payload_len = g_new( guint,
                                    g_list_length( protocol->msgs_classes ) );
  guint i = 0;
  GList *msgs_classes = protocol->msgs_classes;
  while ( msgs_classes ) {
    fixed_payload_len[ i ] = ( ( struct PprzMsgClass * )msgs_classes->data )->size;
    //    printf("fixed_payload_len : %d %d\n", i, fixed_payload_len[ i ]);
    i++;
    msgs_classes = msgs_classes->next;
  }

  transport = transport_new( TRUE,                   /* fixed_size         */
                             TRUE,                    /* two bytes checksum */
                             i,                       /* nb_msg             */
                             fixed_payload_len,       /* size_msg           */
                             100,                     /* max_msg_size       */
                             DL_STX, DL_ETX,          /* STX, ETX           */
                             err_callback,            /* err callback       */
                             msg_callback,            /* msg callback       */
                             callback_data );         /* callback data      */
  return transport;
}
