gtkmoduledir = /somewhere

gtkmodule_LTLIBRARIES = libautocorrect.la

libautocorrect_la_SOURCES = \
	gtk-im-module.c		\
	gtk-auto-corrector.c	\
	gtk-auto-corrector.h

libautocorrect_la_CFLAGS = -Wall
libautocorrect_la_CPPFLAGS = $(GTK_CFLAGS)
libautocorrect_la_LIBADD = $(GTK_LIBS)
