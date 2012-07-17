INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libsock++
METASOURCES = AUTO
lib_LTLIBRARIES = libdsserver.la
libdsserver_la_SOURCES = cstreamconnection.cpp cstreamconnection.h\
	cstreamserver.cpp cstreamserver.h
libdsserver_la_LIBADD = -lstdc++
libdsserver_la_LDFLAGS = -version-info 0:0:0 -shrext .so
