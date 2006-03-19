INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libsock++ \
	-I$(top_srcdir)/libdsserver
METASOURCES = AUTO
bin_PROGRAMS = dsserver
dsserver_SOURCES = dsserver.cpp
dsserver_LDADD = $(top_builddir)/libdsserver/libdsserver.so.la \
	$(top_builddir)/libsock++/libsock++.la $(top_builddir)/libdsaudio/libdsaudio.la

