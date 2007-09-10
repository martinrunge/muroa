



# METASOURCES = AUTO
#AM_CFLAGS=$(PTHREAD_CFLAGS)
#AM_LIBADD=$(PTHREAD_LIBS)

MUROA_CFLAGS=-I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libdsserver -I$(top_srcdir)/libsock++
MUROA_LIBADD=$(top_builddir)/libsock++/libsock++.la $(top_builddir)/libdsserver/libdsserver.la $(top_builddir)/libdsaudio/libdsaudio.la  

XMMS_OUTPUTPLUGINDIR = /usr/lib/xmms/Output

xmmsplugindir=$(XMMS_OUTPUTPLUGINDIR)
xmmsplugin_LTLIBRARIES=libds-plugin.la

#libdir = $(XMMS_OUTPUTPLUGINDIR)


libds_plugin_la_SOURCES = ds_plugin.c ds_cpp_plugin.cpp
libds_plugin_la_LDFLAGS = -avoid-version -version-info 0:0:0 -shrext .so 
INCLUDES = $(MUROA_CFLAGS) 
libds_plugin_la_CFLAGS = $(MUROA_CFLAGS) `gtk-config --cflags` `glib-config --cflags`
libds_plugin_la_LIBADD = $(MUROA_LIBADD) 


#$(XMMS_LIBS)
# noinst_HEADERS = ds_cpp_plugin.h
