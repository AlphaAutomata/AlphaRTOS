########################
# Configurable Options #
########################

# Xilinx SDK version
set(XSDK_VERSION "NONE" CACHE STRING "Xilinx SDK version.")
set(XSDK_VERSION_OPTS 2018.2)

# Xilinx SDK installation location
set(XSDK_INSTALL_PATH "NONE" CACHE STRING "Xilinx SDK installation path.")

#######################
# Configure Toolchain #
#######################

# compile for bare metal ARM
set(CMAKE_SYSTEM_NAME      Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# configure variables based on Xilinx SDK version
if(XSDK_VERSION STREQUAL "NONE")
	# inform the user that they are cross-compiling without Xilinx SDK
	message(STATUS "Cross-compiling without Xilinx SDK.")
else()
	# check if an installation path is specified
	if(XSDK_INSTALL_PATH STREQUAL "NONE")
		message(FATAL_ERROR "No Xilinx SDK installed location specified. Set XSDK_INSTALL_PATH.")
	endif()
	
	# set expected Eclipse version based on the Xilinx SDK version
	if(XSDK_VERSION STREQUAL "2018.2")
		set(CMAKE_ECLIPSE_VERSION 4.6.1 CACHE PATH "" FORCE)
	else()
		message(FATAL_ERROR "Unrecognized Xilinx SDK version: ${XSDK_VERSION}. Set XSDK_VERSION to one of the following: ${XSDK_VERSION_OPTS}")
	endif()
	
	# inform the user of Xilinx SDK configuration
	message(STATUS "Generating Makefile for cross-compiling with Xilinx SDK ${XSDK_VERSION}.")
	
	# set paths to Xilinx-distributed compilers and build tools
	set(XSDK_ROOT_DIR "${XSDK_INSTALL_PATH}/${XSDK_VERSION}")
	set(TC_PATH       "${XSDK_ROOT_DIR}/gnu/aarch32/nt/gcc-arm-none-eabi/bin/")
	set(EXTENSION     ".exe")
	# use Make from Xilinx
	set(CMAKE_MAKE_PROGRAM "${XSDK_ROOT_DIR}/gnuwin/bin/make.exe" CACHE PATH "" FORCE)
endif()

# set the target architecture variable for use by the generator script
set(ARCH ARMCA9 CACHE PATH "" FORCE)

# toolchain binary path and toolchain exacutable prefix
set(CROSS_COMPILE arm-none-eabi-)

# set compilers, linker, and archiver
set(CMAKE_ASM_COMPILER "${TC_PATH}${CROSS_COMPILE}as${EXTENSION}"  CACHE PATH "" FORCE)
set(CMAKE_C_COMPILER   "${TC_PATH}${CROSS_COMPILE}gcc${EXTENSION}" CACHE PATH "" FORCE)
set(CMAKE_CXX_COMPILER "${TC_PATH}${CROSS_COMPILE}g++${EXTENSION}" CACHE PATH "" FORCE)
set(CMAKE_LINKER       "${TC_PATH}${CROSS_COMPILE}ld${EXTENSION}"  CACHE PATH "" FORCE)
set(CMAKE_AR           "${TC_PATH}${CROSS_COMPILE}ar${EXTENSION}"  CACHE FILEPATH "Archiver")

# force convert .elf executables into .bin raw binary maps
set(
	CMAKE_OBJCOPY "$(TC_PATH)$(CROSS_COMPILE)objcopy"
	CACHE FILEPATH "The toolchain command objcopy command " FORCE
)

# force compile test library instead of a test executable to avoid testing for startup code, etc.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# disable various system calls
set(CMAKE_EXE_LINKER_FLAGS "--specs=nosys.specs" CACHE INTERNAL "")

# do not search for binaries on compile host
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# do not use system includes and libraries
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# specify core and FPU architectures
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mcpu=cortex-a9")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon-fp16")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mtune=cortex-a9")

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mcpu=cortex-a9")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mfloat-abi=hard")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon-fp16")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -mtune=cortex-a9")

# cache the flags
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}"   CACHE STRING "CFLAGS")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" CACHE STRING "CXXFLAGS")
set(CMAKE_ASM_FLAGS "${CMAKE_C_FLAGS}"   CACHE STRING "")
