#include "transport.h"


#include "geometry.h"
#include "traces.h"

static void parse_buf( struct Transport *this );
static gboolean check_checksum( struct Transport *this, guint msg_len );
static void remove_n_from_buf( struct Transport *this, guint len );
static void remove_until_stx( struct Transport *this );

struct Transport *transport_new( gboolean fixed_size,
                                 gboolean  two_bytes_checksum, guint nb_msg,
                                 guint *size_msg, guint max_msg_size,
                                 guchar stx, guchar etx,
                                 void( *err_callback )( gpointer callback_data, GError *error ),
                                 void( *msg_callback )( gpointer callback_data, struct TransportMsg *msg ),
                                 gpointer callback_data )
{
  struct Transport *this = g_new( struct Transport, 1 );
  if ( this ) {
    this->stx = stx;
    this->etx = etx;
    this->err_callback = err_callback;
    this->msg_callback = msg_callback;
    this->callback_data = callback_data;
    this->buf_len = 0;
    this->fixed_size = fixed_size;
    this->two_bytes_checksum = two_bytes_checksum;
    this->quark = g_quark_from_string( "Transport" );
    g_get_current_time( &this->start_date );
    if ( fixed_size ) {
      this->nb_msg_type = nb_msg;
      this->size_msg = size_msg;
      //this->max_msg_size = FIXME find max size in array;;
    } else
      this->max_msg_size = max_msg_size;
    this->nb_byte_last_status = 0;
    this->nb_msg_last_status = 0;
    this->status.nb_byte = 0;
    this->status.nb_msg  = 0;
    this->status.nb_err  = 0;
  }
  return this;
}

#define GDOUBLE_FROM_DIF_TV(a,b) ((gdouble)(a.tv_sec-b.tv_sec) + 1e-6 * (gdouble)(a.tv_usec-b.tv_usec))

struct TransportStatus *transport_get_status( struct Transport *this )
{
  GTimeVal now;
  gdouble duration;

  g_get_current_time( &now );
  this->status.run_time = DELAY_SEC_OF_TIMEVAL( now, this->start_date );

  duration = GDOUBLE_FROM_DIF_TV( now, this->last_status_date );
  this->status.byte_rate = this->nb_byte_last_status / duration;
  this->status.msg_rate = this->nb_msg_last_status / duration;

  this->nb_byte_last_status = 0;
  this->nb_msg_last_status = 0;
  this->last_status_date = now;

  return &this->status;
}



void transport_free( struct Transport *this )
{
  g_free( this );
}

void transport_feed_data( struct Transport *this, const guchar *buf,
                          guint len )
{
  if ( len >= TRANSPORT_BUF_LEN - this->buf_len ) {
    GError *err = g_error_new( this->quark,
                               TRANSPORT_BUF_OVFW,  /* guint code */
                               "buffer overflow" /* const gchar *format */
                             );
    this->err_callback( this->callback_data, err );
    g_error_free( err );
  } else {
    this->status.nb_byte += len;
    this->nb_byte_last_status += len;
    memcpy( this->buf + this->buf_len, buf, len );
    this->buf_len += len;
    parse_buf( this );
  }
}

static gboolean check_checksum( struct Transport *this, guint msg_len )
{
  guint checksum_idx = msg_len - TRANSPORT_TAIL_LEN;
  guint i;
  if ( this->two_bytes_checksum ) {
    guchar cka = 0, ckb = 0;
    for ( i = TRANSPORT_PAYLOAD_OFFSET; i < checksum_idx; i++ ) {
      cka += this->buf[ i ];
      ckb += cka;
    }
    return ( cka == this->buf[ checksum_idx ] &&
             ckb ==  this->buf[ checksum_idx + 1 ] );
  } else {
    guchar checksum = 0;
    for ( i = TRANSPORT_PAYLOAD_OFFSET; i < checksum_idx; i++ )
      checksum ^= this->buf[ i ];
    if ( checksum != this->buf[ checksum_idx ] )
      TRACE( TRACE_ERROR, "transport checksum error (found : 0x%02X read : 0x%02X)\n",
             checksum, this->buf[ checksum_idx ] );
    return checksum == this->buf[ checksum_idx ];
  }
}

static void remove_n_from_buf( struct Transport *this, guint len )
{
  memmove( this->buf, this->buf + len, this->buf_len - len );
  this->buf_len -= len;
}

static void remove_until_stx( struct Transport *this )
{
  guchar *stx = memchr( this->buf, this->stx, this->buf_len );
  if ( stx ) {
    if ( stx != this->buf ) {
      memmove( this->buf, stx, this->buf_len - ( stx - this->buf ) );
      this->buf_len -= ( stx - this->buf );
    }
  } else
    this->buf_len = 0;
}

