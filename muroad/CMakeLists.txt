cmake_minimum_required (VERSION 2.6)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

if( "${ARCH}" STREQUAL "")
MESSAGE("Setting ARCH to x86")
SET(ARCH x86)
endif( "${ARCH}" STREQUAL "")

include( MacroGDBIndex )

find_package(PkgConfig)
find_package( Threads REQUIRED )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )
#find_package( EXPAT REQUIRED )
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

set( SRCS             main.cpp
                      cmuroad.cpp
                      CPlayer.cpp
                      CCtrlConnectionMgr.cpp
                      CCtrlConnection.cpp
                      CMediaStreamConnection.cpp
                      crecvloop.cpp
                      cplayloop.cpp
                      CPerSSRCRingBuffer.cpp
                      CPacketRingBuffer.cpp
                      cresampler.cpp
		              cfilterdef.cpp
                      cfixpointresampler.cpp
                      cfloatresampler.cpp
                      cringbuffer.cpp 
                      )

set( HEADERS          cmuroad.h
                      CPlayer.h
                      CCtrlConnectionMgr.h
                      CCtrlConnection.h
                      CMediaStreamConnection.h
                      crecvloop.h
                      cplayloop.h
                      CPerSSRCRingBuffer.h
                      CPacketRingBuffer.h
                      cresampler.h
		              cfilterdef.h
                      cfixpointresampler.h
                      cfloatresampler.h
                      cringbuffer.h
                      )                    


include_directories(${SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS}
                    ${libavcodec_INCLUDE_DIRS} 
                    ${libavformat_INCLUDE_DIRS} 
                    ${log4cplus_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmcommons 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroadaemon 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroa++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libsock++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmstream
                    ${ALSA_INCLUDE_DIR}
                    ) 
                 
link_directories(${BINARY_DIR})
ADD_EXECUTABLE(muroad ${SRCS} ${HEADERS} )

set_target_properties( muroad PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 
# SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,--eh-frame-hdr)
# SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,-shared-libgcc)

TARGET_LINK_LIBRARIES(muroad       ${CMAKE_THREAD_LIBS_INIT} 
                                   ${Boost_LIBRARIES}
                                   ${libavcodec_LIBRARIES} 
                                   ${libavformat_LIBRARIES} 
                                   ${libavavutil_LIBRARIES} 
                                   ${libsamplerate_LIBRARY}
                                   ${ALSA_LIBRARY}
                                   ${log4cplus_LIBRARIES}
                                   ${EXPAT_LIBRARIES}
                                   ${libavahiclient_LIBRARY} 
                                   ${libavahicommon_LIBRARY}
                                   sock++
                                   mstream
                                   muroadaemon
                                   rt
                                   )
                                   

file(COPY   etc/muroad.conf      DESTINATION .)                              
file(COPY   log.properties       DESTINATION .)                              
      
                                   
ADD_GDB_INDEX( muroad )            

install(TARGETS muroad               DESTINATION bin)
install(FILES   etc/muroad.conf      DESTINATION /etc)
install(FILES   files/muroad.service DESTINATION /lib/systemd/system)

                                   
