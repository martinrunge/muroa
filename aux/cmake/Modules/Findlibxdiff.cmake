# Find libxdiff in standard paths
# 

FIND_PATH(libxdiff_INCLUDE_DIRS xdiff.h /usr/include /usr/local/include)

FIND_LIBRARY(libxdiff_LIBRARIES NAMES xdiff PATH /usr/lib /usr/local/lib) 

IF (libxdiff_INCLUDE_DIRS AND libxdiff_LIBRARIES)
  SET(libxdiff_FOUND TRUE)
ENDIF (libxdiff_INCLUDE_DIRS AND libxdiff_LIBRARIES)

IF (libxdiff_FOUND)
   IF (NOT libxdiff_FIND_QUIETLY)
      MESSAGE(STATUS "Found libxdiff: ${libxdiff_LIBRARIES}")
   ENDIF (NOT libxdiff_FIND_QUIETLY)
   MARK_AS_ADVANCED(libxdiff_LIBRARIES)
   MARK_AS_ADVANCED(libxdiff_INCLUDE_DIRS)
ELSE (libxdiff_FOUND)
   IF (libxdiff_FIND_REQUIRED)
      MESSAGE(FATAL_ERROR "Could not find libxdiff")
   ENDIF (libxdiff_FIND_REQUIRED)
ENDIF (libxdiff_FOUND)