INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libsock++
METASOURCES = AUTO
bin_PROGRAMS = dsserver
dsserver_SOURCES = dsserver.cpp
dsserver_LDADD = $(top_builddir)/libsock++/libsock++.la \
	$(top_builddir)/libdsaudio/libdsaudio.la
