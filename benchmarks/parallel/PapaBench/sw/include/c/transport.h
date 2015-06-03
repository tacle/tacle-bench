#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <glib.h>

#include <string.h>


#define TRANSPORT_BUF_LEN 1024
#define TRANSPORT_HEAD_LEN       1
#define TRANSPORT_PAYLOAD_OFFSET TRANSPORT_HEAD_LEN
#define TRANSPORT_TAIL_LEN       2

enum TransportError {
  TRANSPORT_NO_ERROR,
  TRANSPORT_CHECKSUM_ERROR,
  TRANSPORT_NO_ETX,
  TRANSPORT_INVALID_MSG_ID,
  TRANSPORT_INVALID_MSG_LEN,
  TRANSPORT_BUF_OVFW,
  TRANSPORT_ERROR_NB
};

struct TransportMsg {
  GTimeVal date;
  guint len;
  guchar* data;
};

struct TransportStatus {
  guint32 run_time;
  guint32 nb_byte;
  guint32 nb_msg;
  guint32 nb_err;
  gfloat  byte_rate;
  gfloat  msg_rate;
};

struct Transport {
  void(*err_callback)(gpointer callback_data, GError *error);
  void(*msg_callback)(gpointer callback_data, struct TransportMsg* msg);
  gpointer callback_data;
  GQuark quark;
  gchar stx;
  gchar etx;
  gboolean fixed_size;
  gboolean two_bytes_checksum;
  guint nb_msg_type;
  guint* size_msg;
  guint max_msg_size;
  struct TransportStatus status;
  gulong nb_msg_last_status;
  gulong nb_byte_last_status;
  guchar buf[TRANSPORT_BUF_LEN];
  guint buf_len;
  GTimeVal start_date, last_status_date;
  
};

struct Transport* transport_new(gboolean fixed_size, gboolean two_bytes_checksum, guint nb_msg, guint* size_msg, guint max_msg_size,
				guchar stx, guchar etx,
				void(*err_callback)(gpointer callback_data, GError *error), 
				void(*msg_callback)(gpointer callback_data, struct TransportMsg* msg),
				gpointer callback_data);

struct TransportStatus* transport_get_status(struct Transport* this);

void transport_feed_data(struct Transport* this, const guchar* buf, guint len);

void transport_free(struct Transport* this);


#endif /* TRANSPORT_H */
