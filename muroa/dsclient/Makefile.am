INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libsock++
METASOURCES = AUTO
bin_PROGRAMS = dsclient
dsclient_SOURCES = dsclient.cpp cplayloop.cpp cplayloop.h cresampler.cpp \
	cresampler.h crecvloop.cpp cplayer.cpp cpacketringbuffer.cpp cpacketringbuffer.h \
	cringbuffer.cpp cringbuffer.h cfloatresampler.cpp cfixpointresampler.cpp cfilterdef.cpp \
	cfilterdef.h
dsclient_LDADD = $(top_builddir)/libsock++/libsock++.la \
	$(top_builddir)/libdsaudio/libdsaudio.la -lsamplerate
noinst_HEADERS = crecvloop.h cplayer.h cringbuffer.h cfloatresampler.h \
	cfixpointresampler.h CFixPointResamplerLargeFilter.h CFixPointResamplerSmallFilter.h


