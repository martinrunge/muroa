cmake_minimum_required (VERSION 2.6)
project (libmcommons)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})


find_package( Threads REQUIRED )
find_package( log4cplus REQUIRED )

include( MacroGDBIndex )

# set(Boost_USE_MULTITHREADED ON )
# find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )

add_definitions( ${log4cplus_DEFINITIONS} )

set( SRC           CUtils.cpp
                )                  

set( HEADERS       CUtils.h
                   CStreamFmt.h
                   MuroaExceptions.h)
                  
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS}  
                    ${log4cplus_INCLUDE_DIRS}
                    )

link_directories( ${BOOST_LIBRARYDIR} )

add_library( mcommons SHARED  ${SRC} ${HEADERS} )

target_link_libraries( mcommons  ${CMAKE_THREAD_LIBS_INIT} 
                                 ${Boost_LIBRARIES} 
                                 ${log4cplus_LIBRARY}
                                 )
                                 
set_target_properties( mcommons  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 

ADD_GDB_INDEX( mcommons )

install(TARGETS mcommons DESTINATION  lib)
