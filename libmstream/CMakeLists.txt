cmake_minimum_required (VERSION 2.6)
project (libmstream)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

include( MacroGDBIndex )

find_package( Threads REQUIRED )
find_package( log4cplus REQUIRED )
find_package( libavutil REQUIRED )


set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )

add_definitions( ${log4cplus_DEFINITIONS} )

set( SRC           CStreamServer.cpp
                   CRenderClientDesc.cpp
                   CStreamClientDiscovery.cpp
                   CStreamCtrlConnection.cpp
                   CMediaStreamProvider.cpp
                   CStreamConnection.cpp
                   CUDPSocket.cpp
                   CStreamSrvSM.cpp
                   CStreamClientSM.cpp
                   caudioframe.cpp
                   caudiooss.cpp
                   crtppacket.cpp
                   iaudioio.cpp
                   caudioioalsa.cpp
                   ccalibratedac.cpp
                   csync.cpp  
                   IRenderCmds.cpp
                   CTimeSyncPkt.cpp
                   CTimeService.cpp
                   CTimeServiceCtrl.cpp
                   cmds/CmdStream.cpp
                   cmds/StreamCtrlIDs.cpp
                   cmds/CmdStreamBase.cpp
                   cmds/CmdStreamReset.cpp
                   cmds/CmdStreamConnect.cpp
                   ctrlrpcxml/CStreamCtrlParserSM.cpp
                   ctrlrpcxml/CStreamCtrlXml.cpp
                   ctrlrpcxml/StreamCtrlXMLCmds.cpp
                   )                  

set( HEADERS       CStreamServer.h
                   CRenderClientDesc.h
                   CStreamClientDiscovery.h
                   CStreamCtrlConnection.h
                   CMediaStreamProvider.h
                   CStreamConnection.h
                   CUDPSocket.h
                   CStreamSrvSM.h
                   ISrvSMActions.h
                   CStreamClientSM.h
                   IClientSMActions.h
                   caudioframe.h
                   caudiooss.h
                   crtppacket.h
                   iaudioio.h
                   caudioioalsa.h
                   ccalibratedac.h
                   csync.h 
                   IRenderCmds.h
                   CTime.h
                   CTimeSyncPkt.h
                   CTimeService.h
                   CTimeServiceCtrl.h
                   Exceptions.h
                   cmds/CmdStream.h
                   cmds/StreamCtrlIDs.h
                   cmds/CmdStreamBase.h
                   cmds/CmdStreamReset.h
                   cmds/CmdStreamConnect.h
                   ctrlrpcxml/IStreamCtrlRPC.h
                   ctrlrpcxml/CStreamCtrlParserSM.h
                   ctrlrpcxml/CStreamCtrlXml.h
                   ctrlrpcxml/StreamCtrlXMLCmds.h
                   )
                  
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmcommons
                    ${Boost_INCLUDE_DIRS}  
                    ${log4cplus_INCLUDE_DIRS}
                    ${libavutil_INCLUDE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libsock++ 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmuroadaemon )

link_directories( "${CMAKE_SOURCE_DIR}/../libsock++/build_x86" 
                   ${BOOST_LIBRARYDIR})

add_library( mstream SHARED  ${SRC} ${HEADERS} )
target_link_libraries( mstream ${CMAKE_THREAD_LIBS_INIT} 
                                ${Boost_LIBRARIES} 
                                ${log4cplus_LIBRARY}
                                mcommons
                                sock++
                                muroadaemon 
                                )
set_target_properties( mstream  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x -pthread" ) 

ADD_GDB_INDEX( mstream )

install(TARGETS mstream DESTINATION  lib)
