# Base Io build system
# Written by Jeremy Tregunna <jeremy.tregunna@me.com>
#
# Find libspeex.

FIND_PATH(LibSpeex_INCLUDE_DIR  speex/speex_resampler.h)

SET(LibSpeex_NAMES ${LibSpeex_NAMES} speexdsp libspeexdsp)
FIND_LIBRARY( LibSpeex_LIBRARY NAMES ${LibSpeex_NAMES} PATH)

IF(LibSpeex_INCLUDE_DIR AND LibSpeex_LIBRARY)
	SET(LibSpeex_FOUND TRUE)
ENDIF(LibSpeex_INCLUDE_DIR AND LibSpeex_LIBRARY)

IF(LibSpeex_FOUND)
	IF(NOT LibSpeex_FIND_QUIETLY)
		MESSAGE(STATUS "Found LibSpeex: ${LibSpeex_LIBRARY}")
	ENDIF (NOT LibSpeex_FIND_QUIETLY)
ELSE(LibSpeex_FOUND)
	IF(LibSpeex_FIND_REQUIRED)
		MESSAGE(FATAL_ERROR "Could not find libspeex")
	ENDIF(LibSpeex_FIND_REQUIRED)
ENDIF (LibSpeex_FOUND)