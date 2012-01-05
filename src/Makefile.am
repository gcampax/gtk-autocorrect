NULL =

gtkmoduledir = $(libdir)/gtk-3.0/3.0.0/immodules

gtkmodule_LTLIBRARIES = libautocorrect.la

libautocorrect_la_SOURCES = \
	gtk-im-module.c		\
	gtk-auto-corrector.c	\
	gtk-auto-corrector.h	\
	gtk-correction-store.c	\
	gtk-correction-store.h	\
	$(NULL)

libautocorrect_la_CFLAGS = -Wall
libautocorrect_la_CPPFLAGS = $(GTK_CFLAGS) -DPKGDATADIR='"$(pkgdatadir)"' -DG_LOG_DOMAIN='"GtkAutoCorrector"'
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
