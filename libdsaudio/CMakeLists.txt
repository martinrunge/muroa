cmake_minimum_required (VERSION 2.6)
project (libdsaudio)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package( Boost 1.36.0 COMPONENTS date_time REQUIRED )
find_package ( ALSA REQUIRED )

add_definitions(  )

set( SRC           caudioframe.cpp
                   caudiooss.cpp
                   crtppacket.cpp
                   iaudioio.cpp
                   caudioioalsa.cpp
                   ccalibratedac.cpp
                   csync.cpp  )
                  

set( HEADERS       caudioframe.h
                   caudiooss.h
                   crtppacket.h
                   iaudioio.h
                   caudioioalsa.h
                   ccalibratedac.h
                   csync.h )
                  
include_directories(${CMAKE_SOURCE_DIR}
                    ${Boost_INCLUDE_DIRS} 
                    ${ALSA_INCLUDE_DIR})

link_directories(   ${BOOST_LIBRARYDIR})

add_library( dsaudio SHARED  ${SRC} ${HEADERS} )
target_link_libraries( dsaudio ${ALSA_LIBRARY} 
                               ${Boost_LIBRARIES} 
                               pthread )
set_target_properties( dsaudio  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 
###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET dsaudio
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/mkgdbindex.sh libdsaudio.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS dsaudio DESTINATION  lib)
