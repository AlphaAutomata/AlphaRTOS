# from 3.3 onwards, detecting whether a string exists in a list has a new syntax using if()
cmake_minimum_required(VERSION 3.3)

####################################
# Set Project-Level Configurations #
####################################

if(NOT DEFINED IPO_SUPPORTED)
	# set the policy to enable inter-procedural optimization if available
	if(POLICY CMP0069)
		cmake_policy(SET CMP0069 NEW)
	endif()
	# check for inter-procedural optimization support
	include(CheckIPOSupported OPTIONAL RESULT_VARIABLE HAS_CHECK_IPO)
	if(HAS_CHECK_IPO)
		message(STATUS "Determining inter-procedural optimization support.")
		
		check_ipo_supported(RESULT IPO_SUPPORTED)
		
		if(IPO_SUPPORTED)
			message(STATUS "Inter-procedural optimization enabled.")
		else()
			message(STATUS "Inter-procedural optimization disabled.")
		endif()
	else()
		message(STATUS "Cannot determine inter-procedural optimization support.")
		message(STATUS "Inter-procedural optimization is disabled.")
		
		set(IPO_SUPPORTED FALSE)
	endif()
	
	set(IPO_SUPPORTED "IPO_SUPPORTED" CACHE STRING "Inter-procedural optimzation support.")
endif()

######################
# Derive Directories #
######################

# get root directory
get_filename_component(HAL_RTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)

# get HAL dependency directories
set(HAL_RTOS_INC_DIR ${HAL_RTOS_DIR}/include)
set(HAL_RTOS_SRC_DIR ${HAL_RTOS_DIR}/src)
set(HAL_RTOS_SIM_DIR ${HAL_RTOS_DIR}/sim)

# get directories containing on-host simulation code
set(SIM_INC_DIR ${HAL_RTOS_SIM_DIR})
set(SIM_SRC_DIR ${HAL_RTOS_SIM_DIR})

#########################
# Gather Target Sources #
#########################

# all HAL dependency files
set(
	HAL_INC_FILES
		${HAL_RTOS_INC_DIR}/hal_platform.h
		${HAL_RTOS_INC_DIR}/hal_core.h
		${HAL_RTOS_INC_DIR}/hal_systick.h
		${HAL_RTOS_INC_DIR}/hal_timer_gp.h
)
set(
	HAL_SRC_FILES
		${HAL_RTOS_SRC_DIR}/hal_core.c
		${HAL_RTOS_SRC_DIR}/hal_systick.c
		${HAL_RTOS_SRC_DIR}/hal_timer_gp.c
)
# include assembly only when cross compiling
if(CMAKE_CROSSCOMPILING)
	message(STATUS "Configuring cross-compile build system.")
	# get the assembly sources
	set(HAL_ASM_FILES ${HAL_RTOS_SRC_DIR}/arm_context.s)
else()
	message(STATUS "Configuring for compilation targeting the host system.")
	# set blank assembly sources
	set(HAL_ASM_FILES "")
endif()

# all on-host simulation files
set(
	SIM_INC_FILES
		${SIM_INC_DIR}/sim.h
)
set(
	SIM_SRC_FILES
		""
)

####################
# Configure Target #
####################

# compile bare-metal HAL as a static library
add_library(
	hal_rtos
	STATIC
		${HAL_INC_FILES}
		${HAL_SRC_FILES}
		${HAL_ASM_FILES}
)
# set include directories
target_include_directories(
	hal_rtos
	PUBLIC
		${HAL_RTOS_INC_DIR}
)
# add simulation files when compiling for host
if(NOT CMAKE_CROSSCOMPILING)
	target_sources(
		hal_rtos
		PUBLIC
			${SIM_INC_FILES}
		PRIVATE
			${SIM_SRC_FILES}
	)
	target_include_directories(
		hal_rtos
		PUBLIC
			${SIM_INC_DIR}
	)
endif()
# define platform architecture constant
target_compile_definitions(
	hal_rtos
	PUBLIC
		-DHAL_PLATFORM=HAL_PLATFORM_${ARTOS_TARGET_PLATFORM}
)
# use link-time optimization if available, and use C11 for standard atomics
set_target_properties(
	hal_rtos
	PROPERTIES
		INTERPROCEDURAL_OPTIMIZATION ${IPO_SUPPORTED}
		C_STANDARD   11
		CXX_STANDARD 11
		C_STANDARD_REQUIRED   TRUE
		CXX_STANDARD_REQUIRED TRUE
)
