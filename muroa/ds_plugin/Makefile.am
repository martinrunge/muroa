



METASOURCES = AUTO
lib_LTLIBRARIES = libds_plugin.la
libds_plugin_la_SOURCES = ds_plugin.c ds_cpp_plugin.cpp
libds_plugin_la_LDFLAGS = -avoid-version -module -no-undefined -export-dynamic \
	-shrext -version-info
INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libdsserver \
	-I$(top_srcdir)/libsock++ `gtk-config --cflags` `glib-config --cflags`
libds_plugin_la_LIBADD = $(top_builddir)/libsock++/libsock++.la \
	$(top_builddir)/libdsserver/libdsserver.la $(top_builddir)/libdsaudio/libdsaudio.la
noinst_HEADERS = ds_cpp_plugin.h
