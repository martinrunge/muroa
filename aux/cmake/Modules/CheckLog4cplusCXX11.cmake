# - Check if the function exists.
# CHECK_LIBRARY_EXISTS (LIBRARY FUNCTION LOCATION VARIABLE)
#
#  LIBRARY  - the name of the library you are looking for
#  FUNCTION - the name of the function
#  LOCATION - location where the library should be found
#  VARIABLE - variable to store the result
#
# The following variables may be set before calling this macro to
# modify the way the check is run:
#
#  CMAKE_REQUIRED_FLAGS = string of compile command line flags
#  CMAKE_REQUIRED_DEFINITIONS = list of macros to define (-DFOO=bar)
#  CMAKE_REQUIRED_LIBRARIES = list of libraries to link

#=============================================================================
# Copyright 2002-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)



macro(CHECK_LOG4CPLUS_CXX11 LIBRARY LOCATION VARIABLE)
  if("${VARIABLE}" MATCHES "^${VARIABLE}$")
    message(STATUS "Checking if we can link against ${LIBRARY} with current CXXFLAGS (c++11 support in liblog4cplus)")
    if(CMAKE_REQUIRED_LIBRARIES)
      set(LIBRARY
        ${LIBRARY} ${CMAKE_REQUIRED_LIBRARIES})
    endif()

    message(STATUS "CMAKE_FLAGS ${CMAKE_FLAGS}")
    try_compile(${VARIABLE}
      ${CMAKE_BINARY_DIR}
      ${CMAKE_CURRENT_LIST_DIR}/CheckLog4cplusCXX11.cpp
      COMPILE_DEFINITIONS ${CMAKE_REQUIRED_DEFINITIONS}
      CMAKE_FLAGS
      -DCOMPILE_DEFINITIONS:STRING=${CMAKE_REQUIRED_FLAGS}
      -DLINK_DIRECTORIES:STRING=${LOCATION}
      -DLINK_LIBRARIES:STRING=${LIBRARY}
      OUTPUT_VARIABLE OUTPUT)

    if(${VARIABLE})
      message(STATUS "Trying to link ${LIBRARY} against sources compiled with C++11 support - success")
      set(${VARIABLE} 1 CACHE INTERNAL "Can link C++11 sources ${LIBRARY}")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeOutput.log
        "Trying to link ${LIBRARY} against sources compiled with C++11 support "
        "passed with the following output:\n"
        "${OUTPUT}\n\n")
    else()
      message(STATUS "Trying to link ${LIBRARY} against sources compiled with C++11 support - failed")
      set(${VARIABLE} "" CACHE INTERNAL "Can link C++11 sources ${LIBRARY}")
      file(APPEND ${CMAKE_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/CMakeError.log
        "Trying to link ${LIBRARY} against sources compiled with C++11 support "
        "failed with the following output:\n"
        "${OUTPUT}\n\n")
    endif()
  endif()
endmacro()

