cmake_minimum_required (VERSION 2.6)


set(VERBOSE 1)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../msessiond/cmake" ${CMAKE_MODULE_PATH})

find_package(PkgConfig)

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )
find_package ( ALSA REQUIRED )
find_package ( LibSamplerate REQUIRED )


# add_subdirectory(unittest)

add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS             cpacketringbuffer.cpp
                      cplayloop.cpp
                      cresampler.cpp
		              cfilterdef.cpp
                      cfixpointresampler.cpp
                      cmuroad.cpp
                      main.cpp
                      cplayer.cpp
                      crecvloop.cpp
                      cringbuffer.cpp  ) 
                  

set( HEADERS          cpacketringbuffer.h
                      cplayloop.h
                      cresampler.h
		              cfilterdef.h
                      cfixpointresampler.h
                      cmuroad.h
                      cplayer.h
                      crecvloop.h
                      cringbuffer.h
                      )                    


include_directories(${SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS} 
                    ${CMAKE_SOURCE_DIR} 
                    ${CMAKE_SOURCE_DIR}/../libmuroad 
                    ${CMAKE_SOURCE_DIR}/../libmuroa++
                    ${CMAKE_SOURCE_DIR}/../libsock++
                    ${CMAKE_SOURCE_DIR}/../libdsaudio
                    ${ALSA_INCLUDE_DIR}
                    ) 

link_directories(${BINARY_DIR} 
                 ${SQLITE_LIBRARY_DIRS} 
                 ${BOOST_LIBRARYDIR}
                 ${LibSamplerate_CFLAGS}
                 "${CMAKE_SOURCE_DIR}/../libmuroad/build" 
                 "${CMAKE_SOURCE_DIR}/../libmuroa++/build"
                 "${CMAKE_SOURCE_DIR}/../libsock++/build"
                 "${CMAKE_SOURCE_DIR}/../libdsaudio/build")

ADD_EXECUTABLE(muroad ${SRCS} ${HEADERS} )

set_target_properties( muroad PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 
# SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,--eh-frame-hdr)
# SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,-shared-libgcc)

TARGET_LINK_LIBRARIES(muroad       ${Boost_LIBRARIES} 
                                   avformat
                                   avcodec
                                   avutil 
                                   ${LibSamplerate_LIBRARIES}
                                   ${ALSA_LIBRARY}
                                   ao
                                   log4cplus
                                   sock++
                                   dsaudio
                                   )
                                   
                                   
################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
################################################################################

add_custom_command(TARGET muroad
                   POST_BUILD  
                   COMMAND  ${CMAKE_SOURCE_DIR}/../scripts/mkgdbindex.sh muroad 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")

###############################################################################                                   
                                   

                                   