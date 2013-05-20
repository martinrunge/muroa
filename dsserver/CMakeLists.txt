cmake_minimum_required (VERSION 2.6)
project (dsserver)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package( Threads REQUIRED )

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )

add_definitions(  )

set( SRC           dsserver.cpp  
                   )
                  

set( HEADERS       )
                  
include_directories( ${CMAKE_SOURCE_DIR} 
                     ${Boost_INCLUDE_DIRS} 
                     ../libstreamsrv 
                     ../libdsaudio 
                     ../libsock++
                     ../libmuroadaemon)

link_directories( "${CMAKE_SOURCE_DIR}/../libstreamsrv/build_x86"
                  "${CMAKE_SOURCE_DIR}/../libdsaudio/build_x86"
                  "${CMAKE_SOURCE_DIR}/../libsock++/build_x86"
                  ${BOOST_LIBRARYDIR} )

ADD_EXECUTABLE( dsserver  ${SRC} ${HEADERS} )
target_link_libraries( dsserver ${CMAKE_THREAD_LIBS_INIT} 
                                ${Boost_LIBRARIES} 
                                sock++ 
                                dsaudio 
                                streamsrv )
set_target_properties( dsserver  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 
###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET dsserver
                   POST_BUILD  
                   COMMAND  ${CMAKE_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh dsserver 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS dsserver DESTINATION  bin)
