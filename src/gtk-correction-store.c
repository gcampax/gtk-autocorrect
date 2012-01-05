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
#include <stdio.h>

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

void
gtk_correction_store_init (GtkCorrectionStore *self)
{
  FILE*f_def;

  self->array = g_array_new(FALSE, FALSE, sizeof(Entry));

  if((f_def=fopen(DATABASE_PATH, "r")))
    {
      Entry entry;

      fscanf(f_def,"%ms %ms", &entry.to_be_corrected, &entry.to_correct);
      while(!feof(f_def))
        {
          g_array_append_val(self->array, entry);

          fscanf(f_def,"%ms %ms", &entry.to_be_corrected, &entry.to_correct);
        }

      qsort(self->array->data, self->array->len, sizeof(Entry), entry_comparator);
    }
  else
    {
      g_warning("Auto correction database not found: %m");
    }
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

      free(e->to_be_corrected);
      free(e->to_correct);
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
