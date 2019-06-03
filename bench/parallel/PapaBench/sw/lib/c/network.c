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

#include "network.h"

#include <string.h>
#include <errno.h>

#include <netdb.h>
#include <sys/types.h>

#include <sys/socket.h>
#include <arpa/inet.h>

static gboolean server_on_data_received( GIOChannel *source,
    GIOCondition condition, gpointer data );
static gboolean client_on_data_received( GIOChannel *source,
    GIOCondition condition, gpointer data );

struct NetworkServer *network_server_new( struct PprzProtocol *protocol,
    const char *bcast_addr, const int bcast_port,
    GFunc rcv_callback, GFunc err_callback, gpointer user_data )
{
  struct NetworkServer *this = g_new( struct NetworkServer, 1 );
  if ( this ) {
    GIOChannel *channel;
    struct protoent *pte = getprotobyname( "UDP" );
    static int so_broadcast = TRUE;

    this->bcast_socket = socket( PF_INET, SOCK_DGRAM, pte->p_proto );
    setsockopt( this->bcast_socket, SOL_SOCKET, SO_BROADCAST, &so_broadcast,
                sizeof( so_broadcast ) );

    this->bcast_addr.sin_family = PF_INET;
    this->bcast_addr.sin_port = htons( bcast_port );
    this->bcast_addr.sin_addr.s_addr = inet_addr( bcast_addr );

    channel = g_io_channel_unix_new( this->bcast_socket );
    g_io_add_watch ( channel, G_IO_IN | G_IO_PRI, server_on_data_received, this );

    this->protocol = protocol;

    this->rcv_callback = rcv_callback;
    this->err_callback = err_callback;
    this->user_data = user_data;
  }
  return this;
}

#define FIXME_LEN 2048

void network_server_dispatch( struct NetworkServer *this, struct PprzMsg *msg )
{
  guchar buf[ FIXME_LEN ];
  GTimeVal *date = ( GTimeVal * )buf;
  guchar *type = buf + sizeof( GTimeVal );
  guchar *data = type + 1;
  guint len = sizeof( GTimeVal ) + msg->class->size;
  *date = msg->date;
  *type = msg->class->id;
  memcpy( data, msg->bytes, msg->class->size - 1 );
  if ( sendto( this->bcast_socket, buf, len, 0,
               ( struct sockaddr * )&this->bcast_addr, sizeof( this->bcast_addr ) ) == -1 )
    g_message( "error broadcasting" );
}

struct NetworkClient *network_client_new( struct PprzProtocol *protocol,
    const char *bcast_addr, const int bcast_port,
    GFunc rcv_callback, GFunc err_callback, gpointer user_data )
{
  struct NetworkClient *this = g_new( struct NetworkClient, 1 );
  if ( this ) {
    int so_reuseaddr = TRUE;
    GIOChannel *channel;
    struct protoent *pte = getprotobyname( "UDP" );

    this->socket = socket( PF_INET, SOCK_DGRAM, pte->p_proto );
    setsockopt( this->socket, SOL_SOCKET, SO_REUSEADDR, &so_reuseaddr,
                sizeof( so_reuseaddr ) );
    this->bcast_addr.sin_family = PF_INET;
    this->bcast_addr.sin_port = htons( bcast_port );
    this->bcast_addr.sin_addr.s_addr = inet_addr( bcast_addr );
    bind( this->socket, ( struct sockaddr * )&this->bcast_addr,
          sizeof( this->bcast_addr ) );

    channel =  g_io_channel_unix_new( this->socket );
    g_io_add_watch ( channel, G_IO_IN | G_IO_PRI, client_on_data_received, this );

    this->protocol = protocol;

    this->rcv_callback = rcv_callback;
    this->err_callback = err_callback;
    this->user_data = user_data;
  }
  return this;
}

void network_client_send_to_server( struct NetworkClient *this,
                                    struct PprzMsg *msg )
{
  guchar buf[ FIXME_LEN ];
  GTimeVal *date = ( GTimeVal * )buf;
  guchar *type = buf + sizeof( GTimeVal );
  guchar *data = type + 1;
  guint len = sizeof( GTimeVal ) + msg->class->size;
  *date = msg->date;
  *type = msg->class->id;
  memcpy( data, msg->bytes, msg->class->size - 1 );
  if ( sendto( this->socket, buf, len, 0,
               ( struct sockaddr * )&this->bcast_addr, sizeof( this->bcast_addr ) ) == -1 )
    g_message( "error sending to server" );
}

static gboolean server_on_data_received( GIOChannel *source,
    GIOCondition condition, gpointer data )
{
  guchar buf[ FIXME_LEN ];
  struct sockaddr_in from;
  int from_len;
  gint len;
  struct NetworkServer *this = ( struct NetworkServer * )data;
  if ( ( len = recvfrom( this->bcast_socket, buf, FIXME_LEN, 0,
                         ( struct sockaddr * )&from, &from_len ) ) == -1 ) {
    this->err_callback( NULL, this->user_data );
    return FALSE;
  } else {
    struct PprzMsg *msg;
    GTimeVal *date = ( GTimeVal * )buf;
    guchar *ptr_type = buf + sizeof( GTimeVal );
    if ( !( msg = pprz_protocol_msg_new_of_bin( this->protocol, ptr_type,
                  *date ) ) ) {
      this->err_callback( NULL, this->user_data );
      return TRUE;
    }
    this->rcv_callback( msg, this->user_data );
    //  g_message("network data received %s %d", inet_ntoa(from.sin_addr), ntohs(from.sin_port));

  }
  return TRUE;
}

static gboolean client_on_data_received( GIOChannel *source,
    GIOCondition condition, gpointer data )
{
  guchar buf[ FIXME_LEN ];
  struct sockaddr_in from;
  int from_len;
  gint len;
  struct NetworkClient *this = ( struct NetworkClient * )data;
  if ( ( len = recvfrom( this->socket, buf, FIXME_LEN, 0,
                         ( struct sockaddr * )&from, &from_len ) ) == -1 ) {
    g_message( "client_on_data_received [ %s ]", strerror( errno ) );
    this->err_callback( NULL, this->user_data );
    return TRUE;
  } else {
    struct PprzMsg *msg;
    GTimeVal *date = ( GTimeVal * )buf;
    guchar *ptr_type = buf + sizeof( GTimeVal );
    if ( !( msg = pprz_protocol_msg_new_of_bin( this->protocol, ptr_type,
                  *date ) ) ) {
      this->err_callback( NULL, this->user_data );
      return TRUE;
    }
    this->rcv_callback( msg, this->user_data );
    //  g_message("network data received %s %d", inet_ntoa(from.sin_addr), ntohs(from.sin_port));
  }
  this->server_addr = from;
  return TRUE;
}
