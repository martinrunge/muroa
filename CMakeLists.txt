cmake_minimum_required (VERSION 2.8.3) 
project ( muroa ) 

# Recurse into the subdirectories. This does not actually 
# cause another cmake executable to run. The same process will walk through 
# the project's entire directory structure. 

option( msessiond "build the muroa session daemon"            "no")
option( wssrv     "build stream searver with web interfave"   "yes")
option( muroad    "build the muroa audio streaming daemon"    "yes")
option( mqtclient "build the Qt based GUI client"             "no")
option( unittests "build unittests"                           "no" )
option( examples  "build the examples"                         "no")


set(MUROA_AUX_DIR "${CMAKE_CURRENT_LIST_DIR}/aux")
set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package(libxdiff)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

if( msessiond OR mqtclient )
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
endif( )

add_subdirectory (libmcommons)

if( msessiond )
  add_subdirectory (mmscanner)
  add_subdirectory (msessiond)
  if( unittests )
    add_subdirectory(msessiond/unittest)
  endif( )
endif( )

if( wssrv )
  add_subdirectory( wssrv )
endif()

if( muroad )
  add_subdirectory (muroad)
  if( unittests )
    add_subdirectory(libmstream/unittest)
  endif( )
endif( )
	  
if( mqtclient )
  add_subdirectory (mqtclient)
endif( )


if( msessiond OR mqtclient )
  add_subdirectory (libmuroa++)
  if( unittests )
    add_subdirectory(libmuroa++/unittest)
  endif( )
endif( )


if( msessiond OR muroad OR wssrv )
  add_subdirectory (libmstream)
  if( examples )
    add_subdirectory (libmstream/examples)
  endif()
  add_subdirectory (libmuroadaemon)  
  add_subdirectory (libsock++)
endif( )
