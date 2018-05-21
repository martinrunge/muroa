INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(ARCH "arm")

#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)

set(ARCH "arm")
set(HOME  $ENV{HOME})
set(CMAKE_LIBRARY_ARCHITECTURE "arm-linux-gnueabihf" )
set(SYSROOT  ${HOME}/x-tools-new/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf/sysroot)

# specify the cross compiler
SET(CMAKE_C_COMPILER   ${HOME}/x-tools-new/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${HOME}/x-tools-new/arm-unknown-linux-gnueabihf/bin/arm-unknown-linux-gnueabihf-g++)

# set(CMAKE_LIBRARY_ARCHITECTURE arm-linux-)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  ${HOME}/x-tools-new/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf/sysroot)
# where is the target environment
SET(SYSROOT_PATH  ${HOME}/x-tools-new/arm-unknown-linux-gnueabihf/arm-unknown-linux-gnueabihf/sysroot)
# SET(CMAKE_C_FLAGS   "-mfloat-abi=hard" CACHE STRING "-mfloat-abi=hard")
# SET(CMAKE_CXX_FLAGS  "-mfloat-abi=hard"  CACHE STRING "-mfloat-abi=hard")
# SET(LINK_FLAGS  "" CACHE STRING "--sysroot=${SYSROOT_PATH}")
# search for programs in the build host directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)

# for libraries and headers in the target directories
# SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
# SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

 
