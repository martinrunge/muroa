cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

set(VERBOSE 1)

find_package(PkgConfig)


set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system regex REQUIRED )
find_package( Sqlite REQUIRED )
find_package( TagLib REQUIRED )
find_package ( ALSA REQUIRED )


# add_subdirectory(unittest)

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


include_directories(${SOURCE_DIR} 
                    ${Boost_INCLUDE_DIRS} 
                    ${SQLITE_INCLUDE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroadaemon 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroa++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libstreamsrv
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libsock++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libdsaudio) 

link_directories(${BINARY_DIR} 
                 ${SQLITE_LIBRARY_DIRS} 
                 "${CMAKE_CURRENT_BINARY_DIR}/../libmuroadaemon/build_x86" 
                 "${CMAKE_CURRENT_BINARY_DIR}/../libmuroa++/build"
                 "${CMAKE_CURRENT_BINARY_DIR}/../libstreamsrv/build_x86"
                 "${CMAKE_CURRENT_BINARY_DIR}/../libsock++/build_x86"
                 )

ADD_EXECUTABLE(msessiond main.cpp ${SRCS} ${HEADERS} )

set_target_properties( msessiond PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiond    muroadaemon
                                   muroa++
                                   streamsrv
                                   sock++
                                   ${Boost_LIBRARIES} 
                                   ${SQLITE_LIBRARIES}
                                   avformat
                                   avcodec
                                   avutil 
                                   ${ALSA_LIBRARY}
                                   ao
                                   xdiff
                                   log4cplus
                                   )
                                   
                                   
################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
################################################################################

add_custom_command(TARGET msessiond
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh msessiond 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")

###############################################################################                                   
                                   
                                   
                                   
find_package ( Qt4 REQUIRED COMPONENTS QtCore QtGui QtNetwork QtXml QtDbus )

include ( ${QT_USE_FILE} )
                                   
set(unittests_SRCS     unittest/SessionTest.cpp
                       unittest/CFakeMediaCollection.cpp 
                       unittest/CDecoderTest.cpp 
                       # unittest/CMediaFileTest.cpp 
                       unittest/CDataBaseTest.cpp
                       # unittest/CTimeTest.cpp
                       # unittest/CSubProcessTest.cpp
                       # unittest/CMediaScannerTest.cpp
                       ../mmscanner/CStateDbUpdater.cpp
                       ../mmscanner/CMediaColUpdater.cpp
                       ../mmscanner/CFsScanner.cpp
                       ../mmscanner/CMediaScanner.cpp
                       ../mmscanner/CEventLoop.cpp
                       ../mmscanner/CMediaItemCategorizer.cpp
                       )

set(unittests_HEADERS  unittest/CFakeMediaCollection.h 
                       unittest/CDecoderTest.h
                       # unittest/CMediaFileTest.h 
                       unittest/CDataBaseTest.h
                       
                       # unittest/CTimeTest.h
                       # unittest/CSubProcessTest.h
                       # unittest/CMediaScannerTest.h
                       ../mmscanner/CStateDbUpdater.h
                       ../mmscanner/CMediaColUpdater.h
                       ../mmscanner/CFsScanner.h
                       ../mmscanner/CMediaScanner.h
                       ../mmscanner/CEventLoop.h
                       ../mmscanner/CMediaItemCategorizer.h
                       )
                                   
ADD_EXECUTABLE(msessiondtest ${unittests_SRCS} ${unittests_HEADERS} ${SRCS} ${HEADERS} )

set_target_properties( msessiondtest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiondtest  cppunit 
                                     muroa++
                                     muroadaemon
                                     streamsrv
                                     sock++
                                     ${Boost_LIBRARIES} 
                                     ${Taglib_LIBRARIES} 
                                     ${QT_LIBRARIES} 
                                     ${SQLITE_LIBRARIES} 
                                     xdiff
                                     ${ALSA_LIBRARY}
                                     avahi-client
                                     avahi-qt4
                                     avformat
                                     avcodec
                                     avutil 
                                     ao 
                                     qxcppunitd 
                                     qxrunnerd
                                     log4cplus
                                     )
                                     
################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
################################################################################

add_custom_command(TARGET msessiondtest
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh msessiondtest 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")

################################################################################                                     
                                   
                                   