INCLUDE(CMakeForceCompiler)

# this one is important
SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
SET(ARCH "arm")

set(HOME  $ENV{HOME})
# specify the cross compiler
SET(CMAKE_C_COMPILER   ${HOME}/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-gcc)
SET(CMAKE_CXX_COMPILER ${HOME}/raspi/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian/bin/arm-linux-gnueabihf-g++)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH  /home/martin/raspi/tools/sysroot)
# where is the target environment
SET(SYSROOT_PATH  /home/martin/raspi/tools/sysroot)

SET(ENV{PKG_CONFIG_PATH} "$ENV{HOME}/home/martin/raspi/tools/sysroot/usr/lib/pkgconfig:$ENV{HOME}/raspi/tools/sysroot/usr/lib/arm-linux-gnueabihf/pkgconfig") 
# SET(CMAKE_C_FLAGS   "-mfloat-abi=hard" CACHE STRING "-mfloat-abi=hard")
# SET(CMAKE_CXX_FLAGS  "-mfloat-abi=hard"  CACHE STRING "-mfloat-abi=hard")
# SET(LINK_FLAGS  "" CACHE STRING "--sysroot=${SYSROOT_PATH}")
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH)

 