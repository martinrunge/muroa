cmake_minimum_required (VERSION 2.6)
project (libmstream)


set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../aux/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package( Threads REQUIRED )
find_package( log4cplus REQUIRED )


set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system date_time program_options REQUIRED )

add_definitions( ${log4cplus_DEFINITIONS} )

set( SRC           CStreamConnection.cpp
                   CStreamServer.cpp
                   CStreamSrvSM.cpp
                   CStreamCtrlConnection.cpp
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
                   cmds/StreamCtrlIDs.cpp
                   cmds/CmdStreamBase.cpp
                   cmds/CmdStreamReset.cpp
                   cmds/CmdStreamConnect.cpp
                   ctrlrpcxml/CStreamCtrlParserSM.cpp
                   ctrlrpcxml/CStreamCtrlXml.cpp
                   ctrlrpcxml/StreamCtrlXMLCmds.cpp
                                      )                  

set( HEADERS       CStreamConnection.h
                   CStreamServer.h
                   CStreamSrvSM.h
                   CStreamCtrlConnection.h
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
                   cmds/StreamCtrlIDs.h
                   cmds/CmdStreamBase.h
                   cmds/cmds.h
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
###################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
###################################################################################
add_custom_command(TARGET mstream
                   POST_BUILD  
                   COMMAND  ${CMAKE_CURRENT_SOURCE_DIR}/../aux/scripts/mkgdbindex.sh libmstream.so 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")
                                   
###################################################################################                                   


install(TARGETS mstream DESTINATION  lib)
