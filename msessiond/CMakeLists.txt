cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(VERBOSE 1)

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem system REQUIRED )
find_package( Sqlite REQUIRED )
find_package( TagLib REQUIRED )

# add_subdirectory(unittest)

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
                      CMediaItemCategorizer.cpp
                      CCmdDispatcher.cpp 
                      
                      ../mmscanner/CMsgBase.cpp
                      ../mmscanner/CMsgError.cpp
                      ../mmscanner/CMsgQuit.cpp
                      ../mmscanner/CMsgResponse.cpp
                      ../mmscanner/CMsgFinished.cpp
                      ../mmscanner/CMsgProgress.cpp
                      ../mmscanner/CMsgOpenDb.cpp
                      ../mmscanner/CMsgScanDir.cpp
                      ../mmscanner/CMsgCollectionChanged.cpp
              
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
                      CMediaItemCategorizer.h 
                      CCmdDispatcher.h 
                       
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

ADD_EXECUTABLE(msessiond main.cpp ${SRCS} ${HEADERS} )

set_target_properties( msessiond PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 
SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,--eh-frame-hdr)
SET_TARGET_PROPERTIES( msessiond PROPERTIES  LINK_FLAGS -Wl,-shared-libgcc)

TARGET_LINK_LIBRARIES(msessiond    muroad
                                   muroa++
                                   ${Boost_LIBRARIES} 
                                   ${SQLITE_LIBRARIES}
                                   xdiff
                                   log4cplus
                                   )
                                   
                                   
################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
################################################################################

add_custom_command(TARGET msessiond
                   POST_BUILD  
                   COMMAND  ${CMAKE_SOURCE_DIR}/../scripts/mkgdbindex.sh msessiond 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")

###############################################################################                                   
                                   
                                   
                                   
find_package ( Qt4 REQUIRED COMPONENTS QtCore QtGui QtNetwork QtXml QtDbus )

include ( ${QT_USE_FILE} )
                                   
set(unittests_SRCS     unittest/SessionTest.cpp
                       unittest/CFakeMediaCollection.cpp 
                       unittest/CDecoderTest.cpp 
                       unittest/CCategorizerTest.cpp
                       # unittest/CMediaFileTest.cpp 
                       unittest/CDataBaseTest.cpp
                       # unittest/CTimeTest.cpp
                       # unittest/CSubProcessTest.cpp
                       # unittest/CMediaScannerTest.cpp
                       ../mmscanner/CStateDbUpdater.cpp
                       ../mmscanner/CFsScanner.cpp
                       ../mmscanner/CMediaScanner.cpp
                       ../mmscanner/CEventLoop.cpp
                       )

set(unittests_HEADERS  unittest/CFakeMediaCollection.h 
                       unittest/CDecoderTest.h
                       # unittest/CMediaFileTest.h 
                       unittest/CDataBaseTest.h
                       unittest/CCategorizerTest.h
                       # unittest/CTimeTest.h
                       # unittest/CSubProcessTest.h
                       # unittest/CMediaScannerTest.h
                       ../mmscanner/CStateDbUpdater.h
                       ../mmscanner/CFsScanner.h
                       ../mmscanner/CMediaScanner.h
                       ../mmscanner/CEventLoop.h
                       )
                                   
ADD_EXECUTABLE(msessiondtest ${unittests_SRCS} ${unittests_HEADERS} ${SRCS} ${HEADERS} )

set_target_properties( msessiondtest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(msessiondtest  cppunit 
                                     muroa++
                                     muroad
                                     ${Boost_LIBRARIES} 
                                     ${Taglib_LIBRARIES} 
                                     ${QT_LIBRARIES} 
                                     ${SQLITE_LIBRARIES} 
                                     xdiff
                                     ${ALSA_LIBRARY}
                                     avahi-client
                                     avahi-qt4
                                     avformat
                                     avcodec
                                     avutil  
                                     qxcppunitd 
                                     qxrunnerd
                                     )
                                     
################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
################################################################################

add_custom_command(TARGET msessiondtest
                   POST_BUILD  
                   COMMAND  ${CMAKE_SOURCE_DIR}/../scripts/mkgdbindex.sh msessiondtest 
                   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                   COMMENT "building gdb index")

################################################################################                                     
                                   
                                   