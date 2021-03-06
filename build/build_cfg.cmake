# from 3.3 onwards, detecting whether a string exists in a list has a new syntax using if()
cmake_minimum_required(VERSION 3.3)

#########################
# Configuration Options #
#########################

set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)

set(CMSIS_LINKER_SCRIPT "FALSE" CACHE BOOL "Flag to use the CMSIS linker script." FORCE)

set(ARTOS_TARGET_PLATFORM "NONE" CACHE STRING "Target SoC platform."          FORCE)
set(ARTOS_TEST_TARGET     "TRUE" CACHE BOOL   "Flag to create a test target." FORCE)

set(XSDK_VERSION      "NONE" CACHE STRING "Xilinx SDK version."           FORCE)
set(XSDK_INSTALL_PATH "NONE" CACHE PATH   "Xilinx SDK installation path." FORCE)
set(XSDK_WORKSPACE    "NONE" CACHE PATH   "Xilinx SDK workspace path."    FORCE)
set(XSDK_BSP          "NONE" CACHE STRING "Target board support package." FORCE)

##############################
# Valid Configuration Values #
##############################

set(
	XC7Z_PLATFORMS
		"XC7Zxxx"  # Zynq-7000 SoC
		"XC7ZxxxS" # Zynq-7000S SoC
	CACHE STRING
		"Supported Xilinx target SoC platforms."
	FORCE
)
set(
	ARTOS_TARGET_PLATFORM_OPTS
		${XC7Z_PLATFORMS} # target Xilinx Zynq device
	CACHE STRING
		"All supported target SoC platforms."
	FORCE
)

set(
	XSDK_VERSION_OPTS
		"NONE"   # use a custom toolchain.
		"2018.2" # use toolchains distributed with Xilinx SDK 2018.2.
	CACHE STRING
		"All supported Xilinx SDK versions."
	FORCE
)

################################
# Toolchain Path Configuration #
################################

# platform-specific configurations
if(ARTOS_TARGET_PLATFORM IN_LIST XC7Z_PLATFORMS)
	# use the ARM toolchain script
	set(CMAKE_TOOLCHAIN_FILE "toolchain.cmake" CACHE PATH "" FORCE)
	
	# configure variables based on Xilinx SDK version
	if(XSDK_VERSION STREQUAL "NONE")
		# inform the user that they are cross-compiling without Xilinx SDK
		message(STATUS "Configuring build system using a default or user-specified toolchain.")
	else()
		# check if an installation path is specified
		if(XSDK_INSTALL_PATH STREQUAL "NONE")
			message(STATUS "Using Xilinx SDK ${XSDK_VERSION} without specifying install location.")
			message(FATAL_ERROR "No Xilinx SDK install location specified. Set XSDK_INSTALL_PATH.")
		endif()
		
		# generate Makefile and Xilinx SDK projects
		set(CMAKE_GENERATOR       "Unix Makefiles" CACHE INTERNAL "" FORCE)
		set(CMAKE_EXTRA_GENERATOR "Eclipse CDT4"   CACHE INTERNAL "" FORCE)

		# set expected Eclipse version based on the Xilinx SDK version
		if(XSDK_VERSION STREQUAL "2018.2")
			set(CMAKE_ECLIPSE_VERSION 4.6.1 CACHE PATH "" FORCE)
		else()
			message(STATUS "Configured Xilinx SDK version: ${XSDK_VERSION}.")
			message(STATUS "Set XSDK_VERSION to one of the following:")
			message(STATUS "    ${XSDK_VERSION_OPTS}")
			message(FATAL_ERROR "Unsupported Xilinx SDK version: ${XSDK_VERSION}.")
		endif()
		
		# inform the user of Xilinx SDK configuration
		message(STATUS "Configuring build system using Xilinx SDK ${XSDK_VERSION}.")
		
		# set paths to Xilinx-distributed compilers and build tools
		set(XSDK_ROOT_DIR "${XSDK_INSTALL_PATH}/${XSDK_VERSION}" CACHE PATH "" FORCE)
		set(TC_PATH "${XSDK_ROOT_DIR}/gnu/aarch32/nt/gcc-arm-none-eabi/bin/" CACHE STRING "" FORCE)
		if(NOT UNIX)
			set(EXTENSION ".exe" CACHE STRING "Host platform executable file extension.")
		endif()
		
		# use Make from Xilinx
		set(CMAKE_MAKE_PROGRAM "${XSDK_ROOT_DIR}/gnuwin/bin/make${EXTENSION}" CACHE PATH "" FORCE)
	endif()
else()
	message(STATUS "Configured target device family: ${ARTOS_TARGET_PLATFORM}")
	message(STATUS "Set ARTOS_TARGET_PLATFORM to one of the following:")
	message(STATUS "    ${ARTOS_TARGET_PLATFORM_OPTS}")
	message(FATAL_ERROR "Unsupported target device family: ${ARTOS_TARGET_PLATFORM}.")
endif()
