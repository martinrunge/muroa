cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})
set(VERBOSE 1)

find_package ( Qt4 REQUIRED COMPONENTS QtCore QtGui )

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )

add_definitions( ${QT4_DEFINITIONS} -D__STDC_CONSTANT_MACROS)

set( unittests_SRCS   CCollectionTest.cpp
                      CDiffTest.cpp
                      CRootItemDiffTest.cpp
                      CMuroaXmlRpcTest.cpp
                      LibMuroaTestApp.cpp
                      )
                    
set( unittests_H   
                      )
                    
 set( HEADERS         CCollectionTest.h
                      CDiffTest.h
                      CRootItemDiffTest.h
                      CMuroaXmlRpcTest.h
                      )                    
  

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${SOURCE_DIR} .. ${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_BINARY_DIR}/..) 

link_directories(${BINARY_DIR}  ${QT_LIB_DIR} "${CMAKE_SOURCE_DIR}/../build" )

ADD_EXECUTABLE(libmuroatest ${unittests_SRCS} ${unittests_H} ${HEADERS} )

set_target_properties( libmuroatest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(libmuroatest cppunit 
                                   muroa++
                                   ${Boost_LIBRARIES} 
                                   ${QT_LIBRARIES} 
                                   xdiff
                                   qxcppunitd 
                                   qxrunnerd
                                   )
