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

/* inclusion guard */
#ifndef __GTK_AUTO_CORRECTOR_H__
#define __GTK_AUTO_CORRECTOR_H__

#include <glib-object.h>

/*
 * Type macros.
 */
#define GTK_TYPE_AUTO_CORRECTOR                  (gtk_auto_corrector_get_type ())
#define GTK_AUTO_CORRECTOR(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_AUTO_CORRECTOR, GtkAutoCorrector))
#define GTK_IS_AUTO_CORRECTOR(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_AUTO_CORRECTOR))
#define GTK_AUTO_CORRECTOR_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_AUTO_CORRECTOR, GtkAutoCorrectorClass))
#define GTK_IS_AUTO_CORRECTOR_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_AUTO_CORRECTOR))
#define GTK_AUTO_CORRECTOR_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_AUTO_CORRECTOR, GtkAutoCorrectorClass))

typedef struct _GtkAutoCorrector        GtkAutoCorrector;
typedef struct _GtkAutoCorrectorClass   GtkAutoCorrectorClass;

GType gtk_auto_corrector_get_type (void) G_GNUC_CONST;
void _gtk_auto_corrector_register_type (GTypeModule *module);

GtkAutoCorrector *gtk_auto_corrector_new (void);

#endif /* __GTK_AUTO_CORRECTOR_H__ */
