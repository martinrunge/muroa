cmake_minimum_required (VERSION 2.8.3) 
project ( muroa ) 

# Recurse into the subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 

option( msessiond "build the muroa session daemon"            "yes")
option( muroad    "build the muroa audio streaming daemon"    "yes")
option( mqtclient "build the Qt based GUI client"             "yes")
option( unittests "build unittests"                           "no" )

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/aux/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(libxdiff)

if( libxdiff_FOUND )
  message(STATUS "libxdiff was found on this system, using it.")
else( libxdiff_FOUND )
  message(STATUS "libxdiff seems not to be present on this system. Using the copy packaged with muroa (3rdparty).")
  add_subdirectory (3rdparty/libxdiff-0.23)
  
  FIND_PATH(libxdiff_INCLUDE_DIRS xdiff.h ${CMAKE_SOURCE_DIR}/3rdparty/libxdiff-0.23/xdiff )

  set( libxdiff_LIBRARIES ${CMAKE_BINARY_DIR}/3rdparty/libxdiff-0.23/libxdiff.so )   
  
  MARK_AS_ADVANCED(libxdiff_LIBRARIES)				
  MARK_AS_ADVANCED(libxdiff_INCLUDE_DIRS)

  set( libxdiff_FOUND "yes")
endif( libxdiff_FOUND )


if( ${msessiond} )
  add_subdirectory (libstreamsrv)
  add_subdirectory (mmscanner)
  add_subdirectory (msessiond)
endif( ${msessiond} )

if( ${muroad} )
  add_subdirectory (libdsaudio)  
  add_subdirectory (muroad)
endif( ${muroad} )
	  
if( ${mqtclient} )
  add_subdirectory (mqtclient)
endif( ${mqtclient} )


if( ${msessiond} OR ${mqtclient} )
  add_subdirectory (libmuroa++)
endif( ${msessiond} OR ${mqtclient} )


if( ${msessiond} OR  ${muroad} )
  add_subdirectory (libmuroadaemon)  
  add_subdirectory (libsock++)
endif( ${msessiond} OR ${muroad} )
