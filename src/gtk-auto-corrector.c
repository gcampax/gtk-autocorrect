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

#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "gtk-auto-corrector.h"

struct _GtkAutoCorrector
{
  GtkIMContext parent;

  gboolean preediting;

  GtkIMContext *simple_context;
  GString      *current_word;
};

struct _GtkAutoCorrectorClass
{
  GtkIMContextClass parent_class;
};

G_DEFINE_DYNAMIC_TYPE(GtkAutoCorrector, gtk_auto_corrector, GTK_TYPE_IM_CONTEXT)

static void
gtk_auto_corrector_apply_autocorrection (GtkAutoCorrector *self)
{
  /* FIXME: fill */
}

static void
gtk_auto_corrector_commit_word (GtkAutoCorrector *self)
{

  if (self->preediting)
    {
      self->preediting = FALSE;

      g_signal_emit_by_name (self, "preedit-end");
    }

  g_signal_emit_by_name (self, "commit", self->current_word->str);

  g_string_truncate (self->current_word, 0);
  g_signal_emit_by_name (self, "preedit-changed");
}

static void
gtk_auto_corrector_simple_commit_cb (GtkIMContext *context,
				     gchar        *str,
				     gpointer      user_data)
{
  GtkAutoCorrector *self = user_data;
  gunichar input;

  input = g_utf8_get_char (str);

  if (!g_unichar_isalpha(input))
    {
      gtk_auto_corrector_apply_autocorrection (self);

      g_string_append (self->current_word, str);

      gtk_auto_corrector_commit_word (self);
     }
  else
    {
      if (!self->preediting)
        {
          self->preediting = TRUE;

          g_signal_emit_by_name (self, "preedit-start");
        }

      g_string_append (self->current_word, str);
      g_signal_emit_by_name (self, "preedit-changed");
    }
}

static void
gtk_auto_corrector_simple_preedit_changed_cb (GtkIMContext *context,
					      gpointer      user_data)
{
  GtkAutoCorrector *self = user_data;

  g_signal_emit_by_name (self, "preeedit-changed");
}

static void
gtk_auto_corrector_simple_preedit_start_cb (GtkIMContext *context,
                                            gpointer      user_data)
{
  GtkAutoCorrector *self = user_data;

  if (!self->preediting)
    {
      self->preediting = TRUE;

      g_signal_emit_by_name (self, "preedit-start");
    }
}

static void
gtk_auto_corrector_init (GtkAutoCorrector *self)
{
  self->current_word = g_string_new (NULL);

  self->simple_context = GTK_IM_CONTEXT (gtk_im_context_simple_new ());
  g_signal_connect (self->simple_context, "commit",
		    G_CALLBACK (gtk_auto_corrector_simple_commit_cb), self);
  g_signal_connect (self->simple_context, "preedit-changed",
		    G_CALLBACK (gtk_auto_corrector_simple_preedit_changed_cb), self);
  g_signal_connect (self->simple_context, "preedit-start",
		    G_CALLBACK (gtk_auto_corrector_simple_preedit_start_cb), self);

}

static void
gtk_auto_corrector_finalize (GObject *object)
{
  GtkAutoCorrector *self = GTK_AUTO_CORRECTOR (object);

  if (self->current_word)
    g_string_free (self->current_word, TRUE);

  g_object_unref (self->simple_context);
}

static gboolean
gtk_auto_corrector_filter_keypress (GtkIMContext *context,
				    GdkEventKey  *event)
{
  GtkAutoCorrector *self = GTK_AUTO_CORRECTOR (context);

  /* forward everything to the simple context */
  return gtk_im_context_filter_keypress (self->simple_context, event);
}

static void
gtk_auto_corrector_filter_reset (GtkIMContext *context)
{
  GtkAutoCorrector *self = GTK_AUTO_CORRECTOR (context);

  /* first reset the simple context */
  gtk_im_context_reset (self->simple_context);

  /* then flush everything we currently have */
  gtk_auto_corrector_apply_autocorrection (self);
  gtk_auto_corrector_commit_word (self);
}

static void
gtk_auto_corrector_get_preeedit_string (GtkIMContext   *context,
                                        gchar         **str,
                                        PangoAttrList **attrs,
                                        gint           *cursor_pos)
{
  GtkAutoCorrector *self = GTK_AUTO_CORRECTOR (context);
  GString *copy;
  gchar *simple_preedit;
  gint simple_preedit_len;

  copy = g_string_new (self->current_word->str);

  /* retrieve the preedit from the simple context */
  gtk_im_context_get_preedit_string (self->simple_context,
                                     &simple_preedit,
                                     NULL, /* no attributes, we generate them later */
                                     NULL);

  /* generate the actual preedit */
  g_string_append (copy, simple_preedit);
  simple_preedit_len = strlen(simple_preedit);

  /* provide feedback for the preedit from simple, if any */
  if (attrs)
    {
      *attrs = pango_attr_list_new ();
      
      if (simple_preedit_len)
	{
	  PangoAttribute *attr = pango_attr_underline_new (PANGO_UNDERLINE_SINGLE);
	  attr->start_index = copy->len - simple_preedit_len;
          attr->end_index = copy->len;
	  pango_attr_list_insert (*attrs, attr);
	}
    }

  if (cursor_pos)
    *cursor_pos = copy->len;

  if (str)
    *str = g_string_free (copy, FALSE);
  else
    g_string_free (copy, TRUE);
}

static void
gtk_auto_corrector_class_init (GtkAutoCorrectorClass *klass)
{
  GtkIMContextClass *imcontext_class = GTK_IM_CONTEXT_CLASS (klass);
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  imcontext_class->filter_keypress = gtk_auto_corrector_filter_keypress;
  imcontext_class->reset = gtk_auto_corrector_filter_reset;
  imcontext_class->get_preedit_string = gtk_auto_corrector_get_preeedit_string;

  object_class->finalize = gtk_auto_corrector_finalize;
}

static void
gtk_auto_corrector_class_finalize (GtkAutoCorrectorClass *klass)
{
  /* nothing to do here */
}

void
_gtk_auto_corrector_register_type (GTypeModule *module)
{
  gtk_auto_corrector_register_type (module);
}

GtkAutoCorrector *
gtk_auto_corrector_new (void)
{
  return g_object_new (GTK_TYPE_AUTO_CORRECTOR, NULL);
}