static gboolean get_msg_len( struct Transport *this, guint *len )
{
  if ( this->fixed_size ) {
    guint type = this->buf[ TRANSPORT_PAYLOAD_OFFSET ];
    if ( type >= this->nb_msg_type ) {             /* BAP ID     */
      TRACE( TRACE_TRANSPORT, "%d bad type\n", this->buf[ TRANSPORT_PAYLOAD_OFFSET ] );
      TRACE( TRACE_TRANSPORT, "parse_buf %s\n", print_hex( this->buf,
             this->buf_len ) );
      this->status.nb_err++;
      {
        GError *err = g_error_new( this->quark,
                                   TRANSPORT_INVALID_MSG_ID, /* guint code */
                                   "invalid msg id" /* const gchar *format */
                                 );
        this->err_callback( this->callback_data, err );
        g_error_free( err );
      }
      return FALSE;
    } else *len = this->size_msg[ type ] + TRANSPORT_HEAD_LEN + TRANSPORT_TAIL_LEN;
  } else {
    *len = this->buf[ TRANSPORT_PAYLOAD_OFFSET ] + TRANSPORT_HEAD_LEN +
           TRANSPORT_TAIL_LEN;
    if ( *len >= this->max_msg_size ) {             /* BAD LEN    */
      TRACE( TRACE_TRANSPORT, "%d bad len\n", this->buf[ TRANSPORT_PAYLOAD_OFFSET ] );
      this->status.nb_err++;
      {
        GError *err = g_error_new( this->quark,
                                   TRANSPORT_INVALID_MSG_LEN, /* guint code */
                                   "invalid message len" /* const gchar *format */
                                 );
        this->err_callback( this->callback_data, err );
        g_error_free( err );
      }
      return FALSE;
    }
  }
  return TRUE;
}


static void parse_buf( struct Transport *this )
{
  /* make sure first byte in buffer is STX */
  remove_until_stx( this );
  //  if (this->fixed_size)
  //    TRACE(TRACE_TRANSPORT, "parse_buf %s\n", print_hex(this->buf, this->buf_len));
  if ( this->buf_len < TRANSPORT_PAYLOAD_OFFSET )             /* NO PAYLOAD */
    TRACE( TRACE_TRANSPORT_VERB, "no payload\n" );
  else {
    guint msg_len;
    if ( !get_msg_len( this,
                       &msg_len ) ) {                    /* BAD LEN ( or TYPE) */
      remove_n_from_buf( this, 1 );
      parse_buf( this );
    } else {
      if ( this->buf_len < msg_len )                      /* NOT ENOUGTH DATA */
        TRACE( TRACE_TRANSPORT_VERB, "not enough data\n" );
      else {
        if ( !check_checksum( this, msg_len ) ) {
          TRACE( TRACE_TRANSPORT, "parse_buf %s\n", print_hex( this->buf,
                 this->buf_len ) );
          this->status.nb_err++;
          {
            GError *err = g_error_new( this->quark,
                                       TRANSPORT_CHECKSUM_ERROR, /* guint code */
                                       "checksum error"       /* const gchar *format */
                                     );
            this->err_callback( this->callback_data, err );
            g_error_free( err );
          }

          remove_n_from_buf( this, 1 );
          parse_buf( this );
        } else {
          if ( !this->two_bytes_checksum && this->buf[ msg_len - 1 ] != this->etx ) {
            TRACE( TRACE_TRANSPORT, "parse_buf %s\n", print_hex( this->buf,
                   this->buf_len ) );
            TRACE( TRACE_TRANSPORT_VERB, "%d is not ETX\n", this->buf[ msg_len - 1 ] );
            this->status.nb_err++;
            {
              GError *err = g_error_new( this->quark,
                                         TRANSPORT_NO_ETX,   /* guint code */
                                         "NO ETX" /* const gchar *format */
                                       );
              this->err_callback( this->callback_data, err );
              g_error_free( err );
            }
            remove_n_from_buf( this, 1 );
            parse_buf( this );
          } else {
            struct TransportMsg msg;
            guint payload_len = msg_len - TRANSPORT_HEAD_LEN - TRANSPORT_TAIL_LEN;
            g_get_current_time( &msg.date );
            msg.len = payload_len;
            msg.data = this->buf + TRANSPORT_PAYLOAD_OFFSET;
            this->status.nb_msg++;
            this->nb_msg_last_status++;

            TRACE( TRACE_TRANSPORT_VERB,
                   "transport msg date : %ld%06ld len : %d data : %s\n", msg.date.tv_sec,
                   msg.date.tv_usec, msg.len, print_hex( msg.data, msg.len ) );
            this->msg_callback( this->callback_data, &msg );
            remove_n_from_buf( this, msg_len );
            parse_buf( this );
          }
        }
      }
    }
  }
}
