/* 
   $Id: protocol.c,v 1.1 2011-01-18 12:48:45 moellmer Exp $
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

#include "protocol.h"
#include <stdlib.h>
#include <string.h>

struct PprzTypInfo {
  gchar* xml_name;
  guint  size;
  gchar* default_format;
};

const struct PprzTypInfo type_info[TYP_NB] = {
  {"uint8",  1, "%hhu"},
  {"uint16", 2, "%hu" },
  {"uint32", 4, "%u"  },
  {"int8",   1, "%hhd"},
  {"int16",  2, "%hd" },
  {"int32",  4, "%d"  },
  {"float",  4, "%f"  },
  {"uint8_array",  1, "%0x"}
};

static gboolean parse_xml_msg(struct PprzProtocol* this, xmlNodePtr ptr);
static gboolean parse_xml_field(struct PprzMsgClass* msg_class, xmlNodePtr ptr);
static void ascii_of_fields(struct PprzMsg* msg, GList* fields_class, GString* buf);
static gboolean field_of_ascii(GList* fields_class, gchar* line, struct PprzMsg* msg); 

static enum PprzFieldType type_of_string(const gchar* type_str);

static struct PprzMsgClass* msg_class_of_name(struct PprzProtocol* this, const gchar* name);
static struct PprzMsgClass* msg_class_of_id(struct PprzProtocol* this, guchar id);

static struct PprzFieldClass* field_class_of_name(struct PprzMsg* msg, const gchar* name);
/* static struct PprzFieldClass* field_class_of_id(struct PprzMsg* msg, guchar id); */

struct PprzProtocol* pprz_protocol_new_from_xml(xmlDocPtr doc, xmlNodePtr cur) {
  struct PprzProtocol* this;
  if (!cur) return NULL;                                  /* empty document */
  if (xmlStrcmp(cur->name, (const xmlChar *) "protocol")) /* not protocol   */
    return cur->next?pprz_protocol_new_from_xml(doc, cur->next):NULL;
  this= g_new(struct PprzProtocol, 1);
  this->msgs_classes = NULL;
  this->msgs_classes_by_name = g_hash_table_new(g_str_hash, g_str_equal);
  if (!parse_xml_msg(this, cur->xmlChildrenNode)) {
    g_free(this);
    return NULL;
  }
  this->nb_msgs_classes = g_list_length(this->msgs_classes);
  this->msgs_classes_by_id = g_new(struct PprzMsgClass*, this->nb_msgs_classes);
  {
    GList* cur = this->msgs_classes;
    guint i = 0;
    while (cur) {
      this->msgs_classes_by_id[i] = cur->data;
      i++;
      cur = cur->next;
    }
  }
  return this;
}

const gchar* pprz_protocol_str_of_field_type(enum PprzFieldType type) {
  return type_info[type].xml_name;
}

const guint pprz_protocol_size_of_field_type(enum PprzFieldType type) {
  return type_info[type].size;
}

static enum PprzFieldType type_of_string(const gchar* type_str) {
  guint ret = 0;
  while (ret<TYP_NB && strcmp(type_info[ret].xml_name, type_str))
    ret++;
  return ret;
}

struct PprzMsg* pprz_message_new(struct PprzMsgClass* msg_class, GTimeVal date) {
  struct PprzMsg* this = g_new(struct PprzMsg, 1);
  this->date = date;
  this->class = msg_class;
  this->bytes = g_new(guchar, this->class->size - 1);
  return this;
}

struct PprzMsg* pprz_protocol_msg_new_by_id(struct PprzProtocol* this, guchar id, GTimeVal date) {
  struct PprzMsgClass* msg_class = msg_class_of_id(this, id);
  if (msg_class) return pprz_message_new(msg_class, date);
  return NULL;
}

