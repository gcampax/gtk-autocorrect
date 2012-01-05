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
#include <stdlib.h>

#include "gtk-correction-store.h"

#define DATABASE_PATH DATADIR "/def.txt"

typedef struct {
  char *to_be_corrected;
  char *to_correct;
} Entry;

struct _GtkCorrectionStore {
  GObject parent;

  GArray *array;
};
struct _GtkCorrectionStoreClass {
  GObjectClass parent_class;
};

G_DEFINE_DYNAMIC_TYPE(GtkCorrectionStore, gtk_correction_store, G_TYPE_OBJECT)

void
_gtk_correction_store_register_type (GTypeModule *module)
{
  gtk_correction_store_register_type (module);
}

GtkCorrectionStore *
gtk_correction_store_new (void)
{
  return g_object_new (GTK_TYPE_CORRECTION_STORE, NULL);
}

static int
entry_comparator(const void *p1, const void *p2)
{
  const Entry *e1 = p1, *e2 = p2;

  return strcmp(e1->to_be_corrected, e2->to_be_corrected);
}

static void
gtk_correction_store_load_database (GtkCorrectionStore *self)
{
  GIOChannel *channel;
  gchar *line, *correction;
  gsize line_terminator;
  GError *error = NULL;

  channel = g_io_channel_new_file (DATABASE_PATH, "r", &error);
  if (error)
    goto error;

  while (g_io_channel_read_line (channel, &line, NULL, &line_terminator, &error) ==
         G_IO_STATUS_NORMAL)
    {
      Entry entry;

      /* kill the line terminator */
      line[line_terminator] = 0;

      /* separate the strings */
      correction = strchr(line, ' ');
      *correction = 0;
      correction++;

      entry.to_be_corrected = line;
      entry.to_correct = correction;

      g_array_append_val (self->array, entry);
    }

  if (error)
    goto error;

  qsort (self->array->data, self->array->len, sizeof(Entry), entry_comparator);

  g_io_channel_unref (channel);
  return;

error:
  /* No warning for No such file or directory */
  if (!g_error_matches (error, G_FILE_ERROR, G_FILE_ERROR_NOENT))
    g_warning ("Failed to load correction database: %s", error->message);
  g_clear_error (&error);
}

void
gtk_correction_store_init (GtkCorrectionStore *self)
{

  self->array = g_array_new(FALSE, FALSE, sizeof(Entry));

  gtk_correction_store_load_database (self);
}

const char*
gtk_correction_store_get_correction (GtkCorrectionStore *self,
                                     const char *word)
{
  Entry key = { (char*) word, NULL };
  Entry* answer = bsearch(&key, self->array->data, self->array->len, sizeof(void*), entry_comparator);

  if (answer)
    return answer->to_correct;
  else
    return word;
}

static void
gtk_correction_store_finalize (GObject *object)
{
  GtkCorrectionStore *self = GTK_CORRECTION_STORE (object);
  int i;

  for (i = 0; i < self->array->len; i++)
    {
      Entry *e = &g_array_index (self->array, Entry, i);

      g_free(e->to_be_corrected);
      /* to_correct is a pointer into to_be_corrected,
         so don't free that */
    }
  g_array_free (self->array, TRUE);

  G_OBJECT_CLASS (gtk_correction_store_parent_class)->finalize (object);
}

void
gtk_correction_store_class_init (GtkCorrectionStoreClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gtk_correction_store_finalize;
}

void
gtk_correction_store_class_finalize (GtkCorrectionStoreClass *klass)
{
}
