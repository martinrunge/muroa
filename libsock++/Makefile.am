
METASOURCES = AUTO
lib_LTLIBRARIES = libsock++.la
libsock___la_SOURCES = cmutex.cpp cmutex.h cposixcond.cpp \
		cposixcond.h	cpthread.cpp cpthread.h csocket.cpp csocket.h \
		ctcpserversocket.cpp	ctcpserversocket.h cthreadslave.cpp cthreadslave.h libsock++.cpp cipaddress.cpp \
	cipv4address.cpp
noinst_HEADERS = libsock++.h cipaddress.h cipv4address.h
AM_CFLAGS = -pthread
AM_CXXFLAGS = -pthread
libsock___la_LIBADD = -lpthread
libsock___la_LDFLAGS = -version-info 0:0:0 -shrext .so