struct PprzMsg* pprz_protocol_msg_new_by_name(struct PprzProtocol* this, const gchar* name, GTimeVal date) {
  struct PprzMsgClass* msg_class = msg_class_of_name(this, name);
  if (msg_class) return pprz_message_new(msg_class, date);
  return NULL;
}

struct PprzMsg* pprz_protocol_msg_new_of_bin(struct PprzProtocol* this, const guchar* buf, GTimeVal date) {
  struct PprzMsg* msg = pprz_protocol_msg_new_by_id(this, buf[0], date);
  if (msg)
    memcpy(msg->bytes, buf+1, msg->class->size-1);
  return msg;
}


struct PprzMsg* pprz_protocol_msg_new_of_ascii(struct PprzProtocol* this, const gchar* line) {
  gchar* kw;
  GTimeVal date;
  struct PprzMsg* msg;
  if (sscanf(line, "%ld.%06ld %as", &date.tv_sec, &date.tv_usec, &kw) != 3)
    return NULL;
  if (!(msg = pprz_protocol_msg_new_by_name(this, kw, date)))
    return NULL;
  if(msg->class->fields_classes) {      // message has arguments
    gchar* space = index(line, ' ');
    g_assert(space); g_assert(space+1); // checked before by sscanf
    space = index(space+1, ' ');
    g_assert(space);                    // checked before by sscanf
    if (!field_of_ascii(msg->class->fields_classes, space, msg)) {
      pprz_msg_free(msg);
      return NULL;
    }
  }
  return msg;
}

void pprz_msg_free(struct PprzMsg* this) {
  g_free(this->bytes);
  g_free(this);
}

gboolean pprz_msg_set_field(struct PprzMsg* msg, struct PprzFieldClass* field_class, gconstpointer value) {
/*#ifdef WITH_SWITCH
  switch (field_class->type) {
  case TYP_UINT_8:
    *(guint8*)(msg->bytes+field_class->offset) = *(guint8*)value;
    break;
  case TYP_UINT_16:
    *(guint16*)(msg->bytes+field_class->offset) = *(guint16*)value;
    break;
  case  TYP_UINT_32:
    *(guint32*)(msg->bytes+field_class->offset) = *(guint32*)value;
    break;
  case  TYP_INT_8:
    *(gint8*)(msg->bytes+field_class->offset) = *(gint8*)value;
    break;
  case  TYP_INT_16:
    *(gint16*)(msg->bytes+field_class->offset) = *(gint16*)value;
    break;
  case  TYP_INT_32:
    *(gint32*)(msg->bytes+field_class->offset) = *(gint32*)value;
    break;
  case  TYP_FLOAT:
    *(gfloat*)(msg->bytes+field_class->offset) = *(gfloat*)value;
    break;
  case  TYP_ARRAY_UINT_8:
    //    g_string_append_printf(buf, format, *((gfloat*)data));
    break;
  default:
    g_warning("in ascii of field : unknown type");
  }
#else*/
  if(field_class->type == TYP_UINT_8)
  {
    *(guint8*)(msg->bytes+field_class->offset) = *(guint8*)value;
    break;
  }
  else if (field_class->type == TYP_UINT_16)
  {
    *(guint16*)(msg->bytes+field_class->offset) = *(guint16*)value;
    break;
  }
  else if (field_class->type == TYP_UINT_32)
  {
    *(guint32*)(msg->bytes+field_class->offset) = *(guint32*)value;
    break;
  }
  else if (field_class->type == TYP_INT_8)
  {
    *(gint8*)(msg->bytes+field_class->offset) = *(gint8*)value;
    break;
  }
  else if(field_class->type == TYP_INT_16)
  {
    *(gint16*)(msg->bytes+field_class->offset) = *(gint16*)value;
    break;
  }
  else if (field_class->type == TYP_INT_32)
  {
    *(gint32*)(msg->bytes+field_class->offset) = *(gint32*)value;
    break;
  }
  else if (field_class->type == TYP_FLOAT)
  {
    *(gfloat*)(msg->bytes+field_class->offset) = *(gfloat*)value;
    break;
  }
  else if (field_class->type == TYP_ARRAY_UINT_8)
  {
    //    g_string_append_printf(buf, format, *((gfloat*)data));
    break;
  }
  else 
  {
    g_warning("in ascii of field : unknown type");
  }
//#endif
  return TRUE;
}

