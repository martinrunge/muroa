INCLUDES = 
METASOURCES = AUTO
lib_LTLIBRARIES = libdsaudio.la
libdsaudio_la_SOURCES = caudioframe.cpp caudioframe.h \
		caudioioalsa.cpp	caudioioalsa.h caudiooss.cpp caudiooss.h ccalibratedac.cpp	ccalibratedac.h \
		iaudioio.cpp iaudioio.h csync.cpp crtppacket.cpp
noinst_HEADERS = libdsaudio.h csync.h crtppacket.h
libdsaudio_la_LIBADD = -lasound -lboost_date_time
libdsaudio_la_LDFLAGS = -version-info 0:0:0
