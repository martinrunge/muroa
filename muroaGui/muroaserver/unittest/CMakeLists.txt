cmake_minimum_required (VERSION 2.6)

set(unittests_SRCS  MediaprocessingTestApp.cpp CDecoderTest.cpp)

include_directories(${SOURCE_DIR}) 
link_directories(${BINARY_DIR})

ADD_EXECUTABLE(mediatest ${unittests_SRCS})

TARGET_LINK_LIBRARIES(mediatest cppunit)
