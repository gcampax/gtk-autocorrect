AC_CONFIG_MACRO_DIR([m4])

AC_INIT([gtk-autocorr],[0.1])

AM_INIT_AUTOMAKE([1.11 foreign -Wno-portability])

AM_SILENT_RULES([yes])

AC_PROG_CC
LT_INIT

PKG_CHECK_MODULES(GTK, [gobject-2.0 gtk+-3.0])

AC_OUTPUT([
	Makefile
	src/Makefile
])
