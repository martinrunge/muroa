cmake_minimum_required (VERSION 2.6)
project (mqtclient)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 COMPONENTS QtGui QtCore QtNetwork REQUIRED )

add_definitions( ${QT4_DEFINITIONS} )


set( SRC           CQtClient.cpp
                   CSession.cpp
                   CConnection.cpp
                   CSessionSM.cpp
                   CServiceBrowser.cpp
                   CServiceDesc.cpp
                   CDnsSdBase.cpp
                   CDnsSdServiceBrowserAvahi.cpp
                   CPreferencesDlg.cpp
                   CMuroaListModel.cpp
                   CMuroaTreeModel.cpp
                   CMediaColView.cpp
                   CPlaylistView.cpp
                   CModelDiff.cpp
                   CDiffBuilder.cpp
                   
                   cmds/CmdBase.cpp
                   cmds/CmdGetMediaCol.cpp
                   cmds/CmdGetPlaylist.cpp
                   cmds/CmdGetNextlist.cpp
                   cmds/CmdEditMediaCol.cpp
                   cmds/CmdEditPlaylist.cpp
                   cmds/CmdEditNextlist.cpp
                   cmds/CmdScanCollection.cpp
                   
                   main.cpp )
                  

set( MOC_H         CQtClient.h
                   CSession.h
                   CConnection.h
                   CSessionSM.h
                   CServiceBrowser.h
                   CServiceDesc.h
                   CDnsSdBase.h
                   CDnsSdServiceBrowserAvahi.h
                   CPreferencesDlg.h
                   CMuroaListModel.h
                   CMuroaTreeModel.h
                   CMediaColView.h
                   CPlaylistView.h
                   CModelDiff.h
                   CDiffBuilder.h

                   cmds/CmdBase.h
                   cmds/CmdGetMediaCol.h
                   cmds/CmdGetPlaylist.h
                   cmds/CmdGetNextlist.h
                   cmds/CmdEditMediaCol.h
                   cmds/CmdEditPlaylist.h
                   cmds/CmdEditNextlist.h
                   cmds/CmdScanCollection.h
                   )
                   
SET(UIS            mQtClient.ui  
                   ServiceBrowser.ui 
                   PreferencesDlg.ui )

set(RCCS           mQtClient.qrc )      

QT4_WRAP_UI(UIS_H ${UIS})

QT4_ADD_RESOURCES( RCCS_SRC  ${RCCS} )

# Don't forget to include output directory, otherwise
# the UI file won't be wrapped!
# include_directories( ${ALSA_INCLUDE_DIRS} )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} )

include_directories( ${CMAKE_CURRENT_BINARY_DIR}  ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../libmuroa++)

link_directories( ${QT_LIB_DIR} "${CMAKE_SOURCE_DIR}/../libmuroa++/build")

add_executable( mqtclient ${SRC} ${MOC_SOURCES} ${UIS_H} ${RCCS_SRC})
target_link_libraries( mqtclient   avahi-client
                                   avahi-qt4
                                   muroa++
                                   xdiff
                                   ${QT_LIBRARIES} )

set_target_properties( mqtclient PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET mqtclient
                   POST_BUILD  
                   COMMAND  ${CMAKE_SOURCE_DIR}/../scripts/mkgdbindex.sh mqtclient 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS   mqtclient
        DESTINATION  bin)
