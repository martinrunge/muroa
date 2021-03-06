cmake_minimum_required (VERSION 2.6)
project (libmuroa++)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

include( MacroGDBIndex )

if(unittests)
       # unittests triggered by toplevel CMakeLists.txt
       #  add_subdirectory(unittest)
  add_subdirectory(util/rehash)
endif()

find_package( Threads REQUIRED )

set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.36.0 COMPONENTS system filesystem regex REQUIRED )
find_package( EXPAT REQUIRED )
find_package( libxdiff REQUIRED )

add_definitions(  )


set( SRC           CRootItem.cpp
                   CRootItemIterator.cpp
                   CItemType.cpp
                   CItemBase.cpp
                   CCategoryItem.cpp
                   # CCategoryItemIterator.cpp
                   IContentItem.cpp
                   CMediaItem.cpp
                   CPlaylistItem.cpp
                   CNextlistItem.cpp
                   CStreamClientItem.cpp
                   CDiff.cpp
                   # CUtils.cpp
                   # libmain.cpp
                   muroaxml/IRPC.cpp
                   muroaxml/CMuroaXml.cpp
                   muroaxml/xmlCommands.cpp
                   muroaxml/CParserStateMachine.cpp
                   cmds/Cmd.cpp 
                   cmds/SimpleCmds.cpp
                   cmds/CmdError.cpp 
                   cmds/CmdProgress.cpp 
                   cmds/CmdFinished.cpp 
                   cmds/CmdEditMediaCol.cpp 
                   cmds/CmdEditPlaylist.cpp
                   cmds/CmdEditNextlist.cpp
                   cmds/CmdEditSessionState.cpp
                   cmds/CmdAddUnassignedStreamClient.cpp
                   cmds/CmdRmUnassignedStreamClient.cpp
                   cmds/CmdAddStreamClient.cpp
                   cmds/CmdRmStreamClient.cpp
                   )
                  

set( HEADERS       IItemModel.h
                   CRootItem.h
                   CRootItemIterator.h
                   CItemType.h
                   CItemBase.h
                   CCategoryItem.h
                   # CCategoryItemIterator.h
                   IContentItem.h
                   IIdProvider.h
                   CMediaItem.h
                   CPlaylistItem.h
                   CNextlistItem.h
                   CStreamClientItem.h
                   CDiff.h
                   # CUtils.h
                   # MuroaExceptions.h
                   muroaxml/IRPC.h
                   muroaxml/CMuroaXml.h
                   muroaxml/xmlCommands.h
                   muroaxml/CParserStateMachine.h
                   cmds/Cmd.h 
                   cmds/SimpleCmds.h 
                   cmds/CmdError.h 
                   cmds/CmdProgress.h 
                   cmds/CmdFinished.h
                   cmds/CmdEditMediaCol.h 
                   cmds/CmdEditPlaylist.h
                   cmds/CmdEditNextlist.h
                   cmds/CmdEditSessionState.h
                   cmds/CmdAddUnassignedStreamClient.h
                   cmds/CmdRmUnassignedStreamClient.h
                   cmds/CmdAddStreamClient.h
                   cmds/CmdRmStreamClient.h
                   )
                   
include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_SOURCE_DIR}/../libmcommons 
                    ${Boost_INCLUDE_DIRS} 
                    ${EXPAT_INCLUDE_DIRS}
                    ${libxdiff_INCLUDE_DIRS} 
                   )

link_directories(  )

add_library( muroa++ SHARED ${SRC} ${HEADERS} )
target_link_libraries( muroa++  mcommons
                                ${Boost_LIBRARIES} 
                                ${EXPAT_LIBRARIES} 
                                ${LIBXDIFF_LIBRARIES} )
set_target_properties( muroa++  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" )

ADD_GDB_INDEX( muroa++ ) 

install(TARGETS   muroa++ DESTINATION  lib)
