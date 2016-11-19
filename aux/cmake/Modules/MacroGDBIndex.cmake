# This file defines the following macro to build the GDB index into binaries
#
# MACRO_MK_GDB_INDEX          - build gdb index into binaryew code
#
# Copyright (c) 2015, Martin Runge, <martin.runge@web.de>
#
# Redistribution and use is allowed according to the terms of the BSD license.


MACRO(ADD_GDB_INDEX target)



if(CMAKE_BUILD_TYPE MATCHES debug) 


if(MUROA_AUX_DIR)
################################################################################
# This step merges a gdb_index section into the binary to speed up debugging.
# Doing this by add_custom_command will become obsolete as soon as the linker does it. 
################################################################################

add_custom_command(TARGET ${target}
                POST_BUILD  
                COMMAND  ${MUROA_AUX_DIR}/scripts/mkgdbindex.sh $<TARGET_FILE_NAME:${target}> 
                WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                COMMENT "building gdb index")

################################################################################                                     
else()
message(warning "MUROA_AUX_DIR not set. Cannot build GDB index.")
endif()

endif()

ENDMACRO(ADD_GDB_INDEX)