gboolean pprz_msg_set_field_by_name(struct PprzMsg* msg, const gchar* name, gconstpointer value) {
  struct PprzFieldClass* field_class = field_class_of_name(msg, name);
  if (field_class)
    return pprz_msg_set_field(msg, field_class, value);
  return FALSE;
}

gboolean pprz_msg_set_field_by_id(struct PprzMsg* msg, guint id, gconstpointer value) {
  GList* cell = g_list_nth(msg->class->fields_classes, id);
  if (cell)
    return pprz_msg_set_field(msg, cell->data, value);
  return FALSE;
}


gpointer pprz_msg_get_field(struct PprzMsg* msg, struct PprzFieldClass* field_class) {
  return msg->bytes + field_class->offset;
}

gpointer pprz_msg_get_field_by_name(struct PprzMsg* msg, const gchar* name) {
  struct PprzFieldClass* field_class = field_class_of_name(msg, name);
  if (field_class)
    return pprz_msg_get_field(msg, field_class);
  g_message("pprz_msg_get_field_by_name : unknown field [%s] in msg [%s]",name, msg->class->name); 
  return NULL;
}



#define DEFAULT_LINE_LEN 256
void pprz_protocol_ascii_of_msg(struct PprzMsg* msg, GString** buf) {
  *buf = g_string_sized_new(DEFAULT_LINE_LEN);
  g_string_printf(*buf, "%ld.%06ld %s  ", msg->date.tv_sec, msg->date.tv_usec, msg->class->name);
  if (msg->class->fields_classes) ascii_of_fields(msg, msg->class->fields_classes, *buf);
  g_string_append_printf(*buf, "\n");
}

void pprz_msg_ascii_of_field(struct PprzMsg* msg, struct PprzFieldClass* field_class, GString* buf) {
  guchar* format;
  guchar* data;
  format = field_class->format;
  if(!format) format = type_info[field_class->type].default_format;
  data = msg->bytes + field_class->offset;
/*#ifdef WITH_SWITCH
  switch (field_class->type) {
  case TYP_UINT_8:
    g_string_append_printf(buf, format, *((guint8*)data));
    break;
  case TYP_UINT_16:
    g_string_append_printf(buf, format, *((guint16*)data));
    break;
  case  TYP_UINT_32:
    g_string_append_printf(buf, format, *((guint32*)data));
    break;
  case  TYP_INT_8:
    g_string_append_printf(buf, format, *((gint8*)data));
    break;
  case  TYP_INT_16:
    g_string_append_printf(buf, format, *((gint16*)data));
    break;
  case  TYP_INT_32:
    g_string_append_printf(buf, format, *((gint32*)data));
    break;
  case  TYP_FLOAT:
    g_string_append_printf(buf, format, *((gfloat*)data));
    break;
  case  TYP_ARRAY_UINT_8:
    //    g_string_append_printf(buf, format, *((gfloat*)data));
    break;
  default:
    g_warning("in ascii of field : unknown type");
  }
#else*/
  if(field_class->type == TYP_UINT_8)
  {
    g_string_append_printf(buf, format, *((guint8*)data));
    break;
  }
  else if(field_class->type == TYP_UINT_16)
  {
    g_string_append_printf(buf, format, *((guint16*)data));
    break;
  }
  else if(field_class->type == TYP_UINT_32)
  {
    g_string_append_printf(buf, format, *((guint32*)data));
    break;
  }
  else if(field_class->type == TYP_INT_8)
  {
    g_string_append_printf(buf, format, *((gint8*)data));
    break;
  }
  else if(field_class->type == TYP_INT_16)
  {
    g_string_append_printf(buf, format, *((gint16*)data));
    break;
  }
  else if(field_class->type == TYP_INT_32)
  {
    g_string_append_printf(buf, format, *((gint32*)data));
    break;
  }
  else if(field_class->type == TYP_FLOAT)
  {
    g_string_append_printf(buf, format, *((gfloat*)data));
    break;
  }
  else if(field_class->type == TYP_ARRAY_UINT_8)
  {
    //    g_string_append_printf(buf, format, *((gfloat*)data));
    break;
  }
  else
    g_warning("in ascii of field : unknown type");
//#endif
}


