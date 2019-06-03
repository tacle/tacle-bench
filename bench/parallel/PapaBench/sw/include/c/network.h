/*
   $id:$
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
  network functions
*/

#ifndef NETWORK_H
#define NETWORK_H

#include <glib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "protocol.h"

struct NetworkClient {
  int socket;
  struct sockaddr_in bcast_addr;
  struct sockaddr_in server_addr;
  GFunc rcv_callback;
  GFunc err_callback;
  gpointer user_data;
  struct PprzProtocol *protocol;
};

struct NetworkServer {
  int bcast_socket;
  struct sockaddr_in bcast_addr;
  int listening_socket;
  GFunc rcv_callback;
  GFunc err_callback;
  gpointer user_data;
  struct PprzProtocol *protocol;
};


struct NetworkServer *network_server_new( struct PprzProtocol *protocol,
    const char *bcast_addr, const int bcast_port,
    GFunc rcv_callback, GFunc err_callback, gpointer user_data );
void network_server_dispatch( struct NetworkServer *this, struct PprzMsg *msg );


struct NetworkClient *network_client_new( struct PprzProtocol *protocol,
    const char *bcast_addr, const int bcast_port,
    GFunc rcv_callback, GFunc err_callback, gpointer user_data );
void network_client_send_to_server( struct NetworkClient *this,
                                    struct PprzMsg *msg );


#endif /* NETWORK_H */
