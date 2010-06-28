cmake_minimum_required (VERSION 2.6)
project (muroaserver)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 REQUIRED )
find_package ( ALSA REQUIRED )

add_definitions( ${QT4_DEFINITIONS} )


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
                  main.cpp )
                  
set( MEDPROC_SRC    mediaprocessing/CAudioIOAlsa.cpp
                    mediaprocessing/CAudioIOlibao.cpp
                    mediaprocessing/CDecoder.cpp
                    mediaprocessing/CStream.cpp
                    mediaprocessing/IAudioIO.cpp ) 
                  

set( MEDPROC_MOC_H  mediaprocessing/CAudioIOAlsa.h
                    mediaprocessing/CAudioIOlibao.h
                    mediaprocessing/CDecoder.h
                    mediaprocessing/CStream.h
                    mediaprocessing/IAudioIO.h )

set( MOC_H          CCollection.h
                    CConnection.h
                    CDiff.h
                    CDnsSdAvahiViaQtDBus.h
                    CMuroaServer.h
                    CPlaylistItem.h
                    CStateMachine.h
                    CCollectionItem.h
                    CContentHandler.h
                    CDnsSdAvahi.h
                    CItemBase.h
                    CNetwork.h
                    CSession.h
                    IDnsSd.h 
                    mediaprocessing/CStream.h )
 
SET(UIS             MuroaServer.ui )


QT4_WRAP_UI(UIS_H ${UIS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )

set (QT_USE_QTXML true )
set (QT_USE_QTDBUS true )
set (QT_USE_QTNETWORK true )
include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${CMAKE_CURRENT_BINARY_DIR} )

link_directories( ${QT_LIB_DIR} )

add_executable( muroaserver ${SRC} ${MEDPROC_SRC} ${MOC_SOURCES} ${UIS_H})
target_link_libraries( muroaserver xdiff 
                                   avahi-client
                                   avahi-qt4
                                   ao
                                   ${ALSA_LIBRARY}
                                   avformat
                                   avcodec
                                   ${QT_LIBRARIES} )

set_target_properties( muroaserver PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   muroaserver
        DESTINATION  bin)
