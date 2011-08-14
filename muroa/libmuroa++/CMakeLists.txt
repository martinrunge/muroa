cmake_minimum_required (VERSION 2.6)
project (libmuroa++)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

add_subdirectory(unittest)
add_subdirectory(util/rehash)

set(Boost_USE_MULTITHREADED ON)
find_package( Boost 1.36.0 COMPONENTS system regex REQUIRED )


add_definitions(  )


set( SRC           CRootItem.cpp
                   CItemType.cpp
                   CItemBase.cpp
                   CCategoryItem.cpp
                   IContentItem.cpp
                   CMediaItem.cpp
                   CPlaylistItem.cpp
                   CNextlistItem.cpp
                   CDiff.cpp
                   MuroaExceptions.cpp
                   CUtils.cpp
                   libmain.cpp
                   IRPC.cpp
                   muroaxml/CMuroaXml.cpp
                   muroaxml/CParserStateMachine.cpp 
                   )
                  

set( HEADERS       CRootItem.h
                   CItemType.h
                   CItemBase.h
                   CCategoryItem.h
                   IContentItem.h
                   CMediaItem.h
                   CPlaylistItem.cpp
                   CNextlistItem.cpp
                   CDiff.h
                   CUtils.h
                   MuroaExceptions.h
                   IRPC.h
                   muroaxml/CMuroaXml.h
                   muroaxml/CParserStateMachine.cpp
                   )
                   
include_directories(${CMAKE_SOURCE_DIR} ${Boost_INCLUDE_DIRS})

link_directories(  )

add_library( muroa++ SHARED ${SRC} ${HEADERS} )
target_link_libraries( muroa++  ${Boost_LIBRARIES} )
set_target_properties( muroa++  PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

install(TARGETS   muroa++ DESTINATION  lib)
