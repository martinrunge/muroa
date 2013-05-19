cmake_minimum_required (VERSION 2.6)
project (libsock++)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(Boost_USE_MULTITHREADED ON)
# find_package( Boost 1.36.0 COMPONENTS system regex REQUIRED )
# find_package( EXPAT REQUIRED )

add_definitions(  )


set( SRC           cipaddress.cpp
                   cposixcond.cpp
                   ctcpserversocket.cpp
                   cipv4address.cpp
                   cpthread.cpp
                   cthreadslave.cpp
                   cmutex.cpp
                   csocket.cpp
                   libsock++.cpp  )
                  

set( HEADERS       cipaddress.h
                   cposixcond.h
                   ctcpserversocket.h
                   cipv4address.h
                   cpthread.h
                   cthreadslave.h
                   cmutex.h
                   csocket.h
                   libsock++.h  )
                  
include_directories(${CMAKE_CURRENT_SOURCE_DIR} )

link_directories(  )

add_library(  sock++ SHARED  ${SRC} ${HEADERS} )
target_link_libraries( sock++ ${ALSA_LIBRARY} pthread )
set_target_properties( sock++  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 
###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET sock++
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/mkgdbindex.sh libsock++.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS  sock++ DESTINATION  lib)