static gboolean field_of_ascii(GList* fields_classes, gchar* line, struct PprzMsg* msg) {
  struct PprzFieldClass* field_class = (struct PprzFieldClass*)fields_classes->data;
  gchar* end_ptr;
  gchar* ptr_field = msg->bytes + field_class->offset;
/*#ifdef WITH_SWITCH
  switch (field_class->type) {
  case TYP_UINT_8:
    *((guint8*)ptr_field) = (guint8)strtoul(line, &end_ptr, 10);
    break;
  case TYP_UINT_16:
    *((guint16*)ptr_field) = (guint16)strtoul(line, &end_ptr, 10);
    break;
  case TYP_UINT_32:
    *((guint32*)ptr_field) = (guint32)strtoul(line, &end_ptr, 10);
    break;
  case TYP_INT_8:
    *((gint8*)ptr_field) = (gint8)strtol(line, &end_ptr, 10);
    break;
  case TYP_INT_16:
    *((gint16*)ptr_field) = (gint16)strtol(line, &end_ptr, 10);
    break;
  case TYP_INT_32:
    *((gint32*)ptr_field) = (gint32)strtol(line, &end_ptr, 10);
    break;
  case TYP_FLOAT:
    *((gfloat*)ptr_field) = (gfloat)strtod(line, &end_ptr);
    break;
  case  TYP_ARRAY_UINT_8:
    break;
  default:
    printf("in field of ascii: unknown type\n");
  }
#else*/
if (field_class->type == TYP_UINT_8)
{
    *((guint8*)ptr_field) = (guint8)strtoul(line, &end_ptr, 10);
    break;
}
else if (field_class->type == TYP_UINT_16)
{
    *((guint16*)ptr_field) = (guint16)strtoul(line, &end_ptr, 10);
    break;
}
else if (field_class->type == TYP_UINT_32)
{
    *((guint32*)ptr_field) = (guint32)strtoul(line, &end_ptr, 10);
    break;
}
else if (field_class->type == TYP_INT_8)
{
    *((gint8*)ptr_field) = (gint8)strtol(line, &end_ptr, 10);
    break;
}
else if (field_class->type == TYP_INT_16)
{
    *((gint16*)ptr_field) = (gint16)strtol(line, &end_ptr, 10);
    break;
}
else if (field_class->type == TYP_INT_32)
{
    *((gint32*)ptr_field) = (gint32)strtol(line, &end_ptr, 10);
    break;
}
else if (field_class->type == TYP_FLOAT)
{
    *((gfloat*)ptr_field) = (gfloat)strtod(line, &end_ptr);
    break;
}
else if (field_class->type == TYP_ARRAY_UINT_8)
    break;
else
    printf("in field of ascii: unknown type\n");
//#endif
  if (fields_classes->next)
    return field_of_ascii(fields_classes->next, end_ptr, msg);
  return TRUE;
}

static void ascii_of_fields(struct PprzMsg* msg, GList* fields_class, GString* buf) {
  struct PprzFieldClass* field_class = (struct PprzFieldClass*)fields_class->data;
  pprz_msg_ascii_of_field(msg, field_class, buf);
  if (fields_class->next) {
    g_string_append_printf(buf, " ");
    ascii_of_fields(msg, fields_class->next, buf); 
  }
}

