cmake_minimum_required (VERSION 2.6)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})
include( MacroGDBIndex )

set(VERBOSE 1)

find_package(PkgConfig)


set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system regex REQUIRED )
find_package( Sqlite REQUIRED )
find_package( libavcodec REQUIRED )
find_package( libavformat REQUIRED )
find_package( libavutil REQUIRED )
find_package( TagLib REQUIRED )
find_package( ALSA REQUIRED )
find_package( LibAO REQUIRED)
find_package( log4cplus REQUIRED )
find_package( libxdiff REQUIRED )
find_package( Avahi COMPONENTS common client REQUIRED ) 


add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS             sessionEx.cpp
                      CSessionContainer.cpp
                      CSession.cpp
                      CSessionStorage.cpp
                      CStreamClientHdl.cpp
                      CPlaylistIdProvider.cpp
                      CConnection.cpp
                      CRpc.cpp
                      CMediaScannerCtrl.cpp 
                      CSubProcess.cpp
                      CStateDbBase.cpp
                      CStateDB.cpp
                      CCmdDispatcher.cpp 
                      
                      ../mmscanner/CMsgBase.cpp
                      ../mmscanner/CMsgError.cpp
                      ../mmscanner/CMsgQuit.cpp
                      ../mmscanner/CMsgResponse.cpp
                      ../mmscanner/CMsgFinished.cpp
                      ../mmscanner/CMsgProgress.cpp
                      ../mmscanner/CMsgOpenDb.cpp
                      ../mmscanner/CMsgScanDir.cpp
                      ../mmscanner/CMsgCollectionChanged.cpp
                      
                      mediaprocessing/IAudioIO.cpp
                      mediaprocessing/CAudioIOAlsa.cpp
                      mediaprocessing/CAudioIOlibao.cpp
                      mediaprocessing/CDecoder.cpp
                      mediaprocessing/CStream.cpp  ) 
                  

set( HEADERS          sessionEx.h
                      CSessionContainer.h
                      CSession.h
                      CSessionStorage.h
                      CStreamClientHdl.h
                      CPlaylistIdProvider.h
                      CConnection.h
                      CRpc.h
                      CMediaScannerCtrl.h 
                      CSubProcess.h
                      CStateDbBase.h
                      CStateDB.h
                      CCmdDispatcher.h 
                       
                      ../mmscanner/CMsgBase.h
                      ../mmscanner/CMsgError.h
                      ../mmscanner/CMsgQuit.h
                      ../mmscanner/CMsgResponse.h
                      ../mmscanner/CMsgFinished.h
                      ../mmscanner/CMsgProgress.h
                      ../mmscanner/CMsgOpenDb.h
                      ../mmscanner/CMsgScanDir.h
                      ../mmscanner/CMsgCollectionChanged.h
                      
                      mediaprocessing/IAudioIO.h
                      mediaprocessing/CAudioIOAlsa.h
                      mediaprocessing/CAudioIOlibao.h
                      mediaprocessing/CDecoder.h
                      mediaprocessing/CStream.h
                      )                    


include_directories(${Boost_INCLUDE_DIRS} 
                    ${log4cplus_INCLUDE_DIRS}
                    ${SQLITE_INCLUDE_DIRS}
                    ${libxdiff_INCLUDE_DIRS}
                    ${libavcodec_INCLUDE_DIR}
                    ${libavformat_INCLUDE_DIR}
                    ${libavutil_INCLUDE_DIR}
                    ${LibAO_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmcommons
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroadaemon 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroa++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmstream
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libsock++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libdsaudio) 

link_directories(${BINARY_DIR} 
                 "${CMAKE_CURRENT_BINARY_DIR}/../libmuroadaemon/build_x86" 
                 "${CMAKE_CURRENT_BINARY_DIR}/../libmuroa++/build"
                 "${CMAKE_CURRENT_BINARY_DIR}/../libmstream/build_x86"
                 "${CMAKE_CURRENT_BINARY_DIR}/../libsock++/build_x86"
                 )

ADD_EXECUTABLE(msessiond main.cpp ${SRCS} ${HEADERS} )

set_target_properties( msessiond PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiond    muroadaemon
                                   muroa++
                                   mstream
                                   sock++
                                   ${Boost_LIBRARIES}
                                   ${SQLITE_LIBRARIES}
                                   ${libavformat_LIBRARY}
                                   ${libavcodec_LIBRARY}
                                   ${libavutil_LIBRARY}
                                   ${ALSA_LIBRARY}
                                   ${LibAO_LIBRARIES}
                                   ${libxdiff_LIBRARIES}
                                   ${log4cplus_LIBRARIES}                              
                                   )
                                   
file(COPY   etc/muroa.conf  DESTINATION etc)
file(COPY   log.properties  DESTINATION .)                                                            
                                   
ADD_GDB_INDEX( msessiond )                                   
                                   
