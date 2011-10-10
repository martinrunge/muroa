INCLUDES = -I$(top_srcdir)/libdsaudio -I$(top_srcdir)/libsock++
METASOURCES = AUTO
bin_PROGRAMS = muroad
muroad_SOURCES = cfilterdef.cpp cfixpointresampler.cpp cfloatresampler.cpp \
	cmuroad.cpp cpacketringbuffer.cpp cplayer.cpp cplayloop.cpp crecvloop.cpp \
	cresampler.cpp cringbuffer.cpp main.cpp
muroad_LDADD = $(top_builddir)/libdsaudio/libdsaudio.la \
	$(top_builddir)/libsock++/libsock++.la -lboost_program_options -lsamplerate
noinst_HEADERS = CFixPointResamplerLargeFilter.h \
	CFixPointResamplerSmallFilter.h cfilterdef.h cfixpointresampler.h cfloatresampler.h cmuroad.h \
	cpacketringbuffer.h cplayer.h cplayloop.h crecvloop.h cresampler.h cringbuffer.h


