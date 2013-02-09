cmake_minimum_required (VERSION 2.6)


set(VERBOSE 1)

if( "${ARCH}" STREQUAL "")
MESSAGE("Setting ARCH to x86")
SET(ARCH x86)
endif( "${ARCH}" STREQUAL "")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../msessiond/cmake" ${CMAKE_MODULE_PATH})

find_package(PkgConfig)

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )
find_package( EXPAT REQUIRED )
find_package( libavahiclient REQUIRED )
find_package( libavahicommon REQUIRED )
find_package ( ALSA REQUIRED )
find_package ( LibSamplerate REQUIRED )
find_package ( ZLIB REQUIRED )
find_package ( libavcodec REQUIRED )
find_package ( libavformat REQUIRED )
find_package ( libavutil REQUIRED )
find_package ( log4cplus REQUIRED )

# add_subdirectory(unittest)

add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS             cpacketringbuffer.cpp
                      cplayloop.cpp
                      cresampler.cpp
		              cfilterdef.cpp
                      cfixpointresampler.cpp
                      cfloatresampler.cpp
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
                      cfloatresampler.h
                      cmuroad.h
                      cplayer.h
                      crecvloop.h
                      cringbuffer.h
                      )                    


include_directories(${SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS}
                    ${libavcodec_INCLUDE_DIRS} 
                    ${libavformat_INCLUDE_DIRS} 
                    ${log4cplus_INCLUDE_DIRS}
                    ${CMAKE_SOURCE_DIR} 
                    ${CMAKE_SOURCE_DIR}/../libmuroadaemon 
                    ${CMAKE_SOURCE_DIR}/../libmuroa++
                    ${CMAKE_SOURCE_DIR}/../libsock++
                    ${CMAKE_SOURCE_DIR}/../libdsaudio
                    ${ALSA_INCLUDE_DIR}
                    ) 

#
#                 "/home/martin/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/lib"
#                 "/home/martin/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/arm-linux-gnueabihf/libc/lib/arm-linux-gnueabihf"
#                 "/home/martin/raspi/tools/sysroot/lib/arm-linux-gnueabihf"
#
                 
link_directories(/home/martin/x-tools/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/lib/arm-linux-gnueabihf
                 ${BINARY_DIR} 
                 ${SQLITE_LIBRARY_DIRS} 
                 ${BOOST_LIBRARYDIR}
                 ${libavcodec_LIBRARY_DIRS}
                 ${LibSamplerate_CFLAGS}
                 "${CMAKE_SOURCE_DIR}/../libmuroadaemon/build_${ARCH}" 
#                 "${CMAKE_SOURCE_DIR}/../libmuroa++/build_${ARCH}"
                 "${CMAKE_SOURCE_DIR}/../libsock++/build_${ARCH}"
                 "${CMAKE_SOURCE_DIR}/../libdsaudio/build_${ARCH}")

ADD_EXECUTABLE(muroad ${SRCS} ${HEADERS} )

set_target_properties( muroad PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 
# SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,--eh-frame-hdr)
# SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,-shared-libgcc)

TARGET_LINK_LIBRARIES(muroad       ${Boost_LIBRARIES}
                                   ${libavcodec_LIBRARIES} 
                                   ${libavformat_LIBRARIES} 
                                   ${libavavutil_LIBRARIES} 
                                   ${libsamplerate_LIBRARY}
                                   ${ALSA_LIBRARY}
                                   ${log4cplus_LIBRARY}
                                   ${EXPAT_LIBRARIES}
                                   ${libavahiclient_LIBRARY} 
                                   ${libavahicommon_LIBRARY}
                                   pthread
                                   sock++
                                   dsaudio
                                   muroadaemon
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
                                   

                                   