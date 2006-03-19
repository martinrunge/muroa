



METASOURCES = AUTO
lib_LTLIBRARIES = libds_plugin.la
libds_plugin_la_SOURCES = ds_plugin.c ds_cpp_plugin.cpp
libds_plugin_la_LDFLAGS = -avoid-version -module -no-undefined -export-dynamic
INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libdsserver \
	-I$(top_srcdir)/libsock++ -I/usr/lib/glib/include -I/usr/include/glib-1.2 -I/usr/include/gtk-1.2
libds_plugin_la_LIBADD = $(top_builddir)/libsock++/libsock++.la \
	$(top_builddir)/libdsserver/libdsserver.so.la $(top_builddir)/libdsaudio/libdsaudio.la
noinst_HEADERS = ds_cpp_plugin.h
