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

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include <glib.h>

typedef struct _Entry {
  char *to_be_corrected;
  char *to_correct;
} Entry;

GArray *load(const char*);
const char* correct(const char*, GArray*);

static int compar(const void *p1, const void *p2)
{
	const Entry *e1 = p1, *e2 = p2;

	return strcmp(e1->to_be_corrected, e2->to_be_corrected);
}

int main(int argc, char **argv)
{
	GArray *list;
	char *prova;
	const char *path = "./def";
	int i;

	list = load(path);
	if(list) {
		for(i = 0; i < list->len; i++) {
                  Entry *entry = ((Entry*)(list->data)) + i;

			printf("%s → %s\n", entry->to_be_corrected, entry->to_correct);
		}
		printf("\n");

		scanf("%ms", &prova);
		printf("%s\n", correct(prova, list));

		g_free(prova);
		/* FIXME: need to free individual strings */
		g_array_unref(list);
	}

	return 0;
}


const char* correct(const char *tofind, GArray *list)
{
        Entry key = { (char*) tofind, NULL };
	Entry* answer = bsearch(&key, list->data, list->len, sizeof(void*), compar);

	if (answer)
		return answer->to_correct;
	else
		return tofind;
}

GArray *load(const char *path)
{
	FILE*f_def;

	if((f_def=fopen(path,"r"))) {
		GArray *array = g_array_new(FALSE, FALSE, sizeof(Entry));
		Entry entry;

		fscanf(f_def,"%ms %ms", &entry.to_be_corrected, &entry.to_correct);
		while(!feof(f_def)) {
			g_array_append_val(array, entry);

			fscanf(f_def,"%ms %ms", &entry.to_be_corrected, &entry.to_correct);
		}

		qsort(array->data, array->len, sizeof(Entry), compar);
		return array;
	} else {
		fprintf(stderr, "File of definitions not found: %m");
		return NULL;
	}
}
