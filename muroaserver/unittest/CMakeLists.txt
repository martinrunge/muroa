cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)

find_package ( Qt4 REQUIRED COMPONENTS QtCore QtGui QtNetwork QtXml QtDbus )
find_package ( ALSA REQUIRED )

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )
find_package( TagLib REQUIRED )
find_package( Sqlite REQUIRED )

add_definitions( ${QT4_DEFINITIONS} ${SQLITE_DEFINITIONS} -D__STDC_CONSTANT_MACROS)

set(unittests_SRCS  MediaprocessingTestApp.cpp
                    CFakeMediaCollection.cpp 
                    CDecoderTest.cpp 
                    CMediaFileTest.cpp 
                    CDataBaseTest.cpp
                    CTimeTest.cpp
                    CSubProcessTest.cpp
                    CMediaScannerTest.cpp

                    ../mediaprocessing/CCollectionUpdater.cpp
                    ../mediaprocessing/CStream.cpp
                    ../mediaprocessing/CDecoder.cpp
                    ../mediaprocessing/CStream.cpp
                    ../mediaprocessing/CAudioIOAlsa.cpp
                    ../mediaprocessing/IAudioIO.cpp

                    ../CCollectionItem.cpp
                    ../CPlaylistItem.cpp  
                    ../CItemBase.cpp 
                    ../CSession.cpp
                    ../CDiff.cpp
                    ../CConnection.cpp
                    ../CStateMachine.cpp
                    ../CContentHandler.cpp
                    ../CStateDbBase.cpp
                    ../CStateDB.cpp
                    ../Exceptions.cpp 
                    ../CSubProcess.cpp
                    ../CMediaScannerCtrl.cpp
                    ../CMuroaServer.cpp 
                    ../CDnsSdAvahi.cpp
                    ../CNetwork.cpp
                    ../IDnsSd.cpp
                     )
                    
set( MOC_H          ../CSession.h 
                    ../CConnection.h 
                    ../CStateMachine.h
                    ../CMuroaServer.h
                    ../CDnsSdAvahi.h
                    ../CNetwork.h )
                    
                     
 set( MSG_SRC       ../mediascanner/CEventLoop.cpp
                    ../mediascanner/CMediaScanner.cpp
                    ../mediascanner/CFsScanner.cpp
                    ../mediascanner/CStateDbUpdater.cpp
                    ../mediascanner/CMediaItem.cpp       
                    ../mediascanner/CMsgBase.cpp
                    ../mediascanner/CMsgError.cpp
                    ../mediascanner/CMsgQuit.cpp
                    ../mediascanner/CMsgResponse.cpp
                    ../mediascanner/CMsgOpenDb.cpp
                    ../mediascanner/CMsgScanDir.cpp 
                    ../mediascanner/CMsgFinished.cpp 
                    ../mediascanner/CMsgProgress.cpp 
                    ../mediascanner/CMsgCollectionChanged.cpp
                    )
                    
 set( MSG_HDR       ../mediascanner/CEventLoop.h
                    ../mediascanner/CMediaScanner.h
                    ../mediascanner/CFsScanner.h
                    ../mediascanner/CStateDbUpdater.h
                    ../mediascanner/CMediaItem.h
                    ../mediascanner/CMsgBase.h
                    ../mediascanner/CMsgError.h
                    ../mediascanner/CMsgQuit.h
                    ../mediascanner/CMsgResponse.h
                    ../mediascanner/CMsgOpenDb.h
                    ../mediascanner/CMsgScanDir.h 
                    ../mediascanner/CMsgFinished.h 
                    ../mediascanner/CMsgProgress.h
                    ../mediascanner/CMsgCollectionChanged.h
                    )
 
                    

set( MOC_MEDPROC_H  ../mediaprocessing/CStream.h )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${SOURCE_DIR} ../mediaprocessing ${Boost_INCLUDE_DIRS} ${Taglib_CFLAGS} ${SQLITE_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR}/..) 

link_directories(${BINARY_DIR}  ${QT_LIB_DIR} ${SQLITE_LIBRARY_DIRS}  )

ADD_EXECUTABLE(mediatest ${unittests_SRCS} ${MOC_SOURCES} ${MSG_SRC} ${MSG_HDR})

set_target_properties( mediatest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(mediatest cppunit 
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
                                qxcppunitd 
                                qxrunnerd
                                )
