cmake_minimum_required (VERSION 2.6)
project (libmcommons)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package( Threads REQUIRED )
find_package( log4cplus REQUIRED )

# set(Boost_USE_MULTITHREADED ON )
# find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )

add_definitions( ${log4cplus_DEFINITIONS} )

set( SRC           CUtils.cpp
                                      )                  

set( HEADERS       CUtils.h
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

###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET mcommons
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh libmcommons.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS mcommons DESTINATION  lib)
