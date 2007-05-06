INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libsock++
METASOURCES = AUTO
bin_PROGRAMS = dsclient
dsclient_SOURCES = cfilterdef.cpp cfilterdef.h cfixpointresampler.cpp \
	cfloatresampler.cpp cmuroad.cpp cpacketringbuffer.cpp cpacketringbuffer.h cplayer.cpp \
	cplayloop.cpp cplayloop.h crecvloop.cpp cresampler.cpp cresampler.h cringbuffer.cpp \
	cringbuffer.h dsclient.cpp muroa.conf
dsclient_LDADD = $(top_builddir)/libdsaudio/libdsaudio.la \
	$(top_builddir)/libsock++/libsock++.la -lboost_program_options -lsamplerate
noinst_HEADERS = CFixPointResamplerLargeFilter.h \
	CFixPointResamplerSmallFilter.h cfixpointresampler.h cfloatresampler.h cmuroad.h cplayer.h crecvloop.h \
	cringbuffer.h


