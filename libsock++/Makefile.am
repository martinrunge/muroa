
METASOURCES = AUTO
lib_LTLIBRARIES = libsock++.la
libsock___la_SOURCES = cmutex.cpp cmutex.h cposixcond.cpp \
	cposixcond.h	cpthread.cpp cpthread.h csocket.cpp csocket.h \
	ctcpserversocket.cpp	ctcpserversocket.h cthreadslave.cpp cthreadslave.h libsock++.cpp
noinst_HEADERS = libsock++.h
AM_CFLAGS = -pthread
AM_CXXFLAGS = -pthread
libsock___la_LIBADD = -lpthread
