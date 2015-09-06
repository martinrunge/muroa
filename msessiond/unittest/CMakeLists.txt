cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

include( MacroGDBIndex )

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
find_package( Qt4 REQUIRED COMPONENTS QtCore QtGui QtNetwork QtXml QtDbus )
find_package( CppUnit REQUIRED )

include ( ${QT_USE_FILE} )


add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS             ../sessionEx.cpp
                      ../CSessionContainer.cpp
                      ../CSession.cpp
                      ../CSessionStorage.cpp
                      ../CStreamClientHdl.cpp
                      ../CPlaylistIdProvider.cpp
                      ../CConnection.cpp
                      ../CRpc.cpp
                      ../CMediaScannerCtrl.cpp 
                      ../CSubProcess.cpp
                      ../CStateDbBase.cpp
                      ../CStateDB.cpp
                      ../CCmdDispatcher.cpp 
                      
                      ../../mmscanner/CMsgBase.cpp
                      ../../mmscanner/CMsgError.cpp
                      ../../mmscanner/CMsgQuit.cpp
                      ../../mmscanner/CMsgResponse.cpp
                      ../../mmscanner/CMsgFinished.cpp
                      ../../mmscanner/CMsgProgress.cpp
                      ../../mmscanner/CMsgOpenDb.cpp
                      ../../mmscanner/CMsgScanDir.cpp
                      ../../mmscanner/CMsgCollectionChanged.cpp
                      
                      ../mediaprocessing/IAudioIO.cpp
                      ../mediaprocessing/CAudioIOAlsa.cpp
                      ../mediaprocessing/CAudioIOlibao.cpp
                      ../mediaprocessing/CDecoder.cpp
                      ../mediaprocessing/CStream.cpp  ) 
                  

set( HEADERS          ../sessionEx.h
                      ../CSessionContainer.h
                      ../CSession.h
                      ../CSessionStorage.h
                      ../CStreamClientHdl.h
                      ../CPlaylistIdProvider.h
                      ../CConnection.h
                      ../CRpc.h
                      ../CMediaScannerCtrl.h 
                      ../CSubProcess.h
                      ../CStateDbBase.h
                      ../CStateDB.h
                      ../CCmdDispatcher.h 
                       
                      ../../mmscanner/CMsgBase.h
                      ../../mmscanner/CMsgError.h
                      ../../mmscanner/CMsgQuit.h
                      ../../mmscanner/CMsgResponse.h
                      ../../mmscanner/CMsgFinished.h
                      ../../mmscanner/CMsgProgress.h
                      ../../mmscanner/CMsgOpenDb.h
                      ../../mmscanner/CMsgScanDir.h
                      ../../mmscanner/CMsgCollectionChanged.h
                      
                      ../mediaprocessing/IAudioIO.h
                      ../mediaprocessing/CAudioIOAlsa.h
                      ../mediaprocessing/CAudioIOlibao.h
                      ../mediaprocessing/CDecoder.h
                      ../mediaprocessing/CStream.h
                      )                    


                                
set(unittests_SRCS      SessionTest.cpp
                        CFakeMediaCollection.cpp 
                        CDecoderTest.cpp 
                        # unittest/CMediaFileTest.cpp 
                        # unittest/CDataBaseTest.cpp
                        # unittest/CTimeTest.cpp
                        # unittest/CSubProcessTest.cpp
                        # unittest/CMediaScannerTest.cpp
                        ../../mmscanner/CStateDbUpdater.cpp
                        ../../mmscanner/CMediaColUpdater.cpp
                        ../../mmscanner/CFsScanner.cpp
                        ../../mmscanner/CMediaScanner.cpp
                        ../../mmscanner/CEventLoop.cpp
                        ../../mmscanner/CMediaItemCategorizer.cpp
                        )

set(unittests_HEADERS   CFakeMediaCollection.h 
                        CDecoderTest.h
                        # unittest/CMediaFileTest.h 
                        # unittest/CDataBaseTest.h
                        
                        # unittest/CTimeTest.h
                        # unittest/CSubProcessTest.h
                        # unittest/CMediaScannerTest.h
                        ../../mmscanner/CStateDbUpdater.h
                        ../../mmscanner/CMediaColUpdater.h
                        ../../mmscanner/CFsScanner.h
                        ../../mmscanner/CMediaScanner.h
                        ../../mmscanner/CEventLoop.h
                        ../../mmscanner/CMediaItemCategorizer.h
                        )
                        
                        
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/.. 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmcommons
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmuroa++
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmuroadaemon
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmstream
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libsock++
                    ${CMAKE_CURRENT_BINARY_DIR}/..
                    ${libavcodec_INCLUDE_DIR}
                    ${libavformat_INCLUDE_DIR}
                    ${libavutil_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS} 
                    ${libxdiff_INCLUDE_DIRS} 
                    ${CPPUNIT_INCLUDE_DIRS} ) 

                                
ADD_EXECUTABLE(msessiondtest ${unittests_SRCS} ${unittests_HEADERS} ${SRCS} ${HEADERS} ${CPPUNIT_INCLUDE_DIRS})

set_target_properties( msessiondtest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiondtest  ${CPPUNIT_LIBRARIES} 
                                    muroa++
                                    muroadaemon
                                    mstream
                                    sock++
                                    ${Boost_LIBRARIES} 
                                    ${TagLib_LIBRARIES} 
                                    ${QT_LIBRARIES} 
                                    ${SQLITE_LIBRARIES} 
                                    ${libxdiff_LIBRARIES}
                                    ${ALSA_LIBRARY}
                                    ${LibAO_LIBRARIES}
                                    avahi-client
                                    avahi-qt4
                                    ${libavformat_LIBRARY}
                                    ${libavcodec_LIBRARY}
                                    ${libavutil_LIBRARY} 
                                    ${log4cplus_LIBRARIES}                              
                                    qxcppunitd 
                                    qxrunnerd
                                    )
                                    
ADD_GDB_INDEX( msessiondtest )
                                    