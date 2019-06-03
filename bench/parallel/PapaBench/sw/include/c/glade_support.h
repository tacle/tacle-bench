/*
   glade_support.h
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

  glade hack.

*/

#ifndef GLADE_SUPPORT_H
#define GLADE_SUPPORT_H

#define DEFAULT_LEN 256

#define UPDATE_LABEL(object, label_name, fmt, args...)              \
{                                                                   \
  GtkLabel* label = GTK_LABEL(gtk_object_get_data(GTK_OBJECT(object), label_name));  \
  GString* str = g_string_sized_new(DEFAULT_LEN);                   \
  if (label) {                                                      \
    g_string_printf(str, fmt, ##args);                              \
    gtk_label_set_text(label, str->str);                            \
  }                                                                 \
  else                                                              \
    g_message("##### unknown label [ %s ]", label_name);        \
  g_string_free(str, TRUE);                                         \
}                                                                   \

#define UPDATE_LABEL_COLOR(object, label_name, test, fmt, args...) \
{                                                                   \
  GtkStateType state; \
  GtkLabel* label = GTK_LABEL(gtk_object_get_data(GTK_OBJECT(object), label_name));  \
  GString* str = g_string_sized_new(DEFAULT_LEN);                   \
  if (label) {                                                      \
    g_string_printf(str, fmt, ##args);                              \
    gtk_label_set_text(label, str->str);                            \
  }                                                                 \
  else                                                              \
    g_message("##### unknown label [ %s ]", label_name);        \
  g_string_free(str, TRUE);                                         \
  for (state = GTK_STATE_NORMAL; state <= GTK_STATE_INSENSITIVE; state++) \
    gtk_widget_modify_fg(GTK_WIDGET(label), state, (test)?&my_red_color:&gtk_widget_get_default_style()->fg[ state ]);             \
}                                                                   \




#endif /* GLADE_SUPPORT_H */
