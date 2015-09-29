/* 
   $Id: protocol.h,v 1.1 2011-01-18 12:48:44 moellmer Exp $
   Copyright (C) 2003 Pascal Brisset, Antoine Drouin

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

#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <glib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

enum PprzFieldType {
  TYP_UINT_8,
  TYP_UINT_16,
  TYP_UINT_32,
  TYP_INT_8,
  TYP_INT_16,
  TYP_INT_32,
  TYP_FLOAT,
  TYP_ARRAY_UINT_8,
  TYP_NB
};

struct PprzFieldClass {
  gchar* name;
  enum PprzFieldType type;
  guint offset;
  gchar* format;
  gchar* unit;
  gchar* description;
};

struct PprzMsgClass {
  guint  id;
  gchar* name;
  GList* fields_classes;
  GHashTable* fields_classes_by_name;
  struct PprzFieldClass** fields_classes_by_id;
  guint nb_fields_classes;
  guint size; /* size in byte off all fields + 1 for id */
};

struct PprzMsg {
  GTimeVal date;
  struct PprzMsgClass* class;
  guchar* bytes;
};

struct PprzProtocol {
  GList* msgs_classes;
  GHashTable* msgs_classes_by_name;
  struct PprzMsgClass** msgs_classes_by_id;
  guint nb_msgs_classes;
};

struct PprzProtocol* pprz_protocol_new_from_xml(xmlDocPtr doc, xmlNodePtr cur);
void pprz_protocol_free(struct PprzProtocol* this);

struct PprzMsg* pprz_msg_new(struct PprzMsgClass* msg_class, GTimeVal date); 
struct PprzMsg* pprz_protocol_msg_new_by_id(struct PprzProtocol* this, guchar id, GTimeVal date); 
struct PprzMsg* pprz_protocol_msg_new_by_name(struct PprzProtocol* this, const gchar* name, GTimeVal date); 
struct PprzMsg* pprz_protocol_msg_new_of_bin(struct PprzProtocol* this, const guchar* buf, GTimeVal date);
struct PprzMsg* pprz_protocol_msg_new_of_ascii(struct PprzProtocol* this, const gchar* line);
void pprz_msg_free(struct PprzMsg* msg);

void pprz_protocol_ascii_of_msg(struct PprzMsg* msg, GString** buf);

const gchar* pprz_protocol_str_of_field_type(enum PprzFieldType type);
const guint  pprz_protocol_size_of_field_type(enum PprzFieldType type);

const struct PprzMsgClass* protocol_msg_class_of_id(struct PprzProtocol* this, guint id);

void pprz_msg_ascii_of_field(struct PprzMsg* msg, struct PprzFieldClass* field_class, GString* buf);

gpointer pprz_msg_get_field(struct PprzMsg* msg, struct PprzFieldClass* field_class);
gpointer pprz_msg_get_field_by_name(struct PprzMsg* msg, const gchar* name);
gpointer pprz_msg_get_field_by_place(struct PprzMsg* msg, guint place);

gboolean pprz_msg_set_field(struct PprzMsg* msg, struct PprzFieldClass* field_class, gconstpointer value);
gboolean pprz_msg_set_field_by_name(struct PprzMsg* msg, const gchar* name, gconstpointer value);
gboolean pprz_msg_set_field_by_id(struct PprzMsg* msg, guint id, gconstpointer value);

gboolean pprz_msg_set_field_ascii(struct PprzMsg* msg, struct PprzFieldClass* field_class, const gchar* str_val);
gboolean pprz_msg_set_field_ascii_by_name(struct PprzMsg* msg, const gchar* name, const gchar* str_val);
gboolean pprz_msg_set_field_ascii_by_id(struct PprzMsg* msg, guint id, const gchar* str_val);


#endif /* PROTOCOL_H */
