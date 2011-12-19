cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(VERBOSE 1)

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system REQUIRED )
find_package( Sqlite REQUIRED )


add_definitions(-D__STDC_CONSTANT_MACROS)

set( SRCS             sessionEx.cpp
                      CSessionContainer.cpp
                      CSession.cpp
                      CConnection.cpp
                      CRpc.cpp
                      CMediaScannerCtrl.cpp 
                      CSubProcess.cpp
                      CStateDbBase.cpp
                      CStateDB.cpp 
                      
                      ../mmscanner/CMsgBase.cpp
                      ../mmscanner/CMsgError.cpp
                      ../mmscanner/CMsgQuit.cpp
                      ../mmscanner/CMsgResponse.cpp
                      ../mmscanner/CMsgFinished.cpp
                      ../mmscanner/CMsgProgress.cpp
                      ../mmscanner/CMsgOpenDb.cpp
                      ../mmscanner/CMsgScanDir.cpp
                      ../mmscanner/CMsgCollectionChanged.cpp
 
                      main.cpp
                      )
                    
set( HEADERS          sessionEx.h
                      CSessionContainer.h
                      CSession.h
                      CConnection.h
                      CRpc.h
                      CMediaScannerCtrl.h 
                      CSubProcess.h
                      CStateDbBase.h
                      CStateDB.h
                       
                      ../mmscanner/CMsgBase.h
                      ../mmscanner/CMsgError.h
                      ../mmscanner/CMsgQuit.h
                      ../mmscanner/CMsgResponse.h
                      ../mmscanner/CMsgFinished.h
                      ../mmscanner/CMsgProgress.h
                      ../mmscanner/CMsgOpenDb.h
                      ../mmscanner/CMsgScanDir.h
                      ../mmscanner/CMsgCollectionChanged.h
                      )                    
  

include_directories(${SOURCE_DIR} ${Boost_INCLUDE_DIRS} ${SQLITE_INCLUDE_DIR} 
                    ${CMAKE_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/../libmuroad ${CMAKE_SOURCE_DIR}/../libmuroa++) 

link_directories(${BINARY_DIR} ${SQLITE_LIBRARY_DIRS} "${CMAKE_SOURCE_DIR}/../libmuroad/build" "${CMAKE_SOURCE_DIR}/../libmuroa++/build")

ADD_EXECUTABLE(msessiond ${SRCS} ${HEADERS} )

set_target_properties( msessiond PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiond    muroad
                                   muroa++
                                   ${Boost_LIBRARIES} 
                                   ${SQLITE_LIBRARIES}
                                   xdiff
                                   log4cplus
                                   )
