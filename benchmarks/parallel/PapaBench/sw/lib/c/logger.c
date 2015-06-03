/* 
   logger.c
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


#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include <glib.h>

#include "logger.h"

//static GList* logger_parse_messages(struct PprzProtocol* protocol, xmlDocPtr doc, xmlNodePtr cur);
static GList* logger_parse_message(struct PprzProtocol* protocol, GList* msgs, const guchar* buf);

struct Logger* logger_new(const gchar* out_file, xmlDocPtr doc) {
  const gchar* opening = "<data>\n";
  struct Logger* this = g_new(struct Logger, 1);
  if (this) {
    GError *err = NULL;
    
    gchar* copy = strdup(out_file);
    gchar* out_dir = dirname(copy);
    DIR *d;
    if (!(d=opendir(out_dir))) {
      if  (errno == ENOENT) {
	if (mkdir(out_dir, S_IRWXU | S_IRWXG)) {
	  g_message("log directory %s doesn't exist and could not create it", out_dir);
	  /* FIXME : FREE */
	  return NULL;
	}
      }
      else {
	g_message("could not acces log directory %s %s", out_dir, g_strerror(errno));
	/* FIXME : FREE */
	return NULL;
      }
    }
    closedir(d);
    g_free(copy);

    xmlSaveFormatFile (out_file, doc, 1); 
    /* FIXME : check result */
    
    this->channel = g_io_channel_new_file(out_file, "a", &err);
    if (err) {
      g_message("could not open log file %s : %s", out_file, err->message);
      g_error_free(err);
      g_free(this);
      return NULL;
    }
    g_io_channel_write_chars(this->channel, opening, strlen(opening), NULL, NULL);
  }
  return this;

}


void logger_free(struct Logger* this) 
{
  const gchar* closing = "</data>\n</log>\n";
  g_io_channel_write_chars(this->channel, closing, strlen(closing), NULL, NULL);
  g_io_channel_flush(this->channel, NULL);
  //  g_io_channel_free(this->channel);
  g_free(this);
}

void logger_log(struct Logger* this, struct PprzMsg* msg) {
  GString *str;
  pprz_protocol_ascii_of_msg(msg, &str);
  if (str) {
    GError *err = NULL;
    guint len;
    g_io_channel_write_chars (this->channel, str->str, str->len, &len, &err);
    if (err) {
      g_warning("log write failed %s", err->message);
      g_error_free(err);
      g_string_free(str, TRUE);
      return;
    }
    g_string_free(str, TRUE);
    g_io_channel_flush(this->channel, &err);
    if (err) {
      g_warning("log flush failed %s", err->message);
      g_error_free(err);
      return;
    }
  }
}


GList*  logger_parse_log(const gchar* filename, struct PprzProtocol** protocol) {
  xmlDocPtr doc;
  xmlNodePtr cur;

  GList* msgs = NULL;
  const gchar* protocol_closure = "</protocol>";
  gchar *contents, *data_start;
  gsize len;
  GError* err;
  gboolean res = g_file_get_contents(filename, &contents, &len, &err);
  
  if (!res) {
    g_message("could not read file %s : %s", filename, err->message);
    g_error_free(err);
    return NULL;
  }

  if (!(data_start = g_strstr_len(contents, len, protocol_closure))) {
    g_free(contents);
    g_message("protocol closing not found");
    return NULL;
  }

  data_start += strlen(protocol_closure);
  
  if (!( doc = xmlParseMemory(contents, data_start - contents))) {
    g_free(contents);
    g_message("error parsing protocol xml");
    return NULL;
  }
  
  if (!(cur = xmlDocGetRootElement(doc))) {
    g_free(contents);
    g_message("empty protocol");
    return NULL;
  }

  if (!(*protocol = pprz_protocol_new_from_xml(doc, cur))) {
    g_free(contents);
    g_message("error interpreting protocol");
    return NULL;
  }

  msgs = logger_parse_message(*protocol, msgs, data_start);
  g_free(contents);

  return msgs;
}

/* static GList* logger_parse_messages(struct PprzProtocol* protocol, xmlDocPtr doc, xmlNodePtr cur) { */
  
/* if (!cur) return NULL;                                   */
/*   if (xmlStrcmp(cur->name, (const xmlChar *) "data"))    */
/*     return cur->next?logger_parse_messages(protocol, doc, cur->next):NULL; */
/*   cur = cur->xmlChildrenNode; */
  
/*   if (xmlStrcmp(cur->name, (const xmlChar *) "text"))    */
/*     return NULL; */
  
/*   return logger_parse_message(protocol, NULL, cur->content); */
/* } */

static GList* logger_parse_message(struct PprzProtocol* protocol, GList* msgs, const guchar* buf) {
  guchar* next;
  struct PprzMsg* msg =  pprz_protocol_msg_new_of_ascii(protocol, buf);
  if (msg)
    msgs = g_list_append(msgs, msg);
  if ((next = index(buf, '\n'))) 
    return logger_parse_message(protocol, msgs, next+1);
  return msgs;
}
