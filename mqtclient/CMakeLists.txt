cmake_minimum_required (VERSION 2.6)
project (mqtclient)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

include( MacroGDBIndex )

find_package ( Qt4 COMPONENTS QtGui QtCore QtNetwork REQUIRED )
find_package( Avahi COMPONENTS common client qt4 REQUIRED ) 


add_definitions( ${QT4_DEFINITIONS} )


set( SRC           CQtClient.cpp
                   CSession.cpp
                   CConnection.cpp
                   CClientSM.cpp
                   CServiceBrowser.cpp
                   CServiceDesc.cpp
                   CDnsSdBase.cpp
                   CDnsSdServiceBrowserAvahi.cpp
                   CPreferencesDlg.cpp
                   CSessionAdminDlg.cpp
                   CMuroaListModel.cpp
                   CMuroaTreeModel.cpp
                   CRenderClientsListModel.cpp
                   CMediaColView.cpp
                   CPlaylistView.cpp
                   CNextlistView.cpp
                   CRenderClientsListView.cpp
                   CModelDiff.cpp
                   CDiffBuilder.cpp
                   CRenderClientsDiffBuilder.cpp
                   
                   cmds/CmdBase.cpp
                   cmds/CmdGetMediaCol.cpp
                   cmds/CmdGetPlaylist.cpp
                   cmds/CmdGetNextlist.cpp
                   cmds/CmdGetSessionState.cpp
                   cmds/CmdEditMediaCol.cpp
                   cmds/CmdEditPlaylist.cpp
                   cmds/CmdEditNextlist.cpp
                   cmds/CmdEditSessionState.cpp
                   cmds/CmdScanCollection.cpp
                   
                   main.cpp )
                  

set( MOC_H         CQtClient.h
                   CSession.h
                   CConnection.h
                   CClientSM.h
                   CServiceBrowser.h
                   CServiceDesc.h
                   CDnsSdBase.h
                   CDnsSdServiceBrowserAvahi.h
                   CPreferencesDlg.h
                   CSessionAdminDlg.h
                   CMuroaListModel.h
                   CMuroaTreeModel.h
                   CRenderClientsListModel.cpp
                   CMediaColView.h
                   CPlaylistView.h
                   CNextlistView.h
                   CRenderClientsListView.h
                   CModelDiff.h
                   CDiffBuilder.h
                   CRenderClientsDiffBuilder.h

                   cmds/CmdBase.h
                   cmds/CmdGetMediaCol.h
                   cmds/CmdGetPlaylist.h
                   cmds/CmdGetNextlist.h
                   cmds/CmdGetSessionState.h
                   cmds/CmdEditMediaCol.h
                   cmds/CmdEditPlaylist.h
                   cmds/CmdEditNextlist.h
                   cmds/CmdEditSessionState.h
                   cmds/CmdScanCollection.h
                   )
                   
SET(UIS            mQtClient.ui  
                   ServiceBrowser.ui 
                   PreferencesDlg.ui 
                   SessionAdminDlg.ui )

set(RCCS           mQtClient.qrc )      

QT4_WRAP_UI(UIS_H ${UIS})

QT4_ADD_RESOURCES( RCCS_SRC  ${RCCS} )

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR}  
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroa++
                     ${CMAKE_CURRENT_SOURCE_DIR}/../libmcommons
                     ${Avahi_INCLUDE_DIRS} 
                     ${libxdiff_INCLUDE_DIRS} )

link_directories( ${QT_LIB_DIR} "${CMAKE_SOURCE_DIR}/../libmuroa++/build")

add_executable( mqtclient ${SRC} ${MOC_SOURCES} ${UIS_H} ${RCCS_SRC})
target_link_libraries( mqtclient   muroa++
                                   mcommons
                                   ${Avahi_LIBRARIES}
                                   ${libxdiff_LIBRARIES}
                                   ${QT_LIBRARIES} )

set_target_properties( mqtclient PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

ADD_GDB_INDEX( mqtclient )

install(TARGETS   mqtclient
        DESTINATION  bin)
