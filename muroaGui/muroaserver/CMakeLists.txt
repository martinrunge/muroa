cmake_minimum_required (VERSION 2.6)
project (muroaserver)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package(PkgConfig)

find_package ( Qt4 REQUIRED )
find_package ( ALSA REQUIRED )

set(Boost_USE_STATIC_LIBS   ON)
set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.36.0 COMPONENTS filesystem system REQUIRED )
find_package( TagLib REQUIRED)
find_package( Sqlite REQUIRED )

add_subdirectory(unittest)
add_subdirectory(mediascanner)


add_definitions( ${QT4_DEFINITIONS} ${SQLITE_DEFINITIONS} -D__STDC_CONSTANT_MACROS)


set( SRC          CCollectionItem.cpp
                  CContentHandler.cpp
                  CDnsSdAvahi.cpp
                  CItemBase.cpp
                  CNetwork.cpp
                  CSession.cpp
                  IDnsSd.cpp
                  CConnection.cpp
                  CDiff.cpp
                  CDnsSdAvahiViaQtDBus.cpp
                  CMuroaServer.cpp
                  CPlaylistItem.cpp
                  CStateMachine.cpp
                  CStateDB.cpp
                  CSubProcess.cpp
                  CMediaScannerCtrl.cpp
                  Exceptions.cpp
                  main.cpp )
                  
set( MEDPROC_SRC    mediaprocessing/IAudioIO.cpp
                    mediaprocessing/CAudioIOAlsa.cpp
                    mediaprocessing/CAudioIOlibao.cpp
                    mediaprocessing/CDecoder.cpp
                    mediaprocessing/CStream.cpp
                    mediaprocessing/CCollectionUpdater.cpp ) 
                  

set( MEDPROC_MOC_H  mediaprocessing/CStream.h
                    mediaprocessing/CStream.h )
                    
set( MOC_H          CConnection.h
                    CDnsSdAvahiViaQtDBus.h
                    CMuroaServer.h
                    CStateMachine.h
                    CNetwork.h
                    CSession.h )
 
set(HEADERS         CCollection.h   
                    CDiff.h
                    CTime.h
                    CItemBase.h
                    CCollectionItem.h
                    CPlaylistItem.h
                    CContentHandler.h
                    IDnsSd.h 
                    CDnsSdAvahi.h
                    Exceptions.h
                    CSubProcess.h
                    CMediaScannerCtrl.h
                    
                    mediaprocessing/IAudioIO.h
                    mediaprocessing/CAudioIOAlsa.h
                    mediaprocessing/CAudioIOlibao.h
                    mediaprocessing/CDecoder.h
                    mediaprocessing/CCollectionUpdater.h )
 
 set( MSG_SRC       mediascanner/CMsgBase.cpp
                    mediascanner/CMsgQuit.cpp
                    mediascanner/CMsgResponse.cpp )
                    
 set( MSG_HDR       mediascanner/CMsgBase.h
                    mediascanner/CMsgQuit.h
                    mediascanner/CMsgResponse.h )
 
 
SET(UIS             MuroaServer.ui )


QT4_WRAP_UI(UIS_H ${UIS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )

set (QT_USE_QTXML true )
set (QT_USE_QTDBUS true )
set (QT_USE_QTNETWORK true )
include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MEDPROC_MOC_H} )

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${Boost_INCLUDE_DIRS} ${Taglib_CFLAGS} ${SQLITE_INCLUDE_DIR} )

link_directories( ${QT_LIB_DIR} )

add_executable( muroaserver ${SRC} ${MEDPROC_SRC} ${HEADERS} ${MOC_SOURCES} ${UIS_H} ${MSG_SRC} ${MSG_HDR})
target_link_libraries( muroaserver xdiff 
                                   avahi-client
                                   avahi-qt4
                                   ao
                                   ${ALSA_LIBRARY}
                                   avformat
                                   avcodec
                                   ${QT_LIBRARIES} 
                                   ${Boost_LIBRARIES}
                                   ${Taglib_LIBRARIES} 
                                   ${SQLITE_LIBRARIES} )

set_target_properties( muroaserver PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   muroaserver
        DESTINATION  bin)
