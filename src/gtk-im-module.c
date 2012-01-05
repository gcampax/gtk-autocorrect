/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*- */
/* gtk-autocorr - Gtk Autocorrection module
 * Copyright (C) 2011 Giovanni Campagna <scampa.giovanni@gmail.com>
 *                    Roberto Clapis
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gtk/gtkimmodule.h>

#include "gtk-auto-corrector.h"
#include "gtk-correction-store.h"

void im_module_init(GTypeModule *module);
void im_module_exit(void);
void im_module_list(GtkIMContextInfo ***contexts, int *n_contexts);
GtkIMContext *im_module_create(gchar *context_id);

GtkIMContextInfo module_info = {
  "auto-corrector",
  "Automatic correction",
  "gtk-auto-corrector",
  "/usr/share/foo", /* FIXME */
  "*"
};

GtkIMContextInfo *info_list[] = {
  &module_info
};

void
im_module_list (GtkIMContextInfo ***contexts, int *n_contexts)
{
  *contexts = info_list;
  *n_contexts = G_N_ELEMENTS(info_list);
}

void
im_module_init (GTypeModule *module)
{
  _gtk_auto_corrector_register_type (module);
  _gtk_correction_store_register_type (module);
}

void
im_module_exit (void)
{
  /* nothing to do here */
}

GtkIMContext *
im_module_create(gchar *context_id)
{
  g_return_val_if_fail (strcmp(context_id, "auto-corrector") == 0, NULL);

  return GTK_IM_CONTEXT (gtk_auto_corrector_new ());
}
