cmake_minimum_required (VERSION 2.6)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake" ${CMAKE_MODULE_PATH})

find_package ( Qt4 REQUIRED )

set(Boost_USE_STATIC_LIBS   ON )
set(Boost_USE_MULTITHREADED ON )
find_package( Boost 1.36.0 COMPONENTS filesystem REQUIRED )
find_package( TagLib REQUIRED )
find_package( Sqlite REQUIRED )

add_definitions( ${QT4_DEFINITIONS} ${SQLITE_DEFINITIONS})

set(unittests_SRCS  MediaprocessingTestApp.cpp 
                    CDecoderTest.cpp CMediaFileTest.cpp 
                    ../mediaprocessing/CCollectionUpdater.cpp 
                    ../CCollectionItem.cpp 
                    ../CItemBase.cpp 
                    CDataBaseTest.cpp
                    ../CStateDB.cpp )

include ( ${QT_USE_FILE} )
qt4_wrap_cpp( MOC_SOURCES ${MOC_H} ${MOC_MEDPROC_H} )

include_directories(${SOURCE_DIR} ../mediaprocessing ${Boost_INCLUDE_DIRS} ${Taglib_CFLAGS} ${SQLITE_INCLUDE_DIR}) 
link_directories(${BINARY_DIR}  ${QT_LIB_DIR} ${SQLITE_LIBRARY_DIRS})

ADD_EXECUTABLE(mediatest ${unittests_SRCS})

TARGET_LINK_LIBRARIES(mediatest cppunit ${Boost_LIBRARIES} ${Taglib_LIBRARIES} ${QT_LIBRARIES} ${SQLITE_LIBRARIES})
