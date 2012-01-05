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

#ifndef __GTK_CORRECTION_STORE_H__
#define __GTK_CORRECTION_STORE_H__

#include <glib.h>
#include <glib-object.h>

#define GTK_TYPE_CORRECTION_STORE                  (gtk_correction_store_get_type ())
#define GTK_CORRECTION_STORE(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CORRECTION_STORE, GtkCorrectionStore))
#define GTK_IS_CORRECTION_STORE(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CORRECTION_STORE))
#define GTK_CORRECTION_STORE_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CORRECTION_STORE, GtkCorrectionStoreClass))
#define GTK_IS_CORRECTION_STORE_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CORRECTION_STORE))
#define GTK_CORRECTION_STORE_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CORRECTION_STORE, GtkCorrectionStoreClass))

typedef struct _GtkCorrectionStore        GtkCorrectionStore;
typedef struct _GtkCorrectionStoreClass   GtkCorrectionStoreClass;

GType gtk_correction_store_get_type (void) G_GNUC_CONST;
void _gtk_correction_store_register_type (GTypeModule *module);

GtkCorrectionStore *gtk_correction_store_new (void);

const gchar *gtk_correction_store_get_correction (GtkCorrectionStore *store, const gchar *word);

#endif /* __GTK_CORRECTION_STORE_H__ */
