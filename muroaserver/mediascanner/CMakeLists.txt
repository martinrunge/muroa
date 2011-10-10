cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )
find_package( TagLib REQUIRED )
find_package( Sqlite REQUIRED )

add_definitions( ${SQLITE_DEFINITIONS} )

set(SRCS     mediascanner.cpp
             CEventLoop.cpp
             CMediaScanner.cpp
             CFsScanner.cpp
             CStateDbUpdater.cpp
             ../CStateDbBase.cpp
             CMediaItem.cpp 
             ../CSubProcess.cpp
             ../Exceptions.cpp 
             CMsgBase.cpp
             CMsgError.cpp
             CMsgQuit.cpp
             CMsgResponse.cpp
             CMsgOpenDb.cpp
             CMsgScanDir.cpp
             CMsgFinished.cpp
             CMsgProgress.cpp
             CMsgCollectionChanged.cpp
             )

set( HEADER  CMediaScanner.h
             CEventLoop.h
             CFsScanner.h
             CStateDbUpdater.h
             ../CStateDbBase.h
             CMediaItem.h 
             ../CSubProcess.h
             ../Exceptions.h 
             CMsgBase.h
             CMsgError.h
             CMsgQuit.h
             CMsgResponse.h
             CMsgOpenDb.h
             CMsgScanDir.h
             CMsgFinished.h
             CMsgProgress.h
             CMsgCollectionChanged.h
             )


include_directories(${SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${Taglib_CFLAGS} ${SQLITE_INCLUDE_DIR}) 

link_directories(${BINARY_DIR} ${SQLITE_LIBRARY_DIRS}  )

ADD_EXECUTABLE(mediascanner ${SRCS} )

set_target_properties( mediascanner PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(mediascanner  
                                ${Boost_LIBRARIES} 
                                ${Taglib_LIBRARIES}  
                                ${SQLITE_LIBRARIES} 
                                )
