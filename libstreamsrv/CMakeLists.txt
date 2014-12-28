cmake_minimum_required (VERSION 2.6)
project (libstreamsrv)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package( Threads REQUIRED )

set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )

add_definitions(  )

set( SRC           CStreamConnection.cpp
                   CStreamServer.cpp
                   )
                  

set( HEADERS       CStreamConnection.h
                   CStreamServer.h
                   )
                  
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS}  
				    ../libsock++ 
				    ../libdsaudio
				    ../libmuroadaemon )

link_directories( "${CMAKE_SOURCE_DIR}/../libsock++/build_x86" 
                  "${CMAKE_SOURCE_DIR}/../libdsaudio/build_x86"
                  ${BOOST_LIBRARYDIR})

add_library( streamsrv SHARED  ${SRC} ${HEADERS} )
target_link_libraries( streamsrv ${CMAKE_THREAD_LIBS_INIT} 
                                 ${Boost_LIBRARIES} 
                                 sock++ 
                                 dsaudio )
set_target_properties( streamsrv  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 
###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET streamsrv
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh libstreamsrv.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS streamsrv DESTINATION  lib)
