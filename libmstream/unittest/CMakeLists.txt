cmake_minimum_required (VERSION 2.6)

if(NOT MUROA_AUX_DIR)
set(MUROA_AUX_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../aux")
endif()

set(CMAKE_MODULE_PATH "${MUROA_AUX_DIR}/cmake/Modules" ${CMAKE_MODULE_PATH})

find_package ( Qt4 REQUIRED COMPONENTS QtCore QtGui )
find_package ( CppUnit REQUIRED )

include( MacroGDBIndex )

add_definitions( ${QT4_DEFINITIONS} -D__STDC_CONSTANT_MACROS ${CPPUNIT_DEFINITIONS})

set( unittests_SRCS   CCmdStreamTests.cpp
                      CSrvSMActions.cpp
                      CStreamSMTest.cpp
                      CClientSMActions.cpp
                      CStreamClientSMTest.cpp
                      CStreamCtrlRpcDummy.cpp
                      LibMStreamTestApp.cpp
                      )
                    
set( unittests_H   
                      )
                    
set( HEADERS          CCmdStreamTests.h
                      CSrvSMActions.h
                      CClientSMActions.h
                      CStreamSMTest.h
                      CStreamClientSMTest.h
                      CStreamCtrlRpcDummy.h 
                      )                    
  

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/..
                    ${CMAKE_CURRENT_SOURCE_DIR}/../../libmcommons
                    ${Boost_INCLUDE_DIRS} 
                    ${CMAKE_CURRENT_BINARY_DIR}/..
                    ${CPPUNIT_INCLUDE_DIRS} ) 

link_directories(${BINARY_DIR}  ${QT_LIB_DIR} "${CMAKE_SOURCE_DIR}/../build" )

ADD_EXECUTABLE(libmstreamtest ${unittests_SRCS} ${unittests_H} ${HEADERS} )

set_target_properties( libmstreamtest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES( libmstreamtest ${CPPUNIT_LIBRARIES} 
                                      mstream
                                      ${QT_LIBRARIES} 
                                      qxcppunitd 
                                      qxrunnerd
                                      )

ADD_GDB_INDEX( libmstreamtest )
