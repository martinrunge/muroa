cmake_minimum_required (VERSION 2.6)
project (libsock++)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

include( MacroGDBIndex )
find_package( Threads REQUIRED )

# set(Boost_USE_MULTITHREADED ON)
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

target_link_libraries( sock++ ${CMAKE_THREAD_LIBS_INIT} 
                              ${ALSA_LIBRARY} )
                              
set_target_properties( sock++  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 

ADD_GDB_INDEX( sock++ )

install(TARGETS  sock++ DESTINATION  lib)
