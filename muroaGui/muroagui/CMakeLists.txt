cmake_minimum_required (VERSION 2.6)
project (muroaserver)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 REQUIRED )

add_definitions( ${QT4_DEFINITIONS} )


set( SRC           CCollectionCommand.cpp
                   CCommandBase.cpp
                   CDnsSdBase.cpp
                   CModelDiff.cpp
                   CPlaylistItem.cpp
                   CServiceDesc.cpp
                   CCollectionItem.cpp
                   CConnection.cpp
                   CDnsSdServiceBrowserAvahi.cpp
                   CMuroaGui.cpp
                   CPlaylistModel.cpp
                   CSessionCommand.cpp
                   CCollectionModel.cpp
                   CDiffBuilder.cpp
                   CItemBase.cpp
                   CNextlistCommand.cpp
                   CPlaylistView.cpp
                   CStateMachine.cpp
                   CCollectionView.cpp
                   CDnsSdAvahiViaQtDBus.cpp
                   CModelBase.cpp
                   CPlaylistCommand.cpp
                   CServiceBrowser.cpp
                   main.cpp )
                  

set( MOC_H         CCollectionCommand.h
                   CCollectionModel.h
                   CConnection.h
                   CDnsSdBase.h
                   CModelBase.h
                   CNextlistCommand.h
                   CPlaylistModel.h
                   CServiceDesc.h
                   CCollection.h
                   CCollectionView.h
                   CDiffBuilder.h
                   CDnsSdServiceBrowserAvahi.h
                   CModelDiff.h
                   CPlaylistCommand.h
                   CPlaylistView.h
                   CSessionCommand.h
                   CCollectionItem.h
                   CCommandBase.h
                   CDnsSdAvahiViaQtDBus.h
                   CItemBase.h
                   CMuroaGui.h
                   CPlaylistItem.h
                   CServiceBrowser.h
                   CStateMachine.h )
                   
SET(UIS            MuroaGui.ui  
                   ServiceBrowser.ui )


QT4_WRAP_UI(UIS_H ${UIS})


# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )

set (QT_USE_QTXML true )
set (QT_USE_QTDBUS true )
set (QT_USE_QTNETWORK true )
include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR}  ${CMAKE_SOURCE_DIR})

link_directories( ${QT_LIB_DIR} )

add_executable( muroagui ${SRC} ${MOC_SOURCES} ${UIS_H})
target_link_libraries( muroagui    avahi-client
                                   avahi-qt4
                                   ${QT_LIBRARIES} )

set_target_properties( muroagui PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   muroagui
        DESTINATION  bin)
