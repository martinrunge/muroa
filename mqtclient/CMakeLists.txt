cmake_minimum_required (VERSION 2.6)
project (mqtclient)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 COMPONENTS QtGui QtCore QtNetwork REQUIRED )

add_definitions( ${QT4_DEFINITIONS} )


set( SRC           CQtClient.cpp
                   CConnection.cpp
                   CServiceBrowser.cpp
                   CServiceDesc.cpp
                   CDnsSdBase.cpp
                   CDnsSdServiceBrowserAvahi.cpp
                   CPreferencesDlg.cpp
                   main.cpp )
                  

set( MOC_H         CQtClient.h
                   CConnection.h
                   CServiceBrowser.h
                   CServiceDesc.h
                   CDnsSdBase.h
                   CDnsSdServiceBrowserAvahi.h
                   CPreferencesDlg.h
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

link_directories( ${QT_LIB_DIR} )

add_executable( mqtclient ${SRC} ${MOC_SOURCES} ${UIS_H} ${RCCS_SRC})
target_link_libraries( mqtclient   avahi-client
#                                   avahi-service
                                   avahi-qt4
                                   ${QT_LIBRARIES} )

set_target_properties( mqtclient PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   mqtclient
        DESTINATION  bin)