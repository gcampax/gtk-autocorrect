gtkmoduledir = $(libdir)/gtk-3.0/3.0.0/immodules

gtkmodule_LTLIBRARIES = libautocorrect.la

libautocorrect_la_SOURCES = \
	gtk-im-module.c		\
	gtk-auto-corrector.c	\
	gtk-auto-corrector.h

libautocorrect_la_CFLAGS = -Wall
libautocorrect_la_CPPFLAGS = $(GTK_CFLAGS)
libautocorrect_la_LIBADD = $(GTK_LIBS)
libautocorrect_la_LDFLAGS = -module -avoid-version

install-exec-local:
	@if [ -n "$(DESTDIR)" ]; then \
		echo "Not updating IM module cache..."; \
	else \
		echo "Updating IM module cache..."; \
		(cd $(gtkmoduledir)/..; \
		gtk-query-immodules-3.0 immodules/*.so > immodules.cache; ) \
	fi;

noinst_PROGRAMS = clapis
clapis_CFLAGS = -Wall
clapis_CPPFLAGS = $(GTK_CFLAGS)
clapis_LDADD = $(GTK_LIBS)
