cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 REQUIRED COMPONENTS QtCore QtNetwork QtXml QtDbus )
find_package ( ALSA REQUIRED )

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )
find_package( TagLib REQUIRED )
find_package( Sqlite REQUIRED )

add_definitions( ${QT4_DEFINITIONS} ${SQLITE_DEFINITIONS})

set(unittests_SRCS  MediaprocessingTestApp.cpp 
                    CDecoderTest.cpp 
                    CMediaFileTest.cpp 
                    CDataBaseTest.cpp
                    CTimeTest.cpp
                    ../mediaprocessing/CCollectionUpdater.cpp 
                    ../mediaprocessing/CStream.cpp
                    ../mediaprocessing/CDecoder.cpp
                    ../CCollectionItem.cpp
                    ../CPlaylistItem.cpp  
                    ../CItemBase.cpp 
                    ../CSession.cpp
                    ../CDiff.cpp
                    ../CConnection.cpp
                    ../CStateMachine.cpp
                    ../CContentHandler.cpp
                    ../mediaprocessing/CStream.cpp
                    ../mediaprocessing/CAudioIOAlsa.cpp
                    ../mediaprocessing/IAudioIO.cpp
                    ../CStateDB.cpp
                    ../Exceptions.cpp )
                    
set( MOC_H          ../CSession.h 
                    ../CConnection.h 
                    ../CStateMachine.h )

set( MOC_MEDPROC_H  ../mediaprocessing/CStream.h )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${SOURCE_DIR} ../mediaprocessing ${Boost_INCLUDE_DIRS} ${Taglib_CFLAGS} ${SQLITE_INCLUDE_DIR}) 

link_directories(${BINARY_DIR}  ${QT_LIB_DIR} ${SQLITE_LIBRARY_DIRS})

ADD_EXECUTABLE(mediatest ${unittests_SRCS} ${MOC_SOURCES})

set_target_properties( mediatest PROPERTIES  COMPILE_FLAGS "-std=gnu++0x" ) 

TARGET_LINK_LIBRARIES(mediatest cppunit 
                                ${Boost_LIBRARIES} 
                                ${Taglib_LIBRARIES} 
                                ${QT_LIBRARIES} 
                                ${SQLITE_LIBRARIES} 
                                xdiff
                                ${ALSA_LIBRARY}
                                avformat
                                avcodec )