static gboolean parse_xml_msg(struct PprzProtocol* this, xmlNodePtr ptr) {
  struct PprzMsgClass* new_msg_class;
  if ( !xmlNodeIsText (ptr)) { 
    if (xmlStrcmp(ptr->name, (const xmlChar *)"message")) /* not a message */
      return FALSE;
    new_msg_class = g_new(struct PprzMsgClass, 1);
    new_msg_class->id =  g_list_length(this->msgs_classes);
    new_msg_class->name = xmlGetProp(ptr, "id");
    new_msg_class->fields_classes = NULL;
    new_msg_class->fields_classes_by_name = g_hash_table_new(g_str_hash, g_str_equal);
    new_msg_class->size = 1;
    this->msgs_classes = g_list_append(this->msgs_classes, new_msg_class);
    g_hash_table_insert(this->msgs_classes_by_name, new_msg_class->name, new_msg_class);
    if (ptr->xmlChildrenNode && !parse_xml_field(new_msg_class, ptr->xmlChildrenNode))
      /* fixme : free space */
      return FALSE;
    new_msg_class->nb_fields_classes = g_list_length(new_msg_class->fields_classes);
    new_msg_class->fields_classes_by_id = g_new(struct PprzFieldClass*, new_msg_class->nb_fields_classes);
    {
      GList* cur = new_msg_class->fields_classes;
      guint i = 0;
      while (cur) {
	new_msg_class->fields_classes_by_id[i] = cur->data;
	i++;
	cur = cur->next;
      }
    }
  }
  if (ptr->next)
    return parse_xml_msg(this, ptr->next);
  return TRUE;
}

static gboolean parse_xml_field(struct PprzMsgClass* msg_class, xmlNodePtr ptr) {
  struct PprzFieldClass* new_field_class;
  xmlChar* type_str;
  if ( !xmlNodeIsText (ptr)) { 
    if (xmlStrcmp(ptr->name, (const xmlChar *)"field")) /* not a field    */
      return FALSE;
    new_field_class = g_new(struct PprzFieldClass, 1);
    new_field_class->name = xmlGetProp(ptr, "id");
    new_field_class->format = xmlGetProp(ptr, "format");
    new_field_class->unit = xmlGetProp(ptr, "unit");
    new_field_class->description = xmlGetProp(ptr, "description");
    type_str = xmlGetProp(ptr, "type");
    if ((new_field_class->type = type_of_string(type_str)) >= TYP_NB) {
      /* fixme : free space */
      return FALSE;
    }
    xmlFree(type_str);
    msg_class->fields_classes = g_list_append(msg_class->fields_classes, new_field_class);
    g_hash_table_insert(msg_class->fields_classes_by_name, new_field_class->name, new_field_class);
    new_field_class->offset = msg_class->size - 1;
    msg_class->size += type_info[new_field_class->type].size;
  }
  if (ptr->next)
    /* check return and free space */
    return parse_xml_field(msg_class, ptr->next);
  return TRUE;
}


static struct PprzMsgClass* msg_class_of_name(struct PprzProtocol* this, const gchar* name) {
 return g_hash_table_lookup(this->msgs_classes_by_name, name);
}

static struct PprzMsgClass* msg_class_of_id(struct PprzProtocol* this, guchar id) {
  if (id >= this->nb_msgs_classes)
    return NULL;
  return this->msgs_classes_by_id[id];
}

static struct PprzFieldClass* field_class_of_name(struct PprzMsg* msg, const gchar* name) {
  return g_hash_table_lookup(msg->class->fields_classes_by_name, name);
}
 
/* static struct PprzFieldClass* field_class_of_id(struct PprzMsg* msg, guchar id) { */
/*   GList* cell = g_list_nth(msg->class->fields_classes, id); */
/*   return cell?cell->data:NULL; */
/* } */
